#include "track_json.h"
#include "cjson.h"
#include "stdlib.h"
#include "usart_4gmoudle.h"
#include "stdint.h"
#include "stdbool.h"
#include "track_queue.h"
#include "cmsis_os.h"
#include "math.h"
#include "bsp_led.h"
#include "bsp_Flash.h"
extern uint64_t getTaskExecutionCnt(void);

extern char sn[20];
extern char flynum[6]; // 起降次数
extern bool taskflag;
extern double fdistance;
extern bool SendTaskStateFlag;
extern LinkedList pTrackList;
extern TrackInfo pTrackInfo;

bool tag_flag = true;     // 是否收到云网回复
bool jobid_ready = false; // 是否从云网获取到任务ID
char token[10] = "0";
char cv[10] = "0";      // 固件版本
char fv[10] = "0";      // 飞控版本
uint8_t client = 0;     // 0:地面站
uint64_t timestamp = 0; // 云网发送的时间戳
uint32_t jobId = 0;     // 任务序号
uint32_t trackNum = 0;  // 发送的航点序号
uint32_t duration = 0;  // 持续时间
uint32_t id = 777;//0;        // 飞机固定ID，从云网获取

// 向云网发送数据
void sendData(cJSON *cjson, char *type)
{
    uint8_t sendNum = 0;
    char *jsonString = cJSON_Print(cjson);

    char *buffer = pvPortMalloc(strlen("1,") + strlen(jsonString) + 5);

    if (buffer == NULL)
    {
        printf("Memory allocation failed\n");
        return;
    }

    if (strcmp(type, "/Job/Ready") == 0)
    {
        strcpy(buffer, "1,");
    }
    else
    {
        strcpy(buffer, "2,");
    }
    strcat(buffer, jsonString);

    if (strcmp(type, "/Job/Track") == 0)
    {
//        while (tag_flag)
//        {
            printf("send track\r\n");
            USART1_SendBytes((uint8_t *)buffer, strlen(buffer) + 1);
//            if (sendNum++ >= 1) // 防止和RTCM报文冲突
//            {
//                sendNum = 0;
//                break;
//            }
            SetLEDState(1,3);
//            osDelay(1000);
//        }
//        tag_flag = 1;
    }
    else
    {
        USART1_SendBytes((uint8_t *)buffer, strlen(buffer) + 1);
    }
    cJSON_Delete(cjson);
    vPortFree(jsonString);
    vPortFree(buffer);
}

void finishTask(void)
{
    cJSON *cjson = NULL;
    printf("fdistance is %.2f", fdistance);
    cjson = cJSON_CreateObject();
    cJSON_AddStringToObject(cjson, "token", token);
    cJSON_AddStringToObject(cjson, "type", "/Job/Finish");
    cJSON_AddStringToObject(cjson, "Flight_ID", sn);
    cJSON_AddNumberToObject(cjson, "job", pTrackInfo.day_job_id);
    cJSON_AddNumberToObject(cjson, "ft", getTaskExecutionCnt());
    cJSON_AddNumberToObject(cjson, "fd", fdistance); // 飞行的总距离
    cJSON_AddStringToObject(cjson, "source", "4G");
    cJSON_AddStringToObject(cjson, "flyTtime", "0");

    sendData(cjson, "/Job/Finish");
}

// 拼接向云网发送的航点
void uploadTrack(void)
{
    cJSON *cjson_test = NULL;
    cJSON *cjson_track = NULL;

    TrackInfo dataTest;

    cjson_test = cJSON_CreateObject();
    cjson_track = cJSON_CreateArray();

    cJSON_AddStringToObject(cjson_test, "token", token);
    cJSON_AddStringToObject(cjson_test, "type", "/Job/Track");
    cJSON_AddStringToObject(cjson_test, "Flight_ID", sn);
    cJSON_AddStringToObject(cjson_test, "source", "4G");

    cJSON_AddNumberToObject(cjson_test, "job", jobId);
    cJSON_AddNumberToObject(cjson_test, "i", trackNum++);
    for (uint8_t i = 0; i < 5; i++)
    {
        dataTest = getFirstData(&pTrackList);

        cJSON *data = cJSON_CreateObject();
        cJSON_AddNumberToObject(data, "p", round(dataTest.pitch * pow(10, 2)) / pow(10, 1));
        cJSON_AddNumberToObject(data, "a", round(dataTest.lat * pow(10, 7)) / pow(10, 7));
        cJSON_AddNumberToObject(data, "r", round(dataTest.roll * pow(10, 2)) / pow(10, 1));
        cJSON_AddNumberToObject(data, "h", round(dataTest.alt * pow(10, 2)) / pow(10, 1));
        cJSON_AddNumberToObject(data, "y", round(dataTest.yaw * pow(10, 2)) / pow(10, 1));
        cJSON_AddNumberToObject(data, "l", round(dataTest.l * pow(10, 2)) / pow(10, 1));
        cJSON_AddNumberToObject(data, "n", round(dataTest.lon * pow(10, 7)) / pow(10, 7));
        cJSON_AddItemToArray(cjson_track, data);

        popFromLinkedList(&pTrackList);
    }
    cJSON_AddItemToObject(cjson_test, "track", cjson_track);

    sendData(cjson_test, "/Job/Track");
}

void normal_status_Action(void)
{
		cJSON *cjson_test = NULL;
    cJSON *cjson_normal_vehicle_status = NULL;
		cJSON *cjson_normal_drone_status = NULL;
		cJSON *cjson_normal_vehicle_control = NULL;
		cJSON *cjson_normal_vehicle_Q = NULL;
	
		cJSON *cjson_normal_esc_status = NULL;
		cJSON *cjson_normal_e1_status = NULL;
		cJSON *cjson_normal_e2_status = NULL;
		cJSON *cjson_normal_e3_status = NULL;
		cJSON *cjson_normal_e4_status = NULL;
		cJSON *cjson_normal_e5_status = NULL;
		cJSON *cjson_normal_e6_status = NULL;
		cJSON *cjson_normal_e7_status = NULL;
		cJSON *cjson_normal_e8_status = NULL;
	
		cJSON *cjson_normal_rc_status = NULL;
	
		cJSON *cjson_normal_battery_data = NULL;
	
    cjson_test = cJSON_CreateObject();
    cjson_normal_vehicle_status = cJSON_CreateObject();
		cjson_normal_drone_status = cJSON_CreateObject();
		cjson_normal_vehicle_control = cJSON_CreateObject();
		cjson_normal_vehicle_Q = cJSON_CreateObject();
		
		cjson_normal_esc_status = cJSON_CreateObject();
		cjson_normal_e1_status = cJSON_CreateObject();
		cjson_normal_e2_status = cJSON_CreateObject();
		cjson_normal_e3_status = cJSON_CreateObject();
		cjson_normal_e4_status = cJSON_CreateObject();
		cjson_normal_e5_status = cJSON_CreateObject();
		cjson_normal_e6_status = cJSON_CreateObject();
		cjson_normal_e7_status = cJSON_CreateObject();
		cjson_normal_e8_status = cJSON_CreateObject();
	
		cjson_normal_rc_status = cJSON_CreateObject();
		
		cjson_normal_battery_data = cJSON_CreateObject();
	
		//飞控身份类信息
    cJSON_AddStringToObject(cjson_test, "token", token);
    cJSON_AddStringToObject(cjson_test, "type", "/Job/Track");
    cJSON_AddStringToObject(cjson_test, "Flight_ID", sn);
    cJSON_AddStringToObject(cjson_test, "source", "4G");
		cJSON_AddStringToObject(cjson_test, "fly_num", flynum);
    cJSON_AddNumberToObject(cjson_test, "job", pTrackInfo.day_job_id);//判断飞控是否重启的标志位
    cJSON_AddNumberToObject(cjson_test, "i", trackNum++);
		//飞控状态信息
		cJSON_AddBoolToObject(cjson_normal_drone_status, "armed", pTrackInfo.armed);
		cJSON_AddBoolToObject(cjson_normal_drone_status, "ready_to_arm", pTrackInfo.ready_to_fly);
		cJSON_AddNumberToObject(cjson_normal_vehicle_control, "M1", pTrackInfo.vehicle_control[0]);
		cJSON_AddNumberToObject(cjson_normal_vehicle_control, "M2", pTrackInfo.vehicle_control[1]);
		cJSON_AddNumberToObject(cjson_normal_vehicle_control, "M3", pTrackInfo.vehicle_control[2]);
		cJSON_AddNumberToObject(cjson_normal_vehicle_control, "M4", pTrackInfo.vehicle_control[3]);
		cJSON_AddNumberToObject(cjson_normal_vehicle_control, "M5", pTrackInfo.vehicle_control[4]);
		cJSON_AddNumberToObject(cjson_normal_vehicle_control, "M6", pTrackInfo.vehicle_control[5]);
		cJSON_AddNumberToObject(cjson_normal_vehicle_control, "M7", pTrackInfo.vehicle_control[6]);
		cJSON_AddNumberToObject(cjson_normal_vehicle_control, "M8", pTrackInfo.vehicle_control[7]);
		cJSON_AddItemToObject(cjson_normal_drone_status, "control", cjson_normal_vehicle_control);
		cJSON_AddNumberToObject(cjson_normal_vehicle_Q, "q1", pTrackInfo.vehicle_q[0]);
		cJSON_AddNumberToObject(cjson_normal_vehicle_Q, "q2", pTrackInfo.vehicle_q[1]);
		cJSON_AddNumberToObject(cjson_normal_vehicle_Q, "q3", pTrackInfo.vehicle_q[2]);
		cJSON_AddNumberToObject(cjson_normal_vehicle_Q, "q4", pTrackInfo.vehicle_q[3]);
		cJSON_AddItemToObject(cjson_normal_drone_status, "Q", cjson_normal_vehicle_Q);
		cJSON_AddNumberToObject(cjson_normal_drone_status, "lat", pTrackInfo.lat);
		cJSON_AddNumberToObject(cjson_normal_drone_status, "lon", pTrackInfo.lon);
		cJSON_AddNumberToObject(cjson_normal_drone_status, "alt", pTrackInfo.alt);
		cJSON_AddNumberToObject(cjson_normal_drone_status, "vx", pTrackInfo.vehicle_vx);
		cJSON_AddNumberToObject(cjson_normal_drone_status, "vy", pTrackInfo.vehicle_vy);
		cJSON_AddNumberToObject(cjson_normal_drone_status, "vz", pTrackInfo.vehicle_vz);
		cJSON_AddItemToObject(cjson_normal_vehicle_status, "vehicle_status", cjson_normal_drone_status);
		
		//电调状态信息
		//E1
		cJSON_AddNumberToObject(cjson_normal_e1_status, "rpm", pTrackInfo.esc_rpm[0]);
		cJSON_AddNumberToObject(cjson_normal_e1_status, "voltage", pTrackInfo.esc_voltage[0]);
		cJSON_AddNumberToObject(cjson_normal_e1_status, "current", pTrackInfo.esc_current[0]);
		cJSON_AddNumberToObject(cjson_normal_e1_status, "temperature", pTrackInfo.esc_temple[0]);
		cJSON_AddNumberToObject(cjson_normal_e1_status, "errcnt", pTrackInfo.esc_errorcount[0]);
		cJSON_AddItemToObject(cjson_normal_esc_status, "E1", cjson_normal_e1_status);
		//E2
		cJSON_AddNumberToObject(cjson_normal_e2_status, "rpm", pTrackInfo.esc_rpm[1]);
		cJSON_AddNumberToObject(cjson_normal_e2_status, "voltage", pTrackInfo.esc_voltage[1]);
		cJSON_AddNumberToObject(cjson_normal_e2_status, "current", pTrackInfo.esc_current[1]);
		cJSON_AddNumberToObject(cjson_normal_e2_status, "temperature", pTrackInfo.esc_temple[1]);
		cJSON_AddNumberToObject(cjson_normal_e2_status, "errcnt", pTrackInfo.esc_errorcount[1]);
		cJSON_AddItemToObject(cjson_normal_esc_status, "E2", cjson_normal_e2_status);
		//E3
		cJSON_AddNumberToObject(cjson_normal_e3_status, "rpm", pTrackInfo.esc_rpm[2]);
		cJSON_AddNumberToObject(cjson_normal_e3_status, "voltage", pTrackInfo.esc_voltage[2]);
		cJSON_AddNumberToObject(cjson_normal_e3_status, "current", pTrackInfo.esc_current[2]);
		cJSON_AddNumberToObject(cjson_normal_e3_status, "temperature", pTrackInfo.esc_temple[2]);
		cJSON_AddNumberToObject(cjson_normal_e3_status, "errcnt", pTrackInfo.esc_errorcount[2]);
		cJSON_AddItemToObject(cjson_normal_esc_status, "E3", cjson_normal_e3_status);
		//E4
		cJSON_AddNumberToObject(cjson_normal_e4_status, "rpm", pTrackInfo.esc_rpm[3]);
		cJSON_AddNumberToObject(cjson_normal_e4_status, "voltage", pTrackInfo.esc_voltage[3]);
		cJSON_AddNumberToObject(cjson_normal_e4_status, "current", pTrackInfo.esc_current[3]);
		cJSON_AddNumberToObject(cjson_normal_e4_status, "temperature", pTrackInfo.esc_temple[3]);
		cJSON_AddNumberToObject(cjson_normal_e4_status, "errcnt", pTrackInfo.esc_errorcount[3]);
		cJSON_AddItemToObject(cjson_normal_esc_status, "E4", cjson_normal_e4_status);
		//E5
		cJSON_AddNumberToObject(cjson_normal_e5_status, "rpm", pTrackInfo.esc_rpm[4]);
		cJSON_AddNumberToObject(cjson_normal_e5_status, "voltage", pTrackInfo.esc_voltage[4]);
		cJSON_AddNumberToObject(cjson_normal_e5_status, "current", pTrackInfo.esc_current[4]);
		cJSON_AddNumberToObject(cjson_normal_e5_status, "temperature", pTrackInfo.esc_temple[4]);
		cJSON_AddNumberToObject(cjson_normal_e5_status, "errcnt", pTrackInfo.esc_errorcount[4]);
		cJSON_AddItemToObject(cjson_normal_esc_status, "E5", cjson_normal_e5_status);
		//E6
		cJSON_AddNumberToObject(cjson_normal_e6_status, "rpm", pTrackInfo.esc_rpm[5]);
		cJSON_AddNumberToObject(cjson_normal_e6_status, "voltage", pTrackInfo.esc_voltage[5]);
		cJSON_AddNumberToObject(cjson_normal_e6_status, "current", pTrackInfo.esc_current[5]);
		cJSON_AddNumberToObject(cjson_normal_e6_status, "temperature", pTrackInfo.esc_temple[5]);
		cJSON_AddNumberToObject(cjson_normal_e6_status, "errcnt", pTrackInfo.esc_errorcount[5]);
		cJSON_AddItemToObject(cjson_normal_esc_status, "E6", cjson_normal_e6_status);
		//E7
		cJSON_AddNumberToObject(cjson_normal_e7_status, "rpm", pTrackInfo.esc_rpm[6]);
		cJSON_AddNumberToObject(cjson_normal_e7_status, "voltage", pTrackInfo.esc_voltage[6]);
		cJSON_AddNumberToObject(cjson_normal_e7_status, "current", pTrackInfo.esc_current[6]);
		cJSON_AddNumberToObject(cjson_normal_e7_status, "temperature", pTrackInfo.esc_temple[6]);
		cJSON_AddNumberToObject(cjson_normal_e7_status, "errcnt", pTrackInfo.esc_errorcount[6]);
		cJSON_AddItemToObject(cjson_normal_esc_status, "E7", cjson_normal_e7_status);
		//E8
		cJSON_AddNumberToObject(cjson_normal_e8_status, "rpm", pTrackInfo.esc_rpm[7]);
		cJSON_AddNumberToObject(cjson_normal_e8_status, "voltage", pTrackInfo.esc_voltage[7]);
		cJSON_AddNumberToObject(cjson_normal_e8_status, "current", pTrackInfo.esc_current[7]);
		cJSON_AddNumberToObject(cjson_normal_e8_status, "temperature", pTrackInfo.esc_temple[7]);
		cJSON_AddNumberToObject(cjson_normal_e8_status, "errcnt", pTrackInfo.esc_errorcount[7]);
		cJSON_AddItemToObject(cjson_normal_esc_status, "E8", cjson_normal_e8_status);
		cJSON_AddItemToObject(cjson_normal_vehicle_status, "esc", cjson_normal_esc_status);
		//飞控遥控信息
		cJSON_AddNumberToObject(cjson_normal_rc_status, "Roll", pTrackInfo.rc_roll );
		cJSON_AddNumberToObject(cjson_normal_rc_status, "Pitch",pTrackInfo.rc_pitch );
		cJSON_AddNumberToObject(cjson_normal_rc_status, "Yaw", pTrackInfo.rc_yaw );
		cJSON_AddNumberToObject(cjson_normal_rc_status, "Throttle", pTrackInfo.rc_throttle);
		cJSON_AddNumberToObject(cjson_normal_rc_status, "ModeExp", pTrackInfo.nav_state_requset);
		cJSON_AddNumberToObject(cjson_normal_rc_status, "ModeNow", pTrackInfo.nav_state_current);
		cJSON_AddItemToObject(cjson_normal_vehicle_status, "input", cjson_normal_rc_status);
		
		//电池状态信息
		cJSON_AddNumberToObject(cjson_normal_battery_data, "VoltageV", pTrackInfo.voltage_v);
		cJSON_AddNumberToObject(cjson_normal_battery_data, "CurrentA", pTrackInfo.current_a );
		cJSON_AddNumberToObject(cjson_normal_battery_data, "RemainingMah", pTrackInfo.remaining );
		cJSON_AddNumberToObject(cjson_normal_battery_data, "TimeRemainingS", pTrackInfo.time_remaining_s);
		cJSON_AddNumberToObject(cjson_normal_battery_data, "Temperature", pTrackInfo.temperature );
		cJSON_AddItemToObject(cjson_normal_vehicle_status, "battery_status", cjson_normal_battery_data);
		
		cJSON_AddItemToObject(cjson_test, "normal_status", cjson_normal_vehicle_status);
		
		//printf("rev q1:%f,	q2:%f,	q3:%f,	q4:%f\r\n",pTrackInfo.vehicle_q[0],pTrackInfo.vehicle_q[1],pTrackInfo.vehicle_q[2],pTrackInfo.vehicle_q[3]);
		
		sendData(cjson_test, "/Job/Track");
}

void readyTask(void)
{
    cJSON *cjson = NULL;

    cjson = cJSON_CreateObject();

    cJSON_AddStringToObject(cjson, "token", token);
    cJSON_AddStringToObject(cjson, "type", "/Job/Ready");
    cJSON_AddStringToObject(cjson, "Flight_ID", sn);
    cJSON_AddNumberToObject(cjson, "duration", duration);
    cJSON_AddNumberToObject(cjson, "client", client);
    cJSON_AddStringToObject(cjson, "cv", cv);
    cJSON_AddStringToObject(cjson, "fv", fv);
    cJSON_AddStringToObject(cjson, "flynum", flynum);
    cJSON_AddStringToObject(cjson, "source", "4G");

    sendData(cjson, "/Job/Ready");
}

void startTask(void)
{
    cJSON *cjson = NULL;

    cjson = cJSON_CreateObject();
    cJSON_AddStringToObject(cjson, "token", token);
    cJSON_AddStringToObject(cjson, "type", "/Job/Start");
    cJSON_AddStringToObject(cjson, "Flight_ID", sn);
    cJSON_AddNumberToObject(cjson, "id", pTrackInfo.day_job_id);
    cJSON_AddNumberToObject(cjson, "st", getTaskExecutionCnt());
    cJSON_AddNumberToObject(cjson, "lat", pTrackInfo.lat);
    cJSON_AddNumberToObject(cjson, "lon", pTrackInfo.lon);
    cJSON_AddNumberToObject(cjson, "alt", pTrackInfo.alt);
    cJSON_AddNumberToObject(cjson, "spacing", 5);
    cJSON_AddStringToObject(cjson, "flynum", flynum);
    cJSON_AddStringToObject(cjson, "source", "4G");

    sendData(cjson, "/Job/Start");
}

void taskAction(void)
{
    if (SendTaskStateFlag == true && taskflag == true)
    {
        SendTaskStateFlag = false;
			
				if(pTrackInfo.armed == true){
					pTrackInfo.day_job_id ++;
					Flash_write(pTrackInfo.day_job_id);
				}

        //while (jobid_ready == 0 && taskflag == true) // 获取任务ID
//				while (taskflag == true) // 获取任务ID
//        {
            startTask(); // 发送开始作业
            printf("startTask\r\n");
            osDelay(2000);
//        }
    }
}

void json_prarse(cJSON *cjson)
{
    printf("json parse\r\n");
    cJSON *cjson_info = NULL;
    cJSON *cjson_type = NULL;
    cJSON *cjson_souce = NULL;
    cJSON *cjson_tag = NULL;
    cJSON *cjson_id = NULL;
    cJSON *cjson_st = NULL;

    cjson_type = cJSON_GetObjectItem(cjson, "type"); //
    cjson_info = cJSON_GetObjectItem(cjson, "info"); //
    cjson_souce = cJSON_GetObjectItem(cjson, "source");

    if ((strcmp(cjson_souce->valuestring, "4G") == 0))
    {
        SetLEDState(1,3);
        if (strcmp(cjson_type->valuestring, "/Job/Ready") == 0)
        {
            cjson_id = cJSON_GetObjectItem(cjson_info, "id");   //
            cjson_st = cJSON_GetObjectItem(cjson_info, "time"); //
            if (cJSON_IsNumber(cjson_id))
            {
                id = cjson_id->valueint;
            }
            if (cJSON_IsNumber(cjson_st))
            {
                timestamp = (uint64_t)cjson_st->valuedouble;
            }
        }
        if (strcmp(cjson_type->valuestring, "/Job/Start") == 0)
        {
            cjson_info = cJSON_GetObjectItem(cjson, "info"); //

            if (cJSON_IsNumber(cjson_info))
            {
                jobid_ready = 1;
                jobId = cjson_info->valueint;
            }
        }
        if (strcmp(cjson_type->valuestring, "/Job/Track") == 0)
        {
            cjson_tag = cJSON_GetObjectItem(cjson, "tag"); //
            if (cJSON_IsNumber(cjson_tag))
            {
                if (cjson_tag->valueint == 0)
                {
                    tag_flag = 0;
                }
                else
                {
                    tag_flag = 1;
                }
                printf("tag: %d \r\n", cjson_tag->valueint);
            }
            else
            {
                printf("tag: error\r\n");
            }
        }
    }
}

static uint32_t utc_sec_to_day(uint64_t utc_sec)
{
    return (uint32_t)(utc_sec / 86400ULL);
}


bool gnss_time_is_valid(void)
{
	return true;
}

uint64_t gnss_get_utc_seconds()
{
	return 77;
}

static uint64_t generate_task_id_from_utc(uint64_t utc_sec)
{
    uint32_t day_now = utc_sec_to_day(utc_sec);

    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();

//    uint32_t magic    = HAL_RTCEx_BKUPRead(&hrtc, BKP_MAGIC_REG);
//    uint32_t last_day = HAL_RTCEx_BKUPRead(&hrtc, BKP_LAST_DAY_REG);
//    uint32_t boot_seq = HAL_RTCEx_BKUPRead(&hrtc, BKP_BOOT_SEQ_REG);
	
//    uint32_t magic    = 10;
//    uint32_t last_day = 20;
//    uint32_t boot_seq = 30;
	
//    if (magic != BKP_MAGIC_VALUE) {
//        last_day = day_now;
//        boot_seq = 0;
//        HAL_RTCEx_BKUPWrite(&hrtc, BKP_MAGIC_REG, BKP_MAGIC_VALUE);
//        HAL_RTCEx_BKUPWrite(&hrtc, BKP_LAST_DAY_REG, last_day);
//        HAL_RTCEx_BKUPWrite(&hrtc, BKP_BOOT_SEQ_REG, boot_seq);
//    }

//    if (day_now != last_day) {
//        last_day = day_now;
//        boot_seq = 0;
//    }

//    boot_seq++;

//    HAL_RTCEx_BKUPWrite(&hrtc, BKP_LAST_DAY_REG, last_day);
//    HAL_RTCEx_BKUPWrite(&hrtc, BKP_BOOT_SEQ_REG, boot_seq);

//    HAL_PWR_DisableBkUpAccess();

    return ((uint64_t)day_now << 32) ;//| (uint64_t)boot_seq;
}

/* 对外：在 GNSS 时间有效后调用一次，生成并写入全局 g_task_id */
void taskid_initonce_fromgnss(void)
{
    if (!gnss_time_is_valid()) {
        return;
    }

    uint64_t utc = gnss_get_utc_seconds();//todo read from mavlink gps utc 
    g_task_id = generate_task_id_from_utc(utc);
}



