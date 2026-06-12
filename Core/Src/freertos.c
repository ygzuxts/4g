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
extern char flynum[6]; // 起降次数

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
    while ((id == 0 || timestamp == 0)) // 获取任务ID和云网时间戳
    {
        printf("send ready task\r\n");
        readyTask(); // 发送准备作业
        osDelay(5000);
    }
		readyTask(); // 发送准备作业
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
            printf("send heap track\r\n");
            uploadTrack();
        }
        else if (SendTaskStateFlag == true && taskflag == false && getLinkedListLength(&pTrackList) < 5)
				//else if (taskflag == false )//for finish info test
        {
            printf("send task finish\r\n");
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
    while (1)
    {
        update();
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
    uint32_t lastWakeTime = getSysTickCnt();
    while (1)
    {
        if (RecieveFinishFlag == true)
        {
            RecieveFinishFlag = false;
            if (RxBuffer[0] == 0x31 && RxBuffer[1] == 0x2C)
            {
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
            else if (RxBuffer[0] == 0x32 && RxBuffer[1] == 0x2C)
            {
                process_rtcm_data(&RxBuffer[2], ucRxCnt - 2);
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
