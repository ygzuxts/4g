#pragma once
// MESSAGE FLIGHT_NUM_MSG PACKING

#define MAVLINK_MSG_ID_FLIGHT_NUM_MSG 12925


typedef struct __mavlink_flight_num_msg_t {
 char flight_num[6]; /*<  flight num*/
} mavlink_flight_num_msg_t;

#define MAVLINK_MSG_ID_FLIGHT_NUM_MSG_LEN 6
#define MAVLINK_MSG_ID_FLIGHT_NUM_MSG_MIN_LEN 6
#define MAVLINK_MSG_ID_12925_LEN 6
#define MAVLINK_MSG_ID_12925_MIN_LEN 6

#define MAVLINK_MSG_ID_FLIGHT_NUM_MSG_CRC 232
#define MAVLINK_MSG_ID_12925_CRC 232

#define MAVLINK_MSG_FLIGHT_NUM_MSG_FIELD_FLIGHT_NUM_LEN 6

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_FLIGHT_NUM_MSG { \
    12925, \
    "FLIGHT_NUM_MSG", \
    1, \
    {  { "flight_num", NULL, MAVLINK_TYPE_CHAR, 6, 0, offsetof(mavlink_flight_num_msg_t, flight_num) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_FLIGHT_NUM_MSG { \
    "FLIGHT_NUM_MSG", \
    1, \
    {  { "flight_num", NULL, MAVLINK_TYPE_CHAR, 6, 0, offsetof(mavlink_flight_num_msg_t, flight_num) }, \
         } \
}
#endif

/**
 * @brief Pack a flight_num_msg message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param flight_num  flight num
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_flight_num_msg_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               const char *flight_num)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_FLIGHT_NUM_MSG_LEN];

    _mav_put_char_array(buf, 0, flight_num, 6);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_FLIGHT_NUM_MSG_LEN);
#else
    mavlink_flight_num_msg_t packet;

    mav_array_memcpy(packet.flight_num, flight_num, sizeof(char)*6);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_FLIGHT_NUM_MSG_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_FLIGHT_NUM_MSG;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_FLIGHT_NUM_MSG_MIN_LEN, MAVLINK_MSG_ID_FLIGHT_NUM_MSG_LEN, MAVLINK_MSG_ID_FLIGHT_NUM_MSG_CRC);
}

/**
 * @brief Pack a flight_num_msg message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param flight_num  flight num
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_flight_num_msg_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   const char *flight_num)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_FLIGHT_NUM_MSG_LEN];

    _mav_put_char_array(buf, 0, flight_num, 6);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_FLIGHT_NUM_MSG_LEN);
#else
    mavlink_flight_num_msg_t packet;

    mav_array_memcpy(packet.flight_num, flight_num, sizeof(char)*6);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_FLIGHT_NUM_MSG_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_FLIGHT_NUM_MSG;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_FLIGHT_NUM_MSG_MIN_LEN, MAVLINK_MSG_ID_FLIGHT_NUM_MSG_LEN, MAVLINK_MSG_ID_FLIGHT_NUM_MSG_CRC);
}

/**
 * @brief Encode a flight_num_msg struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param flight_num_msg C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_flight_num_msg_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_flight_num_msg_t* flight_num_msg)
{
    return mavlink_msg_flight_num_msg_pack(system_id, component_id, msg, flight_num_msg->flight_num);
}

/**
 * @brief Encode a flight_num_msg struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param flight_num_msg C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_flight_num_msg_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_flight_num_msg_t* flight_num_msg)
{
    return mavlink_msg_flight_num_msg_pack_chan(system_id, component_id, chan, msg, flight_num_msg->flight_num);
}

/**
 * @brief Send a flight_num_msg message
 * @param chan MAVLink channel to send the message
 *
 * @param flight_num  flight num
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_flight_num_msg_send(mavlink_channel_t chan, const char *flight_num)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_FLIGHT_NUM_MSG_LEN];

    _mav_put_char_array(buf, 0, flight_num, 6);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FLIGHT_NUM_MSG, buf, MAVLINK_MSG_ID_FLIGHT_NUM_MSG_MIN_LEN, MAVLINK_MSG_ID_FLIGHT_NUM_MSG_LEN, MAVLINK_MSG_ID_FLIGHT_NUM_MSG_CRC);
#else
    mavlink_flight_num_msg_t packet;

    mav_array_memcpy(packet.flight_num, flight_num, sizeof(char)*6);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FLIGHT_NUM_MSG, (const char *)&packet, MAVLINK_MSG_ID_FLIGHT_NUM_MSG_MIN_LEN, MAVLINK_MSG_ID_FLIGHT_NUM_MSG_LEN, MAVLINK_MSG_ID_FLIGHT_NUM_MSG_CRC);
#endif
}

/**
 * @brief Send a flight_num_msg message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_flight_num_msg_send_struct(mavlink_channel_t chan, const mavlink_flight_num_msg_t* flight_num_msg)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_flight_num_msg_send(chan, flight_num_msg->flight_num);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FLIGHT_NUM_MSG, (const char *)flight_num_msg, MAVLINK_MSG_ID_FLIGHT_NUM_MSG_MIN_LEN, MAVLINK_MSG_ID_FLIGHT_NUM_MSG_LEN, MAVLINK_MSG_ID_FLIGHT_NUM_MSG_CRC);
#endif
}

#if MAVLINK_MSG_ID_FLIGHT_NUM_MSG_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_flight_num_msg_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  const char *flight_num)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;

    _mav_put_char_array(buf, 0, flight_num, 6);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FLIGHT_NUM_MSG, buf, MAVLINK_MSG_ID_FLIGHT_NUM_MSG_MIN_LEN, MAVLINK_MSG_ID_FLIGHT_NUM_MSG_LEN, MAVLINK_MSG_ID_FLIGHT_NUM_MSG_CRC);
#else
    mavlink_flight_num_msg_t *packet = (mavlink_flight_num_msg_t *)msgbuf;

    mav_array_memcpy(packet->flight_num, flight_num, sizeof(char)*6);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FLIGHT_NUM_MSG, (const char *)packet, MAVLINK_MSG_ID_FLIGHT_NUM_MSG_MIN_LEN, MAVLINK_MSG_ID_FLIGHT_NUM_MSG_LEN, MAVLINK_MSG_ID_FLIGHT_NUM_MSG_CRC);
#endif
}
#endif

#endif

// MESSAGE FLIGHT_NUM_MSG UNPACKING


/**
 * @brief Get field flight_num from flight_num_msg message
 *
 * @return  flight num
 */
static inline uint16_t mavlink_msg_flight_num_msg_get_flight_num(const mavlink_message_t* msg, char *flight_num)
{
    return _MAV_RETURN_char_array(msg, flight_num, 6,  0);
}

/**
 * @brief Decode a flight_num_msg message into a struct
 *
 * @param msg The message to decode
 * @param flight_num_msg C-struct to decode the message contents into
 */
static inline void mavlink_msg_flight_num_msg_decode(const mavlink_message_t* msg, mavlink_flight_num_msg_t* flight_num_msg)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_flight_num_msg_get_flight_num(msg, flight_num_msg->flight_num);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_FLIGHT_NUM_MSG_LEN? msg->len : MAVLINK_MSG_ID_FLIGHT_NUM_MSG_LEN;
        memset(flight_num_msg, 0, MAVLINK_MSG_ID_FLIGHT_NUM_MSG_LEN);
    memcpy(flight_num_msg, _MAV_PAYLOAD(msg), len);
#endif
}
