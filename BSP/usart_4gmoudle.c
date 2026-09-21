#include "usart_4gmoudle.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include <stdarg.h>
#include <stdlib.h>
#include "cmsis_os.h"
#include "usart_mavlink.h"

UART_HandleTypeDef huart1;

/* USART1 init function */

uint8_t RxBuffer[1500] = {0};
uint32_t ucRxCnt = 0;
bool RecieveFinishFlag = false;
bool usrMoudleInintSuccess = false; //! 需要改为false
extern char sn[20];
#define USRMoudle_UART_RX_BUF_SIZE 128
#define ATK_MW8266D_UART_TX_BUF_SIZE 256
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

#define NTRIP_DEFAULT_HOST "103.143.19.54"
#define NTRIP_DEFAULT_PORT 8003
#define NTRIP_DEFAULT_MOUNT_POINT "RTCM33GRCEJpro"
#define NTRIP_DEFAULT_AUTH_BASIC "eG5ueTU2MjMzOjQyOTc0"
#define MQTT_DEFAULT_HOST "47.112.204.68"
#define MQTT_DEFAULT_PORT 1883

#define NTRIP_HOST_MAX_LEN 63
#define NTRIP_MOUNT_MAX_LEN 63
#define NTRIP_AUTH_MAX_LEN 123

typedef struct
{
    char host[NTRIP_HOST_MAX_LEN + 1];
    uint16_t port;
    char mountpoint[NTRIP_MOUNT_MAX_LEN + 1];
    char auth_basic[NTRIP_AUTH_MAX_LEN + 1];
} NtripRuntimeConfig;

static NtripRuntimeConfig ntrip_config = {
    NTRIP_DEFAULT_HOST,
    NTRIP_DEFAULT_PORT,
    NTRIP_DEFAULT_MOUNT_POINT,
    NTRIP_DEFAULT_AUTH_BASIC};

static NtripRuntimeConfig ntrip_pending_config = {
    NTRIP_DEFAULT_HOST,
    NTRIP_DEFAULT_PORT,
    NTRIP_DEFAULT_MOUNT_POINT,
    NTRIP_DEFAULT_AUTH_BASIC};

static volatile bool ntrip_remote_apply_pending = false;
static uint8_t ntrip_remote_field_mask = 0;

#define NTRIP_CONFIG_MASK_HOST  (1U << 0)
#define NTRIP_CONFIG_MASK_PORT  (1U << 1)
#define NTRIP_CONFIG_MASK_MOUNT (1U << 2)
#define NTRIP_CONFIG_MASK_AUTH  (1U << 3)
#define NTRIP_CONFIG_MASK_ALL   (NTRIP_CONFIG_MASK_HOST | NTRIP_CONFIG_MASK_PORT | NTRIP_CONFIG_MASK_MOUNT | NTRIP_CONFIG_MASK_AUTH)

static bool ntrip_copy_field(char *destination, uint16_t capacity, const uint8_t *data, uint8_t length)
{
    if (length >= capacity || (length > 0 && data == NULL))
    {
        return false;
    }

    if (length > 0)
    {
        memcpy(destination, data, length);
    }
    destination[length] = '\0';
    return true;
}

static void usr_debug_print(const char *text)
{
    printf("%s", text);
    USART2_SendDebugText(text);
}

static void ntrip_debug_print_config(const char *prefix)
{
    char debug_buf[180];

    snprintf(debug_buf,
             sizeof(debug_buf),
             "%s host=%s port=%u mount=%s auth=%s\r\n",
             prefix,
             ntrip_config.host,
             ntrip_config.port,
             ntrip_config.mountpoint,
             (ntrip_config.auth_basic[0] != '\0') ? "set" : "none");
    usr_debug_print(debug_buf);
}

static uint8_t nmea_checksum(const char *sentence)
{
    uint8_t checksum = 0;

    while (*sentence != '\0')
    {
        checksum ^= (uint8_t)*sentence;
        sentence++;
    }

    return checksum;
}

static void nmea_format_latlon(double value, bool is_lat, char *value_out, char *dir_out)
{
    double abs_value = value;
    int degrees;
    uint32_t minutes_scaled;
    uint32_t minutes_integer;
    uint32_t minutes_fraction;

    if (abs_value < 0)
    {
        abs_value = -abs_value;
    }

    degrees = (int)abs_value;
    minutes_scaled = (uint32_t)(((abs_value - degrees) * 60.0 * 1000000.0) + 0.5);
    if (minutes_scaled >= 60000000UL)
    {
        degrees++;
        minutes_scaled -= 60000000UL;
    }
    minutes_integer = minutes_scaled / 1000000UL;
    minutes_fraction = minutes_scaled % 1000000UL;

    if (is_lat)
    {
        sprintf(value_out, "%02d%02lu.%06lu", degrees, (unsigned long)minutes_integer, (unsigned long)minutes_fraction);
        *dir_out = (value >= 0) ? 'N' : 'S';
    }
    else
    {
        sprintf(value_out, "%03d%02lu.%06lu", degrees, (unsigned long)minutes_integer, (unsigned long)minutes_fraction);
        *dir_out = (value >= 0) ? 'E' : 'W';
    }
}

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
    uint8_t *last_ret = NULL;
    char debug_buf[160];

    USRMoudle_uart_rx_restart();

    USRMoudle_uart_printf("%s", cmd);
    snprintf(debug_buf, sizeof(debug_buf), "AT>> %s", cmd);
    usr_debug_print(debug_buf);

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
                last_ret = ret;
                if (strstr((const char *)ret, ack) != NULL)
                {
                    snprintf(debug_buf, sizeof(debug_buf), "AT OK: wait [%s]\r\n", ack);
                    usr_debug_print(debug_buf);
                    return 0;
                }
                else
                {
                    snprintf(debug_buf, sizeof(debug_buf), "AT recv no match: %.80s\r\n", ret);
                    usr_debug_print(debug_buf);
                    USRMoudle_uart_rx_restart();
                }
            }
            timeout--;
            HAL_Delay(1);
        }

        snprintf(debug_buf, sizeof(debug_buf), "AT FAIL: wait [%s], last [%.80s]\r\n", ack, last_ret ? (char *)last_ret : "no response");
        usr_debug_print(debug_buf);
        return 1;
    }
}

void Ntrip_SetRemoteConfigField(uint8_t field, const uint8_t *data, uint8_t length)
{
    char debug_buf[160];
    char port_text[8];
    unsigned long port;

    switch (field)
    {
    case NTRIP_CONFIG_FIELD_HOST:
        if (length > 0 && ntrip_copy_field(ntrip_pending_config.host, sizeof(ntrip_pending_config.host), data, length))
        {
            ntrip_remote_field_mask |= NTRIP_CONFIG_MASK_HOST;
            snprintf(debug_buf,
                     sizeof(debug_buf),
                     "DBG: QGC NTRIP host received: %s\r\n",
                     ntrip_pending_config.host);
            usr_debug_print(debug_buf);
        }
        else
        {
            USART2_ReportWarning("4G config: invalid NTRIP host");
        }
        break;

    case NTRIP_CONFIG_FIELD_PORT:
        if (length > 0 && length < sizeof(port_text))
        {
            memcpy(port_text, data, length);
            port_text[length] = '\0';
            port = strtoul(port_text, NULL, 10);

            if (port > 0 && port <= 65535UL)
            {
                ntrip_pending_config.port = (uint16_t)port;
                ntrip_remote_field_mask |= NTRIP_CONFIG_MASK_PORT;
                snprintf(debug_buf,
                         sizeof(debug_buf),
                         "DBG: QGC NTRIP port received: %u\r\n",
                         ntrip_pending_config.port);
                usr_debug_print(debug_buf);
                break;
            }
        }
        USART2_ReportWarning("4G config: invalid NTRIP port");
        break;

    case NTRIP_CONFIG_FIELD_MOUNTPOINT:
        if (ntrip_copy_field(ntrip_pending_config.mountpoint, sizeof(ntrip_pending_config.mountpoint), data, length))
        {
            ntrip_remote_field_mask |= NTRIP_CONFIG_MASK_MOUNT;
            snprintf(debug_buf,
                     sizeof(debug_buf),
                     "DBG: QGC NTRIP mountpoint received: %s\r\n",
                     ntrip_pending_config.mountpoint);
            usr_debug_print(debug_buf);
        }
        else
        {
            USART2_ReportWarning("4G config: mountpoint too long");
        }
        break;

    case NTRIP_CONFIG_FIELD_AUTH_BASIC:
        if (ntrip_copy_field(ntrip_pending_config.auth_basic, sizeof(ntrip_pending_config.auth_basic), data, length))
        {
            ntrip_remote_field_mask |= NTRIP_CONFIG_MASK_AUTH;
            usr_debug_print("DBG: QGC NTRIP credentials received\r\n");
        }
        else
        {
            USART2_ReportWarning("4G config: credentials too long");
        }
        break;

    case NTRIP_CONFIG_FIELD_APPLY:
        if ((ntrip_remote_field_mask & NTRIP_CONFIG_MASK_ALL) == NTRIP_CONFIG_MASK_ALL)
        {
            ntrip_config = ntrip_pending_config;
            ntrip_remote_field_mask = 0;
            ntrip_remote_apply_pending = true;
            ntrip_debug_print_config("DBG: QGC NTRIP complete config received");
            usr_debug_print("DBG: QGC NTRIP apply requested\r\n");
            USART2_ReportInfo("4G config: received from QGC");
        }
        else
        {
            USART2_ReportWarning("4G config: incomplete QGC fields");
        }
        break;

    default:
        break;
    }
}

bool Ntrip_TakeRemoteConfigApplyRequest(void)
{
    if (ntrip_remote_apply_pending)
    {
        ntrip_remote_apply_pending = false;
        return true;
    }

    return false;
}

static uint8_t usrMoudle_EnterConfigMode(void)
{
    /* A previous interrupted configuration may have left the module in AT mode. */
    if (usrmoudle_send_at_cmd("AT\r\n", "OK", 800) == 0)
    {
        USART2_ReportInfo("4G AT: module already in config mode");
        return 0;
    }

    for (uint8_t attempt = 0; attempt < 3; attempt++)
    {
        /* The escape sequence requires a quiet UART period before "+++". */
        USRMoudle_uart_rx_restart();
        HAL_Delay(1200);

        if (usrmoudle_send_at_cmd("+++", "a", 1500) != 0)
        {
            USART2_ReportWarning("4G AT: no 'a' response to +++");
            HAL_Delay(500);
            continue;
        }

        if (usrmoudle_send_at_cmd("a", "+ok", 1500) == 0)
        {
            USART2_ReportInfo("4G AT: config mode entered");
            return 0;
        }

        USART2_ReportWarning("4G AT: no +ok response after 'a'");
        HAL_Delay(500);
    }

    return 1;
}

uint8_t usrMoudle_Clear(void)
{
    if (usrMoudle_EnterConfigMode() == 0)
    {
        usrmoudle_send_at_cmd("AT+CLEAR\r\n", "OK", 2000);
        printf("usr clear\r\n");
        return 0;
    }
    return 1;
    
}

uint8_t usrMoudle_PrepareForQgcNtrip(void)
{
#if (USR_MODULE_WORK_MODE == USR_MODULE_MODE_NTRIP)
    usr_debug_print("DBG: disabling saved NTRIP sockets before QGC config\r\n");

    if (usrMoudle_EnterConfigMode() != 0)
    {
        usr_debug_print("DBG: failed to enter AT mode while disabling NTRIP\r\n");
        return 1;
    }

    if (usrmoudle_send_at_cmd("AT+SOCKAEN=OFF\r\n", "OK", 2000) != 0 ||
        usrmoudle_send_at_cmd("AT+SOCKBEN=OFF\r\n", "OK", 2000) != 0 ||
        usrmoudle_send_at_cmd("AT+S\r\n", "OK", 2000) != 0)
    {
        usr_debug_print("DBG: failed to disable saved NTRIP sockets\r\n");
        return 1;
    }

    usr_debug_print("DBG: saved NTRIP sockets disabled\r\n");
#endif
    return 0;
}

uint8_t usrMoudle_Init(void)
{
    char EC_topic[100];

    usr_debug_print("4G init: enter config mode\r\n");
    if (usrMoudle_EnterConfigMode() == 0)
    {
        usr_debug_print("4G init: config mode ok\r\n");
#if (USR_MODULE_WORK_MODE == USR_MODULE_MODE_MQTT)
        /*
         * MQTT 云平台模式：这是原来的业务模式。
         * 用途：
         * 1. 连接 MQTT 服务器；
         * 2. 发布 /Job、/Job/<sn>/4G；
         * 3. 订阅 /device/<sn>/4G 和 /gps。
         *
         * 注意：轨迹上传、任务下发、状态上报仍然依赖这个模式。
         */

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
        printf("%s\r\n", EC_topic);
        usrmoudle_send_at_cmd(EC_topic, "OK", 2000);
        memset(EC_topic, 0, sizeof(EC_topic));

        usrmoudle_send_at_cmd("AT+MQTTVER=4\r\n", "OK", 2000);
        usrmoudle_send_at_cmd("AT+MQTTMOD=1\r\n", "OK", 2000);

        usrmoudle_send_at_cmd("AT+MQTTPUBTP=1,1,/Job,0,0\r\n", "OK", 2000);
        sprintf(EC_topic, "AT+MQTTPUBTP=2,1,/Job/%s/4G,0,0\r\n", sn);
        printf("sn set = %s\r\n", sn);

        usrmoudle_send_at_cmd(EC_topic, "OK", 2000);
        memset(EC_topic, 0, sizeof(EC_topic));

        sprintf(EC_topic, "AT+MQTTSUBTP=1,1,/device/%s/4G,0\r\n", sn);
        usrmoudle_send_at_cmd(EC_topic, "OK", 2000);

        usrmoudle_send_at_cmd("AT+MQTTSUBTP=2,1,/gps,0\r\n", "OK", 2000);

        memset(EC_topic, 0, sizeof(EC_topic));

        usrmoudle_send_at_cmd("AT+UARTFT=20\r\n", "OK", 2000);
        usrmoudle_send_at_cmd("AT+S\r\n", "OK", 2000);

#elif (USR_MODULE_WORK_MODE == USR_MODULE_MODE_NTRIP)
        /*
         * NTRIP 直连模式：只用于验证/开发 4G 模块直接连接 NTRIP caster。
         * 用途：
         * 1. 模块进入 NET 网络透传模式；
         * 2. Socket A 作为 TCP Client 连接 NTRIP 服务器；
         * 3. main.c 里会调用 Ntrip_SendRequest() 发送 NTRIP GET 请求。
         *
         * 注意：这个模式会占用 USART1 的网络透传通道，原 MQTT 云平台功能不可同时使用。
         */
        ntrip_debug_print_config("DBG: ntrip config");
        usrmoudle_send_at_cmd("AT+E=OFF\r\n", "OK", 2000);
        usrmoudle_send_at_cmd("AT+CPIN?\r\n", "READY", 2000);
        usrmoudle_send_at_cmd("AT+CSQ\r\n", "OK", 2000);
        usrmoudle_send_at_cmd("AT+CEREG?\r\n", "OK", 2000);
        usrmoudle_send_at_cmd("AT+SYSINFO\r\n", "OK", 2000);
        usrmoudle_send_at_cmd("AT+WKMOD=NET\r\n", "OK", 2000);
        usrmoudle_send_at_cmd("AT+HEARTEN=OFF\r\n", "OK", 2000);
        usrmoudle_send_at_cmd("AT+SOCKBEN=OFF\r\n", "OK", 2000);
        sprintf(EC_topic, "AT+SOCKA=TCP,%s,%u\r\n", ntrip_config.host, ntrip_config.port);
        usrmoudle_send_at_cmd(EC_topic, "OK", 2000);
        memset(EC_topic, 0, sizeof(EC_topic));
        usrmoudle_send_at_cmd("AT+SOCKAEN=ON\r\n", "OK", 2000);
        usrmoudle_send_at_cmd("AT+SOCKASL=LONG\r\n", "OK", 2000);
        usrmoudle_send_at_cmd("AT+RSTIM=0\r\n", "OK", 2000);
        usrmoudle_send_at_cmd("AT+UARTFL=4096\r\n", "OK", 2000);
        usrmoudle_send_at_cmd("AT+UARTFT=20\r\n", "OK", 2000);
        usrmoudle_send_at_cmd("AT+S\r\n", "OK", 2000);
#elif (USR_MODULE_WORK_MODE == USR_MODULE_MODE_DUAL_TCP)
        /*
         * 原来的 GM800 内置 MQTT 模式配置如下：
         *
         *   AT+WKMOD=MQTT,NOR
         *   AT+MQTTSVR=47.112.204.68,1883
         *   AT+MQTTUSER=antg!@211520
         *   AT+MQTTPSW=admin
         *   AT+MQTTCID=<sn>
         *   AT+MQTTVER=4
         *   AT+MQTTMOD=1
         *   AT+MQTTPUBTP=1,1,/Job,0,0
         *   AT+MQTTPUBTP=2,1,/Job/<sn>/4G,0,0
         *   AT+MQTTSUBTP=1,1,/device/<sn>/4G,0
         *   AT+MQTTSUBTP=2,1,/gps,0
         *
         * 现在为了和 NTRIP 并发，不再让 GM800 工作在 MQTT 模式。
         * GM800 只工作在 NET + SDP 套接字分发：
         *
         *   Socket A -> NTRIP caster
         *   Socket B -> MQTT broker
         *
         * MQTT CONNECT/PUBLISH/SUBSCRIBE/PING 报文由 STM32 在 mqtt_client.c
         * 内按 MQTT 3.1.1 自己生成，然后通过 GM800_SdpSend(Socket B) 发出。
         */
        ntrip_debug_print_config("DBG: dual tcp ntrip config");
        usrmoudle_send_at_cmd("AT+E=OFF\r\n", "OK", 2000);
        usrmoudle_send_at_cmd("AT+CPIN?\r\n", "READY", 2000);
        usrmoudle_send_at_cmd("AT+CSQ\r\n", "OK", 2000);
        usrmoudle_send_at_cmd("AT+CEREG?\r\n", "OK", 2000);
        usrmoudle_send_at_cmd("AT+SYSINFO\r\n", "OK", 2000);
        usrmoudle_send_at_cmd("AT+WKMOD=NET\r\n", "OK", 2000);
        usrmoudle_send_at_cmd("AT+HEARTEN=OFF\r\n", "OK", 2000);

        sprintf(EC_topic, "AT+SOCKA=TCP,%s,%u\r\n", ntrip_config.host, ntrip_config.port);
        usrmoudle_send_at_cmd(EC_topic, "OK", 2000);
        memset(EC_topic, 0, sizeof(EC_topic));
        usrmoudle_send_at_cmd("AT+SOCKAEN=ON\r\n", "OK", 2000);
        usrmoudle_send_at_cmd("AT+SOCKASL=LONG\r\n", "OK", 2000);

        sprintf(EC_topic, "AT+SOCKB=TCP,%s,%u\r\n", MQTT_DEFAULT_HOST, MQTT_DEFAULT_PORT);
        usrmoudle_send_at_cmd(EC_topic, "OK", 2000);
        memset(EC_topic, 0, sizeof(EC_topic));
        usrmoudle_send_at_cmd("AT+SOCKBEN=ON\r\n", "OK", 2000);

        usrmoudle_send_at_cmd("AT+SDPEN=ON\r\n", "OK", 2000);
        usrmoudle_send_at_cmd("AT+RSTIM=0\r\n", "OK", 2000);
        usrmoudle_send_at_cmd("AT+UARTFL=4096\r\n", "OK", 2000);
        usrmoudle_send_at_cmd("AT+UARTFT=20\r\n", "OK", 2000);
        usrmoudle_send_at_cmd("AT+S\r\n", "OK", 2000);
#endif

        return 0;
    }
    else
    {
        usr_debug_print("4G init: enter config mode failed\r\n");
    }
    return 1;
}

void Ntrip_SendRequest(void)
{
    ntrip_debug_print_config("DBG: ntrip request");
#if (USR_MODULE_WORK_MODE == USR_MODULE_MODE_DUAL_TCP)
    char request[512];
    int len = snprintf(request,
                       sizeof(request),
                       "GET /%s HTTP/1.1\r\n"
                       "Host: %s:%u\r\n"
                       "Ntrip-Version: Ntrip/2.0\r\n"
                       "User-Agent: NTRIP STM32Client/1.0\r\n"
                       "%s%s%s"
                       "Connection: keep-alive\r\n"
                       "\r\n",
                       ntrip_config.mountpoint,
                       ntrip_config.host,
                       ntrip_config.port,
                       (ntrip_config.auth_basic[0] != '\0') ? "Authorization: Basic " : "",
                       (ntrip_config.auth_basic[0] != '\0') ? ntrip_config.auth_basic : "",
                       (ntrip_config.auth_basic[0] != '\0') ? "\r\n" : "");
    if (len > 0 && len < (int)sizeof(request))
    {
        char debug_buf[120];
        snprintf(debug_buf,
                 sizeof(debug_buf),
                 "DBG: ntrip GET send socket=A len=%u mount=%s auth=%s\r\n",
                 (unsigned int)len,
                 ntrip_config.mountpoint,
                 (ntrip_config.auth_basic[0] != '\0') ? "set" : "none");
        usr_debug_print(debug_buf);
        GM800_SdpSend(GM800_SOCKET_A_NTRIP, (const uint8_t *)request, (uint16_t)len);
    }
    else
    {
        usr_debug_print("DBG: ntrip GET build failed or too long\r\n");
    }
#else
    USRMoudle_uart_printf("GET /%s HTTP/1.1\r\n", ntrip_config.mountpoint);
    USRMoudle_uart_printf("Host: %s:%u\r\n", ntrip_config.host, ntrip_config.port);
    USRMoudle_uart_printf("Ntrip-Version: Ntrip/2.0\r\n");
    USRMoudle_uart_printf("User-Agent: NTRIP STM32Client/1.0\r\n");
    if (ntrip_config.auth_basic[0] != '\0')
    {
        USRMoudle_uart_printf("Authorization: Basic %s\r\n", ntrip_config.auth_basic);
    }
    USRMoudle_uart_printf("Connection: keep-alive\r\n");
    USRMoudle_uart_printf("\r\n");
#endif
}

void Ntrip_SendGGA(double lat, double lon, uint64_t utc_sec)
{
    static bool gga_sent_reported = false;
    static uint8_t gga_debug_divider = 0;
#if !NTRIP_USE_FIXED_GGA_FOR_TEST
    static bool zero_position_reported = false;
#endif
    char lat_text[16];
    char lon_text[16];
    char body[128];
    char lat_dir;
    char lon_dir;
    uint32_t day_seconds;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t checksum;
    char debug_buf[120];

    if (lat == 0.0 && lon == 0.0)
    {
#if NTRIP_USE_FIXED_GGA_FOR_TEST
        lat = NTRIP_TEST_LAT;
        lon = NTRIP_TEST_LON;
        usr_debug_print("DBG: ntrip using fixed test gga position\r\n");
        if (!gga_sent_reported)
        {
            USART2_ReportWarning("NTRIP: using fixed test GGA");
        }
#else
        if (!zero_position_reported)
        {
            zero_position_reported = true;
            usr_debug_print("DBG: ntrip gga skipped, lat/lon is zero\r\n");
            USART2_ReportWarning("NTRIP: GGA skipped, GPS position zero");
        }
        return;
#endif
    }

    nmea_format_latlon(lat, true, lat_text, &lat_dir);
    nmea_format_latlon(lon, false, lon_text, &lon_dir);

    day_seconds = (uint32_t)(utc_sec % 86400ULL);
    hour = day_seconds / 3600;
    minute = (day_seconds % 3600) / 60;
    second = day_seconds % 60;

    sprintf(body,
            "GPGGA,%02u%02u%02u.00,%s,%c,%s,%c,1,08,1.0,0.0,M,0.0,M,,",
            hour,
            minute,
            second,
            lat_text,
            lat_dir,
            lon_text,
            lon_dir);
    checksum = nmea_checksum(body);
#if (USR_MODULE_WORK_MODE == USR_MODULE_MODE_DUAL_TCP)
    char gga[128];
    int gga_len = snprintf(gga, sizeof(gga), "$%s*%02X\r\n", body, checksum);
    if (gga_len > 0 && gga_len < (int)sizeof(gga))
    {
        {
            char debug_buf[160];
            snprintf(debug_buf,
                     sizeof(debug_buf),
                     "DBG: ntrip GGA send socket=A len=%d lat=%.7f lon=%.7f utc=%lu\r\n",
                     gga_len,
                     lat,
                     lon,
                     (unsigned long)utc_sec);
            usr_debug_print(debug_buf);
        }
        GM800_SdpSend(GM800_SOCKET_A_NTRIP, (const uint8_t *)gga, (uint16_t)gga_len);
    }
#else
    USRMoudle_uart_printf("$%s*%02X\r\n", body, checksum);
#endif
    usr_debug_print("DBG: ntrip gga sent\r\n");

    /*
     * USART3 debug: print the detailed position every 10 GGA messages. This
     * keeps the debug port readable while proving that location updates are
     * still being sent to the NTRIP caster.
     */
    if (++gga_debug_divider >= 10)
    {
        gga_debug_divider = 0;
        snprintf(debug_buf,
                 sizeof(debug_buf),
                 "DBG: gga detail lat_e7=%ld lon_e7=%ld utc=%lu\r\n",
                 (long)(lat * 10000000.0),
                 (long)(lon * 10000000.0),
                 (unsigned long)utc_sec);
        usr_debug_print(debug_buf);
    }

    if (!gga_sent_reported)
    {
        gga_sent_reported = true;
        USART2_ReportInfo("NTRIP: GGA sent");
    }
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
