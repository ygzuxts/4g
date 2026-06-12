#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "track_queue.h"
#include "stdint.h"
#include "FreeRTOS.h"

#define MAX_LENGTH 50

// 初始化链表
void initLinkedList(LinkedList *list)
{
    list->head = NULL;
    list->length = 0;
}

// 销毁链表
void destroyLinkedList(LinkedList *list)
{
    Node *current = list->head;
    while (current != NULL)
    {
        Node *temp = current;
        current = current->next;
        vPortFree(temp);
    }
    list->head = NULL;
    list->length = 0;
}

// 获取链表长度
uint8_t getLinkedListLength(LinkedList *list)
{
    return list->length;
}

// 获取链表第一个数据
TrackInfo getFirstData(LinkedList *list)
{
    return list->head->data;
}

// 向链表最后插入数据
void insertDataAtEnd(LinkedList *list, TrackInfo data)
{
    Node *newNode = (Node *)pvPortMalloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;

    if (list->head == NULL)
    {
        list->head = newNode;
    }
    else
    {
        Node *current = list->head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
    }

    list->length++;
    // 如果链表长度超过最大长度，替换最后一个
    if (list->length > MAX_LENGTH)
    {
        Node *current = list->head;
        while (current->next->next != NULL)
        {
            current = current->next;
        }
        vPortFree(current->next); // 释放最后一个节点的内存
        current->next = NULL;
        list->length--;
    }
}

// 链表弹出
TrackInfo popFromLinkedList(LinkedList *list)
{
    if (list->head == NULL)
    {
        TrackInfo emptyData = {0}; // Assuming default values for GPSInfo
        return emptyData;
    }

    Node *temp = list->head;
    TrackInfo data = temp->data;
    list->head = list->head->next;
    vPortFree(temp);
    list->length--;

    return data;
}

// 获取链表头部数据
TrackInfo getHeadData(LinkedList *list)
{
    if (list->head == NULL)
    {
        TrackInfo emptyData = {0}; // 返回默认值
        return emptyData;
    }

    return list->head->data;
}
