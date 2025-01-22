#ifndef _TRACE_QUEUE_H
#define _TRACE_QUEUE_H

#include "stdint.h"
// 结构体定于，
typedef struct
{
    float pitch;
    float roll;
    float yaw;
    float alt;
    double lon;
    double lat;
    float l;
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
