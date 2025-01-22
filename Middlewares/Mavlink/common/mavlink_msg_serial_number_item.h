#pragma once
// MESSAGE SERIAL_NUMBER_ITEM PACKING

#define MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM 960


typedef struct __mavlink_serial_number_item_t {
 char serial_num[16]; /*<  S*/
} mavlink_serial_number_item_t;

#define MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_LEN 16
#define MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_MIN_LEN 16
#define MAVLINK_MSG_ID_960_LEN 16
#define MAVLINK_MSG_ID_960_MIN_LEN 16

#define MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_CRC 101
#define MAVLINK_MSG_ID_960_CRC 101

#define MAVLINK_MSG_SERIAL_NUMBER_ITEM_FIELD_SERIAL_NUM_LEN 16

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SERIAL_NUMBER_ITEM { \
    960, \
    "SERIAL_NUMBER_ITEM", \
    1, \
    {  { "serial_num", NULL, MAVLINK_TYPE_CHAR, 16, 0, offsetof(mavlink_serial_number_item_t, serial_num) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SERIAL_NUMBER_ITEM { \
    "SERIAL_NUMBER_ITEM", \
    1, \
    {  { "serial_num", NULL, MAVLINK_TYPE_CHAR, 16, 0, offsetof(mavlink_serial_number_item_t, serial_num) }, \
         } \
}
#endif

/**
 * @brief Pack a serial_number_item message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param serial_num  S
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_serial_number_item_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               const char *serial_num)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_LEN];

    _mav_put_char_array(buf, 0, serial_num, 16);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_LEN);
#else
    mavlink_serial_number_item_t packet;

    mav_array_memcpy(packet.serial_num, serial_num, sizeof(char)*16);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_MIN_LEN, MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_LEN, MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_CRC);
}

/**
 * @brief Pack a serial_number_item message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param serial_num  S
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_serial_number_item_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   const char *serial_num)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_LEN];

    _mav_put_char_array(buf, 0, serial_num, 16);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_LEN);
#else
    mavlink_serial_number_item_t packet;

    mav_array_memcpy(packet.serial_num, serial_num, sizeof(char)*16);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_MIN_LEN, MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_LEN, MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_CRC);
}

/**
 * @brief Encode a serial_number_item struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param serial_number_item C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_serial_number_item_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_serial_number_item_t* serial_number_item)
{
    return mavlink_msg_serial_number_item_pack(system_id, component_id, msg, serial_number_item->serial_num);
}

/**
 * @brief Encode a serial_number_item struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param serial_number_item C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_serial_number_item_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_serial_number_item_t* serial_number_item)
{
    return mavlink_msg_serial_number_item_pack_chan(system_id, component_id, chan, msg, serial_number_item->serial_num);
}

/**
 * @brief Send a serial_number_item message
 * @param chan MAVLink channel to send the message
 *
 * @param serial_num  S
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_serial_number_item_send(mavlink_channel_t chan, const char *serial_num)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_LEN];

    _mav_put_char_array(buf, 0, serial_num, 16);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM, buf, MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_MIN_LEN, MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_LEN, MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_CRC);
#else
    mavlink_serial_number_item_t packet;

    mav_array_memcpy(packet.serial_num, serial_num, sizeof(char)*16);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM, (const char *)&packet, MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_MIN_LEN, MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_LEN, MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_CRC);
#endif
}

/**
 * @brief Send a serial_number_item message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_serial_number_item_send_struct(mavlink_channel_t chan, const mavlink_serial_number_item_t* serial_number_item)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_serial_number_item_send(chan, serial_number_item->serial_num);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM, (const char *)serial_number_item, MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_MIN_LEN, MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_LEN, MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_CRC);
#endif
}

#if MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_serial_number_item_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  const char *serial_num)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;

    _mav_put_char_array(buf, 0, serial_num, 16);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM, buf, MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_MIN_LEN, MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_LEN, MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_CRC);
#else
    mavlink_serial_number_item_t *packet = (mavlink_serial_number_item_t *)msgbuf;

    mav_array_memcpy(packet->serial_num, serial_num, sizeof(char)*16);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM, (const char *)packet, MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_MIN_LEN, MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_LEN, MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_CRC);
#endif
}
#endif

#endif

// MESSAGE SERIAL_NUMBER_ITEM UNPACKING


/**
 * @brief Get field serial_num from serial_number_item message
 *
 * @return  S
 */
static inline uint16_t mavlink_msg_serial_number_item_get_serial_num(const mavlink_message_t* msg, char *serial_num)
{
    return _MAV_RETURN_char_array(msg, serial_num, 16,  0);
}

/**
 * @brief Decode a serial_number_item message into a struct
 *
 * @param msg The message to decode
 * @param serial_number_item C-struct to decode the message contents into
 */
static inline void mavlink_msg_serial_number_item_decode(const mavlink_message_t* msg, mavlink_serial_number_item_t* serial_number_item)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_serial_number_item_get_serial_num(msg, serial_number_item->serial_num);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_LEN? msg->len : MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_LEN;
        memset(serial_number_item, 0, MAVLINK_MSG_ID_SERIAL_NUMBER_ITEM_LEN);
    memcpy(serial_number_item, _MAV_PAYLOAD(msg), len);
#endif
}
