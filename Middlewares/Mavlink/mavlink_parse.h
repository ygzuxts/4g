#ifndef __MAVLINKPARSE_H_
#define __MAVLINKPARSE_H_

//#include "mavlink_avoid_errors.h"
#include <time.h>
#include "mavlink_types.h"
#include "mavlink.h"
#include "mavlink_helpers.h"
#include "stdio.h"
#include "stdint.h"
#include "mavlink_usart_fifo.h"
#include "mavlink_msg_flight_num_msg.h"

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



void MAVLINK_MSG_ID_ATTITUDE_ACTION(mavlink_message_t MAVLinkMsg);
void MAVLINK_MSG_ID_GLOBAL_POSITION_INT_ACTION(mavlink_message_t MAVLinkMsg);
void MAVLINK_MSG_ID_VFR_HUD_ACTION(mavlink_message_t MAVLinkMsg);
void MAVLINK_MSG_FLIGHT_NUM_MSG_ACTION(mavlink_message_t MAVLinkMsg);
void MAVLINK_MSG_ID_GPS_RAW_INT_ACTION(mavlink_message_t MAVLinkMsg);

void uid_to_sn_dec(uint64_t uid, char sn[20]);																			//encode vid 2 sn 
void MAVLINK_MSG_ID_AUTOPILOT_VERSION_ACTION(mavlink_message_t MAVLinkMsg);					//sn get 
void MAVLINK_MSG_ID_HEARTBEAT_ACTION(mavlink_message_t MAVLinkMsg);									//had 
void MAVLINK_MSG_ID_ACTUATOR_OUTPUT_STATUS_ACTION(mavlink_message_t MAVLinkMsg);		//had
void MAVLINK_MSG_ID_ATTITUDE_QUATERNION_ACTION(mavlink_message_t MAVLinkMsg);				//had 
void MAVLINK_MSG_ID_GPS_GLOBAL_ORIGIN_ACTION(mavlink_message_t MAVLinkMsg);					//not use
void MAVLINK_MSG_ID_LOCAL_POSITION_NED_ACTION(mavlink_message_t MAVLinkMsg);				//had 
void MAVLINK_MSG_ID_ESC_STATUS_ACTION(mavlink_message_t MAVLinkMsg);								//had 
void MAVLINK_MSG_ID_ESC_INFO_ACTION(mavlink_message_t MAVLinkMsg);									//had
void MAVLINK_MSG_ID_RC_CHANNELS_ACTION(mavlink_message_t MAVLinkMsg);								//had 
void MAVLINK_MSG_ID_BATTERY_STATUS_ACTION(mavlink_message_t MAVLinkMsg);						//had half
void MAVLINK_MSG_ID_MANUAL_CONTROL_ACTION(mavlink_message_t MAVLinkMsg);						//had
void MAVLINK_MSG_ID_CURRENT_MODE_ACTION(mavlink_message_t MAVLinkMsg);							//had 
void MAVLINK_MSG_ID_SYSTEM_TIME_ACTION(mavlink_message_t MAVLinkMsg);

//still need nav status mavlink add 

#endif
