#include "usart_4gmoudle.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include <stdarg.h>
#include "cmsis_os.h"

UART_HandleTypeDef huart1;

/* USART1 init function */

uint8_t RxBuffer[1500] = {0};
uint32_t ucRxCnt = 0;
bool RecieveFinishFlag = false;
bool usrMoudleInintSuccess = false; //! 需要改为false
extern char sn[20];
#define USRMoudle_UART_RX_BUF_SIZE 128
#define ATK_MW8266D_UART_TX_BUF_SIZE 64
static struct
{
    uint8_t buf[USRMoudle_UART_RX_BUF_SIZE]; // 帧接收缓�?
    struct
    {
        uint16_t len : 15;                                // 帧接收长度，sta[14:0]
        uint16_t finish : 1;                              // 帧接收完成标志，sta[15]
    } sta;                                                // 帧状态信�?
} uart_rx_frame = {0};                                    // USRMoudle UART接收帧缓冲信�?结构�?
static uint8_t uart_tx_buf[ATK_MW8266D_UART_TX_BUF_SIZE]; // USRMoudle UART发送缓�?

/**
 * @brief       USRMoudle UART printf
 * @param       fmt: 待打印的数据
 * @retval      �?
 */
void USRMoudle_uart_printf(char *fmt, ...)
{
    va_list ap;
    uint16_t len;

    va_start(ap, fmt);
    vsprintf((char *)uart_tx_buf, fmt, ap);
    va_end(ap);

    len = strlen((const char *)uart_tx_buf);
    HAL_UART_Transmit(&huart1, uart_tx_buf, len, 1000);
    memset(uart_tx_buf, 0, sizeof(uart_tx_buf));
}

/**
 * @brief       USRMoudle UART重新开始接收数�?
 * @param       �?
 * @retval      �?
 */
void USRMoudle_uart_rx_restart(void)
{
    uart_rx_frame.sta.len = 0;
    uart_rx_frame.sta.finish = 0;
    memset(uart_rx_frame.buf, 0, sizeof(uart_rx_frame.buf));
}

/**
 * @brief       获取USRMoudle UART接收到的一帧数�?
 * @param       �?
 * @retval      NULL: �?接收到一帧数�?
 *              其他: 接收到的一帧数�?
 */
uint8_t *USRMoudle_uart_rx_get_frame(void)
{
    if (uart_rx_frame.sta.finish == 1)
    {
        return uart_rx_frame.buf;
    }
    else
    {
        return NULL;
    }
}

/**
 * @brief       获取USRMoudle UART接收到的一帧数�?的长�?
 * @param       �?
 * @retval      0   : �?接收到一帧数�?
 *              其他: 接收到的一帧数�?的长�?
 */
uint16_t USRMoudle_uart_rx_get_frame_len(void)
{
    if (uart_rx_frame.sta.finish == 1)
    {
        return uart_rx_frame.sta.len;
    }
    else
    {
        return 0;
    }
}

uint8_t usrmoudle_send_at_cmd(char *cmd, char *ack, uint32_t timeout)
{
    uint8_t *ret = NULL;

    USRMoudle_uart_rx_restart();

    USRMoudle_uart_printf("%s", cmd);

    if ((ack == NULL) || (timeout == 0))
    {
        return 0;
    }
    else
    {
        while (timeout > 0)
        {
            ret = USRMoudle_uart_rx_get_frame();

            if (ret != NULL)
            {
                if (strstr((const char *)ret, ack) != NULL)
                {
                    return 0;
                }
                else
                {
                    USRMoudle_uart_rx_restart();
                }
            }
            timeout--;
            HAL_Delay(1);
        }

        return 1;
    }
}

uint8_t usrMoudle_Clear(void)
{
    usrmoudle_send_at_cmd("+++", "a", 300);
    if (usrmoudle_send_at_cmd("a", "+ok", 300) == 0)
    {
        usrmoudle_send_at_cmd("AT+CLEAR\r\n", "OK", 2000);
        printf("usr clear\r\n");
        return 0;
    }
    return 1;
    
}
uint8_t usrMoudle_Init(void)
{
    char EC_topic[100];

    usrmoudle_send_at_cmd("+++", "a", 300);
    if (usrmoudle_send_at_cmd("a", "+ok", 300) == 0)
    {
//        usrmoudle_send_at_cmd("AT+E=OFF\r\n", "OK", 2000);
//        usrmoudle_send_at_cmd("AT+WKMOD=MQTT,NOR\r\n", "OK", 2000);
//        usrmoudle_send_at_cmd("AT+HEARTEN=OFF\r\n", "OK", 2000);
//        usrmoudle_send_at_cmd("AT+MQTTSVR=112.126.73.65,1883\r\n", "OK", 2000);
//        usrmoudle_send_at_cmd("AT+MQTTUSER=wkzy\r\n", "OK", 2000);
//        usrmoudle_send_at_cmd("AT+MQTTPSW=wkzy\r\n", "OK", 2000);
//        usrmoudle_send_at_cmd("AT+MQTTCID=0240132208302\r\n", "OK", 2000);
//        usrmoudle_send_at_cmd("AT+MQTTVER=3\r\n", "OK", 2000);
//        usrmoudle_send_at_cmd("AT+MQTTMOD=1\r\n", "OK", 2000);

				usrmoudle_send_at_cmd("AT+E=OFF\r\n", "OK", 2000);
        usrmoudle_send_at_cmd("AT+WKMOD=MQTT,NOR\r\n", "OK", 2000);
        usrmoudle_send_at_cmd("AT+HEARTEN=OFF\r\n", "OK", 2000);
        usrmoudle_send_at_cmd("AT+MQTTSVR=47.112.204.68,1883\r\n", "OK", 2000);
        usrmoudle_send_at_cmd("AT+MQTTUSER=antg!@211520\r\n", "OK", 2000);
        usrmoudle_send_at_cmd("AT+MQTTPSW=admin\r\n", "OK", 2000);
				usrmoudle_send_at_cmd("AT+UARTFL=4096\r\n", "OK", 2000);			
			
			  sprintf(EC_topic, "AT+MQTTCID=%s\r\n", sn);
				printf("%s\r\n",EC_topic);
        usrmoudle_send_at_cmd(EC_topic, "OK", 2000);
				memset(EC_topic, 0, sizeof(EC_topic));
			
        usrmoudle_send_at_cmd("AT+MQTTVER=4\r\n", "OK", 2000);
        usrmoudle_send_at_cmd("AT+MQTTMOD=1\r\n", "OK", 2000);
			
        usrmoudle_send_at_cmd("AT+MQTTPUBTP=1,1,/Job,0,0\r\n", "OK", 2000);
        sprintf(EC_topic, "AT+MQTTPUBTP=2,1,/Job/%s/4G,0,0\r\n", sn);
				printf("sn set = %s\r\n",sn);

        usrmoudle_send_at_cmd(EC_topic, "OK", 2000);
        memset(EC_topic, 0, sizeof(EC_topic));

        sprintf(EC_topic, "AT+MQTTSUBTP=1,1,/device/%s/4G,0\r\n", sn);
        usrmoudle_send_at_cmd(EC_topic, "OK", 2000);

        usrmoudle_send_at_cmd("AT+MQTTSUBTP=2,1,/gps,0\r\n", "OK", 2000);

        memset(EC_topic, 0, sizeof(EC_topic));

        usrmoudle_send_at_cmd("AT+UARTFT=20\r\n", "OK", 2000);

        usrmoudle_send_at_cmd("AT+S\r\n", "OK", 2000);

        return 0;
    }
    else
    {
        printf("Config failed\r\n");
    }
    return 1;
}
void MX_USART1_UART_Init(void)
{

    /* USER CODE BEGIN USART1_Init 0 */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* USER CODE END USART1_Init 0 */
    /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USER CODE BEGIN USART1_MspInit 1 */

    /* USER CODE END USART1_MspInit 1 */
    /* USER CODE BEGIN USART1_Init 1 */

    /* USER CODE END USART1_Init 1 */
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart1);
    /* USER CODE BEGIN USART1_Init 2 */
    /* USART1 interrupt Init */
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    __HAL_UART_CLEAR_IDLEFLAG(&huart1);
    HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    /* USER CODE END USART1_Init 2 */
}

/**
 * @brief This function handles USART1 global interrupt.
 */
void USART1_IRQHandler(void)
{
    uint8_t RxData = 0;
    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_ORE) != RESET) // UART接收过载�?�?
    {
        __HAL_UART_CLEAR_OREFLAG(&huart1); // 清空标志�?
        (void)huart1.Instance->SR;         // 清空SR寄存�?，清空DR寄存�?
        (void)huart1.Instance->DR;
    }

    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) != RESET)
    {
        if (usrMoudleInintSuccess == true)
        {
            if (ucRxCnt >= sizeof(RxBuffer))
                ucRxCnt = 0;
            HAL_UART_Receive(&huart1, &RxData, 1, 1000);
            RxBuffer[ucRxCnt++] = RxData;
        }
        else
        {
            HAL_UART_Receive(&huart1, &RxData, 1, 1000);
            if (uart_rx_frame.sta.len < (USRMoudle_UART_RX_BUF_SIZE - 1)) // 判断UART接收缓冲�?否溢出留出一位给结束�?'\0'
            {
                uart_rx_frame.buf[uart_rx_frame.sta.len] = RxData; // 将接收到的数�?写入缓冲
                uart_rx_frame.sta.len++;                           // 更新接收到的数据长度
            }
            else
            {
                uart_rx_frame.sta.len = 0;                         // 覆盖之前收到的数�?
                uart_rx_frame.buf[uart_rx_frame.sta.len] = RxData; // 将接收到的数�?写入缓冲
                uart_rx_frame.sta.len++;                           // 更新接收到的数据长度
            }
        }
    }

    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE) != RESET)
    {
        if (usrMoudleInintSuccess == true)
        {
            RecieveFinishFlag = true;
        }
        else
        {
            uart_rx_frame.sta.finish = 1;
        }

        __HAL_UART_CLEAR_IDLEFLAG(&huart1);
    }
}

void USART1_SendBytes(uint8_t *buf, uint16_t len)
{
    HAL_UART_Transmit(&huart1, buf, len, 1000); // 串口发送数�?
}
