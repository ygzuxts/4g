#include "usart_4gmoudle.h"
#include "usart_mavlink.h"
#include <stdio.h>

void GM800_SdpSend(GM800_Socket socket, const uint8_t *data, uint16_t len)
{
    uint8_t header[7];
    uint16_t payload_len = (uint16_t)(len + 2U);
    uint8_t checksum;

    header[0] = 0x55;
    header[1] = 0xFD;
    header[2] = 0xAA;
    header[3] = (uint8_t)(payload_len >> 8);
    header[4] = (uint8_t)(payload_len & 0xFF);
    header[5] = (uint8_t)socket;
    header[6] = 0x00;

    checksum = (uint8_t)(header[5] + header[6]);
    for (uint16_t i = 0; i < len; i++)
    {
        checksum = (uint8_t)(checksum + data[i]);
    }

#if (USR_MODULE_WORK_MODE == USR_MODULE_MODE_DUAL_TCP)
    char debug[96];
    snprintf(debug,
             sizeof(debug),
             "DBG: gm800 sdp tx socket=%02X data_len=%u payload_len=%u checksum=%02X\r\n",
             (unsigned int)socket,
             (unsigned int)len,
             (unsigned int)payload_len,
             (unsigned int)checksum);
    USART2_SendDebugText(debug);
#endif

    USART1_SendBytes(header, sizeof(header));
    if (len > 0)
    {
        USART1_SendBytes((uint8_t *)data, len);
    }
    USART1_SendBytes(&checksum, 1);
}
