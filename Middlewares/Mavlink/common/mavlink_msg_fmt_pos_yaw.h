#pragma once
// MESSAGE FMT_POS_YAW PACKING

#define MAVLINK_MSG_ID_FMT_POS_YAW 150


typedef struct __mavlink_fmt_pos_yaw_t {
 int32_t lat; /*< [degE7] Latitude*/
 int32_t lon; /*< [degE7] Longitude*/
 float vx; /*< [m/s]  velocity*/
 float vy; /*< [m/s]  velocity*/
 float alt; /*< [m] Altitude (MSL).*/
 float yaw; /*< [cdeg] yaw*/
 uint8_t sys_id; /*<  sys_id*/
 uint8_t node_type; /*<  tnode_type*/
 uint8_t team_id; /*<  team_id*/
} mavlink_fmt_pos_yaw_t;

#define MAVLINK_MSG_ID_FMT_POS_YAW_LEN 27
#define MAVLINK_MSG_ID_FMT_POS_YAW_MIN_LEN 27
#define MAVLINK_MSG_ID_150_LEN 27
#define MAVLINK_MSG_ID_150_MIN_LEN 27

#define MAVLINK_MSG_ID_FMT_POS_YAW_CRC 206
#define MAVLINK_MSG_ID_150_CRC 206



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_FMT_POS_YAW { \
    150, \
    "FMT_POS_YAW", \
    9, \
    {  { "lat", NULL, MAVLINK_TYPE_INT32_T, 0, 0, offsetof(mavlink_fmt_pos_yaw_t, lat) }, \
         { "lon", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_fmt_pos_yaw_t, lon) }, \
         { "vx", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_fmt_pos_yaw_t, vx) }, \
         { "vy", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_fmt_pos_yaw_t, vy) }, \
         { "alt", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_fmt_pos_yaw_t, alt) }, \
         { "yaw", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_fmt_pos_yaw_t, yaw) }, \
         { "sys_id", NULL, MAVLINK_TYPE_UINT8_T, 0, 24, offsetof(mavlink_fmt_pos_yaw_t, sys_id) }, \
         { "node_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 25, offsetof(mavlink_fmt_pos_yaw_t, node_type) }, \
         { "team_id", NULL, MAVLINK_TYPE_UINT8_T, 0, 26, offsetof(mavlink_fmt_pos_yaw_t, team_id) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_FMT_POS_YAW { \
    "FMT_POS_YAW", \
    9, \
    {  { "lat", NULL, MAVLINK_TYPE_INT32_T, 0, 0, offsetof(mavlink_fmt_pos_yaw_t, lat) }, \
         { "lon", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_fmt_pos_yaw_t, lon) }, \
         { "vx", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_fmt_pos_yaw_t, vx) }, \
         { "vy", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_fmt_pos_yaw_t, vy) }, \
         { "alt", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_fmt_pos_yaw_t, alt) }, \
         { "yaw", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_fmt_pos_yaw_t, yaw) }, \
         { "sys_id", NULL, MAVLINK_TYPE_UINT8_T, 0, 24, offsetof(mavlink_fmt_pos_yaw_t, sys_id) }, \
         { "node_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 25, offsetof(mavlink_fmt_pos_yaw_t, node_type) }, \
         { "team_id", NULL, MAVLINK_TYPE_UINT8_T, 0, 26, offsetof(mavlink_fmt_pos_yaw_t, team_id) }, \
         } \
}
#endif

/**
 * @brief Pack a fmt_pos_yaw message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param lat [degE7] Latitude
 * @param lon [degE7] Longitude
 * @param vx [m/s]  velocity
 * @param vy [m/s]  velocity
 * @param alt [m] Altitude (MSL).
 * @param yaw [cdeg] yaw
 * @param sys_id  sys_id
 * @param node_type  tnode_type
 * @param team_id  team_id
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_fmt_pos_yaw_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               int32_t lat, int32_t lon, float vx, float vy, float alt, float yaw, uint8_t sys_id, uint8_t node_type, uint8_t team_id)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_FMT_POS_YAW_LEN];
    _mav_put_int32_t(buf, 0, lat);
    _mav_put_int32_t(buf, 4, lon);
    _mav_put_float(buf, 8, vx);
    _mav_put_float(buf, 12, vy);
    _mav_put_float(buf, 16, alt);
    _mav_put_float(buf, 20, yaw);
    _mav_put_uint8_t(buf, 24, sys_id);
    _mav_put_uint8_t(buf, 25, node_type);
    _mav_put_uint8_t(buf, 26, team_id);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_FMT_POS_YAW_LEN);
#else
    mavlink_fmt_pos_yaw_t packet;
    packet.lat = lat;
    packet.lon = lon;
    packet.vx = vx;
    packet.vy = vy;
    packet.alt = alt;
    packet.yaw = yaw;
    packet.sys_id = sys_id;
    packet.node_type = node_type;
    packet.team_id = team_id;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_FMT_POS_YAW_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_FMT_POS_YAW;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_FMT_POS_YAW_MIN_LEN, MAVLINK_MSG_ID_FMT_POS_YAW_LEN, MAVLINK_MSG_ID_FMT_POS_YAW_CRC);
}

/**
 * @brief Pack a fmt_pos_yaw message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param lat [degE7] Latitude
 * @param lon [degE7] Longitude
 * @param vx [m/s]  velocity
 * @param vy [m/s]  velocity
 * @param alt [m] Altitude (MSL).
 * @param yaw [cdeg] yaw
 * @param sys_id  sys_id
 * @param node_type  tnode_type
 * @param team_id  team_id
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_fmt_pos_yaw_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   int32_t lat,int32_t lon,float vx,float vy,float alt,float yaw,uint8_t sys_id,uint8_t node_type,uint8_t team_id)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_FMT_POS_YAW_LEN];
    _mav_put_int32_t(buf, 0, lat);
    _mav_put_int32_t(buf, 4, lon);
    _mav_put_float(buf, 8, vx);
    _mav_put_float(buf, 12, vy);
    _mav_put_float(buf, 16, alt);
    _mav_put_float(buf, 20, yaw);
    _mav_put_uint8_t(buf, 24, sys_id);
    _mav_put_uint8_t(buf, 25, node_type);
    _mav_put_uint8_t(buf, 26, team_id);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_FMT_POS_YAW_LEN);
#else
    mavlink_fmt_pos_yaw_t packet;
    packet.lat = lat;
    packet.lon = lon;
    packet.vx = vx;
    packet.vy = vy;
    packet.alt = alt;
    packet.yaw = yaw;
    packet.sys_id = sys_id;
    packet.node_type = node_type;
    packet.team_id = team_id;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_FMT_POS_YAW_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_FMT_POS_YAW;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_FMT_POS_YAW_MIN_LEN, MAVLINK_MSG_ID_FMT_POS_YAW_LEN, MAVLINK_MSG_ID_FMT_POS_YAW_CRC);
}

/**
 * @brief Encode a fmt_pos_yaw struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param fmt_pos_yaw C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_fmt_pos_yaw_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_fmt_pos_yaw_t* fmt_pos_yaw)
{
    return mavlink_msg_fmt_pos_yaw_pack(system_id, component_id, msg, fmt_pos_yaw->lat, fmt_pos_yaw->lon, fmt_pos_yaw->vx, fmt_pos_yaw->vy, fmt_pos_yaw->alt, fmt_pos_yaw->yaw, fmt_pos_yaw->sys_id, fmt_pos_yaw->node_type, fmt_pos_yaw->team_id);
}

/**
 * @brief Encode a fmt_pos_yaw struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param fmt_pos_yaw C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_fmt_pos_yaw_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_fmt_pos_yaw_t* fmt_pos_yaw)
{
    return mavlink_msg_fmt_pos_yaw_pack_chan(system_id, component_id, chan, msg, fmt_pos_yaw->lat, fmt_pos_yaw->lon, fmt_pos_yaw->vx, fmt_pos_yaw->vy, fmt_pos_yaw->alt, fmt_pos_yaw->yaw, fmt_pos_yaw->sys_id, fmt_pos_yaw->node_type, fmt_pos_yaw->team_id);
}

/**
 * @brief Send a fmt_pos_yaw message
 * @param chan MAVLink channel to send the message
 *
 * @param lat [degE7] Latitude
 * @param lon [degE7] Longitude
 * @param vx [m/s]  velocity
 * @param vy [m/s]  velocity
 * @param alt [m] Altitude (MSL).
 * @param yaw [cdeg] yaw
 * @param sys_id  sys_id
 * @param node_type  tnode_type
 * @param team_id  team_id
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_fmt_pos_yaw_send(mavlink_channel_t chan, int32_t lat, int32_t lon, float vx, float vy, float alt, float yaw, uint8_t sys_id, uint8_t node_type, uint8_t team_id)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_FMT_POS_YAW_LEN];
    _mav_put_int32_t(buf, 0, lat);
    _mav_put_int32_t(buf, 4, lon);
    _mav_put_float(buf, 8, vx);
    _mav_put_float(buf, 12, vy);
    _mav_put_float(buf, 16, alt);
    _mav_put_float(buf, 20, yaw);
    _mav_put_uint8_t(buf, 24, sys_id);
    _mav_put_uint8_t(buf, 25, node_type);
    _mav_put_uint8_t(buf, 26, team_id);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FMT_POS_YAW, buf, MAVLINK_MSG_ID_FMT_POS_YAW_MIN_LEN, MAVLINK_MSG_ID_FMT_POS_YAW_LEN, MAVLINK_MSG_ID_FMT_POS_YAW_CRC);
#else
    mavlink_fmt_pos_yaw_t packet;
    packet.lat = lat;
    packet.lon = lon;
    packet.vx = vx;
    packet.vy = vy;
    packet.alt = alt;
    packet.yaw = yaw;
    packet.sys_id = sys_id;
    packet.node_type = node_type;
    packet.team_id = team_id;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FMT_POS_YAW, (const char *)&packet, MAVLINK_MSG_ID_FMT_POS_YAW_MIN_LEN, MAVLINK_MSG_ID_FMT_POS_YAW_LEN, MAVLINK_MSG_ID_FMT_POS_YAW_CRC);
#endif
}

/**
 * @brief Send a fmt_pos_yaw message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_fmt_pos_yaw_send_struct(mavlink_channel_t chan, const mavlink_fmt_pos_yaw_t* fmt_pos_yaw)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_fmt_pos_yaw_send(chan, fmt_pos_yaw->lat, fmt_pos_yaw->lon, fmt_pos_yaw->vx, fmt_pos_yaw->vy, fmt_pos_yaw->alt, fmt_pos_yaw->yaw, fmt_pos_yaw->sys_id, fmt_pos_yaw->node_type, fmt_pos_yaw->team_id);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FMT_POS_YAW, (const char *)fmt_pos_yaw, MAVLINK_MSG_ID_FMT_POS_YAW_MIN_LEN, MAVLINK_MSG_ID_FMT_POS_YAW_LEN, MAVLINK_MSG_ID_FMT_POS_YAW_CRC);
#endif
}

#if MAVLINK_MSG_ID_FMT_POS_YAW_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_fmt_pos_yaw_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  int32_t lat, int32_t lon, float vx, float vy, float alt, float yaw, uint8_t sys_id, uint8_t node_type, uint8_t team_id)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_int32_t(buf, 0, lat);
    _mav_put_int32_t(buf, 4, lon);
    _mav_put_float(buf, 8, vx);
    _mav_put_float(buf, 12, vy);
    _mav_put_float(buf, 16, alt);
    _mav_put_float(buf, 20, yaw);
    _mav_put_uint8_t(buf, 24, sys_id);
    _mav_put_uint8_t(buf, 25, node_type);
    _mav_put_uint8_t(buf, 26, team_id);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FMT_POS_YAW, buf, MAVLINK_MSG_ID_FMT_POS_YAW_MIN_LEN, MAVLINK_MSG_ID_FMT_POS_YAW_LEN, MAVLINK_MSG_ID_FMT_POS_YAW_CRC);
#else
    mavlink_fmt_pos_yaw_t *packet = (mavlink_fmt_pos_yaw_t *)msgbuf;
    packet->lat = lat;
    packet->lon = lon;
    packet->vx = vx;
    packet->vy = vy;
    packet->alt = alt;
    packet->yaw = yaw;
    packet->sys_id = sys_id;
    packet->node_type = node_type;
    packet->team_id = team_id;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FMT_POS_YAW, (const char *)packet, MAVLINK_MSG_ID_FMT_POS_YAW_MIN_LEN, MAVLINK_MSG_ID_FMT_POS_YAW_LEN, MAVLINK_MSG_ID_FMT_POS_YAW_CRC);
#endif
}
#endif

#endif

// MESSAGE FMT_POS_YAW UNPACKING


/**
 * @brief Get field lat from fmt_pos_yaw message
 *
 * @return [degE7] Latitude
 */
static inline int32_t mavlink_msg_fmt_pos_yaw_get_lat(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  0);
}

/**
 * @brief Get field lon from fmt_pos_yaw message
 *
 * @return [degE7] Longitude
 */
static inline int32_t mavlink_msg_fmt_pos_yaw_get_lon(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  4);
}

/**
 * @brief Get field vx from fmt_pos_yaw message
 *
 * @return [m/s]  velocity
 */
static inline float mavlink_msg_fmt_pos_yaw_get_vx(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field vy from fmt_pos_yaw message
 *
 * @return [m/s]  velocity
 */
static inline float mavlink_msg_fmt_pos_yaw_get_vy(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field alt from fmt_pos_yaw message
 *
 * @return [m] Altitude (MSL).
 */
static inline float mavlink_msg_fmt_pos_yaw_get_alt(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Get field yaw from fmt_pos_yaw message
 *
 * @return [cdeg] yaw
 */
static inline float mavlink_msg_fmt_pos_yaw_get_yaw(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  20);
}

/**
 * @brief Get field sys_id from fmt_pos_yaw message
 *
 * @return  sys_id
 */
static inline uint8_t mavlink_msg_fmt_pos_yaw_get_sys_id(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  24);
}

/**
 * @brief Get field node_type from fmt_pos_yaw message
 *
 * @return  tnode_type
 */
static inline uint8_t mavlink_msg_fmt_pos_yaw_get_node_type(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  25);
}

/**
 * @brief Get field team_id from fmt_pos_yaw message
 *
 * @return  team_id
 */
static inline uint8_t mavlink_msg_fmt_pos_yaw_get_team_id(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  26);
}

/**
 * @brief Decode a fmt_pos_yaw message into a struct
 *
 * @param msg The message to decode
 * @param fmt_pos_yaw C-struct to decode the message contents into
 */
static inline void mavlink_msg_fmt_pos_yaw_decode(const mavlink_message_t* msg, mavlink_fmt_pos_yaw_t* fmt_pos_yaw)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    fmt_pos_yaw->lat = mavlink_msg_fmt_pos_yaw_get_lat(msg);
    fmt_pos_yaw->lon = mavlink_msg_fmt_pos_yaw_get_lon(msg);
    fmt_pos_yaw->vx = mavlink_msg_fmt_pos_yaw_get_vx(msg);
    fmt_pos_yaw->vy = mavlink_msg_fmt_pos_yaw_get_vy(msg);
    fmt_pos_yaw->alt = mavlink_msg_fmt_pos_yaw_get_alt(msg);
    fmt_pos_yaw->yaw = mavlink_msg_fmt_pos_yaw_get_yaw(msg);
    fmt_pos_yaw->sys_id = mavlink_msg_fmt_pos_yaw_get_sys_id(msg);
    fmt_pos_yaw->node_type = mavlink_msg_fmt_pos_yaw_get_node_type(msg);
    fmt_pos_yaw->team_id = mavlink_msg_fmt_pos_yaw_get_team_id(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_FMT_POS_YAW_LEN? msg->len : MAVLINK_MSG_ID_FMT_POS_YAW_LEN;
        memset(fmt_pos_yaw, 0, MAVLINK_MSG_ID_FMT_POS_YAW_LEN);
    memcpy(fmt_pos_yaw, _MAV_PAYLOAD(msg), len);
#endif
}
