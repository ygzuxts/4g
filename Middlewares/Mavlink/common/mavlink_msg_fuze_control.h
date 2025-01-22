#pragma once
// MESSAGE FUZE_CONTROL PACKING

#define MAVLINK_MSG_ID_FUZE_CONTROL 12931


typedef struct __mavlink_fuze_control_t {
 uint32_t delay_ms; /*<  delay ms*/
 uint8_t cmd; /*<  command*/
} mavlink_fuze_control_t;

#define MAVLINK_MSG_ID_FUZE_CONTROL_LEN 5
#define MAVLINK_MSG_ID_FUZE_CONTROL_MIN_LEN 5
#define MAVLINK_MSG_ID_12931_LEN 5
#define MAVLINK_MSG_ID_12931_MIN_LEN 5

#define MAVLINK_MSG_ID_FUZE_CONTROL_CRC 126
#define MAVLINK_MSG_ID_12931_CRC 126



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_FUZE_CONTROL { \
    12931, \
    "FUZE_CONTROL", \
    2, \
    {  { "delay_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_fuze_control_t, delay_ms) }, \
         { "cmd", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_fuze_control_t, cmd) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_FUZE_CONTROL { \
    "FUZE_CONTROL", \
    2, \
    {  { "delay_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_fuze_control_t, delay_ms) }, \
         { "cmd", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_fuze_control_t, cmd) }, \
         } \
}
#endif

/**
 * @brief Pack a fuze_control message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param delay_ms  delay ms
 * @param cmd  command
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_fuze_control_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t delay_ms, uint8_t cmd)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_FUZE_CONTROL_LEN];
    _mav_put_uint32_t(buf, 0, delay_ms);
    _mav_put_uint8_t(buf, 4, cmd);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_FUZE_CONTROL_LEN);
#else
    mavlink_fuze_control_t packet;
    packet.delay_ms = delay_ms;
    packet.cmd = cmd;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_FUZE_CONTROL_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_FUZE_CONTROL;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_FUZE_CONTROL_MIN_LEN, MAVLINK_MSG_ID_FUZE_CONTROL_LEN, MAVLINK_MSG_ID_FUZE_CONTROL_CRC);
}

/**
 * @brief Pack a fuze_control message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param delay_ms  delay ms
 * @param cmd  command
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_fuze_control_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t delay_ms,uint8_t cmd)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_FUZE_CONTROL_LEN];
    _mav_put_uint32_t(buf, 0, delay_ms);
    _mav_put_uint8_t(buf, 4, cmd);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_FUZE_CONTROL_LEN);
#else
    mavlink_fuze_control_t packet;
    packet.delay_ms = delay_ms;
    packet.cmd = cmd;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_FUZE_CONTROL_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_FUZE_CONTROL;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_FUZE_CONTROL_MIN_LEN, MAVLINK_MSG_ID_FUZE_CONTROL_LEN, MAVLINK_MSG_ID_FUZE_CONTROL_CRC);
}

/**
 * @brief Encode a fuze_control struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param fuze_control C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_fuze_control_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_fuze_control_t* fuze_control)
{
    return mavlink_msg_fuze_control_pack(system_id, component_id, msg, fuze_control->delay_ms, fuze_control->cmd);
}

/**
 * @brief Encode a fuze_control struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param fuze_control C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_fuze_control_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_fuze_control_t* fuze_control)
{
    return mavlink_msg_fuze_control_pack_chan(system_id, component_id, chan, msg, fuze_control->delay_ms, fuze_control->cmd);
}

/**
 * @brief Send a fuze_control message
 * @param chan MAVLink channel to send the message
 *
 * @param delay_ms  delay ms
 * @param cmd  command
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_fuze_control_send(mavlink_channel_t chan, uint32_t delay_ms, uint8_t cmd)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_FUZE_CONTROL_LEN];
    _mav_put_uint32_t(buf, 0, delay_ms);
    _mav_put_uint8_t(buf, 4, cmd);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FUZE_CONTROL, buf, MAVLINK_MSG_ID_FUZE_CONTROL_MIN_LEN, MAVLINK_MSG_ID_FUZE_CONTROL_LEN, MAVLINK_MSG_ID_FUZE_CONTROL_CRC);
#else
    mavlink_fuze_control_t packet;
    packet.delay_ms = delay_ms;
    packet.cmd = cmd;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FUZE_CONTROL, (const char *)&packet, MAVLINK_MSG_ID_FUZE_CONTROL_MIN_LEN, MAVLINK_MSG_ID_FUZE_CONTROL_LEN, MAVLINK_MSG_ID_FUZE_CONTROL_CRC);
#endif
}

/**
 * @brief Send a fuze_control message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_fuze_control_send_struct(mavlink_channel_t chan, const mavlink_fuze_control_t* fuze_control)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_fuze_control_send(chan, fuze_control->delay_ms, fuze_control->cmd);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FUZE_CONTROL, (const char *)fuze_control, MAVLINK_MSG_ID_FUZE_CONTROL_MIN_LEN, MAVLINK_MSG_ID_FUZE_CONTROL_LEN, MAVLINK_MSG_ID_FUZE_CONTROL_CRC);
#endif
}

#if MAVLINK_MSG_ID_FUZE_CONTROL_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_fuze_control_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t delay_ms, uint8_t cmd)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, delay_ms);
    _mav_put_uint8_t(buf, 4, cmd);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FUZE_CONTROL, buf, MAVLINK_MSG_ID_FUZE_CONTROL_MIN_LEN, MAVLINK_MSG_ID_FUZE_CONTROL_LEN, MAVLINK_MSG_ID_FUZE_CONTROL_CRC);
#else
    mavlink_fuze_control_t *packet = (mavlink_fuze_control_t *)msgbuf;
    packet->delay_ms = delay_ms;
    packet->cmd = cmd;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FUZE_CONTROL, (const char *)packet, MAVLINK_MSG_ID_FUZE_CONTROL_MIN_LEN, MAVLINK_MSG_ID_FUZE_CONTROL_LEN, MAVLINK_MSG_ID_FUZE_CONTROL_CRC);
#endif
}
#endif

#endif

// MESSAGE FUZE_CONTROL UNPACKING


/**
 * @brief Get field delay_ms from fuze_control message
 *
 * @return  delay ms
 */
static inline uint32_t mavlink_msg_fuze_control_get_delay_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field cmd from fuze_control message
 *
 * @return  command
 */
static inline uint8_t mavlink_msg_fuze_control_get_cmd(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  4);
}

/**
 * @brief Decode a fuze_control message into a struct
 *
 * @param msg The message to decode
 * @param fuze_control C-struct to decode the message contents into
 */
static inline void mavlink_msg_fuze_control_decode(const mavlink_message_t* msg, mavlink_fuze_control_t* fuze_control)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    fuze_control->delay_ms = mavlink_msg_fuze_control_get_delay_ms(msg);
    fuze_control->cmd = mavlink_msg_fuze_control_get_cmd(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_FUZE_CONTROL_LEN? msg->len : MAVLINK_MSG_ID_FUZE_CONTROL_LEN;
        memset(fuze_control, 0, MAVLINK_MSG_ID_FUZE_CONTROL_LEN);
    memcpy(fuze_control, _MAV_PAYLOAD(msg), len);
#endif
}
