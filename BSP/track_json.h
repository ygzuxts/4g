#ifndef _TASKJSON_H_
#define _TASKJSON_H_

#include "stdio.h"
#include "string.h"
#include "cjson.h"

void readyTask(void);
void sendData(cJSON *cjson, char *type);
void finishTask(void);
void startTask(void);
void taskAction(void);
void json_prarse(cJSON *cjson);
void uploadTrack(void);
#endif
