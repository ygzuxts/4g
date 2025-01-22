#pragma once
// MESSAGE CAMERA_POD_STATES PACKING

#define MAVLINK_MSG_ID_CAMERA_POD_STATES 12934


typedef struct __mavlink_camera_pod_states_t {
 float yaw; /*<  camera yaw*/
 float pitch; /*<  camera pitch*/
 uint8_t ptz_status; /*<  ptz status*/
 uint8_t trace_status; /*<  tracking target status*/
 uint8_t deviceid; /*<  device id*/
} mavlink_camera_pod_states_t;

#define MAVLINK_MSG_ID_CAMERA_POD_STATES_LEN 11
#define MAVLINK_MSG_ID_CAMERA_POD_STATES_MIN_LEN 11
#define MAVLINK_MSG_ID_12934_LEN 11
#define MAVLINK_MSG_ID_12934_MIN_LEN 11

#define MAVLINK_MSG_ID_CAMERA_POD_STATES_CRC 84
#define MAVLINK_MSG_ID_12934_CRC 84



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_CAMERA_POD_STATES { \
    12934, \
    "CAMERA_POD_STATES", \
    5, \
    {  { "yaw", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_camera_pod_states_t, yaw) }, \
         { "pitch", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_camera_pod_states_t, pitch) }, \
         { "ptz_status", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_camera_pod_states_t, ptz_status) }, \
         { "trace_status", NULL, MAVLINK_TYPE_UINT8_T, 0, 9, offsetof(mavlink_camera_pod_states_t, trace_status) }, \
         { "deviceid", NULL, MAVLINK_TYPE_UINT8_T, 0, 10, offsetof(mavlink_camera_pod_states_t, deviceid) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_CAMERA_POD_STATES { \
    "CAMERA_POD_STATES", \
    5, \
    {  { "yaw", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_camera_pod_states_t, yaw) }, \
         { "pitch", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_camera_pod_states_t, pitch) }, \
         { "ptz_status", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_camera_pod_states_t, ptz_status) }, \
         { "trace_status", NULL, MAVLINK_TYPE_UINT8_T, 0, 9, offsetof(mavlink_camera_pod_states_t, trace_status) }, \
         { "deviceid", NULL, MAVLINK_TYPE_UINT8_T, 0, 10, offsetof(mavlink_camera_pod_states_t, deviceid) }, \
         } \
}
#endif

/**
 * @brief Pack a camera_pod_states message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param yaw  camera yaw
 * @param pitch  camera pitch
 * @param ptz_status  ptz status
 * @param trace_status  tracking target status
 * @param deviceid  device id
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_camera_pod_states_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               float yaw, float pitch, uint8_t ptz_status, uint8_t trace_status, uint8_t deviceid)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CAMERA_POD_STATES_LEN];
    _mav_put_float(buf, 0, yaw);
    _mav_put_float(buf, 4, pitch);
    _mav_put_uint8_t(buf, 8, ptz_status);
    _mav_put_uint8_t(buf, 9, trace_status);
    _mav_put_uint8_t(buf, 10, deviceid);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CAMERA_POD_STATES_LEN);
#else
    mavlink_camera_pod_states_t packet;
    packet.yaw = yaw;
    packet.pitch = pitch;
    packet.ptz_status = ptz_status;
    packet.trace_status = trace_status;
    packet.deviceid = deviceid;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CAMERA_POD_STATES_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CAMERA_POD_STATES;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_CAMERA_POD_STATES_MIN_LEN, MAVLINK_MSG_ID_CAMERA_POD_STATES_LEN, MAVLINK_MSG_ID_CAMERA_POD_STATES_CRC);
}

/**
 * @brief Pack a camera_pod_states message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param yaw  camera yaw
 * @param pitch  camera pitch
 * @param ptz_status  ptz status
 * @param trace_status  tracking target status
 * @param deviceid  device id
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_camera_pod_states_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   float yaw,float pitch,uint8_t ptz_status,uint8_t trace_status,uint8_t deviceid)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CAMERA_POD_STATES_LEN];
    _mav_put_float(buf, 0, yaw);
    _mav_put_float(buf, 4, pitch);
    _mav_put_uint8_t(buf, 8, ptz_status);
    _mav_put_uint8_t(buf, 9, trace_status);
    _mav_put_uint8_t(buf, 10, deviceid);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CAMERA_POD_STATES_LEN);
#else
    mavlink_camera_pod_states_t packet;
    packet.yaw = yaw;
    packet.pitch = pitch;
    packet.ptz_status = ptz_status;
    packet.trace_status = trace_status;
    packet.deviceid = deviceid;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CAMERA_POD_STATES_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CAMERA_POD_STATES;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_CAMERA_POD_STATES_MIN_LEN, MAVLINK_MSG_ID_CAMERA_POD_STATES_LEN, MAVLINK_MSG_ID_CAMERA_POD_STATES_CRC);
}

/**
 * @brief Encode a camera_pod_states struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param camera_pod_states C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_camera_pod_states_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_camera_pod_states_t* camera_pod_states)
{
    return mavlink_msg_camera_pod_states_pack(system_id, component_id, msg, camera_pod_states->yaw, camera_pod_states->pitch, camera_pod_states->ptz_status, camera_pod_states->trace_status, camera_pod_states->deviceid);
}

/**
 * @brief Encode a camera_pod_states struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param camera_pod_states C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_camera_pod_states_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_camera_pod_states_t* camera_pod_states)
{
    return mavlink_msg_camera_pod_states_pack_chan(system_id, component_id, chan, msg, camera_pod_states->yaw, camera_pod_states->pitch, camera_pod_states->ptz_status, camera_pod_states->trace_status, camera_pod_states->deviceid);
}

/**
 * @brief Send a camera_pod_states message
 * @param chan MAVLink channel to send the message
 *
 * @param yaw  camera yaw
 * @param pitch  camera pitch
 * @param ptz_status  ptz status
 * @param trace_status  tracking target status
 * @param deviceid  device id
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_camera_pod_states_send(mavlink_channel_t chan, float yaw, float pitch, uint8_t ptz_status, uint8_t trace_status, uint8_t deviceid)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CAMERA_POD_STATES_LEN];
    _mav_put_float(buf, 0, yaw);
    _mav_put_float(buf, 4, pitch);
    _mav_put_uint8_t(buf, 8, ptz_status);
    _mav_put_uint8_t(buf, 9, trace_status);
    _mav_put_uint8_t(buf, 10, deviceid);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CAMERA_POD_STATES, buf, MAVLINK_MSG_ID_CAMERA_POD_STATES_MIN_LEN, MAVLINK_MSG_ID_CAMERA_POD_STATES_LEN, MAVLINK_MSG_ID_CAMERA_POD_STATES_CRC);
#else
    mavlink_camera_pod_states_t packet;
    packet.yaw = yaw;
    packet.pitch = pitch;
    packet.ptz_status = ptz_status;
    packet.trace_status = trace_status;
    packet.deviceid = deviceid;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CAMERA_POD_STATES, (const char *)&packet, MAVLINK_MSG_ID_CAMERA_POD_STATES_MIN_LEN, MAVLINK_MSG_ID_CAMERA_POD_STATES_LEN, MAVLINK_MSG_ID_CAMERA_POD_STATES_CRC);
#endif
}

/**
 * @brief Send a camera_pod_states message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_camera_pod_states_send_struct(mavlink_channel_t chan, const mavlink_camera_pod_states_t* camera_pod_states)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_camera_pod_states_send(chan, camera_pod_states->yaw, camera_pod_states->pitch, camera_pod_states->ptz_status, camera_pod_states->trace_status, camera_pod_states->deviceid);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CAMERA_POD_STATES, (const char *)camera_pod_states, MAVLINK_MSG_ID_CAMERA_POD_STATES_MIN_LEN, MAVLINK_MSG_ID_CAMERA_POD_STATES_LEN, MAVLINK_MSG_ID_CAMERA_POD_STATES_CRC);
#endif
}

#if MAVLINK_MSG_ID_CAMERA_POD_STATES_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_camera_pod_states_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float yaw, float pitch, uint8_t ptz_status, uint8_t trace_status, uint8_t deviceid)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, yaw);
    _mav_put_float(buf, 4, pitch);
    _mav_put_uint8_t(buf, 8, ptz_status);
    _mav_put_uint8_t(buf, 9, trace_status);
    _mav_put_uint8_t(buf, 10, deviceid);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CAMERA_POD_STATES, buf, MAVLINK_MSG_ID_CAMERA_POD_STATES_MIN_LEN, MAVLINK_MSG_ID_CAMERA_POD_STATES_LEN, MAVLINK_MSG_ID_CAMERA_POD_STATES_CRC);
#else
    mavlink_camera_pod_states_t *packet = (mavlink_camera_pod_states_t *)msgbuf;
    packet->yaw = yaw;
    packet->pitch = pitch;
    packet->ptz_status = ptz_status;
    packet->trace_status = trace_status;
    packet->deviceid = deviceid;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CAMERA_POD_STATES, (const char *)packet, MAVLINK_MSG_ID_CAMERA_POD_STATES_MIN_LEN, MAVLINK_MSG_ID_CAMERA_POD_STATES_LEN, MAVLINK_MSG_ID_CAMERA_POD_STATES_CRC);
#endif
}
#endif

#endif

// MESSAGE CAMERA_POD_STATES UNPACKING


/**
 * @brief Get field yaw from camera_pod_states message
 *
 * @return  camera yaw
 */
static inline float mavlink_msg_camera_pod_states_get_yaw(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field pitch from camera_pod_states message
 *
 * @return  camera pitch
 */
static inline float mavlink_msg_camera_pod_states_get_pitch(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field ptz_status from camera_pod_states message
 *
 * @return  ptz status
 */
static inline uint8_t mavlink_msg_camera_pod_states_get_ptz_status(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  8);
}

/**
 * @brief Get field trace_status from camera_pod_states message
 *
 * @return  tracking target status
 */
static inline uint8_t mavlink_msg_camera_pod_states_get_trace_status(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  9);
}

/**
 * @brief Get field deviceid from camera_pod_states message
 *
 * @return  device id
 */
static inline uint8_t mavlink_msg_camera_pod_states_get_deviceid(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  10);
}

/**
 * @brief Decode a camera_pod_states message into a struct
 *
 * @param msg The message to decode
 * @param camera_pod_states C-struct to decode the message contents into
 */
static inline void mavlink_msg_camera_pod_states_decode(const mavlink_message_t* msg, mavlink_camera_pod_states_t* camera_pod_states)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    camera_pod_states->yaw = mavlink_msg_camera_pod_states_get_yaw(msg);
    camera_pod_states->pitch = mavlink_msg_camera_pod_states_get_pitch(msg);
    camera_pod_states->ptz_status = mavlink_msg_camera_pod_states_get_ptz_status(msg);
    camera_pod_states->trace_status = mavlink_msg_camera_pod_states_get_trace_status(msg);
    camera_pod_states->deviceid = mavlink_msg_camera_pod_states_get_deviceid(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_CAMERA_POD_STATES_LEN? msg->len : MAVLINK_MSG_ID_CAMERA_POD_STATES_LEN;
        memset(camera_pod_states, 0, MAVLINK_MSG_ID_CAMERA_POD_STATES_LEN);
    memcpy(camera_pod_states, _MAV_PAYLOAD(msg), len);
#endif
}
