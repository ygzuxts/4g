#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include "stdint.h"

void MqttClient_Start(const char *client_id);
void MqttClient_Poll(uint32_t now_ms);
void MqttClient_Input(const uint8_t *data, uint16_t len);
void MqttClient_Publish(const char *topic, const uint8_t *payload, uint16_t payload_len);
uint8_t MqttClient_IsConnected(void);

#endif
