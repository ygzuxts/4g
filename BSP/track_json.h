#ifndef _TASKJSON_H_
#define _TASKJSON_H_

#include "stdio.h"
#include "string.h"
#include "cjson.h"
#include "stdint.h"
#include "stdbool.h"

extern volatile uint64_t g_task_id;

void readyTask(void);
void sendData(cJSON *cjson, char *type);
void finishTask(void);
void startTask(void);
void taskAction(void);
void json_prarse(cJSON *cjson);
void uploadTrack(void);
/* use for creat task id antegou*/
void normal_status_Action(void);
void taskid_initonce_fromgnss(void);
bool gnss_time_is_valid(void);
static uint64_t generate_task_id_from_utc(uint64_t utc_sec);

#endif
