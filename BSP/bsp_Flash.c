/**
  ******************************************************************************
  * @file    bsp_Flash.c
  * @brief   STM32F407 Sector11 Flash persistence:
  *          Store daily-unique TaskID boot sequence using append-only records.
  *
  * 功能：
  *  - 同一天内每次启动生成不同 TaskID（boot_seq 自增）
  *  - 第二天（UTC day变化）自动重置 boot_seq
  *  - 数据存储在内部 Flash Sector11，采用追加记录方式，写满后再擦除
  *
  * TaskID 格式（64位）：
  *   task_id = ( (uint64_t)utc_day << 32 ) | boot_seq
  *
  * 依赖：
  *  - 你工程中的 bsp_Flash.h 定义了 ADDR_FLASH_SECTOR_0..11
  *  - 使用 STM32 HAL 的 Flash 编程接口
  *
  ******************************************************************************
  */

#include "bsp_Flash.h"
#include "stdio.h"
#include "usart_debug.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>

/* ========================= 用户可配置区 ========================= */

/* 选择用于存储的扇区：这里固定使用 Sector11 */
#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_11
#define FLASH_USER_SECTOR       FLASH_SECTOR_11

/* F407 Sector11 通常为 128KB（S5~S11 为 128KB），如你芯片/布局不同请调整 */
#define FLASH_USER_SECTOR_SIZE  (128u * 1024u)

/* 超时（保留你的定义） */
#define FLASH_TIMEOUT_VALUE     1000u

/* ======================= TaskID 记录定义 ======================== */

#define DCREC_MAGIC   ((uint32_t)0x44434E54u)   /* 'DCNT' */
#define DCREC_EMPTY   ((uint32_t)0xFFFFFFFFu)

typedef struct {
    uint32_t magic;
    uint32_t day;     /* UTC day = utc_sec / 86400 */
    uint32_t counter;     /* boot sequence within day, start from 1 */
    uint32_t check;   /* simple xor check */
} dcrec_t;

/* ========================= 内部静态函数 ========================= */

static uint32_t dcrec_check(const dcrec_t *r)
{
    return (r->magic ^ r->day ^ r->counter ^ 0x5A5A5A5Au);
}

static int dcrec_is_empty(const dcrec_t *r)
{
    return (r->magic   == DCREC_EMPTY &&
            r->day     == DCREC_EMPTY &&
            r->counter == DCREC_EMPTY &&
            r->check   == DCREC_EMPTY);
}

static int dcrec_is_valid(const dcrec_t *r)
{
    if (r->magic != DCREC_MAGIC) return 0;
    return (r->check == dcrec_check(r));
}

static uint32_t utc_sec_to_day(uint64_t utc_sec)
{
    return (uint32_t)(utc_sec / 86400ULL);
}

static int flash_erase_user_sector(void)
{
    HAL_StatusTypeDef st;
    uint32_t sector_error = 0;
    FLASH_EraseInitTypeDef erase = {0};

    HAL_FLASH_Unlock();

    erase.TypeErase    = FLASH_TYPEERASE_SECTORS;
    erase.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    erase.Sector       = FLASH_USER_SECTOR;
    erase.NbSectors    = 1;

    st = HAL_FLASHEx_Erase(&erase, &sector_error);

    HAL_FLASH_Lock();
    return (st == HAL_OK) ? 0 : -1;
}

/* 在 Sector11 中找到下一条可写地址（追加写） */
static uint32_t dcrec_find_next_addr(void)
{
    uint32_t addr = FLASH_USER_START_ADDR;
    uint32_t end  = FLASH_USER_START_ADDR + FLASH_USER_SECTOR_SIZE;

    while (addr + sizeof(dcrec_t) <= end) {
        const dcrec_t *r = (const dcrec_t *)addr;
        if (dcrec_is_empty(r)) {
            return addr;
        }
        addr += sizeof(dcrec_t);
    }
    return 0; /* 写满 */
}

/* 读取最后一条有效记录 */
static int dcrec_read_last(dcrec_t *out)
{
    uint32_t addr = FLASH_USER_START_ADDR;
    uint32_t end  = FLASH_USER_START_ADDR + FLASH_USER_SECTOR_SIZE;

    dcrec_t last_valid;
    int found = 0;

    while (addr + sizeof(dcrec_t) <= end) {
        const dcrec_t *r = (const dcrec_t *)addr;

        if (dcrec_is_empty(r)) {
            break;
        }

        if (dcrec_is_valid(r)) {
            last_valid = *r;
            found = 1;
        }

        addr += sizeof(dcrec_t);
    }

    if (found && out) {
        *out = last_valid;
        return 0;
    }
    return -1;
}

/* 追加写入一条记录（16字节，4个word写入） */
static int dcrec_append(const dcrec_t *rec)
{
    uint32_t addr = dcrec_find_next_addr();
    if (addr == 0) {
        if (flash_erase_user_sector() != 0) {
            printf("erase user sector failed\r\n");
            return -1;
        }
        addr = FLASH_USER_START_ADDR;
    }

    HAL_FLASH_Unlock();

    const uint32_t *w = (const uint32_t *)rec;
    for (uint32_t i = 0; i < (sizeof(dcrec_t) / 4); i++) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr + i * 4, w[i]) != HAL_OK) {
            HAL_FLASH_Lock();
            printf("flash program failed @0x%08lx\r\n", (unsigned long)(addr + i * 4));
            return -1;
        }
    }

    HAL_FLASH_Lock();
    return 0;
}

/* ========================= 对外功能接口 ========================= */

/**
 * @brief  启动初始化：读取上次值 -> 判断是否新一天 -> counter++ -> 写回
 * @param  utc_sec: 当前 UTC 秒（建议 GNSS 或 RTC 校准后的 Unix time）
 * @retval 当前启动后的 counter 值（已自增并写入）
 */
uint32_t Flash_DailyCounter_Init_Inc_And_Save(uint64_t utc_sec)
{
    uint32_t day_now = utc_sec_to_day(utc_sec);

    dcrec_t last;
    uint32_t last_day = day_now;
    uint32_t counter = 0;

    if (dcrec_read_last(&last) == 0) {
        last_day = last.day;
        counter  = last.counter;
    }

    /* 新的一天：重置 */
    if (day_now != last_day) {
        counter = 0;
    }

    /* 本次启动自增 */
    //counter++;

    dcrec_t rec;
    rec.magic   = DCREC_MAGIC;
    rec.day     = day_now;
    //rec.counter = counter;
    rec.check   = dcrec_check(&rec);

//    if (dcrec_append(&rec) != 0) {
//        printf("DailyCounter save failed\r\n");
//        /* 写失败也返回自增后的值，但不会持久化 */
//    }

    return counter;
}

void Flash_write(uint32_t nmu){
    dcrec_t Rec;
    Rec.magic   = DCREC_MAGIC;
    Rec.counter = nmu;
    Rec.check   = dcrec_check(&Rec);

    if (dcrec_append(&Rec) != 0) {
        printf("DailyCounter save failed\r\n");
        /* 写失败也返回自增后的值，但不会持久化 */
    }
}

int Flash_DailyCounter_ReadLast(uint32_t *day_out, uint32_t *counter_out)
{
    dcrec_t last;
    if (dcrec_read_last(&last) != 0) {
        return -1;
    }
    if (day_out) *day_out = last.day;
    if (counter_out) *counter_out = last.counter;
    return 0;
}

int Flash_DailyCounter_Clear(void)
{
    return flash_erase_user_sector();
}






