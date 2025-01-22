#include "track_json.h"
#include "cjson.h"
#include "stdlib.h"
#include "usart_4gmoudle.h"
#include "stdint.h"
#include "stdbool.h"
#include "track_queue.h"
#include "cmsis_os.h"
#include "math.h"


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
char cv[10] = "0";        // 固件版本
char fv[10] = "0";        // 飞控版本
uint8_t client = 0;       // 0:地面站
uint64_t timestamp = 0;   // 云网发送的时间戳
uint32_t jobId = 0;       // 任务序号
uint32_t trackNum = 0;    // 发送的航点序号
uint32_t duration = 0;    // 持续时间
uint32_t id = 0;          // 飞机固定ID，从云网获取




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
        while (tag_flag)
        {
            printf("send track\r\n");
            USART3_SendBytes((uint8_t *)buffer, strlen(buffer) + 1);
            if (sendNum++ >= 2) // 防止和RTCM报文冲突
            {
                sendNum = 0;
                break;
            }
            osDelay(1000);
        }
        tag_flag = 1;
    }
    else
    {
        USART3_SendBytes((uint8_t *)buffer, strlen(buffer) + 1);
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
    cJSON_AddStringToObject(cjson, "sn", sn);
    cJSON_AddNumberToObject(cjson, "job", jobId);
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
    cJSON_AddStringToObject(cjson_test, "sn", sn);
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

void readyTask(void)
{
    cJSON *cjson = NULL;

    cjson = cJSON_CreateObject();

    cJSON_AddStringToObject(cjson, "token", token);
    cJSON_AddStringToObject(cjson, "type", "/Job/Ready");
    cJSON_AddStringToObject(cjson, "sn", sn);
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
    cJSON_AddStringToObject(cjson, "sn", sn);
    cJSON_AddNumberToObject(cjson, "id", id);
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

        while (jobid_ready == 0 && taskflag == true) // 获取任务ID
        {
            startTask(); // 发送开始作业
            printf("startTask\r\n");
            osDelay(2000);
        }
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
        if (strcmp(cjson_type->valuestring, "/Job/Ready") == 0)
        {
            cjson_id = cJSON_GetObjectItem(cjson_info, "id");   //
            cjson_st = cJSON_GetObjectItem(cjson_info, "time"); //
            if (cJSON_IsNumber(cjson_id))
            {
                id = cjson_id->valueint;
                printf("id is %d\r\n",id);
            }
            if (cJSON_IsNumber(cjson_st))
            {
                timestamp = (uint64_t)cjson_st->valuedouble;
                printf("timestamp is %lld\r\n",timestamp);
            }
        }
        if (strcmp(cjson_type->valuestring, "/Job/Start") == 0)
        {
            cjson_info = cJSON_GetObjectItem(cjson, "info"); //

            if (cJSON_IsNumber(cjson_info))
            {
                printf("jobId: %d \r\n", cjson_info->valueint);
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
