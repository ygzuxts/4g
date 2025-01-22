#pragma once
// MESSAGE ATTACK_STATUS PACKING

#define MAVLINK_MSG_ID_ATTACK_STATUS 700


typedef struct __mavlink_attack_status_t {
 float attack_alt_error; /*< [m] Altitude deviation(error)*/
 float attack_hori_distance_error; /*< [m] Horizontal deviation(error)*/
 float attack_tilt_angle_error; /*< [deg] Deviation(error) in tilt angle*/
 float attack_xtrack_error; /*< [m] Cross track error*/
 float attack_along_track_error; /*< [m] Along track error*/
 uint8_t attack_mode; /*< [m] Attacking mode*/
} mavlink_attack_status_t;

#define MAVLINK_MSG_ID_ATTACK_STATUS_LEN 21
#define MAVLINK_MSG_ID_ATTACK_STATUS_MIN_LEN 21
#define MAVLINK_MSG_ID_700_LEN 21
#define MAVLINK_MSG_ID_700_MIN_LEN 21

#define MAVLINK_MSG_ID_ATTACK_STATUS_CRC 17
#define MAVLINK_MSG_ID_700_CRC 17



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_ATTACK_STATUS { \
    700, \
    "ATTACK_STATUS", \
    6, \
    {  { "attack_mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 20, offsetof(mavlink_attack_status_t, attack_mode) }, \
         { "attack_alt_error", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_attack_status_t, attack_alt_error) }, \
         { "attack_hori_distance_error", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_attack_status_t, attack_hori_distance_error) }, \
         { "attack_tilt_angle_error", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_attack_status_t, attack_tilt_angle_error) }, \
         { "attack_xtrack_error", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_attack_status_t, attack_xtrack_error) }, \
         { "attack_along_track_error", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_attack_status_t, attack_along_track_error) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_ATTACK_STATUS { \
    "ATTACK_STATUS", \
    6, \
    {  { "attack_mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 20, offsetof(mavlink_attack_status_t, attack_mode) }, \
         { "attack_alt_error", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_attack_status_t, attack_alt_error) }, \
         { "attack_hori_distance_error", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_attack_status_t, attack_hori_distance_error) }, \
         { "attack_tilt_angle_error", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_attack_status_t, attack_tilt_angle_error) }, \
         { "attack_xtrack_error", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_attack_status_t, attack_xtrack_error) }, \
         { "attack_along_track_error", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_attack_status_t, attack_along_track_error) }, \
         } \
}
#endif

/**
 * @brief Pack a attack_status message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param attack_mode [m] Attacking mode
 * @param attack_alt_error [m] Altitude deviation(error)
 * @param attack_hori_distance_error [m] Horizontal deviation(error)
 * @param attack_tilt_angle_error [deg] Deviation(error) in tilt angle
 * @param attack_xtrack_error [m] Cross track error
 * @param attack_along_track_error [m] Along track error
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_attack_status_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t attack_mode, float attack_alt_error, float attack_hori_distance_error, float attack_tilt_angle_error, float attack_xtrack_error, float attack_along_track_error)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ATTACK_STATUS_LEN];
    _mav_put_float(buf, 0, attack_alt_error);
    _mav_put_float(buf, 4, attack_hori_distance_error);
    _mav_put_float(buf, 8, attack_tilt_angle_error);
    _mav_put_float(buf, 12, attack_xtrack_error);
    _mav_put_float(buf, 16, attack_along_track_error);
    _mav_put_uint8_t(buf, 20, attack_mode);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ATTACK_STATUS_LEN);
#else
    mavlink_attack_status_t packet;
    packet.attack_alt_error = attack_alt_error;
    packet.attack_hori_distance_error = attack_hori_distance_error;
    packet.attack_tilt_angle_error = attack_tilt_angle_error;
    packet.attack_xtrack_error = attack_xtrack_error;
    packet.attack_along_track_error = attack_along_track_error;
    packet.attack_mode = attack_mode;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ATTACK_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ATTACK_STATUS;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_ATTACK_STATUS_MIN_LEN, MAVLINK_MSG_ID_ATTACK_STATUS_LEN, MAVLINK_MSG_ID_ATTACK_STATUS_CRC);
}

/**
 * @brief Pack a attack_status message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param attack_mode [m] Attacking mode
 * @param attack_alt_error [m] Altitude deviation(error)
 * @param attack_hori_distance_error [m] Horizontal deviation(error)
 * @param attack_tilt_angle_error [deg] Deviation(error) in tilt angle
 * @param attack_xtrack_error [m] Cross track error
 * @param attack_along_track_error [m] Along track error
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_attack_status_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t attack_mode,float attack_alt_error,float attack_hori_distance_error,float attack_tilt_angle_error,float attack_xtrack_error,float attack_along_track_error)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ATTACK_STATUS_LEN];
    _mav_put_float(buf, 0, attack_alt_error);
    _mav_put_float(buf, 4, attack_hori_distance_error);
    _mav_put_float(buf, 8, attack_tilt_angle_error);
    _mav_put_float(buf, 12, attack_xtrack_error);
    _mav_put_float(buf, 16, attack_along_track_error);
    _mav_put_uint8_t(buf, 20, attack_mode);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ATTACK_STATUS_LEN);
#else
    mavlink_attack_status_t packet;
    packet.attack_alt_error = attack_alt_error;
    packet.attack_hori_distance_error = attack_hori_distance_error;
    packet.attack_tilt_angle_error = attack_tilt_angle_error;
    packet.attack_xtrack_error = attack_xtrack_error;
    packet.attack_along_track_error = attack_along_track_error;
    packet.attack_mode = attack_mode;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ATTACK_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ATTACK_STATUS;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_ATTACK_STATUS_MIN_LEN, MAVLINK_MSG_ID_ATTACK_STATUS_LEN, MAVLINK_MSG_ID_ATTACK_STATUS_CRC);
}

/**
 * @brief Encode a attack_status struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param attack_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_attack_status_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_attack_status_t* attack_status)
{
    return mavlink_msg_attack_status_pack(system_id, component_id, msg, attack_status->attack_mode, attack_status->attack_alt_error, attack_status->attack_hori_distance_error, attack_status->attack_tilt_angle_error, attack_status->attack_xtrack_error, attack_status->attack_along_track_error);
}

/**
 * @brief Encode a attack_status struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param attack_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_attack_status_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_attack_status_t* attack_status)
{
    return mavlink_msg_attack_status_pack_chan(system_id, component_id, chan, msg, attack_status->attack_mode, attack_status->attack_alt_error, attack_status->attack_hori_distance_error, attack_status->attack_tilt_angle_error, attack_status->attack_xtrack_error, attack_status->attack_along_track_error);
}

/**
 * @brief Send a attack_status message
 * @param chan MAVLink channel to send the message
 *
 * @param attack_mode [m] Attacking mode
 * @param attack_alt_error [m] Altitude deviation(error)
 * @param attack_hori_distance_error [m] Horizontal deviation(error)
 * @param attack_tilt_angle_error [deg] Deviation(error) in tilt angle
 * @param attack_xtrack_error [m] Cross track error
 * @param attack_along_track_error [m] Along track error
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_attack_status_send(mavlink_channel_t chan, uint8_t attack_mode, float attack_alt_error, float attack_hori_distance_error, float attack_tilt_angle_error, float attack_xtrack_error, float attack_along_track_error)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ATTACK_STATUS_LEN];
    _mav_put_float(buf, 0, attack_alt_error);
    _mav_put_float(buf, 4, attack_hori_distance_error);
    _mav_put_float(buf, 8, attack_tilt_angle_error);
    _mav_put_float(buf, 12, attack_xtrack_error);
    _mav_put_float(buf, 16, attack_along_track_error);
    _mav_put_uint8_t(buf, 20, attack_mode);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ATTACK_STATUS, buf, MAVLINK_MSG_ID_ATTACK_STATUS_MIN_LEN, MAVLINK_MSG_ID_ATTACK_STATUS_LEN, MAVLINK_MSG_ID_ATTACK_STATUS_CRC);
#else
    mavlink_attack_status_t packet;
    packet.attack_alt_error = attack_alt_error;
    packet.attack_hori_distance_error = attack_hori_distance_error;
    packet.attack_tilt_angle_error = attack_tilt_angle_error;
    packet.attack_xtrack_error = attack_xtrack_error;
    packet.attack_along_track_error = attack_along_track_error;
    packet.attack_mode = attack_mode;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ATTACK_STATUS, (const char *)&packet, MAVLINK_MSG_ID_ATTACK_STATUS_MIN_LEN, MAVLINK_MSG_ID_ATTACK_STATUS_LEN, MAVLINK_MSG_ID_ATTACK_STATUS_CRC);
#endif
}

/**
 * @brief Send a attack_status message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_attack_status_send_struct(mavlink_channel_t chan, const mavlink_attack_status_t* attack_status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_attack_status_send(chan, attack_status->attack_mode, attack_status->attack_alt_error, attack_status->attack_hori_distance_error, attack_status->attack_tilt_angle_error, attack_status->attack_xtrack_error, attack_status->attack_along_track_error);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ATTACK_STATUS, (const char *)attack_status, MAVLINK_MSG_ID_ATTACK_STATUS_MIN_LEN, MAVLINK_MSG_ID_ATTACK_STATUS_LEN, MAVLINK_MSG_ID_ATTACK_STATUS_CRC);
#endif
}

#if MAVLINK_MSG_ID_ATTACK_STATUS_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_attack_status_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t attack_mode, float attack_alt_error, float attack_hori_distance_error, float attack_tilt_angle_error, float attack_xtrack_error, float attack_along_track_error)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, attack_alt_error);
    _mav_put_float(buf, 4, attack_hori_distance_error);
    _mav_put_float(buf, 8, attack_tilt_angle_error);
    _mav_put_float(buf, 12, attack_xtrack_error);
    _mav_put_float(buf, 16, attack_along_track_error);
    _mav_put_uint8_t(buf, 20, attack_mode);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ATTACK_STATUS, buf, MAVLINK_MSG_ID_ATTACK_STATUS_MIN_LEN, MAVLINK_MSG_ID_ATTACK_STATUS_LEN, MAVLINK_MSG_ID_ATTACK_STATUS_CRC);
#else
    mavlink_attack_status_t *packet = (mavlink_attack_status_t *)msgbuf;
    packet->attack_alt_error = attack_alt_error;
    packet->attack_hori_distance_error = attack_hori_distance_error;
    packet->attack_tilt_angle_error = attack_tilt_angle_error;
    packet->attack_xtrack_error = attack_xtrack_error;
    packet->attack_along_track_error = attack_along_track_error;
    packet->attack_mode = attack_mode;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ATTACK_STATUS, (const char *)packet, MAVLINK_MSG_ID_ATTACK_STATUS_MIN_LEN, MAVLINK_MSG_ID_ATTACK_STATUS_LEN, MAVLINK_MSG_ID_ATTACK_STATUS_CRC);
#endif
}
#endif

#endif

// MESSAGE ATTACK_STATUS UNPACKING


/**
 * @brief Get field attack_mode from attack_status message
 *
 * @return [m] Attacking mode
 */
static inline uint8_t mavlink_msg_attack_status_get_attack_mode(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  20);
}

/**
 * @brief Get field attack_alt_error from attack_status message
 *
 * @return [m] Altitude deviation(error)
 */
static inline float mavlink_msg_attack_status_get_attack_alt_error(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field attack_hori_distance_error from attack_status message
 *
 * @return [m] Horizontal deviation(error)
 */
static inline float mavlink_msg_attack_status_get_attack_hori_distance_error(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field attack_tilt_angle_error from attack_status message
 *
 * @return [deg] Deviation(error) in tilt angle
 */
static inline float mavlink_msg_attack_status_get_attack_tilt_angle_error(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field attack_xtrack_error from attack_status message
 *
 * @return [m] Cross track error
 */
static inline float mavlink_msg_attack_status_get_attack_xtrack_error(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field attack_along_track_error from attack_status message
 *
 * @return [m] Along track error
 */
static inline float mavlink_msg_attack_status_get_attack_along_track_error(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Decode a attack_status message into a struct
 *
 * @param msg The message to decode
 * @param attack_status C-struct to decode the message contents into
 */
static inline void mavlink_msg_attack_status_decode(const mavlink_message_t* msg, mavlink_attack_status_t* attack_status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    attack_status->attack_alt_error = mavlink_msg_attack_status_get_attack_alt_error(msg);
    attack_status->attack_hori_distance_error = mavlink_msg_attack_status_get_attack_hori_distance_error(msg);
    attack_status->attack_tilt_angle_error = mavlink_msg_attack_status_get_attack_tilt_angle_error(msg);
    attack_status->attack_xtrack_error = mavlink_msg_attack_status_get_attack_xtrack_error(msg);
    attack_status->attack_along_track_error = mavlink_msg_attack_status_get_attack_along_track_error(msg);
    attack_status->attack_mode = mavlink_msg_attack_status_get_attack_mode(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_ATTACK_STATUS_LEN? msg->len : MAVLINK_MSG_ID_ATTACK_STATUS_LEN;
        memset(attack_status, 0, MAVLINK_MSG_ID_ATTACK_STATUS_LEN);
    memcpy(attack_status, _MAV_PAYLOAD(msg), len);
#endif
}
