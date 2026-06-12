#ifndef _TRACE_QUEUE_H
#define _TRACE_QUEUE_H

#include "stdint.h"
// 结构体定于，
typedef struct
{
    float pitch;
    float roll;
    float yaw;

    float l;
	
		//飞机状态信息
		_Bool armed;
		_Bool ready_to_fly;
		uint32_t day_job_id;
		uint64_t utc_sec;
	
		float vehicle_control[8];

    double lon;
    double lat;
	  float alt;

		float vehicle_q[4];
	
		float vehicle_vx;
		float vehicle_vy;
		float vehicle_vz;
		
		//电调状态
		int32_t esc_rpm[8];
		float esc_voltage[8];
		float esc_current[8];
		float esc_temple[8];
		uint32_t esc_errorcount[8];
		
		//飞控接收输入
		float rc_roll;
		float rc_pitch;
		float rc_yaw;
		float rc_throttle;
		
		uint8_t nav_state_requset;
		uint8_t nav_state_current;
		
		//电池状态
		float voltage_v;
		float current_a;
		float discharged_mah;
		float remaining;
		float time_remaining_s;
		float temperature;
		
} TrackInfo;

// 链表节点结构体
typedef struct Node
{
    TrackInfo data;
    struct Node *next;
} Node;

// 链表结构体
typedef struct
{
    Node *head;
    int length;
} LinkedList;

// 初始化链表
void initLinkedList(LinkedList *list);
// 销毁链表
void destroyLinkedList(LinkedList *list);
// 获取链表长度
uint8_t getLinkedListLength(LinkedList *list);
// 获取链表第一个数据
TrackInfo getFirstData(LinkedList *list);
// 向链表最后插入数据
void insertDataAtEnd(LinkedList *list, TrackInfo data);
// 链表弹出
TrackInfo popFromLinkedList(LinkedList *list);

// 获取链表第一个数据
TrackInfo getHeadData(LinkedList *list);
#endif
