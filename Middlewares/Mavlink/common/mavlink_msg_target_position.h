#pragma once
// MESSAGE TARGET_POSITION PACKING

#define MAVLINK_MSG_ID_TARGET_POSITION 12930


typedef struct __mavlink_target_position_t {
 float distance; /*<  distance*/
 float latitude; /*<  latitude*/
 float longitude; /*<  longitude*/
 float altitude; /*<  altitude*/
} mavlink_target_position_t;

#define MAVLINK_MSG_ID_TARGET_POSITION_LEN 16
#define MAVLINK_MSG_ID_TARGET_POSITION_MIN_LEN 16
#define MAVLINK_MSG_ID_12930_LEN 16
#define MAVLINK_MSG_ID_12930_MIN_LEN 16

#define MAVLINK_MSG_ID_TARGET_POSITION_CRC 156
#define MAVLINK_MSG_ID_12930_CRC 156



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_TARGET_POSITION { \
    12930, \
    "TARGET_POSITION", \
    4, \
    {  { "distance", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_target_position_t, distance) }, \
         { "latitude", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_target_position_t, latitude) }, \
         { "longitude", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_target_position_t, longitude) }, \
         { "altitude", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_target_position_t, altitude) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_TARGET_POSITION { \
    "TARGET_POSITION", \
    4, \
    {  { "distance", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_target_position_t, distance) }, \
         { "latitude", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_target_position_t, latitude) }, \
         { "longitude", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_target_position_t, longitude) }, \
         { "altitude", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_target_position_t, altitude) }, \
         } \
}
#endif

/**
 * @brief Pack a target_position message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param distance  distance
 * @param latitude  latitude
 * @param longitude  longitude
 * @param altitude  altitude
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_target_position_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               float distance, float latitude, float longitude, float altitude)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_TARGET_POSITION_LEN];
    _mav_put_float(buf, 0, distance);
    _mav_put_float(buf, 4, latitude);
    _mav_put_float(buf, 8, longitude);
    _mav_put_float(buf, 12, altitude);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_TARGET_POSITION_LEN);
#else
    mavlink_target_position_t packet;
    packet.distance = distance;
    packet.latitude = latitude;
    packet.longitude = longitude;
    packet.altitude = altitude;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_TARGET_POSITION_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_TARGET_POSITION;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_TARGET_POSITION_MIN_LEN, MAVLINK_MSG_ID_TARGET_POSITION_LEN, MAVLINK_MSG_ID_TARGET_POSITION_CRC);
}

/**
 * @brief Pack a target_position message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param distance  distance
 * @param latitude  latitude
 * @param longitude  longitude
 * @param altitude  altitude
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_target_position_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   float distance,float latitude,float longitude,float altitude)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_TARGET_POSITION_LEN];
    _mav_put_float(buf, 0, distance);
    _mav_put_float(buf, 4, latitude);
    _mav_put_float(buf, 8, longitude);
    _mav_put_float(buf, 12, altitude);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_TARGET_POSITION_LEN);
#else
    mavlink_target_position_t packet;
    packet.distance = distance;
    packet.latitude = latitude;
    packet.longitude = longitude;
    packet.altitude = altitude;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_TARGET_POSITION_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_TARGET_POSITION;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_TARGET_POSITION_MIN_LEN, MAVLINK_MSG_ID_TARGET_POSITION_LEN, MAVLINK_MSG_ID_TARGET_POSITION_CRC);
}

/**
 * @brief Encode a target_position struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param target_position C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_target_position_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_target_position_t* target_position)
{
    return mavlink_msg_target_position_pack(system_id, component_id, msg, target_position->distance, target_position->latitude, target_position->longitude, target_position->altitude);
}

/**
 * @brief Encode a target_position struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param target_position C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_target_position_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_target_position_t* target_position)
{
    return mavlink_msg_target_position_pack_chan(system_id, component_id, chan, msg, target_position->distance, target_position->latitude, target_position->longitude, target_position->altitude);
}

/**
 * @brief Send a target_position message
 * @param chan MAVLink channel to send the message
 *
 * @param distance  distance
 * @param latitude  latitude
 * @param longitude  longitude
 * @param altitude  altitude
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_target_position_send(mavlink_channel_t chan, float distance, float latitude, float longitude, float altitude)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_TARGET_POSITION_LEN];
    _mav_put_float(buf, 0, distance);
    _mav_put_float(buf, 4, latitude);
    _mav_put_float(buf, 8, longitude);
    _mav_put_float(buf, 12, altitude);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_TARGET_POSITION, buf, MAVLINK_MSG_ID_TARGET_POSITION_MIN_LEN, MAVLINK_MSG_ID_TARGET_POSITION_LEN, MAVLINK_MSG_ID_TARGET_POSITION_CRC);
#else
    mavlink_target_position_t packet;
    packet.distance = distance;
    packet.latitude = latitude;
    packet.longitude = longitude;
    packet.altitude = altitude;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_TARGET_POSITION, (const char *)&packet, MAVLINK_MSG_ID_TARGET_POSITION_MIN_LEN, MAVLINK_MSG_ID_TARGET_POSITION_LEN, MAVLINK_MSG_ID_TARGET_POSITION_CRC);
#endif
}

/**
 * @brief Send a target_position message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_target_position_send_struct(mavlink_channel_t chan, const mavlink_target_position_t* target_position)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_target_position_send(chan, target_position->distance, target_position->latitude, target_position->longitude, target_position->altitude);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_TARGET_POSITION, (const char *)target_position, MAVLINK_MSG_ID_TARGET_POSITION_MIN_LEN, MAVLINK_MSG_ID_TARGET_POSITION_LEN, MAVLINK_MSG_ID_TARGET_POSITION_CRC);
#endif
}

#if MAVLINK_MSG_ID_TARGET_POSITION_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_target_position_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float distance, float latitude, float longitude, float altitude)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, distance);
    _mav_put_float(buf, 4, latitude);
    _mav_put_float(buf, 8, longitude);
    _mav_put_float(buf, 12, altitude);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_TARGET_POSITION, buf, MAVLINK_MSG_ID_TARGET_POSITION_MIN_LEN, MAVLINK_MSG_ID_TARGET_POSITION_LEN, MAVLINK_MSG_ID_TARGET_POSITION_CRC);
#else
    mavlink_target_position_t *packet = (mavlink_target_position_t *)msgbuf;
    packet->distance = distance;
    packet->latitude = latitude;
    packet->longitude = longitude;
    packet->altitude = altitude;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_TARGET_POSITION, (const char *)packet, MAVLINK_MSG_ID_TARGET_POSITION_MIN_LEN, MAVLINK_MSG_ID_TARGET_POSITION_LEN, MAVLINK_MSG_ID_TARGET_POSITION_CRC);
#endif
}
#endif

#endif

// MESSAGE TARGET_POSITION UNPACKING


/**
 * @brief Get field distance from target_position message
 *
 * @return  distance
 */
static inline float mavlink_msg_target_position_get_distance(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field latitude from target_position message
 *
 * @return  latitude
 */
static inline float mavlink_msg_target_position_get_latitude(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field longitude from target_position message
 *
 * @return  longitude
 */
static inline float mavlink_msg_target_position_get_longitude(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field altitude from target_position message
 *
 * @return  altitude
 */
static inline float mavlink_msg_target_position_get_altitude(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Decode a target_position message into a struct
 *
 * @param msg The message to decode
 * @param target_position C-struct to decode the message contents into
 */
static inline void mavlink_msg_target_position_decode(const mavlink_message_t* msg, mavlink_target_position_t* target_position)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    target_position->distance = mavlink_msg_target_position_get_distance(msg);
    target_position->latitude = mavlink_msg_target_position_get_latitude(msg);
    target_position->longitude = mavlink_msg_target_position_get_longitude(msg);
    target_position->altitude = mavlink_msg_target_position_get_altitude(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_TARGET_POSITION_LEN? msg->len : MAVLINK_MSG_ID_TARGET_POSITION_LEN;
        memset(target_position, 0, MAVLINK_MSG_ID_TARGET_POSITION_LEN);
    memcpy(target_position, _MAV_PAYLOAD(msg), len);
#endif
}
