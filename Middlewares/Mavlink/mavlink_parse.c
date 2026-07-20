#include "mavlink_parse.h"
#include "stdbool.h"
#include "usart_mavlink.h"
#include "stdbool.h"
#include "stdint.h"
#include "time.h"
#include "track_queue.h"
#include "usart_4gmoudle.h"

#define PI 3.14159265358979323846
#define RADIUS 6371 // 地球半径，单位为千米

bool distance_flag = false;
char sn[20] = {0};
extern bool jobid_ready;
bool SendTaskStateFlag = false;
double fdistance = 0;
bool taskflag = false;
bool firsttime = true;
bool attitude_ready = false;
bool gps_ready = false;
char flynum[6]; // 起降次数
uint8_t armed_flag = 1;
uint8_t disarme_flag = 2;
uint8_t base_mode = 0;
extern TrackInfo pTrackInfo;
bool esc_rec_finish = true;
bool escif_rec_finish = true;
volatile uint32_t mavlink_rx_message_count = 0;

// 定义 Unix 时间戳起始时间的年份
#define UNIX_START_YEAR 1970

// 每个月的天数（平年）
const uint8_t days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// 判断是否是闰年
int is_leap_year(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// 计算从 1970 年 1 月 1 日开始的秒数对应的日期和时间
void seconds_to_datetime(uint32_t seconds, uint16_t milliseconds, int *year, int *month, int *day, int *hour, int *minute, int *second)
{
    uint32_t days = seconds / 86400;              // 总天数
    uint32_t remaining_seconds = seconds % 86400; // 除去天数后的秒数

    // 计算小时、分钟、秒
    *hour = remaining_seconds / 3600 + 8;
    remaining_seconds %= 3600;
    *minute = remaining_seconds / 60;
    *second = remaining_seconds % 60;

    // 从 1970 年开始逐年减去天数，直到找到当前年份
    *year = UNIX_START_YEAR;
    while (1)
    {
        uint16_t days_in_year = is_leap_year(*year) ? 366 : 365;
        if (days < days_in_year)
        {
            break;
        }
        days -= days_in_year;
        (*year)++;
    }

    // 计算当前月份和日期
    *month = 0;
    for (int i = 0; i < 12; i++)
    {
        uint8_t dim = days_in_month[i];
        if (i == 1 && is_leap_year(*year))
        { // 闰年二月加一天
            dim++;
        }
        if (days < dim)
        {
            *month = i + 1;
            break;
        }
        days -= dim;
    }
    *day = days + 1; // 天数从 1 开始
}

// 主函数：将毫秒时间戳转换为当前时间
void timestamp_to_datetime(uint64_t timestamp_ms, char *time_current)
{
    uint32_t seconds = timestamp_ms / 1000;      // 秒部分
    uint16_t milliseconds = timestamp_ms % 1000; // 毫秒部分

    int year, month, day, hour, minute, second;

    // 调用解析函数
    seconds_to_datetime(seconds, milliseconds, &year, &month, &day, &hour, &minute, &second);

    // 打印结果
    sprintf(time_current, "%04d-%02d-%02d_%02d_%02d_%02d_%03d",
            year, month, day, hour, minute, second, milliseconds);
}

mavlink_channel_t chan;
void handleMessage(mavlink_message_t msg);

void update(void)
{
    // receive new packets
    mavlink_message_t msg;
    mavlink_status_t status;
    status.packet_rx_drop_count = 0;
    while (serial_available(&mavlink_uart_rx_fifo))
    {
        uint8_t rxdata = serial_read_ch(&mavlink_uart_rx_fifo);
        // 从串口读到的数据中解析出每帧消息并写到结构体msg中
        if (mavlink_parse_char(chan, rxdata, &msg, &status))
        {
            mavlink_rx_message_count++;
            handleMessage(msg); // 结构体成员赋值，有解析任务的回复信息
            break;
        }
    }
}

// 解包函数,可按需要仿照如下格式添加case和对应的decode函数
void handleMessage(mavlink_message_t msg)
{
    // printf("msg->msgid is %d\r\n", msg.msgid);
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_5);
    switch (msg.msgid)
    {
    case MAVLINK_MSG_ID_HEARTBEAT:
				
        MAVLINK_MSG_ID_HEARTBEAT_ACTION(msg);
        break;
		case MAVLINK_MSG_ID_SYSTEM_TIME:
				MAVLINK_MSG_ID_SYSTEM_TIME_ACTION(msg);
				break;
    case MAVLINK_MSG_ID_ATTITUDE:
        MAVLINK_MSG_ID_ATTITUDE_ACTION(msg);
        break;
    case MAVLINK_MSG_ID_LOCAL_POSITION_NED:
        MAVLINK_MSG_ID_LOCAL_POSITION_NED_ACTION(msg);
        break;
        //    case MAVLINK_MSG_ID_VFR_HUD:
        //        MAVLINK_MSG_ID_VFR_HUD_ACTION(msg);
        //        break;
    case MAVLINK_MSG_ID_FLIGHT_NUM_MSG:
        MAVLINK_MSG_FLIGHT_NUM_MSG_ACTION(msg);
        break;
    case MAVLINK_MSG_ID_GPS_RAW_INT:
        MAVLINK_MSG_ID_GPS_RAW_INT_ACTION(msg);
        break;
    case MAVLINK_MSG_ID_TUNNEL:
    {
        mavlink_tunnel_t tunnel;
        mavlink_msg_tunnel_decode(&msg, &tunnel);

        if (tunnel.payload_type == NTRIP_CONFIG_TUNNEL_PAYLOAD_TYPE &&
            (tunnel.target_component == 0 || tunnel.target_component == MAV_COMP_ID_ONBOARD_COMPUTER) &&
            tunnel.payload_length >= 3 &&
            tunnel.payload[0] == NTRIP_CONFIG_PROTOCOL_VERSION)
        {
            uint8_t field = tunnel.payload[1];
            uint8_t field_length = tunnel.payload[2];

            if ((uint16_t)field_length + 3U <= tunnel.payload_length)
            {
                Ntrip_SetRemoteConfigField(field, &tunnel.payload[3], field_length);
            }
        }
        break;
    }
    case MAVLINK_MSG_ID_AUTOPILOT_VERSION:
        MAVLINK_MSG_ID_AUTOPILOT_VERSION_ACTION(msg);
        break;
    case MAVLINK_MSG_ID_ACTUATOR_OUTPUT_STATUS:
        MAVLINK_MSG_ID_ACTUATOR_OUTPUT_STATUS_ACTION(msg);
        break;
    case MAVLINK_MSG_ID_ATTITUDE_QUATERNION:
        MAVLINK_MSG_ID_ATTITUDE_QUATERNION_ACTION(msg);
        break;
    case MAVLINK_MSG_ID_ESC_STATUS:
        MAVLINK_MSG_ID_ESC_STATUS_ACTION(msg);
        break;
    case MAVLINK_MSG_ID_ESC_INFO:
        MAVLINK_MSG_ID_ESC_INFO_ACTION(msg);
        break;
    case MAVLINK_MSG_ID_RC_CHANNELS:
        MAVLINK_MSG_ID_RC_CHANNELS_ACTION(msg);
        break;		
		case MAVLINK_MSG_ID_BATTERY_STATUS:
        MAVLINK_MSG_ID_BATTERY_STATUS_ACTION(msg);
        break;	
		case  MAVLINK_MSG_ID_CURRENT_MODE:
        MAVLINK_MSG_ID_CURRENT_MODE_ACTION(msg);
        break;			
		case  MAVLINK_MSG_ID_MANUAL_CONTROL:
        MAVLINK_MSG_ID_MANUAL_CONTROL_ACTION(msg);
        break;
    default:
        break;
		
    }
}

void malvlink_serial_num_request_send(void)
{
    static uint32_t last_request_tick = 0;
    uint32_t now = HAL_GetTick();
    mavlink_message_t message;
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    uint16_t length;

    if ((now - last_request_tick) < 1000U)
    {
        return;
    }
    last_request_tick = now;

    mavlink_msg_command_long_pack(1,
                                  MAV_COMP_ID_ONBOARD_COMPUTER,
                                  &message,
                                  1,
                                  MAV_COMP_ID_AUTOPILOT1,
                                  MAV_CMD_REQUEST_MESSAGE,
                                  0,
                                  MAVLINK_MSG_ID_AUTOPILOT_VERSION,
                                  0, 0, 0, 0, 0, 0);
    length = mavlink_msg_to_send_buffer(buffer, &message);
    USART2_SendMavlinkBytes(buffer, length);

    mavlink_msg_command_long_pack(1,
                                  MAV_COMP_ID_ONBOARD_COMPUTER,
                                  &message,
                                  1,
                                  MAV_COMP_ID_AUTOPILOT1,
                                  MAV_CMD_REQUEST_MESSAGE,
                                  0,
                                  MAVLINK_MSG_ID_SYSTEM_TIME,
                                  0, 0, 0, 0, 0, 0);
    length = mavlink_msg_to_send_buffer(buffer, &message);
    USART2_SendMavlinkBytes(buffer, length);
}
void malvlink_heart_send(void)
{
    mavlink_message_t message_buf;

    uint8_t heartbeat_buffer[MAVLINK_NUM_NON_PAYLOAD_BYTES + MAVLINK_MSG_ID_HEARTBEAT_LEN];

    uint16_t length = 0;

    mavlink_msg_heartbeat_pack(1, MAV_COMP_ID_ONBOARD_COMPUTER, &message_buf, 1, 2, 3, 4, 5);

    length = mavlink_msg_to_send_buffer(heartbeat_buffer, &message_buf);

    USART2_SendMavlinkBytes(heartbeat_buffer, length);
}

// 解析mavlink 心跳包消息
void MAVLINK_MSG_ID_HEARTBEAT_ACTION(mavlink_message_t MAVLinkMsg)
{
    mavlink_heartbeat_t sys_state;
    mavlink_msg_heartbeat_decode(&MAVLinkMsg, &sys_state);
    base_mode = sys_state.base_mode;
    mavlink_IsArmed();
}
void mavlink_IsArmed(void)
{
    static uint8_t old_base_mode;

    if ((old_base_mode >> 7) == 1)
    {
        if ((base_mode >> 7) == 0)
        {
          taskflag = false;
           
					SendTaskStateFlag = true;
            //printf("disarmed\r\n");
            jobid_ready = 0;
        }
    }
    else if ((old_base_mode >> 7) == 0)
    {
        if ((base_mode >> 7) == 1)
        {
            taskflag = true;
            SendTaskStateFlag = true; // 需要向4G模块发送任务开始
            //printf("armed\r\n");
        }
    }
		
		if((base_mode & (0x01<<7)) == 0){
			//未解锁
			pTrackInfo.armed = 0;
			//未准备起飞
			pTrackInfo.ready_to_fly = 0;
		}else{
			//已解锁
			pTrackInfo.armed = 1;
			//准备好起飞
			pTrackInfo.ready_to_fly = 1;
		}
		
    old_base_mode = base_mode;
}
// 转化获取的sn码为字符串
void uid_to_sn_dec(uint64_t uid, char sn[20])
{
    snprintf(sn, 20, "%llu", (unsigned long long)uid);
		printf("SN: %s.\r\n",sn);
}

// 获取SN码
void MAVLINK_MSG_ID_AUTOPILOT_VERSION_ACTION(mavlink_message_t MAVLinkMsg)
{
		uint64_t sum=0;
		mavlink_autopilot_version_t autopilot_version;
    mavlink_msg_autopilot_version_decode(&MAVLinkMsg, &autopilot_version);
		for(uint8_t i=0;i<18;i++){
			sum += autopilot_version.uid2[i];
			
			//printf("UID2[%d]: %d.\r\n",i,autopilot_version.uid2[i]);
		}
		uid_to_sn_dec(sum,sn);//autopilot_version.uid
		printf("UID: %lld.\r\n",sum);
}
// 解析飞控位姿信息_pitch_yaw_roll
void MAVLINK_MSG_ID_ATTITUDE_ACTION(mavlink_message_t MAVLinkMsg)
{
    mavlink_attitude_t attitude;
    mavlink_msg_attitude_decode(&MAVLinkMsg, &attitude);
    pTrackInfo.pitch = attitude.pitch * 57.295779513f;
    pTrackInfo.roll = attitude.roll * 57.295779513f;
    pTrackInfo.yaw = attitude.yaw * 57.295779513f;
    attitude_ready = true;
}

void MAVLINK_MSG_ID_GLOBAL_POSITION_INT_ACTION(mavlink_message_t MAVLinkMsg)
{
    mavlink_global_position_int_t global_position;

    mavlink_msg_global_position_int_decode(&MAVLinkMsg, &global_position);
}


void MAVLINK_MSG_ID_VFR_HUD_ACTION(mavlink_message_t MAVLinkMsg)
{
    mavlink_vfr_hud_t vfr_hud;
    mavlink_msg_vfr_hud_decode(&MAVLinkMsg, &vfr_hud);
}

// 获取飞控飞行架次
void MAVLINK_MSG_FLIGHT_NUM_MSG_ACTION(mavlink_message_t MAVLinkMsg)
{

    //mavlink_msg_flight_num_msg_get_flight_num(&MAVLinkMsg, flynum);
}

double deg2rad(double deg) // 度转弧度
{
    return deg * (PI / 180);
}

double distance(double lat1, double lon1, double lat2, double lon2) // 计算两点之间距离
{
    double dlat, dlon, a, c, distance;

    // 将经纬度转换为弧度
    lat1 = deg2rad(lat1);
    lon1 = deg2rad(lon1);
    lat2 = deg2rad(lat2);
    lon2 = deg2rad(lon2);

    // 使用Haversine公式计算距离
    dlat = lat2 - lat1;
    dlon = lon2 - lon1;
    // 应用Haversine公式计算距离
    a = pow(sin(dlat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(dlon / 2), 2);
    c = 2 * atan2(sqrt(a), sqrt(1 - a));
    distance = RADIUS * c;

    return distance * 1000;
}

//解析获取GPS原始经纬度数据_lat_lon
void MAVLINK_MSG_ID_GPS_RAW_INT_ACTION(mavlink_message_t MAVLinkMsg)
{
    mavlink_gps_raw_int_t gps_raw_int;
    static double lat_last = 0;
    static double lon_last = 0;
    int32_t lat_temp;
    int32_t lon_temp;
    double dis = 0;
    mavlink_msg_gps_raw_int_decode(&MAVLinkMsg, &gps_raw_int);
    lat_temp = gps_raw_int.lat;
    lon_temp = gps_raw_int.lon;

    pTrackInfo.lat = (double)(lat_temp / 10000000.0f);
    pTrackInfo.lon = (double)(lon_temp / 10000000.0f);
    if (firsttime == true)
    {
        firsttime = false;
        lat_last = pTrackInfo.lat;
        lon_last = pTrackInfo.lon;
    }
    // printf("lat is %.7f  lon is %.7f\r\n", pTrackInfo.lat, pTrackInfo.lon);
    gps_ready = true;
    if (taskflag && distance_flag)
    {
        distance_flag = false;
        dis = distance(lat_last, lon_last, pTrackInfo.lat, pTrackInfo.lon);
        (dis > 1000.0f) ? (dis = 0) : (dis = dis); // 做一个限制，防止解锁时未获得GPS信号
        lat_last = pTrackInfo.lat;
        lon_last = pTrackInfo.lon;
        fdistance = fdistance + dis;
    }
}
//解析获取飞控原始通道控制输出值_rc_roll_yaw_pitch_throttle
void MAVLINK_MSG_ID_ACTUATOR_OUTPUT_STATUS_ACTION(mavlink_message_t MAVLinkMsg)
{
		mavlink_actuator_output_status_t actuator_output_data;
		mavlink_msg_actuator_output_status_decode(&MAVLinkMsg, &actuator_output_data);
		
		for(int i = 0;	i<8;	i++){
			pTrackInfo.vehicle_control[i] = actuator_output_data.actuator[i];
		}	
}

//解析获取飞控姿态四元数_q
void MAVLINK_MSG_ID_ATTITUDE_QUATERNION_ACTION(mavlink_message_t MAVLinkMsg)
{
		mavlink_attitude_quaternion_t attitude_quaternion_data;
		mavlink_msg_attitude_quaternion_decode(&MAVLinkMsg, &attitude_quaternion_data);
		
		pTrackInfo.vehicle_q[0] = attitude_quaternion_data.q1;
		pTrackInfo.vehicle_q[1] = attitude_quaternion_data.q2;
		pTrackInfo.vehicle_q[2] = attitude_quaternion_data.q3;
		pTrackInfo.vehicle_q[3] = attitude_quaternion_data.q4;
		
}
//解析获取飞机高度信息_alt_vx_vy_vz
void MAVLINK_MSG_ID_LOCAL_POSITION_NED_ACTION(mavlink_message_t MAVLinkMsg)
{
    mavlink_local_position_ned_t local_position_ned;
    mavlink_msg_local_position_ned_decode(&MAVLinkMsg, &local_position_ned);

    pTrackInfo.alt = -(double)local_position_ned.z;
	
		pTrackInfo.vehicle_vx = local_position_ned.vx;
		pTrackInfo.vehicle_vy =	local_position_ned.vy;
		pTrackInfo.vehicle_vz = local_position_ned.vz;
		
}

//解析获取电调信息_rpm_voltage_current
void MAVLINK_MSG_ID_ESC_STATUS_ACTION(mavlink_message_t MAVLinkMsg)
{
    mavlink_esc_status_t esc_status_data;
    mavlink_msg_esc_status_decode(&MAVLinkMsg, &esc_status_data);
		
		if(esc_status_data.index == 0 && esc_rec_finish){
				for(int i = 0 ; i < 4 ; i++ ){
						pTrackInfo.esc_rpm[i] = esc_status_data.rpm[i];
						pTrackInfo.esc_voltage[i] = esc_status_data.voltage[i];
						pTrackInfo.esc_current[i] = esc_status_data.current[i];
				}
				esc_rec_finish = false;
		}
		else if(esc_status_data.index == 4 && !esc_rec_finish){
				for(int i = 4 ; i < 8 ; i++ ){
						pTrackInfo.esc_rpm[i] = esc_status_data.rpm[i-4];
						pTrackInfo.esc_voltage[i] = esc_status_data.voltage[i-4];
						pTrackInfo.esc_current[i] = esc_status_data.current[i-4];
				}
				esc_rec_finish = true;
		}

}

//解析获取电调信息_temperature_errorcnt
void MAVLINK_MSG_ID_ESC_INFO_ACTION(mavlink_message_t MAVLinkMsg)
{
    mavlink_esc_info_t esc_info_data;
    mavlink_msg_esc_info_decode(&MAVLinkMsg, &esc_info_data);
		
		if(esc_info_data.index == 0 && escif_rec_finish){
				for(int i = 0 ; i < 4 ; i++ ){
					pTrackInfo.esc_temple[i] = esc_info_data.temperature[i] / 100.f;
					pTrackInfo.esc_errorcount[i] = esc_info_data.error_count[i];
				}
				escif_rec_finish = false;
		}
		else if(esc_info_data.index == 4 && !escif_rec_finish){
				for(int i = 4 ; i < 8 ; i++ ){
					pTrackInfo.esc_temple[i] = esc_info_data.temperature[i-4] / 100.f;
					pTrackInfo.esc_errorcount[i] = esc_info_data.error_count[i-4];
				}
				escif_rec_finish = true;
		}
}	

//解析获取飞控输入信息_rc_roll_pitch_yaw_throttle
void MAVLINK_MSG_ID_RC_CHANNELS_ACTION(mavlink_message_t MAVLinkMsg)
{
    mavlink_rc_channels_t rc_channels_data;
    mavlink_msg_rc_channels_decode(&MAVLinkMsg, &rc_channels_data);
	
		pTrackInfo.rc_roll = rc_channels_data.chan10_raw;
		pTrackInfo.rc_throttle = rc_channels_data.chan11_raw;
		pTrackInfo.rc_pitch = rc_channels_data.chan12_raw;
		pTrackInfo.rc_yaw = rc_channels_data.chan13_raw;
}

//解析获取电池状态信息_battery
void MAVLINK_MSG_ID_BATTERY_STATUS_ACTION(mavlink_message_t MAVLinkMsg)
{
		mavlink_battery_status_t battery_status_data;
    mavlink_msg_battery_status_decode(&MAVLinkMsg, &battery_status_data);
		//float temp_voltage_v = (float)(battery_status_data.voltages[1]/1000) + (float)((battery_status_data.voltages[1]/100)%10);
		pTrackInfo.voltage_v = (battery_status_data.voltages[1]+65534) /1000.f;
		pTrackInfo.current_a = battery_status_data.current_battery / 100.f;
		pTrackInfo.remaining = battery_status_data.battery_remaining ;
		pTrackInfo.time_remaining_s = battery_status_data.time_remaining;
		pTrackInfo.temperature = battery_status_data.temperature / 100.f;
	
}

//解析获取的请求执行导航模式命令
void MAVLINK_MSG_ID_MANUAL_CONTROL_ACTION(mavlink_message_t MAVLinkMsg)
{
	mavlink_manual_control_t manual_control_data;
	mavlink_msg_manual_control_decode(&MAVLinkMsg, &manual_control_data);
	pTrackInfo.nav_state_requset = manual_control_data.buttons;
}

//解析获取的真实执行导航模式命令
void MAVLINK_MSG_ID_CURRENT_MODE_ACTION(mavlink_message_t MAVLinkMsg)
{
	mavlink_current_mode_t current_mode_data;
	mavlink_msg_current_mode_decode(&MAVLinkMsg, &current_mode_data);
	pTrackInfo.nav_state_current = current_mode_data.custom_mode;
}

//解析获取飞控发送的utc时间
void MAVLINK_MSG_ID_SYSTEM_TIME_ACTION(mavlink_message_t MAVLinkMsg)
{
	mavlink_system_time_t system_time_data;
	mavlink_msg_system_time_decode(&MAVLinkMsg, &system_time_data);
	pTrackInfo.utc_sec = system_time_data.time_unix_usec/1e6;
}
