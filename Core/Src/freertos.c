/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdint.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mavlink_parse.h"
#include "usart_debug.h"
#include "track_queue.h"
#include "cJSON.h"
#include "usart_4gmoudle.h"
#include "mavlink_parse.h"
#include "track_json.h"
#include "rtcm_parse.h"
#include "bsp_led.h"
#include "bsp_can.h"
#include "mqtt_client.h"
#include "usart_mavlink.h"
#include "mavlink_usart_fifo.h"
/* FatFs includes component */
#include "ff.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h"
#include "task_log.h"
#include "event_groups.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
LinkedList pTrackList;
TrackInfo pTrackInfo;

extern bool taskflag;
extern bool SendTaskStateFlag;
extern bool attitude_ready;
extern bool gps_ready;
extern bool jobid_ready;
extern bool tag_flag;
extern bool RecieveFinishFlag;
extern bool distance_flag;

extern double fdistance;
extern uint64_t timestamp;
extern uint8_t client; // 0：地面站
extern uint32_t jobId;
extern uint32_t trackNum; // 上传序号，发送一次轨迹序号加一
extern uint32_t duration; // 持续时间
extern uint32_t id;

extern uint32_t ucRxCnt;
extern uint8_t RxBuffer[1500];
extern char token[10];
extern char sn[20];
extern char cv[10];    // 固件版本
extern char fv[10];    // 飞控版本
extern char flynum[6];
extern volatile uint32_t mavlink_rx_message_count;
extern volatile uint32_t mavlink_rx_fe_count;
extern volatile uint32_t mavlink_rx_fd_count;
extern volatile uint32_t mavlink_rx_ore_count;
extern volatile uint32_t mavlink_rx_ne_count;
extern volatile uint32_t mavlink_rx_fe_err_count;
extern volatile uint32_t mavlink_rx_fifo_full_count; // 起降次数

extern char SDPath[4];     /* SD逻辑驱动器路径 */
extern FATFS fs;           /* FatFs文件系统对象 */
extern FIL fnew;           /* 文件对象 */
extern FRESULT res_sd;     /* 文件操作结果 */
extern UINT fnum;          /* 文件成功读写数量 */
extern BYTE WriteBuffer[]; /* 写缓冲区*/
extern FATFS flash_fs;
extern Diskio_drvTypeDef SD_Driver;

/* deal with Task time id start */
//获取时间戳信息
uint64_t timestamp_gnsss = 0;
volatile uint64_t g_task_id = 0;

/* 用于保护 RTC/BKP 的互斥锁 */
osMutexId gRtcMutexHandle;
//osMutexDef(gRtcMutex)
/* deal with Task time id end */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BIT_Task01_EVENT (EventBits_t)(0x0001 << 0)
#define BIT_Task02_EVENT (EventBits_t)(0x0001 << 1)
#define BIT_Task03_EVENT (EventBits_t)(0x0001 << 2)
#define BIT_Task04_EVENT (EventBits_t)(0x0001 << 3)
#define BIT_TaskAll_EVENT BIT_Task01_EVENT | BIT_Task02_EVENT | BIT_Task03_EVENT | BIT_Task04_EVENT
#define APP_FLOW_DEBUG 0
#define CLOUD_READY_WAIT_DEBUG 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId TrackRecodeTaskHandle;
osThreadId TrackSendTaskHandle;
osThreadId MavlinkParseTaskHandle;
osThreadId JsonParseTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
uint32_t getSysTickCnt(void)
{
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) // FreeRTOS 是否已经启动
        return xTaskGetTickCount();
    else
        return 0;
}
uint64_t getTaskExecutionCnt(void)
{
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED && timestamp != 0) // FreeRTOS 是否已经启动并且已经从云网获得时间戳
        return (uint64_t)xTaskGetTickCount() + timestamp;
    else
        return timestamp;
}

static int find_byte(const uint8_t *buffer, uint16_t length, uint8_t value)
{
    for (uint16_t i = 0; i < length; i++)
    {
        if (buffer[i] == value)
        {
            return i;
        }
    }
    return -1;
}

static int find_bytes(const uint8_t *buffer, uint16_t length, const uint8_t *pattern, uint16_t pattern_len)
{
    if (pattern_len == 0 || length < pattern_len)
    {
        return -1;
    }

    for (uint16_t i = 0; i <= length - pattern_len; i++)
    {
        if (memcmp(&buffer[i], pattern, pattern_len) == 0)
        {
            return i;
        }
    }
    return -1;
}

static void ntrip_debug_print(const char *text)
{
    printf("%s", text);
    USART2_SendDebugText(text);
}


static void ntrip_debug_dump_hex(const char *prefix, const uint8_t *data, uint16_t length, uint16_t max_dump)
{
    char debug_buf[220];
    int used;
    uint16_t dump_len;

    if (data == NULL)
    {
        return;
    }

    dump_len = (length > max_dump) ? max_dump : length;
    used = snprintf(debug_buf,
                    sizeof(debug_buf),
                    "%s len=%u hex=",
                    prefix,
                    (unsigned int)length);

    for (uint16_t i = 0; i < dump_len && used < (int)(sizeof(debug_buf) - 6); i++)
    {
        used += snprintf(&debug_buf[used], sizeof(debug_buf) - (uint32_t)used, "%02X ", data[i]);
    }

    if (length > dump_len && used < (int)(sizeof(debug_buf) - 8))
    {
        used += snprintf(&debug_buf[used], sizeof(debug_buf) - (uint32_t)used, "...");
    }

    snprintf(&debug_buf[used], sizeof(debug_buf) - (uint32_t)used, "\r\n");
    ntrip_debug_print(debug_buf);
}

#define NTRIP_HANDSHAKE_RETRY_MS 5000U

static bool ntrip_header_done = false;
static bool ntrip_handshake_accepted = false;
static uint8_t ntrip_header_cache[256];
static uint16_t ntrip_header_cache_len = 0;
static uint8_t ntrip_rtcm_cache[1500];
static uint16_t ntrip_rtcm_cache_len = 0;
static uint32_t ntrip_rx_total_bytes = 0;
static uint32_t ntrip_rtcm_feed_total_bytes = 0;
static uint32_t ntrip_rtcm_frame_count = 0;

static void reset_ntrip_stream_state(void)
{
    ntrip_header_done = false;
    ntrip_handshake_accepted = false;
    ntrip_header_cache_len = 0;
    ntrip_rtcm_cache_len = 0;
    ntrip_rx_total_bytes = 0;
    ntrip_rtcm_feed_total_bytes = 0;
    ntrip_rtcm_frame_count = 0;
    memset(ntrip_header_cache, 0, sizeof(ntrip_header_cache));
    memset(ntrip_rtcm_cache, 0, sizeof(ntrip_rtcm_cache));
}

static void ntrip_debug_print_header_status(const uint8_t *header, uint16_t header_len)
{
    char debug_buf[160];
    const char *prefix = "DBG: ntrip header first line: ";
    uint16_t prefix_len = strlen(prefix);
    uint16_t line_len = 0;

    while (line_len < header_len && line_len < 90)
    {
        if (header[line_len] == '\r' || header[line_len] == '\n')
        {
            break;
        }
        line_len++;
    }

    if (line_len >= sizeof(debug_buf) - 40)
    {
        line_len = sizeof(debug_buf) - 40;
    }

    if (find_bytes(header, header_len, (const uint8_t *)"ICY 200", 7) >= 0 ||
        find_bytes(header, header_len, (const uint8_t *)"HTTP/1.1 200", 12) >= 0 ||
        find_bytes(header, header_len, (const uint8_t *)"HTTP/1.0 200", 12) >= 0)
    {
        ntrip_handshake_accepted = true;
        ntrip_debug_print("DBG: NTRIP HANDSHAKE SUCCESS\r\n");
        BSP_CAN_SetDebugStatus(BSP_CAN_STATE_NTRIP_ACCEPTED, 0, 0);
        USART2_ReportInfo("NTRIP: caster accepted");
    }
    else
    {
        ntrip_handshake_accepted = false;
        ntrip_debug_print("DBG: NTRIP HANDSHAKE FAILED\r\n");
        BSP_CAN_SetDebugStatus(BSP_CAN_STATE_ERROR, 2, 0);
        memcpy(debug_buf, prefix, prefix_len);
        memcpy(&debug_buf[prefix_len], header, line_len);
        memcpy(&debug_buf[prefix_len + line_len], "\r\n", 3);
        ntrip_debug_print(debug_buf);
        USART2_ReportWarning("NTRIP: caster rejected request");
    }
}

static void feed_ntrip_rtcm(const uint8_t *data, uint16_t length)
{
    if (length == 0)
    {
        return;
    }

    ntrip_rtcm_feed_total_bytes += length;
    {
        char debug_buf[120];
        snprintf(debug_buf,
                 sizeof(debug_buf),
                 "DBG: ntrip rtcm feed len=%u total=%lu cache_before=%u\r\n",
                 (unsigned int)length,
                 (unsigned long)ntrip_rtcm_feed_total_bytes,
                 (unsigned int)ntrip_rtcm_cache_len);
        ntrip_debug_print(debug_buf);
    }
    ntrip_debug_dump_hex("DBG: ntrip rtcm feed sample", data, length, 32U);

    if (length > sizeof(ntrip_rtcm_cache) - ntrip_rtcm_cache_len)
    {
        ntrip_debug_print("DBG: rtcm cache overflow, reset\r\n");
        USART2_ReportWarning("NTRIP: RTCM cache overflow");
        ntrip_rtcm_cache_len = 0;
    }

    if (length > sizeof(ntrip_rtcm_cache))
    {
        length = sizeof(ntrip_rtcm_cache);
    }

    memcpy(&ntrip_rtcm_cache[ntrip_rtcm_cache_len], data, length);
    ntrip_rtcm_cache_len += length;

    while (ntrip_rtcm_cache_len > 0)
    {
        int d3_pos = find_byte(ntrip_rtcm_cache, ntrip_rtcm_cache_len, 0xD3);

        if (d3_pos < 0)
        {
            ntrip_rtcm_cache_len = 0;
            return;
        }

        if (d3_pos > 0)
        {
            memmove(ntrip_rtcm_cache, &ntrip_rtcm_cache[d3_pos], ntrip_rtcm_cache_len - d3_pos);
            ntrip_rtcm_cache_len -= (uint16_t)d3_pos;
        }

        if (ntrip_rtcm_cache_len < 6)
        {
            return;
        }

        uint16_t payload_length = ((ntrip_rtcm_cache[1] & 0x03) << 8) | ntrip_rtcm_cache[2];
        uint16_t total_length = payload_length + 6;

        if (total_length > sizeof(ntrip_rtcm_cache))
        {
            ntrip_debug_print("DBG: rtcm invalid length, drop header\r\n");
            USART2_ReportWarning("NTRIP: invalid RTCM length");
            memmove(ntrip_rtcm_cache, &ntrip_rtcm_cache[1], ntrip_rtcm_cache_len - 1);
            ntrip_rtcm_cache_len--;
            continue;
        }

        if (ntrip_rtcm_cache_len < total_length)
        {
            char debug_buf[120];
            snprintf(debug_buf,
                     sizeof(debug_buf),
                     "DBG: ntrip rtcm waiting frame need=%u have=%u payload=%u\r\n",
                     (unsigned int)total_length,
                     (unsigned int)ntrip_rtcm_cache_len,
                     (unsigned int)payload_length);
            ntrip_debug_print(debug_buf);
            return;
        }

        ntrip_rtcm_frame_count++;
        {
            char debug_buf[120];
            snprintf(debug_buf,
                     sizeof(debug_buf),
                     "DBG: ntrip rtcm complete frame #%lu total_len=%u payload=%u cache=%u\r\n",
                     (unsigned long)ntrip_rtcm_frame_count,
                     (unsigned int)total_length,
                     (unsigned int)payload_length,
                     (unsigned int)ntrip_rtcm_cache_len);
            ntrip_debug_print(debug_buf);
        }
        process_rtcm_data(ntrip_rtcm_cache, total_length);

        if (ntrip_rtcm_cache_len > total_length)
        {
            memmove(ntrip_rtcm_cache,
                    &ntrip_rtcm_cache[total_length],
                    ntrip_rtcm_cache_len - total_length);
        }
        ntrip_rtcm_cache_len -= total_length;
    }
}

static void process_ntrip_stream(uint8_t *buffer, uint16_t length)
{
    static const uint8_t header_end[] = {'\r', '\n', '\r', '\n'};

    if (length == 0)
    {
        return;
    }

    ntrip_rx_total_bytes += length;
    {
        char debug_buf[120];
        snprintf(debug_buf,
                 sizeof(debug_buf),
                 "DBG: ntrip stream rx len=%u total=%lu header_done=%u accepted=%u\r\n",
                 (unsigned int)length,
                 (unsigned long)ntrip_rx_total_bytes,
                 (unsigned int)ntrip_header_done,
                 (unsigned int)ntrip_handshake_accepted);
        ntrip_debug_print(debug_buf);
    }
    ntrip_debug_dump_hex("DBG: ntrip stream sample", buffer, length, 32U);

    if (!ntrip_header_done)
    {
        uint16_t copy_len = length;
        if (copy_len > sizeof(ntrip_header_cache) - ntrip_header_cache_len)
        {
            copy_len = sizeof(ntrip_header_cache) - ntrip_header_cache_len;
        }
        memcpy(&ntrip_header_cache[ntrip_header_cache_len], buffer, copy_len);
        ntrip_header_cache_len += copy_len;

        int end_pos = find_bytes(ntrip_header_cache,
                                 ntrip_header_cache_len,
                                 header_end,
                                 sizeof(header_end));
        if (end_pos >= 0)
        {
            ntrip_header_done = true;
            ntrip_debug_print("DBG: ntrip header ok\r\n");
            ntrip_debug_print_header_status(ntrip_header_cache, (uint16_t)end_pos);
            ntrip_header_done = ntrip_handshake_accepted;

            uint16_t cached_rtcm_pos = (uint16_t)end_pos + sizeof(header_end);
            if (ntrip_handshake_accepted && cached_rtcm_pos < ntrip_header_cache_len)
            {
                feed_ntrip_rtcm(&ntrip_header_cache[cached_rtcm_pos],
                                ntrip_header_cache_len - cached_rtcm_pos);
            }

            if (ntrip_handshake_accepted && length > copy_len)
            {
                feed_ntrip_rtcm(&buffer[copy_len], length - copy_len);
            }
            ntrip_header_cache_len = 0;
            memset(ntrip_header_cache, 0, sizeof(ntrip_header_cache));
            return;
        }

        int rtcm_pos = find_byte(buffer, length, 0xD3);
        if (rtcm_pos >= 0)
        {
            ntrip_header_done = true;
            ntrip_handshake_accepted = true;
            ntrip_debug_print("DBG: NTRIP HANDSHAKE SUCCESS (RTCM stream detected)\r\n");
            BSP_CAN_SetDebugStatus(BSP_CAN_STATE_NTRIP_ACCEPTED, 0, 0);
            USART2_ReportInfo("NTRIP: RTCM stream detected");
            feed_ntrip_rtcm(&buffer[rtcm_pos], length - rtcm_pos);
            ntrip_header_cache_len = 0;
            memset(ntrip_header_cache, 0, sizeof(ntrip_header_cache));
            return;
        }

        if (ntrip_header_cache_len >= sizeof(ntrip_header_cache))
        {
            ntrip_debug_print("DBG: NTRIP HANDSHAKE FAILED (response header too long)\r\n");
            BSP_CAN_SetDebugStatus(BSP_CAN_STATE_ERROR, 3, 0);
            USART2_ReportWarning("NTRIP: response header too long");
            reset_ntrip_stream_state();
        }
        return;
    }

    feed_ntrip_rtcm(buffer, length);
}

static void process_gm800_sdp_stream(uint8_t *buffer, uint16_t length)
{
    uint16_t pos = 0;

    while ((uint16_t)(length - pos) >= 8U)
    {
        if (buffer[pos] != 0xAA || buffer[pos + 1U] != 0xFD || buffer[pos + 2U] != 0x55)
        {
            pos++;
            continue;
        }

        uint16_t payload_len = ((uint16_t)buffer[pos + 3U] << 8) | buffer[pos + 4U];
        uint16_t frame_len = (uint16_t)(3U + 2U + payload_len + 1U);
        if (payload_len < 2U || frame_len > (uint16_t)(length - pos))
        {
            return;
        }

        uint8_t checksum = 0;
        for (uint16_t i = 0; i < payload_len; i++)
        {
            checksum = (uint8_t)(checksum + buffer[pos + 5U + i]);
        }
        if (checksum != buffer[pos + 5U + payload_len])
        {
            ntrip_debug_print("DBG: gm800 sdp checksum error\r\n");
            pos++;
            continue;
        }

        uint8_t socket = buffer[pos + 5U];
        uint8_t reserve = buffer[pos + 6U];
        uint8_t *data = &buffer[pos + 7U];
        uint16_t data_len = (uint16_t)(payload_len - 2U);

        if (reserve == 0x00 && socket == GM800_SOCKET_A_NTRIP)
        {
            ntrip_debug_print("DBG: gm800 sdp route=socket A ntrip\r\n");
            ntrip_debug_dump_hex("DBG: socket A ntrip data", data, data_len, 32U);
            process_ntrip_stream(data, data_len);
        }
        else if (reserve == 0x00 && socket == GM800_SOCKET_B_MQTT)
        {
            MqttClient_Input(data, data_len);
        }
        else
        {
            char sdp_debug[96];
            snprintf(sdp_debug,
                     sizeof(sdp_debug),
                     "DBG: gm800 sdp module/unknown socket=%02X reserve=%02X len=%u data0=%02X\r\n",
                     socket,
                     reserve,
                     (unsigned int)data_len,
                     (data_len > 0U) ? data[0] : 0U);
            ntrip_debug_print(sdp_debug);
        }

        pos = (uint16_t)(pos + frame_len);
    }
}
/* USER CODE END FunctionPrototypes */

void StartTrackRecodeTask(void const *argument);
void StartTrackSendTask(void const *argument);
void StartMavlinkParseTask(void const *argument);
void StartJsonParseTask(void const *argument);
void StartTaskIdInitTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = &xIdleStack[0];
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
    /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
    /* USER CODE BEGIN Init */
    /* 可选：先把全局ID置0 */
    g_task_id = 0;
    /* USER CODE END Init */

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */
    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */

	
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    /* USER CODE END RTOS_QUEUES */

    /* Create the thread(s) */
    /* definition and creation of TrackRecodeTask */
    osThreadDef(TrackRecodeTask, StartTrackRecodeTask, osPriorityNormal, 0, 2048);
    TrackRecodeTaskHandle = osThreadCreate(osThread(TrackRecodeTask), NULL);

    /* definition and creation of TrackSendTask */
    osThreadDef(TrackSendTask, StartTrackSendTask, osPriorityNormal, 0, 2048);
    TrackSendTaskHandle = osThreadCreate(osThread(TrackSendTask), NULL);

    /* definition and creation of MavlinkParseTask */
    osThreadDef(MavlinkParseTask, StartMavlinkParseTask, osPriorityNormal, 0, 2048);
    MavlinkParseTaskHandle = osThreadCreate(osThread(MavlinkParseTask), NULL);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    osThreadDef(JsonParseTask, StartJsonParseTask, osPriorityNormal, 0, 4096);
    JsonParseTaskHandle = osThreadCreate(osThread(JsonParseTask), NULL);
		
		/* definition and creation of TrackRecodeTask */
    osThreadDef(TaskIdInitTask, StartTaskIdInitTask, osPriorityAboveNormal, 0, 1024);
    TrackRecodeTaskHandle = osThreadCreate(osThread(TaskIdInitTask), NULL);
    /* USER CODE END RTOS_THREADS */
}

/* USER CODE BEGIN Header_StartTrackRecodeTask */
/**
 * @brief  Function implementing the TrackRecodeTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTrackRecodeTask */
void StartTrackRecodeTask(void const *argument)
{

    uint32_t lastWakeTime = getSysTickCnt();

    while (1)
    {
        malvlink_heart_send();

        if (taskflag == true)
        {
//            if (gps_ready && attitude_ready && jobid_ready)
						if (gps_ready && attitude_ready )
            {
                gps_ready = false;
                attitude_ready = false;
                distance_flag = true;
                insertDataAtEnd(&pTrackList, pTrackInfo);
            }
        }


        vTaskDelayUntil(&lastWakeTime, F2T(RATE_1_HZ));
    }
    /* USER CODE END StartTrackRecodeTask */
}

/* USER CODE BEGIN Header_StartTrackSendTask */
/**
 * @brief Function implementing the TrackSendTask thread.
 * @param argument: Not used
 * @retval None
 */

/* USER CODE END Header_StartTrackSendTask */
void StartTrackSendTask(void const *argument)
{
    /* USER CODE BEGIN StartTrackSendTask */
    /* Infinite loop */
		id = 777;
		timestamp = 1759127916;
    while ((id == 0 || timestamp == 0))
    {
#if APP_FLOW_DEBUG
        printf("send ready task\r\n");
#endif
        if (MqttClient_IsConnected())
        {
            readyTask();
        }
        osDelay(5000);
    }
    while (!MqttClient_IsConnected())
    {
#if CLOUD_READY_WAIT_DEBUG
        printf("DBG: cloud ready wait mqtt connected\r\n");
#endif
        osDelay(1000);
    }
		readyTask();
    SetLEDState(1, 2);
    while (1)
    {
        if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_13) == GPIO_PIN_SET) // LED1 闪烁证明4G模块联网正常
        {
            HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_3);
        }
        
//        if (!jobid_ready && taskflag == true) // 向云网发送任务开始
					if ( taskflag == true)
					{
            taskAction();
						
					}
///////////***五个任务航点传输时使用下面代码****/
//        if (taskflag == true && jobid_ready == true) // 等待jobid和时间戳
//				{
//						printf("queue size is %d\r\n", getLinkedListLength(&pTrackList));
//            if (getLinkedListLength(&pTrackList) >= 5)
//            {
//                uploadTrack();
//								
//            }
//						
//        }
				//安特狗项目需求发送数据
				normal_status_Action();
					
				//taskflag = false;//for finish info test
        if (SendTaskStateFlag == true && taskflag == false && getLinkedListLength(&pTrackList) >= 5) //
        {
#if APP_FLOW_DEBUG
            printf("send heap track\r\n");
#endif
            uploadTrack();
        }
        else if (SendTaskStateFlag == true && taskflag == false && getLinkedListLength(&pTrackList) < 5)
				//else if (taskflag == false )//for finish info test
        {
#if APP_FLOW_DEBUG
            printf("send task finish\r\n");
#endif
            SendTaskStateFlag = false;
            finishTask();
            osDelay(2000);
            finishTask();
					
//						osDelay(2000);								//for finish info test
//						startTask(); // 发送开始作业	//for finish info test
//            printf("startTask\r\n");			//for finish info test
					
            osDelay(2000);
            fdistance = 0;
            trackNum = 0;
            jobId = 0;
            jobid_ready = 0;
            destroyLinkedList(&pTrackList);
            SetLEDState(1, 2); // 自检通过
					
						
						//taskflag = true;//for finish info test
        }
        osDelay(1000);
    }
    /* USER CODE END StarttrackSendTask */
}

/* USER CODE BEGIN Header_StartMavlinkParseTask */
/**
 * @brief Function implementing the MavlinkParseTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartMavlinkParseTask */
void StartMavlinkParseTask(void const *argument)
{
    /* USER CODE BEGIN StartMavlinkParseTask */
    /* Infinite loop */
    uint32_t lastWakeTime = getSysTickCnt();
    uint32_t selftest_cnt = 0;
    while (1)
    {
        update();
#if USART2_TEXT_DEBUG_ONLY
        if (++selftest_cnt >= 200U)
        {
            uint8_t sample[16];
            uint8_t sample_len;
            char selftest_debug[220];
            int pos;

            selftest_cnt = 0;
            sample_len = mavlink_rx_copy_sample(sample, sizeof(sample));
            pos = snprintf(selftest_debug,
                           sizeof(selftest_debug),
                           "DBG: usart2 selftest bytes=%lu msgs=%lu fifo=%u fe=%lu fd=%lu ore=%lu ne=%lu ferr=%lu full=%lu hex=",
                           (unsigned long)mavlink_rx_byte_count,
                           (unsigned long)mavlink_rx_message_count,
                           (unsigned int)serial_available(&mavlink_uart_rx_fifo),
                           (unsigned long)mavlink_rx_fe_count,
                           (unsigned long)mavlink_rx_fd_count,
                           (unsigned long)mavlink_rx_ore_count,
                           (unsigned long)mavlink_rx_ne_count,
                           (unsigned long)mavlink_rx_fe_err_count,
                           (unsigned long)mavlink_rx_fifo_full_count);
            for (uint8_t i = 0; i < sample_len && pos < (int)(sizeof(selftest_debug) - 4); i++)
            {
                pos += snprintf(&selftest_debug[pos],
                                sizeof(selftest_debug) - (uint32_t)pos,
                                "%02X ",
                                sample[i]);
            }
            snprintf(&selftest_debug[pos], sizeof(selftest_debug) - (uint32_t)pos, "\r\n");
            USART2_SendDebugText(selftest_debug);
        }
#endif
        vTaskDelayUntil(&lastWakeTime, F2T(RATE_200_HZ));
    }
    /* USER CODE END StartMavlinkParseTask */
}

void StartJsonParseTask(void const *argument)
{
    /* USER CODE BEGIN StartJsonParseTask */
    /* Infinite loop */
    cJSON *cjson_clode = NULL;
    // char *JsonStr = NULL;
    char debug_buf[96];
    uint32_t lastWakeTime = getSysTickCnt();
    uint32_t lastGgaTime = 0;
    uint32_t lastNtripRequestTime = getSysTickCnt();
#if (USR_MODULE_WORK_MODE == USR_MODULE_MODE_NTRIP) && (NTRIP_CONFIG_SOURCE == NTRIP_CONFIG_SOURCE_QGC)
    bool ntrip_runtime_active = false;
#else
    bool ntrip_runtime_active = true;
#endif
    while (1)
    {
        BSP_CAN_DebugHeartbeat();
#if (USR_MODULE_WORK_MODE == USR_MODULE_MODE_NTRIP) || (USR_MODULE_WORK_MODE == USR_MODULE_MODE_DUAL_TCP)
#if (USR_MODULE_WORK_MODE == USR_MODULE_MODE_NTRIP)
        if (Ntrip_TakeRemoteConfigApplyRequest())
        {
            ntrip_runtime_active = false;
            ntrip_debug_print("DBG: applying QGC NTRIP config to 4G module\r\n");
            USART2_ReportInfo("4G config: applying QGC settings");
            usrMoudleInintSuccess = false;
            RecieveFinishFlag = false;
            ucRxCnt = 0;
            memset(RxBuffer, 0, sizeof(RxBuffer));
            USRMoudle_uart_rx_restart();

            if (usrMoudle_Init() == 0)
            {
                HAL_Delay(10000);
                ucRxCnt = 0;
                memset(RxBuffer, 0, sizeof(RxBuffer));
                USRMoudle_uart_rx_restart();
                usrMoudleInintSuccess = true;
                reset_ntrip_stream_state();
                Ntrip_SendRequest();
                lastNtripRequestTime = getSysTickCnt();
                lastGgaTime = getSysTickCnt();
                ntrip_runtime_active = true;
                BSP_CAN_SetDebugStatus(BSP_CAN_STATE_4G_CONFIGURED, 0, 0);
                BSP_CAN_SetDebugStatus(BSP_CAN_STATE_NTRIP_REQUEST_SENT, 0, 0);
                ntrip_debug_print("DBG: QGC NTRIP config applied to 4G\r\n");
                ntrip_debug_print("DBG: NTRIP request sent with QGC config\r\n");
                USART2_ReportInfo("4G config: QGC settings applied");
                USART2_ReportInfo("NTRIP: caster request sent");
            }
            else
            {
                usrMoudleInintSuccess = false;
                ntrip_runtime_active = false;
                ntrip_debug_print("DBG: QGC NTRIP config apply failed; connection remains off\r\n");
                USART2_ReportWarning("4G config: apply failed");
            }
        }
#endif

        if (ntrip_runtime_active &&
            !ntrip_handshake_accepted &&
            (getSysTickCnt() - lastNtripRequestTime) >= NTRIP_HANDSHAKE_RETRY_MS)
        {
            reset_ntrip_stream_state();
            Ntrip_SendRequest();
            lastNtripRequestTime = getSysTickCnt();
            ntrip_debug_print("DBG: NTRIP HANDSHAKE TIMEOUT, request retried\r\n");
            USART2_ReportWarning("NTRIP: handshake timeout, retrying");
        }

        if (ntrip_runtime_active &&
            ntrip_handshake_accepted &&
            (getSysTickCnt() - lastGgaTime) >= F2T(RATE_10_HZ))
        {
            lastGgaTime = getSysTickCnt();
            Ntrip_SendGGA(pTrackInfo.lat, pTrackInfo.lon, pTrackInfo.utc_sec);
        }
#if (USR_MODULE_WORK_MODE == USR_MODULE_MODE_DUAL_TCP)
        MqttClient_Poll(getSysTickCnt());
#endif
#endif

        if (RecieveFinishFlag == true)
        {
            RecieveFinishFlag = false;
            snprintf(debug_buf,
                     sizeof(debug_buf),
                     "DBG: 4g rx packet len=%lu first=%02X %02X %02X\r\n",
                     (unsigned long)ucRxCnt,
                     (ucRxCnt > 0) ? RxBuffer[0] : 0,
                     (ucRxCnt > 1) ? RxBuffer[1] : 0,
                     (ucRxCnt > 2) ? RxBuffer[2] : 0);
            ntrip_debug_print(debug_buf);

#if (USR_MODULE_WORK_MODE == USR_MODULE_MODE_DUAL_TCP)
            if (ucRxCnt >= 3 && RxBuffer[0] == 0xAA && RxBuffer[1] == 0xFD && RxBuffer[2] == 0x55)
            {
                process_gm800_sdp_stream(RxBuffer, ucRxCnt);
            }
            else
#endif
            if (ucRxCnt >= 2 && RxBuffer[0] == 0x31 && RxBuffer[1] == 0x2C)
            {
                ntrip_debug_print("DBG: 4g rx route=json topic 1\r\n");
                cjson_clode = cJSON_Parse((const char *)&RxBuffer[2]);

                if (cjson_clode != NULL)
                {
                    json_prarse(cjson_clode);
                    cJSON_Delete(cjson_clode);
                }
                else
                {
                    printf("cJSON_Parse failed\r\n");
                }
            }
            else if (ucRxCnt >= 2 && RxBuffer[0] == 0x32 && RxBuffer[1] == 0x2C)
            {
                ntrip_debug_print("DBG: 4g rx route=rtcm mqtt topic 2\r\n");
                process_rtcm_data(&RxBuffer[2], ucRxCnt - 2);
            }
            else
            {
                ntrip_debug_print("DBG: 4g rx route=ntrip tcp stream\r\n");
                process_ntrip_stream(RxBuffer, ucRxCnt);
            }
            ucRxCnt = 0;
            memset(RxBuffer, 0, sizeof(RxBuffer));
        }
        vTaskDelayUntil(&lastWakeTime, F2T(RATE_100_HZ));
    }
    /* USER CODE END StartMavlinkParseTask */
}
/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */


void StartTaskIdInitTask(void const * argument)
{
//    /* 等 GNSS 时间有效,轮询 gnss_time_is_valid()*/
			while (!gnss_time_is_valid()) {
					osDelay(200);
			}

    taskid_initonce_fromgnss();

    /* 生成后这个任务就没用了，删除自身 */
    osThreadTerminate(NULL);
}






/* USER CODE END Application */
