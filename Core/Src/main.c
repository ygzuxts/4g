/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "main.h"
#include "cmsis_os.h"
#include "usart_mavlink.h"
#include "mavlink_types.h"
#include "mavlink_parse.h"
#include "common.h"
#include "cJSON.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart_debug.h"
#include "usart_4gmoudle.h"
#include "task_log.h"
#include "bsp_led.h"
#include "bsp_Flash.h"
#include "bsp_can.h"
#include "track_queue.h"
#include <string.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

#define MAVLINK_UART_RX_BUFFER_SIZE 2048
mavlink_system_t mavlink_system;

extern bool usrMoudleInintSuccess;
extern char sn[20];
extern fifo_t mavlink_uart_rx_fifo;
extern uint8_t mavlink_uart_rx_buf[MAVLINK_UART_RX_BUFFER_SIZE];
extern TrackInfo pTrackInfo;
extern uint32_t ucRxCnt;
extern uint8_t RxBuffer[1500];
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/*
 * Debug only:
 * 1 = do not wait for flight controller SN/time before initializing 4G/NTRIP.
 *     Use this when testing the 4G module without the flight controller connected.
 * 0 = normal product flow, wait until MAVLink provides SN and valid UTC time.
 */
#define DEBUG_BYPASS_MAVLINK_WAIT 0

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */
static void DBG_BOTH(const char *msg);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void DBG_BOTH(const char *msg)
{
    printf("%s", msg);
    USART2_SendDebugText(msg);
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_USART1_UART_Init();
    MX_USART2_UART_Init();
    MX_USART3_UART_Init();
    Led_init();
    DBG_BOTH("DBG: uart2/usart3 boot probe\r\n");
    if (BSP_CAN1_Init() == HAL_OK)
    {
#if (BSP_CAN1_PINMAP == BSP_CAN1_PINMAP_PB8_PB9)
        DBG_BOTH("DBG: can1 debug initialized PB8/PB9 1Mbps\r\n");
#else
        DBG_BOTH("DBG: can1 debug initialized PD0/PD1 1Mbps\r\n");
#endif
    }
    else
    {
        DBG_BOTH("DBG: can1 debug init failed\r\n");
    }

    /* USER CODE BEGIN 2 */
    fifo_init(&mavlink_uart_rx_fifo, mavlink_uart_rx_buf, MAVLINK_UART_RX_BUFFER_SIZE); // 初始化mavlink fifo
    mavlink_system.sysid = MAV_TYPE_GENERIC;
    mavlink_system.compid = MAV_COMP_ID_ALL;

    cJSON_Hooks cJSONhooks_freeRTOS;
    cJSONhooks_freeRTOS.malloc_fn = pvPortMalloc;
    cJSONhooks_freeRTOS.free_fn = vPortFree;
    cJSON_InitHooks(&cJSONhooks_freeRTOS);

    HAL_Delay(2000); // 等待4G模块和飞控上电
		printf("sys start1 -------------\r\n");
#if (USR_MODULE_WORK_MODE == USR_MODULE_MODE_NTRIP) && (NTRIP_CONFIG_SOURCE == NTRIP_CONFIG_SOURCE_QGC)
    /* Disable any connection saved in the 4G module before waiting for FC data. */
    USART2_ReportInfo("NTRIP: disabling old connection");
    while (usrMoudle_PrepareForQgcNtrip())
    {
        BSP_CAN_SetDebugStatus(BSP_CAN_STATE_ERROR, 1, 0);
        DBG_BOTH("DBG: failed to disable old NTRIP connection, retry\r\n");
        USART2_ReportWarning("NTRIP: disable old connection failed");
    }
    /* Keep parsing MAVLink while AT+S saves settings and reboots the module. */
    for (uint16_t reboot_wait = 0; reboot_wait < 1000; reboot_wait++)
    {
        update();
        HAL_Delay(10);
    }
    usrMoudleInintSuccess = false;
    DBG_BOTH("DBG: old NTRIP connection disabled\r\n");
#endif
#if DEBUG_BYPASS_MAVLINK_WAIT
    strcpy(sn, "123456789");
    pTrackInfo.utc_sec = 1759127916;
    DBG_BOTH("DBG: bypass mavlink sn/time wait for 4g test\r\n");
#else
    uint32_t wait_mavlink_cnt = 0;
    char wait_debug[128];
    DBG_BOTH("DBG: after 2s delay, waiting mavlink sn/time\r\n");
    USART2_ReportInfo("NTRIP: waiting FC serial/time");
    while (sn[0] == 0 || pTrackInfo.utc_sec < 1609459200ULL)
    {
        malvlink_serial_num_request_send();
        update();
        if (++wait_mavlink_cnt >= 500)
        {
            wait_mavlink_cnt = 0;
            snprintf(wait_debug,
                     sizeof(wait_debug),
                     "DBG: mavlink wait bytes=%lu msgs=%lu fifo=%u sn=%s utc=%lu\r\n",
                     (unsigned long)mavlink_rx_byte_count,
                     (unsigned long)mavlink_rx_message_count,
                     (unsigned int)serial_available(&mavlink_uart_rx_fifo),
                     (sn[0] != 0) ? "ok" : "missing",
                     (unsigned long)pTrackInfo.utc_sec);
            DBG_BOTH(wait_debug);
            USART2_ReportWarning("NTRIP: still waiting FC serial/time");
        }
        HAL_Delay(10);
    }
#endif
//    printf("sn is %s\r\n", sn);
    SetLEDState(2, 2);
		printf("sys start2 -------------\r\n");
    DBG_BOTH("DBG: mavlink sn/time ok\r\n");

#if (USR_MODULE_WORK_MODE == USR_MODULE_MODE_NTRIP) && (NTRIP_CONFIG_SOURCE == NTRIP_CONFIG_SOURCE_QGC)
    /*
     * QGC-controlled NTRIP mode: keep both saved sockets disabled at boot.
     * The FreeRTOS task will configure and connect the module only after all
     * fields and the APPLY packet have arrived from QGC.
     */
    DBG_BOTH("DBG: waiting for QGC NTRIP config\r\n");
    USART2_ReportInfo("NTRIP: waiting for QGC config");
#else
    DBG_BOTH("DBG: init 4g module with compiled config\r\n");
    USART2_ReportInfo("NTRIP: FC ready, starting 4G");
    while (usrMoudle_Init())
    {
        BSP_CAN_SetDebugStatus(BSP_CAN_STATE_ERROR, 1, 0);
        DBG_BOTH("DBG: 4g init failed, retry\r\n");
        USART2_ReportWarning("NTRIP: 4G init failed, retrying");
    }
    HAL_Delay(10000); // 等待4G模块保存参数重启
#endif

    pTrackInfo.day_job_id = Flash_DailyCounter_Init_Inc_And_Save(pTrackInfo.utc_sec);	//读取flash存储的任务ID号，并判断是否需要重置
    
		//printf("Boot daily counter = %lu, utc_time: %lu\r\n", (unsigned long) pTrackInfo.day_job_id,(unsigned long)pTrackInfo.utc_sec);
				
#if !((USR_MODULE_WORK_MODE == USR_MODULE_MODE_NTRIP) && (NTRIP_CONFIG_SOURCE == NTRIP_CONFIG_SOURCE_QGC))
		printf("4G Cat Config Success\r\n");
    DBG_BOTH("DBG: 4g config success\r\n");
    USART2_ReportInfo("NTRIP: 4G configured");
    BSP_CAN_SetDebugStatus(BSP_CAN_STATE_4G_CONFIGURED, 0, 0);
    usrMoudleInintSuccess = true;
#if (USR_MODULE_WORK_MODE == USR_MODULE_MODE_NTRIP)
    ucRxCnt = 0;
    memset(RxBuffer, 0, sizeof(RxBuffer));
    Ntrip_SendRequest();
    BSP_CAN_SetDebugStatus(BSP_CAN_STATE_NTRIP_REQUEST_SENT, 0, 0);
    printf("NTRIP request sent\r\n");
    DBG_BOTH("DBG: ntrip request sent\r\n");
    USART2_ReportInfo("NTRIP: caster request sent");
#endif
#endif
#if (USR_MODULE_WORK_MODE == USR_MODULE_MODE_NTRIP) && (NTRIP_CONFIG_SOURCE == NTRIP_CONFIG_SOURCE_QGC)
    printf("4G Cat Ready - Waiting QGC NTRIP Config\r\n");
#endif

    SetLEDState(3, 2); // 自检通过

    /* USER CODE END 2 */

    /* Call init function for freertos objects (in cmsis_os2.c) */
    MX_FREERTOS_Init();
		printf("sys start3 -------------\r\n");
    DBG_BOTH("DBG: freertos start\r\n");
    /* Start scheduler */
    osKernelStart();

    /* We should never get here as control is now taken by the scheduler */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    //  while (1)
    //  {
    //    /* USER CODE END WHILE */

    //    /* USER CODE BEGIN 3 */
    //  }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM6 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    /* USER CODE BEGIN Callback 0 */

    /* USER CODE END Callback 0 */
    if (htim->Instance == TIM6)
    {
        HAL_IncTick();
    }
    /* USER CODE BEGIN Callback 1 */

    /* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
