#pragma once
// MESSAGE FMT_GLOBAL_POS PACKING

#define MAVLINK_MSG_ID_FMT_GLOBAL_POS 12003


typedef struct __mavlink_fmt_global_pos_t {
 uint32_t time_boot_ms; /*< [us] Timestamp (time since system boot).*/
 int32_t lat; /*< [degE7] Latitude*/
 int32_t lon; /*< [degE7] Longitude*/
 float alt; /*< [m] Altitude (MSL).*/
 float relative_alt; /*< [m] Ground x position.*/
 float vx; /*< [m] vx.*/
 float vy; /*< [m] vy.*/
 float vz; /*< [m/s] vz*/
 float hdg; /*< [cdeg] hdg */
 float yaw_rate; /*< [cdeg/s] Radian Speed*/
 float roll; /*< [cdeg] roll.*/
 float pitch; /*< [cdeg] pitch.*/
 float yaw; /*< [cdeg] yaw*/
 float speed; /*< [m/s] Ground Speed*/
 uint16_t sys_id; /*<  sys_id*/
 uint16_t node_type; /*<  tnode_type*/
 uint16_t team_id; /*<  team_id*/
} mavlink_fmt_global_pos_t;

#define MAVLINK_MSG_ID_FMT_GLOBAL_POS_LEN 62
#define MAVLINK_MSG_ID_FMT_GLOBAL_POS_MIN_LEN 62
#define MAVLINK_MSG_ID_12003_LEN 62
#define MAVLINK_MSG_ID_12003_MIN_LEN 62

#define MAVLINK_MSG_ID_FMT_GLOBAL_POS_CRC 22
#define MAVLINK_MSG_ID_12003_CRC 22



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_FMT_GLOBAL_POS { \
    12003, \
    "FMT_GLOBAL_POS", \
    17, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_fmt_global_pos_t, time_boot_ms) }, \
         { "lat", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_fmt_global_pos_t, lat) }, \
         { "lon", NULL, MAVLINK_TYPE_INT32_T, 0, 8, offsetof(mavlink_fmt_global_pos_t, lon) }, \
         { "alt", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_fmt_global_pos_t, alt) }, \
         { "relative_alt", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_fmt_global_pos_t, relative_alt) }, \
         { "vx", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_fmt_global_pos_t, vx) }, \
         { "vy", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_fmt_global_pos_t, vy) }, \
         { "vz", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_fmt_global_pos_t, vz) }, \
         { "hdg", NULL, MAVLINK_TYPE_FLOAT, 0, 32, offsetof(mavlink_fmt_global_pos_t, hdg) }, \
         { "yaw_rate", NULL, MAVLINK_TYPE_FLOAT, 0, 36, offsetof(mavlink_fmt_global_pos_t, yaw_rate) }, \
         { "roll", NULL, MAVLINK_TYPE_FLOAT, 0, 40, offsetof(mavlink_fmt_global_pos_t, roll) }, \
         { "pitch", NULL, MAVLINK_TYPE_FLOAT, 0, 44, offsetof(mavlink_fmt_global_pos_t, pitch) }, \
         { "yaw", NULL, MAVLINK_TYPE_FLOAT, 0, 48, offsetof(mavlink_fmt_global_pos_t, yaw) }, \
         { "speed", NULL, MAVLINK_TYPE_FLOAT, 0, 52, offsetof(mavlink_fmt_global_pos_t, speed) }, \
         { "sys_id", NULL, MAVLINK_TYPE_UINT16_T, 0, 56, offsetof(mavlink_fmt_global_pos_t, sys_id) }, \
         { "node_type", NULL, MAVLINK_TYPE_UINT16_T, 0, 58, offsetof(mavlink_fmt_global_pos_t, node_type) }, \
         { "team_id", NULL, MAVLINK_TYPE_UINT16_T, 0, 60, offsetof(mavlink_fmt_global_pos_t, team_id) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_FMT_GLOBAL_POS { \
    "FMT_GLOBAL_POS", \
    17, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_fmt_global_pos_t, time_boot_ms) }, \
         { "lat", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_fmt_global_pos_t, lat) }, \
         { "lon", NULL, MAVLINK_TYPE_INT32_T, 0, 8, offsetof(mavlink_fmt_global_pos_t, lon) }, \
         { "alt", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_fmt_global_pos_t, alt) }, \
         { "relative_alt", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_fmt_global_pos_t, relative_alt) }, \
         { "vx", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_fmt_global_pos_t, vx) }, \
         { "vy", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_fmt_global_pos_t, vy) }, \
         { "vz", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_fmt_global_pos_t, vz) }, \
         { "hdg", NULL, MAVLINK_TYPE_FLOAT, 0, 32, offsetof(mavlink_fmt_global_pos_t, hdg) }, \
         { "yaw_rate", NULL, MAVLINK_TYPE_FLOAT, 0, 36, offsetof(mavlink_fmt_global_pos_t, yaw_rate) }, \
         { "roll", NULL, MAVLINK_TYPE_FLOAT, 0, 40, offsetof(mavlink_fmt_global_pos_t, roll) }, \
         { "pitch", NULL, MAVLINK_TYPE_FLOAT, 0, 44, offsetof(mavlink_fmt_global_pos_t, pitch) }, \
         { "yaw", NULL, MAVLINK_TYPE_FLOAT, 0, 48, offsetof(mavlink_fmt_global_pos_t, yaw) }, \
         { "speed", NULL, MAVLINK_TYPE_FLOAT, 0, 52, offsetof(mavlink_fmt_global_pos_t, speed) }, \
         { "sys_id", NULL, MAVLINK_TYPE_UINT16_T, 0, 56, offsetof(mavlink_fmt_global_pos_t, sys_id) }, \
         { "node_type", NULL, MAVLINK_TYPE_UINT16_T, 0, 58, offsetof(mavlink_fmt_global_pos_t, node_type) }, \
         { "team_id", NULL, MAVLINK_TYPE_UINT16_T, 0, 60, offsetof(mavlink_fmt_global_pos_t, team_id) }, \
         } \
}
#endif

/**
 * @brief Pack a fmt_global_pos message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [us] Timestamp (time since system boot).
 * @param lat [degE7] Latitude
 * @param lon [degE7] Longitude
 * @param alt [m] Altitude (MSL).
 * @param relative_alt [m] Ground x position.
 * @param vx [m] vx.
 * @param vy [m] vy.
 * @param vz [m/s] vz
 * @param hdg [cdeg] hdg 
 * @param yaw_rate [cdeg/s] Radian Speed
 * @param roll [cdeg] roll.
 * @param pitch [cdeg] pitch.
 * @param yaw [cdeg] yaw
 * @param speed [m/s] Ground Speed
 * @param sys_id  sys_id
 * @param node_type  tnode_type
 * @param team_id  team_id
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_fmt_global_pos_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t time_boot_ms, int32_t lat, int32_t lon, float alt, float relative_alt, float vx, float vy, float vz, float hdg, float yaw_rate, float roll, float pitch, float yaw, float speed, uint16_t sys_id, uint16_t node_type, uint16_t team_id)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_FMT_GLOBAL_POS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int32_t(buf, 4, lat);
    _mav_put_int32_t(buf, 8, lon);
    _mav_put_float(buf, 12, alt);
    _mav_put_float(buf, 16, relative_alt);
    _mav_put_float(buf, 20, vx);
    _mav_put_float(buf, 24, vy);
    _mav_put_float(buf, 28, vz);
    _mav_put_float(buf, 32, hdg);
    _mav_put_float(buf, 36, yaw_rate);
    _mav_put_float(buf, 40, roll);
    _mav_put_float(buf, 44, pitch);
    _mav_put_float(buf, 48, yaw);
    _mav_put_float(buf, 52, speed);
    _mav_put_uint16_t(buf, 56, sys_id);
    _mav_put_uint16_t(buf, 58, node_type);
    _mav_put_uint16_t(buf, 60, team_id);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_FMT_GLOBAL_POS_LEN);
#else
    mavlink_fmt_global_pos_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.lat = lat;
    packet.lon = lon;
    packet.alt = alt;
    packet.relative_alt = relative_alt;
    packet.vx = vx;
    packet.vy = vy;
    packet.vz = vz;
    packet.hdg = hdg;
    packet.yaw_rate = yaw_rate;
    packet.roll = roll;
    packet.pitch = pitch;
    packet.yaw = yaw;
    packet.speed = speed;
    packet.sys_id = sys_id;
    packet.node_type = node_type;
    packet.team_id = team_id;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_FMT_GLOBAL_POS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_FMT_GLOBAL_POS;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_FMT_GLOBAL_POS_MIN_LEN, MAVLINK_MSG_ID_FMT_GLOBAL_POS_LEN, MAVLINK_MSG_ID_FMT_GLOBAL_POS_CRC);
}

/**
 * @brief Pack a fmt_global_pos message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_boot_ms [us] Timestamp (time since system boot).
 * @param lat [degE7] Latitude
 * @param lon [degE7] Longitude
 * @param alt [m] Altitude (MSL).
 * @param relative_alt [m] Ground x position.
 * @param vx [m] vx.
 * @param vy [m] vy.
 * @param vz [m/s] vz
 * @param hdg [cdeg] hdg 
 * @param yaw_rate [cdeg/s] Radian Speed
 * @param roll [cdeg] roll.
 * @param pitch [cdeg] pitch.
 * @param yaw [cdeg] yaw
 * @param speed [m/s] Ground Speed
 * @param sys_id  sys_id
 * @param node_type  tnode_type
 * @param team_id  team_id
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_fmt_global_pos_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t time_boot_ms,int32_t lat,int32_t lon,float alt,float relative_alt,float vx,float vy,float vz,float hdg,float yaw_rate,float roll,float pitch,float yaw,float speed,uint16_t sys_id,uint16_t node_type,uint16_t team_id)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_FMT_GLOBAL_POS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int32_t(buf, 4, lat);
    _mav_put_int32_t(buf, 8, lon);
    _mav_put_float(buf, 12, alt);
    _mav_put_float(buf, 16, relative_alt);
    _mav_put_float(buf, 20, vx);
    _mav_put_float(buf, 24, vy);
    _mav_put_float(buf, 28, vz);
    _mav_put_float(buf, 32, hdg);
    _mav_put_float(buf, 36, yaw_rate);
    _mav_put_float(buf, 40, roll);
    _mav_put_float(buf, 44, pitch);
    _mav_put_float(buf, 48, yaw);
    _mav_put_float(buf, 52, speed);
    _mav_put_uint16_t(buf, 56, sys_id);
    _mav_put_uint16_t(buf, 58, node_type);
    _mav_put_uint16_t(buf, 60, team_id);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_FMT_GLOBAL_POS_LEN);
#else
    mavlink_fmt_global_pos_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.lat = lat;
    packet.lon = lon;
    packet.alt = alt;
    packet.relative_alt = relative_alt;
    packet.vx = vx;
    packet.vy = vy;
    packet.vz = vz;
    packet.hdg = hdg;
    packet.yaw_rate = yaw_rate;
    packet.roll = roll;
    packet.pitch = pitch;
    packet.yaw = yaw;
    packet.speed = speed;
    packet.sys_id = sys_id;
    packet.node_type = node_type;
    packet.team_id = team_id;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_FMT_GLOBAL_POS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_FMT_GLOBAL_POS;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_FMT_GLOBAL_POS_MIN_LEN, MAVLINK_MSG_ID_FMT_GLOBAL_POS_LEN, MAVLINK_MSG_ID_FMT_GLOBAL_POS_CRC);
}

/**
 * @brief Encode a fmt_global_pos struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param fmt_global_pos C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_fmt_global_pos_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_fmt_global_pos_t* fmt_global_pos)
{
    return mavlink_msg_fmt_global_pos_pack(system_id, component_id, msg, fmt_global_pos->time_boot_ms, fmt_global_pos->lat, fmt_global_pos->lon, fmt_global_pos->alt, fmt_global_pos->relative_alt, fmt_global_pos->vx, fmt_global_pos->vy, fmt_global_pos->vz, fmt_global_pos->hdg, fmt_global_pos->yaw_rate, fmt_global_pos->roll, fmt_global_pos->pitch, fmt_global_pos->yaw, fmt_global_pos->speed, fmt_global_pos->sys_id, fmt_global_pos->node_type, fmt_global_pos->team_id);
}

/**
 * @brief Encode a fmt_global_pos struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param fmt_global_pos C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_fmt_global_pos_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_fmt_global_pos_t* fmt_global_pos)
{
    return mavlink_msg_fmt_global_pos_pack_chan(system_id, component_id, chan, msg, fmt_global_pos->time_boot_ms, fmt_global_pos->lat, fmt_global_pos->lon, fmt_global_pos->alt, fmt_global_pos->relative_alt, fmt_global_pos->vx, fmt_global_pos->vy, fmt_global_pos->vz, fmt_global_pos->hdg, fmt_global_pos->yaw_rate, fmt_global_pos->roll, fmt_global_pos->pitch, fmt_global_pos->yaw, fmt_global_pos->speed, fmt_global_pos->sys_id, fmt_global_pos->node_type, fmt_global_pos->team_id);
}

/**
 * @brief Send a fmt_global_pos message
 * @param chan MAVLink channel to send the message
 *
 * @param time_boot_ms [us] Timestamp (time since system boot).
 * @param lat [degE7] Latitude
 * @param lon [degE7] Longitude
 * @param alt [m] Altitude (MSL).
 * @param relative_alt [m] Ground x position.
 * @param vx [m] vx.
 * @param vy [m] vy.
 * @param vz [m/s] vz
 * @param hdg [cdeg] hdg 
 * @param yaw_rate [cdeg/s] Radian Speed
 * @param roll [cdeg] roll.
 * @param pitch [cdeg] pitch.
 * @param yaw [cdeg] yaw
 * @param speed [m/s] Ground Speed
 * @param sys_id  sys_id
 * @param node_type  tnode_type
 * @param team_id  team_id
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_fmt_global_pos_send(mavlink_channel_t chan, uint32_t time_boot_ms, int32_t lat, int32_t lon, float alt, float relative_alt, float vx, float vy, float vz, float hdg, float yaw_rate, float roll, float pitch, float yaw, float speed, uint16_t sys_id, uint16_t node_type, uint16_t team_id)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_FMT_GLOBAL_POS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int32_t(buf, 4, lat);
    _mav_put_int32_t(buf, 8, lon);
    _mav_put_float(buf, 12, alt);
    _mav_put_float(buf, 16, relative_alt);
    _mav_put_float(buf, 20, vx);
    _mav_put_float(buf, 24, vy);
    _mav_put_float(buf, 28, vz);
    _mav_put_float(buf, 32, hdg);
    _mav_put_float(buf, 36, yaw_rate);
    _mav_put_float(buf, 40, roll);
    _mav_put_float(buf, 44, pitch);
    _mav_put_float(buf, 48, yaw);
    _mav_put_float(buf, 52, speed);
    _mav_put_uint16_t(buf, 56, sys_id);
    _mav_put_uint16_t(buf, 58, node_type);
    _mav_put_uint16_t(buf, 60, team_id);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FMT_GLOBAL_POS, buf, MAVLINK_MSG_ID_FMT_GLOBAL_POS_MIN_LEN, MAVLINK_MSG_ID_FMT_GLOBAL_POS_LEN, MAVLINK_MSG_ID_FMT_GLOBAL_POS_CRC);
#else
    mavlink_fmt_global_pos_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.lat = lat;
    packet.lon = lon;
    packet.alt = alt;
    packet.relative_alt = relative_alt;
    packet.vx = vx;
    packet.vy = vy;
    packet.vz = vz;
    packet.hdg = hdg;
    packet.yaw_rate = yaw_rate;
    packet.roll = roll;
    packet.pitch = pitch;
    packet.yaw = yaw;
    packet.speed = speed;
    packet.sys_id = sys_id;
    packet.node_type = node_type;
    packet.team_id = team_id;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FMT_GLOBAL_POS, (const char *)&packet, MAVLINK_MSG_ID_FMT_GLOBAL_POS_MIN_LEN, MAVLINK_MSG_ID_FMT_GLOBAL_POS_LEN, MAVLINK_MSG_ID_FMT_GLOBAL_POS_CRC);
#endif
}

/**
 * @brief Send a fmt_global_pos message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_fmt_global_pos_send_struct(mavlink_channel_t chan, const mavlink_fmt_global_pos_t* fmt_global_pos)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_fmt_global_pos_send(chan, fmt_global_pos->time_boot_ms, fmt_global_pos->lat, fmt_global_pos->lon, fmt_global_pos->alt, fmt_global_pos->relative_alt, fmt_global_pos->vx, fmt_global_pos->vy, fmt_global_pos->vz, fmt_global_pos->hdg, fmt_global_pos->yaw_rate, fmt_global_pos->roll, fmt_global_pos->pitch, fmt_global_pos->yaw, fmt_global_pos->speed, fmt_global_pos->sys_id, fmt_global_pos->node_type, fmt_global_pos->team_id);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FMT_GLOBAL_POS, (const char *)fmt_global_pos, MAVLINK_MSG_ID_FMT_GLOBAL_POS_MIN_LEN, MAVLINK_MSG_ID_FMT_GLOBAL_POS_LEN, MAVLINK_MSG_ID_FMT_GLOBAL_POS_CRC);
#endif
}

#if MAVLINK_MSG_ID_FMT_GLOBAL_POS_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_fmt_global_pos_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_boot_ms, int32_t lat, int32_t lon, float alt, float relative_alt, float vx, float vy, float vz, float hdg, float yaw_rate, float roll, float pitch, float yaw, float speed, uint16_t sys_id, uint16_t node_type, uint16_t team_id)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int32_t(buf, 4, lat);
    _mav_put_int32_t(buf, 8, lon);
    _mav_put_float(buf, 12, alt);
    _mav_put_float(buf, 16, relative_alt);
    _mav_put_float(buf, 20, vx);
    _mav_put_float(buf, 24, vy);
    _mav_put_float(buf, 28, vz);
    _mav_put_float(buf, 32, hdg);
    _mav_put_float(buf, 36, yaw_rate);
    _mav_put_float(buf, 40, roll);
    _mav_put_float(buf, 44, pitch);
    _mav_put_float(buf, 48, yaw);
    _mav_put_float(buf, 52, speed);
    _mav_put_uint16_t(buf, 56, sys_id);
    _mav_put_uint16_t(buf, 58, node_type);
    _mav_put_uint16_t(buf, 60, team_id);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FMT_GLOBAL_POS, buf, MAVLINK_MSG_ID_FMT_GLOBAL_POS_MIN_LEN, MAVLINK_MSG_ID_FMT_GLOBAL_POS_LEN, MAVLINK_MSG_ID_FMT_GLOBAL_POS_CRC);
#else
    mavlink_fmt_global_pos_t *packet = (mavlink_fmt_global_pos_t *)msgbuf;
    packet->time_boot_ms = time_boot_ms;
    packet->lat = lat;
    packet->lon = lon;
    packet->alt = alt;
    packet->relative_alt = relative_alt;
    packet->vx = vx;
    packet->vy = vy;
    packet->vz = vz;
    packet->hdg = hdg;
    packet->yaw_rate = yaw_rate;
    packet->roll = roll;
    packet->pitch = pitch;
    packet->yaw = yaw;
    packet->speed = speed;
    packet->sys_id = sys_id;
    packet->node_type = node_type;
    packet->team_id = team_id;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FMT_GLOBAL_POS, (const char *)packet, MAVLINK_MSG_ID_FMT_GLOBAL_POS_MIN_LEN, MAVLINK_MSG_ID_FMT_GLOBAL_POS_LEN, MAVLINK_MSG_ID_FMT_GLOBAL_POS_CRC);
#endif
}
#endif

#endif

// MESSAGE FMT_GLOBAL_POS UNPACKING


/**
 * @brief Get field time_boot_ms from fmt_global_pos message
 *
 * @return [us] Timestamp (time since system boot).
 */
static inline uint32_t mavlink_msg_fmt_global_pos_get_time_boot_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field lat from fmt_global_pos message
 *
 * @return [degE7] Latitude
 */
static inline int32_t mavlink_msg_fmt_global_pos_get_lat(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  4);
}

/**
 * @brief Get field lon from fmt_global_pos message
 *
 * @return [degE7] Longitude
 */
static inline int32_t mavlink_msg_fmt_global_pos_get_lon(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  8);
}

/**
 * @brief Get field alt from fmt_global_pos message
 *
 * @return [m] Altitude (MSL).
 */
static inline float mavlink_msg_fmt_global_pos_get_alt(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field relative_alt from fmt_global_pos message
 *
 * @return [m] Ground x position.
 */
static inline float mavlink_msg_fmt_global_pos_get_relative_alt(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Get field vx from fmt_global_pos message
 *
 * @return [m] vx.
 */
static inline float mavlink_msg_fmt_global_pos_get_vx(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  20);
}

/**
 * @brief Get field vy from fmt_global_pos message
 *
 * @return [m] vy.
 */
static inline float mavlink_msg_fmt_global_pos_get_vy(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  24);
}

/**
 * @brief Get field vz from fmt_global_pos message
 *
 * @return [m/s] vz
 */
static inline float mavlink_msg_fmt_global_pos_get_vz(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  28);
}

/**
 * @brief Get field hdg from fmt_global_pos message
 *
 * @return [cdeg] hdg 
 */
static inline float mavlink_msg_fmt_global_pos_get_hdg(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  32);
}

/**
 * @brief Get field yaw_rate from fmt_global_pos message
 *
 * @return [cdeg/s] Radian Speed
 */
static inline float mavlink_msg_fmt_global_pos_get_yaw_rate(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  36);
}

/**
 * @brief Get field roll from fmt_global_pos message
 *
 * @return [cdeg] roll.
 */
static inline float mavlink_msg_fmt_global_pos_get_roll(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  40);
}

/**
 * @brief Get field pitch from fmt_global_pos message
 *
 * @return [cdeg] pitch.
 */
static inline float mavlink_msg_fmt_global_pos_get_pitch(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  44);
}

/**
 * @brief Get field yaw from fmt_global_pos message
 *
 * @return [cdeg] yaw
 */
static inline float mavlink_msg_fmt_global_pos_get_yaw(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  48);
}

/**
 * @brief Get field speed from fmt_global_pos message
 *
 * @return [m/s] Ground Speed
 */
static inline float mavlink_msg_fmt_global_pos_get_speed(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  52);
}

/**
 * @brief Get field sys_id from fmt_global_pos message
 *
 * @return  sys_id
 */
static inline uint16_t mavlink_msg_fmt_global_pos_get_sys_id(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  56);
}

/**
 * @brief Get field node_type from fmt_global_pos message
 *
 * @return  tnode_type
 */
static inline uint16_t mavlink_msg_fmt_global_pos_get_node_type(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  58);
}

/**
 * @brief Get field team_id from fmt_global_pos message
 *
 * @return  team_id
 */
static inline uint16_t mavlink_msg_fmt_global_pos_get_team_id(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  60);
}

/**
 * @brief Decode a fmt_global_pos message into a struct
 *
 * @param msg The message to decode
 * @param fmt_global_pos C-struct to decode the message contents into
 */
static inline void mavlink_msg_fmt_global_pos_decode(const mavlink_message_t* msg, mavlink_fmt_global_pos_t* fmt_global_pos)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    fmt_global_pos->time_boot_ms = mavlink_msg_fmt_global_pos_get_time_boot_ms(msg);
    fmt_global_pos->lat = mavlink_msg_fmt_global_pos_get_lat(msg);
    fmt_global_pos->lon = mavlink_msg_fmt_global_pos_get_lon(msg);
    fmt_global_pos->alt = mavlink_msg_fmt_global_pos_get_alt(msg);
    fmt_global_pos->relative_alt = mavlink_msg_fmt_global_pos_get_relative_alt(msg);
    fmt_global_pos->vx = mavlink_msg_fmt_global_pos_get_vx(msg);
    fmt_global_pos->vy = mavlink_msg_fmt_global_pos_get_vy(msg);
    fmt_global_pos->vz = mavlink_msg_fmt_global_pos_get_vz(msg);
    fmt_global_pos->hdg = mavlink_msg_fmt_global_pos_get_hdg(msg);
    fmt_global_pos->yaw_rate = mavlink_msg_fmt_global_pos_get_yaw_rate(msg);
    fmt_global_pos->roll = mavlink_msg_fmt_global_pos_get_roll(msg);
    fmt_global_pos->pitch = mavlink_msg_fmt_global_pos_get_pitch(msg);
    fmt_global_pos->yaw = mavlink_msg_fmt_global_pos_get_yaw(msg);
    fmt_global_pos->speed = mavlink_msg_fmt_global_pos_get_speed(msg);
    fmt_global_pos->sys_id = mavlink_msg_fmt_global_pos_get_sys_id(msg);
    fmt_global_pos->node_type = mavlink_msg_fmt_global_pos_get_node_type(msg);
    fmt_global_pos->team_id = mavlink_msg_fmt_global_pos_get_team_id(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_FMT_GLOBAL_POS_LEN? msg->len : MAVLINK_MSG_ID_FMT_GLOBAL_POS_LEN;
        memset(fmt_global_pos, 0, MAVLINK_MSG_ID_FMT_GLOBAL_POS_LEN);
    memcpy(fmt_global_pos, _MAV_PAYLOAD(msg), len);
#endif
}
