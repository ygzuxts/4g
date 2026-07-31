#ifndef _USART_MAVLINK_H_
#define _USART_MAVLINK_H_

#include "stm32f4xx.h"
#include <stdbool.h>

#define USR_MODULE_MODE_MQTT     0
#define USR_MODULE_MODE_NTRIP    1
#define USR_MODULE_MODE_DUAL_TCP 2

/*
 * 默认保持原来的 MQTT 云平台模式。
 * 如果只想验证 4G 模块直连 NTRIP，把下面这一行改成 USR_MODULE_MODE_NTRIP。
 */
#define USR_MODULE_WORK_MODE USR_MODULE_MODE_DUAL_TCP

/*
 * NTRIP configuration source. Only change NTRIP_CONFIG_SOURCE:
 *
 * NTRIP_CONFIG_SOURCE_DEFAULT:
 *   Connect immediately with the host/account compiled in usart_4gmoudle.c.
 *
 * NTRIP_CONFIG_SOURCE_QGC:
 *   Keep the 4G connection disabled after boot. Connect only after QGC sends
 *   host, port, mountpoint, account/password and the apply command.
 */
#define NTRIP_CONFIG_SOURCE_DEFAULT 0
#define NTRIP_CONFIG_SOURCE_QGC     1
#define NTRIP_CONFIG_SOURCE         NTRIP_CONFIG_SOURCE_DEFAULT

/*
 * NTRIP test only:
 * 1 = when the flight controller/GPS has not provided lat/lon yet, send this
 *     fixed GGA position to the caster so RTCM downlink can be tested.
 * 0 = normal product mode, only use real lat/lon from MAVLink/GPS.
 *
 * Change this back to 0 before real flight/RTK testing.
 */
#define NTRIP_USE_FIXED_GGA_FOR_TEST 0
#define NTRIP_TEST_LAT 39.908823
#define NTRIP_TEST_LON 116.397470

/* QGC -> navigation board runtime NTRIP configuration over MAVLink TUNNEL. */
#define NTRIP_CONFIG_TUNNEL_PAYLOAD_TYPE 42000U
#define NTRIP_CONFIG_PROTOCOL_VERSION   1U
#define NTRIP_CONFIG_FIELD_HOST         1U
#define NTRIP_CONFIG_FIELD_PORT         2U
#define NTRIP_CONFIG_FIELD_MOUNTPOINT   3U
#define NTRIP_CONFIG_FIELD_AUTH_BASIC   4U
#define NTRIP_CONFIG_FIELD_APPLY        5U

extern UART_HandleTypeDef huart1;
extern bool usrMoudleInintSuccess;

typedef enum {
    GM800_SOCKET_A_NTRIP = 0x61,
    GM800_SOCKET_B_MQTT = 0x62,
} GM800_Socket;

void USART1_SendBytes(uint8_t *buf, uint16_t len);
void GM800_SdpSend(GM800_Socket socket, const uint8_t *data, uint16_t len);
void MX_USART1_UART_Init(void);
uint8_t usrMoudle_Clear(void);
uint8_t usrMoudle_Init(void);
uint8_t usrMoudle_PrepareForQgcNtrip(void);
void Ntrip_SendRequest(void);
void Ntrip_SendGGA(double lat, double lon, uint64_t utc_sec);
void Ntrip_SetRemoteConfigField(uint8_t field, const uint8_t *data, uint8_t length);
bool Ntrip_TakeRemoteConfigApplyRequest(void);
void USRMoudle_uart_rx_restart(void);
uint8_t *USRMoudle_uart_rx_get_frame(void);
uint16_t USRMoudle_uart_rx_get_frame_len(void);
void USRMoudle_uart_printf(char *fmt, ...);
#endif
