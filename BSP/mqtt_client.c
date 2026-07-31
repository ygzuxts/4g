#include "mqtt_client.h"

#include "cJSON.h"
#include "rtcm_parse.h"
#include "track_json.h"
#include "usart_4gmoudle.h"
#include "usart_mavlink.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MQTT_USERNAME "antg!@211520"
#define MQTT_PASSWORD "admin"
#define MQTT_CONNECT_USE_AUTH 1
#define MQTT_KEEPALIVE_SEC 60U
#define MQTT_MAX_PACKET 2048U
#define MQTT_RECONNECT_MS 5000U

extern char sn[20];

static bool mqtt_connected = false;
static uint16_t mqtt_packet_id = 1;
static uint32_t last_ping_ms = 0;
static uint32_t last_connect_ms = 0;
static char mqtt_client_id[32];
static uint8_t rx_cache[MQTT_MAX_PACKET];
static uint16_t rx_cache_len = 0;

static uint16_t put_u16(uint8_t *buf, uint16_t value)
{
    buf[0] = (uint8_t)(value >> 8);
    buf[1] = (uint8_t)(value & 0xFF);
    return 2;
}

static uint16_t put_string(uint8_t *buf, const char *str)
{
    uint16_t len = (uint16_t)strlen(str);
    put_u16(buf, len);
    memcpy(&buf[2], str, len);
    return (uint16_t)(len + 2U);
}

static uint16_t put_remaining_length(uint8_t *buf, uint32_t value)
{
    uint16_t used = 0;
    do
    {
        uint8_t encoded = (uint8_t)(value % 128U);
        value /= 128U;
        if (value > 0U)
        {
            encoded |= 0x80U;
        }
        buf[used++] = encoded;
    } while (value > 0U && used < 4U);
    return used;
}

static void mqtt_send_packet(const uint8_t *packet, uint16_t len)
{
    GM800_SdpSend(GM800_SOCKET_B_MQTT, packet, len);
}

static void mqtt_send_connect(const char *client_id)
{
    uint8_t packet[MQTT_MAX_PACKET];
    uint8_t vh_payload[MQTT_MAX_PACKET];
    uint16_t vp = 0;
    uint16_t p = 0;
    uint16_t remaining_len;

    vp += put_string(&vh_payload[vp], "MQTT");
    vh_payload[vp++] = 4;       /* MQTT 3.1.1 */
    vh_payload[vp++] = MQTT_CONNECT_USE_AUTH ? 0xC2 : 0x02; /* clean session, optional auth */
    vp += put_u16(&vh_payload[vp], MQTT_KEEPALIVE_SEC);
    vp += put_string(&vh_payload[vp], client_id);
#if MQTT_CONNECT_USE_AUTH
    vp += put_string(&vh_payload[vp], MQTT_USERNAME);
    vp += put_string(&vh_payload[vp], MQTT_PASSWORD);
#endif

    remaining_len = vp;
    packet[p++] = 0x10;
    p += put_remaining_length(&packet[p], remaining_len);
    memcpy(&packet[p], vh_payload, vp);
    p += vp;
    char debug[96];
    snprintf(debug,
             sizeof(debug),
             "MQTT: CONNECT len=%u client=%s auth=%u\r\n",
             (unsigned int)p,
             client_id,
             (unsigned int)MQTT_CONNECT_USE_AUTH);
    printf("%s", debug);
    USART2_SendDebugText(debug);
    mqtt_send_packet(packet, p);
}

static void mqtt_send_subscribe_one(const char *topic)
{
    uint8_t packet[MQTT_MAX_PACKET];
    uint8_t body[MQTT_MAX_PACKET];
    uint16_t b = 0;
    uint16_t p = 0;

    b += put_u16(&body[b], mqtt_packet_id++);
    b += put_string(&body[b], topic);
    body[b++] = 0; /* QoS0 */

    packet[p++] = 0x82;
    p += put_remaining_length(&packet[p], b);
    memcpy(&packet[p], body, b);
    p += b;
    mqtt_send_packet(packet, p);
}

void MqttClient_Publish(const char *topic, const uint8_t *payload, uint16_t payload_len)
{
    uint8_t packet[MQTT_MAX_PACKET];
    uint16_t topic_len = (uint16_t)strlen(topic);
    uint32_t remaining_len = 2U + topic_len + payload_len;
    uint16_t p = 0;
    char debug[128];

    snprintf(debug,
             sizeof(debug),
             "MQTT: PUBLISH request connected=%u topic=%s payload_len=%u remaining=%lu\r\n",
             mqtt_connected ? 1U : 0U,
             topic,
             (unsigned int)payload_len,
             (unsigned long)remaining_len);
    printf("%s", debug);
    USART2_SendDebugText(debug);

    if (!mqtt_connected || remaining_len + 5U > MQTT_MAX_PACKET)
    {
        printf("MQTT: publish skipped, not connected or packet too large\r\n");
        USART2_SendDebugText("MQTT: publish skipped, not connected or packet too large\r\n");
        return;
    }

    packet[p++] = 0x30; /* QoS0 PUBLISH */
    p += put_remaining_length(&packet[p], remaining_len);
    p += put_string(&packet[p], topic);
    memcpy(&packet[p], payload, payload_len);
    p += payload_len;
    snprintf(debug,
             sizeof(debug),
             "MQTT: PUBLISH send topic=%s packet_len=%u\r\n",
             topic,
             (unsigned int)p);
    printf("%s", debug);
    USART2_SendDebugText(debug);
    mqtt_send_packet(packet, p);
}

void MqttClient_Start(const char *client_id)
{
    if (client_id == NULL || client_id[0] == '\0')
    {
        snprintf(mqtt_client_id, sizeof(mqtt_client_id), "4GCatModule");
    }
    else
    {
        snprintf(mqtt_client_id, sizeof(mqtt_client_id), "%s", client_id);
    }

    mqtt_connected = false;
    rx_cache_len = 0;
    last_ping_ms = 0;
    last_connect_ms = 0;
    mqtt_packet_id = 1;
    mqtt_send_connect(mqtt_client_id);
    USART2_ReportInfo("MQTT: CONNECT sent over Socket B");
}

void MqttClient_Poll(uint32_t now_ms)
{
    static const uint8_t pingreq[] = {0xC0, 0x00};

    if (!mqtt_connected && (now_ms - last_connect_ms) >= MQTT_RECONNECT_MS)
    {
        last_connect_ms = now_ms;
        USART2_SendDebugText("MQTT: reconnect CONNECT over Socket B\r\n");
        printf("MQTT: reconnect CONNECT over Socket B\r\n");
        mqtt_send_connect(mqtt_client_id);
        return;
    }

    if (mqtt_connected && (now_ms - last_ping_ms) >= (MQTT_KEEPALIVE_SEC * 500U))
    {
        last_ping_ms = now_ms;
        mqtt_send_packet(pingreq, sizeof(pingreq));
    }
}

static bool read_remaining_length(const uint8_t *buf, uint16_t len, uint32_t *value, uint16_t *used)
{
    uint32_t multiplier = 1;
    uint32_t result = 0;

    for (uint16_t i = 0; i < len && i < 4U; i++)
    {
        result += (uint32_t)(buf[i] & 0x7FU) * multiplier;
        if ((buf[i] & 0x80U) == 0U)
        {
            *value = result;
            *used = (uint16_t)(i + 1U);
            return true;
        }
        multiplier *= 128U;
    }
    return false;
}

static void handle_publish(const uint8_t *body, uint16_t len)
{
    char topic[96];
    uint16_t topic_len;
    const uint8_t *payload;
    uint16_t payload_len;

    if (len < 2U)
    {
        return;
    }
    topic_len = ((uint16_t)body[0] << 8) | body[1];
    if (topic_len >= sizeof(topic) || len < (uint16_t)(2U + topic_len))
    {
        return;
    }
    memcpy(topic, &body[2], topic_len);
    topic[topic_len] = '\0';
    payload = &body[2U + topic_len];
    payload_len = (uint16_t)(len - 2U - topic_len);

    if (strcmp(topic, "/gps") == 0)
    {
        process_rtcm_data((uint8_t *)payload, payload_len);
    }
    else
    {
        char json_text[512];
        if (payload_len >= sizeof(json_text))
        {
            return;
        }
        memcpy(json_text, payload, payload_len);
        json_text[payload_len] = '\0';
        cJSON *json = cJSON_Parse(json_text);
        if (json != NULL)
        {
            json_prarse(json);
            cJSON_Delete(json);
        }
    }
}

static void handle_packet(const uint8_t *packet, uint16_t len)
{
    uint8_t type;
    uint32_t remaining_len;
    uint16_t rl_used;
    const uint8_t *body;

    if (len < 2U || !read_remaining_length(&packet[1], (uint16_t)(len - 1U), &remaining_len, &rl_used))
    {
        return;
    }

    type = packet[0] & 0xF0U;
    body = &packet[1U + rl_used];

    char debug[96];
    snprintf(debug,
             sizeof(debug),
             "MQTT: RX packet type=%02X remaining=%lu len=%u\r\n",
             (unsigned int)type,
             (unsigned long)remaining_len,
             (unsigned int)len);
    USART2_SendDebugText(debug);

    if (type == 0x20U && remaining_len >= 2U && body[1] == 0U)
    {
        mqtt_connected = true;
        last_ping_ms = 0;
        USART2_ReportInfo("MQTT: CONNACK OK");

        char topic[64];
        snprintf(topic, sizeof(topic), "/device/%s/4G", sn);
        mqtt_send_subscribe_one(topic);
        mqtt_send_subscribe_one("/gps");
    }
    else if (type == 0x20U && remaining_len >= 2U)
    {
        snprintf(debug,
                 sizeof(debug),
                 "MQTT: CONNACK failed flags=%02X code=%02X\r\n",
                 (unsigned int)body[0],
                 (unsigned int)body[1]);
        USART2_SendDebugText(debug);
    }
    else if (type == 0x30U)
    {
        handle_publish(body, (uint16_t)remaining_len);
    }
}

uint8_t MqttClient_IsConnected(void)
{
    return mqtt_connected ? 1U : 0U;
}

void MqttClient_Input(const uint8_t *data, uint16_t len)
{
    char debug[80];
    snprintf(debug, sizeof(debug), "MQTT: input len=%u first=%02X\r\n", (unsigned int)len, (len > 0U) ? data[0] : 0U);
    USART2_SendDebugText(debug);

    if (len > (uint16_t)(sizeof(rx_cache) - rx_cache_len))
    {
        rx_cache_len = 0;
        return;
    }
    memcpy(&rx_cache[rx_cache_len], data, len);
    rx_cache_len = (uint16_t)(rx_cache_len + len);

    while (rx_cache_len >= 2U)
    {
        uint32_t remaining_len;
        uint16_t rl_used;
        uint16_t packet_len;

        if (!read_remaining_length(&rx_cache[1], (uint16_t)(rx_cache_len - 1U), &remaining_len, &rl_used))
        {
            return;
        }
        packet_len = (uint16_t)(1U + rl_used + remaining_len);
        if (rx_cache_len < packet_len)
        {
            return;
        }

        handle_packet(rx_cache, packet_len);
        memmove(rx_cache, &rx_cache[packet_len], rx_cache_len - packet_len);
        rx_cache_len = (uint16_t)(rx_cache_len - packet_len);
    }
}
