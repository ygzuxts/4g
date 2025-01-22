#include "task_log.h"

/* FatFs includes component */
#include "ff.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h"
#include "cmsis_os.h"
#include "mavlink_parse.h"

char SDPath[4]; // SD逻辑驱动器路径
FATFS fs;       // FatFs文件系统对象
FATFS *pfs;
FIL fnew;                       // 文件对象
FRESULT res_sd;                 // 文件操作结果
DWORD fre_clust;                // 空闲簇数量
UINT fnum;                      // 文件成功读写数量
uint32_t totalSpace, freeSpace; // 总空间，剩余空间
char *currentlogfilename;       // 当前LOG文档名字

extern FATFS flash_fs;
extern Diskio_drvTypeDef SD_Driver;

const struct LogStructure log_structure[] = {
    {LOG_FORMAT_MSG, sizeof(struct log_Format),
     "FMT", "BBnNZ", "Type,Length,Name,Format,Columns"},
};

uint8_t Log_Init(void)
{
    // 链接驱动器，创建盘符
    FATFS_LinkDriver(&SD_Driver, SDPath);
    uint8_t i;
    for (i = 0; i < ARRAY_SIZE(log_structure); i++)
    {
        Write_Format(&log_structure[i]);
    }
    currentlogfilename = pvPortMalloc(_MAX_LFN + 1); // 为LOG文件名分配内存
}

// 根据任务时间戳创建LOG文件
uint8_t CreateLogFile(uint64_t timestamp)
{
    char *name = pvPortMalloc(_MAX_LFN + 1);
    char *filename = pvPortMalloc(_MAX_LFN + 1);
    timestamp_to_datetime(timestamp, name);
    sprintf(filename, "%s%s", name, ".txt");
    currentlogfilename = filename;
    vPortFree(name);
    vPortFree(filename);
}

uint8_t WriteBlock(const void *pBuffer, uint16_t size)
{
    /* Mount SD Card */
    if (f_mount(&fs, "", 0) != FR_OK)
        return 1;

    /* Open file to write */
    if (f_open(&fnew, currentlogfilename, FA_OPEN_EXISTING | FA_WRITE) != FR_OK)
        return 2;

    /* Check freeSpace space */
    if (f_getfree("", &fre_clust, &pfs) != FR_OK)
        return 3;

    totalSpace = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
    freeSpace = (uint32_t)(fre_clust * pfs->csize * 0.5);

    /* free space is less than 1kb */
    if (freeSpace < 1)
        return 4;

    /* Writing*/
    f_write(&fnew, pBuffer, size, &fnum);

    /* Close file */
    if (f_close(&fnew) != FR_OK)
        return 5;

    /* Unmount SDCARD */
    if (f_mount(NULL, "", 1) != FR_OK)
        return 6;

    return 0;
}
void Fill_Format(const struct LogStructure *s, struct log_Format *pkt)
{
    memset(pkt, 0, sizeof(*pkt));
    pkt->head1 = HEAD_BYTE1;
    pkt->head2 = HEAD_BYTE2;
    pkt->msgid = LOG_FORMAT_MSG;
    pkt->type = s->msg_type;
    pkt->length = s->msg_len;
    strncpy(pkt->name, s->name, sizeof(pkt->name));
    strncpy(pkt->format, s->format, sizeof(pkt->format));
    strncpy(pkt->labels, s->labels, sizeof(pkt->labels));
}
/*
  write a structure format to the log
 */
uint8_t Write_Format(const struct LogStructure *s)
{
    struct log_Format pkt;

    Fill_Format(s, &pkt);

    return WriteBlock(&pkt, sizeof(pkt));
}