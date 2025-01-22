#ifndef __MAVLINKPARSE_H_
#define __MAVLINKPARSE_H_

//#include "mavlink_avoid_errors.h"
#include <time.h>
#include "mavlink_types.h"
#include "mavlink.h"
#include "mavlink_helpers.h"
#include "stdio.h"
#include "stdint.h"

#pragma anon_unions
#define inline __inline
    

extern fifo_t mavlink_uart_rx_fifo;

extern mavlink_system_t mavlink_system;

void update(void);
void handleMessage(mavlink_message_t msg);
double distance(double lat1, double lon1, double lat2, double lon2);
void timestamp_to_datetime(uint64_t timestamp_ms, char *time_current);
void mavlink_IsArmed(void);
void malvlink_heart_send(void);
void malvlink_serial_num_request_send(void);
void MAVLINK_MSG_ID_HEARTBEAT_ACTION(mavlink_message_t MAVLinkMsg);
void MAVLINK_MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_ACTION(mavlink_message_t MAVLinkMsg);
void MAVLINK_MSG_ID_ATTITUDE_ACTION(mavlink_message_t MAVLinkMsg);
void MAVLINK_MSG_ID_GLOBAL_POSITION_INT_ACTION(mavlink_message_t MAVLinkMsg);
void MAVLINK_MSG_ID_LOCAL_POSITION_NED_ACTION(mavlink_message_t MAVLinkMsg);
void MAVLINK_MSG_ID_VFR_HUD_ACTION(mavlink_message_t MAVLinkMsg);
void MAVLINK_MSG_FLIGHT_NUM_MSG_ACTION(mavlink_message_t MAVLinkMsg);
void MAVLINK_MSG_ID_GPS_RAW_INT_ACTION(mavlink_message_t MAVLinkMsg);
#endif
