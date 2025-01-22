#include "open_tel_mavlink.h"
#include "usart_fifo.h"
#include "define.h"
#include "stdint.h"
#include "mavlinkparse.h"

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
            handleMessage(msg); // 结构体成员赋值，有解析任务的回复信息
            break;
        }
    }
}

// 解包函数,可按需要仿照如下格式添加case和对应的decode函数
void handleMessage(mavlink_message_t msg)
{
    // printf("msg->msgid is %d\r\n", msg.msgid);
    //  struct Location tell_command = {};                                  // command for telemetry
    switch (msg.msgid)
    {
    case MAVLINK_MSG_ID_HEARTBEAT:
        MAVLINK_MSG_ID_HEARTBEAT_ACTION(msg);
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
    case MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM:
        MAVLINK_MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_ACTION(msg);
        break;
    default:
        break;
    }
}
