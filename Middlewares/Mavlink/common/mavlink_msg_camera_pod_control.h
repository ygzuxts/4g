#pragma once
// MESSAGE CAMERA_POD_CONTROL PACKING

#define MAVLINK_MSG_ID_CAMERA_POD_CONTROL 12933


typedef struct __mavlink_camera_pod_control_t {
 float param1; /*<  control param1*/
 float param2; /*<  control param2*/
 uint8_t cmd; /*<  camera control cmd*/
} mavlink_camera_pod_control_t;

#define MAVLINK_MSG_ID_CAMERA_POD_CONTROL_LEN 9
#define MAVLINK_MSG_ID_CAMERA_POD_CONTROL_MIN_LEN 9
#define MAVLINK_MSG_ID_12933_LEN 9
#define MAVLINK_MSG_ID_12933_MIN_LEN 9

#define MAVLINK_MSG_ID_CAMERA_POD_CONTROL_CRC 221
#define MAVLINK_MSG_ID_12933_CRC 221



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_CAMERA_POD_CONTROL { \
    12933, \
    "CAMERA_POD_CONTROL", \
    3, \
    {  { "cmd", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_camera_pod_control_t, cmd) }, \
         { "param1", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_camera_pod_control_t, param1) }, \
         { "param2", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_camera_pod_control_t, param2) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_CAMERA_POD_CONTROL { \
    "CAMERA_POD_CONTROL", \
    3, \
    {  { "cmd", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_camera_pod_control_t, cmd) }, \
         { "param1", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_camera_pod_control_t, param1) }, \
         { "param2", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_camera_pod_control_t, param2) }, \
         } \
}
#endif

/**
 * @brief Pack a camera_pod_control message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param cmd  camera control cmd
 * @param param1  control param1
 * @param param2  control param2
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_camera_pod_control_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t cmd, float param1, float param2)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CAMERA_POD_CONTROL_LEN];
    _mav_put_float(buf, 0, param1);
    _mav_put_float(buf, 4, param2);
    _mav_put_uint8_t(buf, 8, cmd);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CAMERA_POD_CONTROL_LEN);
#else
    mavlink_camera_pod_control_t packet;
    packet.param1 = param1;
    packet.param2 = param2;
    packet.cmd = cmd;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CAMERA_POD_CONTROL_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CAMERA_POD_CONTROL;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_CAMERA_POD_CONTROL_MIN_LEN, MAVLINK_MSG_ID_CAMERA_POD_CONTROL_LEN, MAVLINK_MSG_ID_CAMERA_POD_CONTROL_CRC);
}

/**
 * @brief Pack a camera_pod_control message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param cmd  camera control cmd
 * @param param1  control param1
 * @param param2  control param2
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_camera_pod_control_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t cmd,float param1,float param2)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CAMERA_POD_CONTROL_LEN];
    _mav_put_float(buf, 0, param1);
    _mav_put_float(buf, 4, param2);
    _mav_put_uint8_t(buf, 8, cmd);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CAMERA_POD_CONTROL_LEN);
#else
    mavlink_camera_pod_control_t packet;
    packet.param1 = param1;
    packet.param2 = param2;
    packet.cmd = cmd;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CAMERA_POD_CONTROL_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CAMERA_POD_CONTROL;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_CAMERA_POD_CONTROL_MIN_LEN, MAVLINK_MSG_ID_CAMERA_POD_CONTROL_LEN, MAVLINK_MSG_ID_CAMERA_POD_CONTROL_CRC);
}

/**
 * @brief Encode a camera_pod_control struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param camera_pod_control C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_camera_pod_control_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_camera_pod_control_t* camera_pod_control)
{
    return mavlink_msg_camera_pod_control_pack(system_id, component_id, msg, camera_pod_control->cmd, camera_pod_control->param1, camera_pod_control->param2);
}

/**
 * @brief Encode a camera_pod_control struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param camera_pod_control C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_camera_pod_control_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_camera_pod_control_t* camera_pod_control)
{
    return mavlink_msg_camera_pod_control_pack_chan(system_id, component_id, chan, msg, camera_pod_control->cmd, camera_pod_control->param1, camera_pod_control->param2);
}

/**
 * @brief Send a camera_pod_control message
 * @param chan MAVLink channel to send the message
 *
 * @param cmd  camera control cmd
 * @param param1  control param1
 * @param param2  control param2
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_camera_pod_control_send(mavlink_channel_t chan, uint8_t cmd, float param1, float param2)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CAMERA_POD_CONTROL_LEN];
    _mav_put_float(buf, 0, param1);
    _mav_put_float(buf, 4, param2);
    _mav_put_uint8_t(buf, 8, cmd);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CAMERA_POD_CONTROL, buf, MAVLINK_MSG_ID_CAMERA_POD_CONTROL_MIN_LEN, MAVLINK_MSG_ID_CAMERA_POD_CONTROL_LEN, MAVLINK_MSG_ID_CAMERA_POD_CONTROL_CRC);
#else
    mavlink_camera_pod_control_t packet;
    packet.param1 = param1;
    packet.param2 = param2;
    packet.cmd = cmd;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CAMERA_POD_CONTROL, (const char *)&packet, MAVLINK_MSG_ID_CAMERA_POD_CONTROL_MIN_LEN, MAVLINK_MSG_ID_CAMERA_POD_CONTROL_LEN, MAVLINK_MSG_ID_CAMERA_POD_CONTROL_CRC);
#endif
}

/**
 * @brief Send a camera_pod_control message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_camera_pod_control_send_struct(mavlink_channel_t chan, const mavlink_camera_pod_control_t* camera_pod_control)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_camera_pod_control_send(chan, camera_pod_control->cmd, camera_pod_control->param1, camera_pod_control->param2);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CAMERA_POD_CONTROL, (const char *)camera_pod_control, MAVLINK_MSG_ID_CAMERA_POD_CONTROL_MIN_LEN, MAVLINK_MSG_ID_CAMERA_POD_CONTROL_LEN, MAVLINK_MSG_ID_CAMERA_POD_CONTROL_CRC);
#endif
}

#if MAVLINK_MSG_ID_CAMERA_POD_CONTROL_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_camera_pod_control_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t cmd, float param1, float param2)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, param1);
    _mav_put_float(buf, 4, param2);
    _mav_put_uint8_t(buf, 8, cmd);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CAMERA_POD_CONTROL, buf, MAVLINK_MSG_ID_CAMERA_POD_CONTROL_MIN_LEN, MAVLINK_MSG_ID_CAMERA_POD_CONTROL_LEN, MAVLINK_MSG_ID_CAMERA_POD_CONTROL_CRC);
#else
    mavlink_camera_pod_control_t *packet = (mavlink_camera_pod_control_t *)msgbuf;
    packet->param1 = param1;
    packet->param2 = param2;
    packet->cmd = cmd;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CAMERA_POD_CONTROL, (const char *)packet, MAVLINK_MSG_ID_CAMERA_POD_CONTROL_MIN_LEN, MAVLINK_MSG_ID_CAMERA_POD_CONTROL_LEN, MAVLINK_MSG_ID_CAMERA_POD_CONTROL_CRC);
#endif
}
#endif

#endif

// MESSAGE CAMERA_POD_CONTROL UNPACKING


/**
 * @brief Get field cmd from camera_pod_control message
 *
 * @return  camera control cmd
 */
static inline uint8_t mavlink_msg_camera_pod_control_get_cmd(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  8);
}

/**
 * @brief Get field param1 from camera_pod_control message
 *
 * @return  control param1
 */
static inline float mavlink_msg_camera_pod_control_get_param1(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field param2 from camera_pod_control message
 *
 * @return  control param2
 */
static inline float mavlink_msg_camera_pod_control_get_param2(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Decode a camera_pod_control message into a struct
 *
 * @param msg The message to decode
 * @param camera_pod_control C-struct to decode the message contents into
 */
static inline void mavlink_msg_camera_pod_control_decode(const mavlink_message_t* msg, mavlink_camera_pod_control_t* camera_pod_control)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    camera_pod_control->param1 = mavlink_msg_camera_pod_control_get_param1(msg);
    camera_pod_control->param2 = mavlink_msg_camera_pod_control_get_param2(msg);
    camera_pod_control->cmd = mavlink_msg_camera_pod_control_get_cmd(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_CAMERA_POD_CONTROL_LEN? msg->len : MAVLINK_MSG_ID_CAMERA_POD_CONTROL_LEN;
        memset(camera_pod_control, 0, MAVLINK_MSG_ID_CAMERA_POD_CONTROL_LEN);
    memcpy(camera_pod_control, _MAV_PAYLOAD(msg), len);
#endif
}
