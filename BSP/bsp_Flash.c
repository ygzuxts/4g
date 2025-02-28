#include "bsp_Flash.h"
#include "stdio.h"
#include "usart_debug.h"

#define DATA_32 ((uint32_t)0x12345678)

#define FLASH_TIMEOUT_VALUE 1000
#define FLASH_USER_START_ADDR ADDR_FLASH_SECTOR_11 // 要擦除内部FLASH的起始地址
#define FLASH_USER_END_ADDR ADDR_FLASH_SECTOR_11   // 要擦除内部FLASH的结束地址

static uint32_t GetSector(uint32_t Address);

// 对内部FLASH进行读写测试
int Flash_Write(void)
{
    // 要擦除的起始扇区(包含)及结束扇区(不包含)，如8-12，表示擦除8、9、10、11扇区
    uint32_t FirstSector = 0;
    uint32_t NbOfSectors = 0;
    HAL_StatusTypeDef FlashStatus = HAL_OK;
    uint32_t SECTORError = 0;

    uint32_t Address = 0;

    __IO uint32_t Data32 = 0;
    __IO uint32_t MemoryProgramStatus = 0;
    static FLASH_EraseInitTypeDef EraseInitStruct;
    int trycnt = 0;
    // FLASH 解锁使能访问FLASH控制寄存器
    HAL_FLASH_Unlock();

    FirstSector = GetSector(FLASH_USER_START_ADDR);
    NbOfSectors = 1;

    // 擦除用户区域 (用户区域指程序本身没有使用的空间，可以自定义)
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3; // 以“字”的大小进行操作
    EraseInitStruct.Sector = FirstSector;
    EraseInitStruct.NbSectors = NbOfSectors;
    // 开始擦除操作
    do
    {
        FlashStatus = HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError);
        if (FlashStatus != HAL_OK)
        {
            /* Infinite loop */
            trycnt++;
            if (trycnt > 3)
            {
                HAL_FLASH_Lock();
                return FlashStatus;
            }
        }
        else
            break;
        FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);
    } while (trycnt < 3);

    Address = FLASH_USER_START_ADDR; // 以“字”的大小为单位写入数据

    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, DATA_32) == HAL_OK)
    {
        // 给FLASH上锁，防止内容被篡改
        HAL_FLASH_Lock();
        return 0;
    }
    else
    {
        HAL_FLASH_Lock();
        printf("Write Error\r\n");
        return -1;
    }
}

int Flash_Read(void)
{
    // 从FLASH中读取出数据进行校验
    // MemoryProgramStatus = 0: 写入的数据正确
    // MemoryProgramStatus != 0: 写入的数据错误，其值为错误的个数
    uint32_t Address = 0;
    __IO uint32_t Data32 = 0;
    Address = FLASH_USER_START_ADDR;

    Data32 = *(__IO uint32_t *)Address;

    if (Data32 != DATA_32)
    {
        return -1;
    }
    else // 数据校验正确
    {
        return 0;
    }
}
// 根据输入的地址给出它所在的sector
static uint32_t GetSector(uint32_t Address)
{
    uint32_t sector = 0;

    if ((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
    {
        sector = FLASH_SECTOR_0;
    }
    else if ((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
    {
        sector = FLASH_SECTOR_1;
    }
    else if ((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
    {
        sector = FLASH_SECTOR_2;
    }
    else if ((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
    {
        sector = FLASH_SECTOR_3;
    }
    else if ((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
    {
        sector = FLASH_SECTOR_4;
    }
    else if ((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
    {
        sector = FLASH_SECTOR_5;
    }
    else if ((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
    {
        sector = FLASH_SECTOR_6;
    }
    else if ((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
    {
        sector = FLASH_SECTOR_7;
    }
    else if ((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
    {
        sector = FLASH_SECTOR_8;
    }
    else if ((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
    {
        sector = FLASH_SECTOR_9;
    }
    else if ((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
    {
        sector = FLASH_SECTOR_10;
    }
    else
    {
        sector = FLASH_SECTOR_11;
    }
    return sector;
}
