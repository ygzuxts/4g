#pragma once
// MESSAGE FORMATION_SET PACKING

#define MAVLINK_MSG_ID_FORMATION_SET 801


typedef struct __mavlink_formation_set_t {
 int32_t team_id; /*<  team id. Values:[0-8], 0: not a team*/
 int32_t node_type; /*<  node type, master or slave*/
 int32_t x_offset; /*< [m] Offset along the direction of maser's x axis*/
 int32_t y_offset; /*< [m] Offset along the direction of maser's y axis*/
 int32_t z_offset; /*< [m] Offset along the direction of maser's z axis*/
 int32_t strength_z_offset; /*< [m] Offset along the direction of maser's z axis*/
 uint8_t target_system; /*<  System ID.*/
} mavlink_formation_set_t;

#define MAVLINK_MSG_ID_FORMATION_SET_LEN 25
#define MAVLINK_MSG_ID_FORMATION_SET_MIN_LEN 25
#define MAVLINK_MSG_ID_801_LEN 25
#define MAVLINK_MSG_ID_801_MIN_LEN 25

#define MAVLINK_MSG_ID_FORMATION_SET_CRC 135
#define MAVLINK_MSG_ID_801_CRC 135



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_FORMATION_SET { \
    801, \
    "FORMATION_SET", \
    7, \
    {  { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 24, offsetof(mavlink_formation_set_t, target_system) }, \
         { "team_id", NULL, MAVLINK_TYPE_INT32_T, 0, 0, offsetof(mavlink_formation_set_t, team_id) }, \
         { "node_type", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_formation_set_t, node_type) }, \
         { "x_offset", NULL, MAVLINK_TYPE_INT32_T, 0, 8, offsetof(mavlink_formation_set_t, x_offset) }, \
         { "y_offset", NULL, MAVLINK_TYPE_INT32_T, 0, 12, offsetof(mavlink_formation_set_t, y_offset) }, \
         { "z_offset", NULL, MAVLINK_TYPE_INT32_T, 0, 16, offsetof(mavlink_formation_set_t, z_offset) }, \
         { "strength_z_offset", NULL, MAVLINK_TYPE_INT32_T, 0, 20, offsetof(mavlink_formation_set_t, strength_z_offset) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_FORMATION_SET { \
    "FORMATION_SET", \
    7, \
    {  { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 24, offsetof(mavlink_formation_set_t, target_system) }, \
         { "team_id", NULL, MAVLINK_TYPE_INT32_T, 0, 0, offsetof(mavlink_formation_set_t, team_id) }, \
         { "node_type", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_formation_set_t, node_type) }, \
         { "x_offset", NULL, MAVLINK_TYPE_INT32_T, 0, 8, offsetof(mavlink_formation_set_t, x_offset) }, \
         { "y_offset", NULL, MAVLINK_TYPE_INT32_T, 0, 12, offsetof(mavlink_formation_set_t, y_offset) }, \
         { "z_offset", NULL, MAVLINK_TYPE_INT32_T, 0, 16, offsetof(mavlink_formation_set_t, z_offset) }, \
         { "strength_z_offset", NULL, MAVLINK_TYPE_INT32_T, 0, 20, offsetof(mavlink_formation_set_t, strength_z_offset) }, \
         } \
}
#endif

/**
 * @brief Pack a formation_set message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param target_system  System ID.
 * @param team_id  team id. Values:[0-8], 0: not a team
 * @param node_type  node type, master or slave
 * @param x_offset [m] Offset along the direction of maser's x axis
 * @param y_offset [m] Offset along the direction of maser's y axis
 * @param z_offset [m] Offset along the direction of maser's z axis
 * @param strength_z_offset [m] Offset along the direction of maser's z axis
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_formation_set_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t target_system, int32_t team_id, int32_t node_type, int32_t x_offset, int32_t y_offset, int32_t z_offset, int32_t strength_z_offset)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_FORMATION_SET_LEN];
    _mav_put_int32_t(buf, 0, team_id);
    _mav_put_int32_t(buf, 4, node_type);
    _mav_put_int32_t(buf, 8, x_offset);
    _mav_put_int32_t(buf, 12, y_offset);
    _mav_put_int32_t(buf, 16, z_offset);
    _mav_put_int32_t(buf, 20, strength_z_offset);
    _mav_put_uint8_t(buf, 24, target_system);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_FORMATION_SET_LEN);
#else
    mavlink_formation_set_t packet;
    packet.team_id = team_id;
    packet.node_type = node_type;
    packet.x_offset = x_offset;
    packet.y_offset = y_offset;
    packet.z_offset = z_offset;
    packet.strength_z_offset = strength_z_offset;
    packet.target_system = target_system;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_FORMATION_SET_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_FORMATION_SET;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_FORMATION_SET_MIN_LEN, MAVLINK_MSG_ID_FORMATION_SET_LEN, MAVLINK_MSG_ID_FORMATION_SET_CRC);
}

/**
 * @brief Pack a formation_set message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param target_system  System ID.
 * @param team_id  team id. Values:[0-8], 0: not a team
 * @param node_type  node type, master or slave
 * @param x_offset [m] Offset along the direction of maser's x axis
 * @param y_offset [m] Offset along the direction of maser's y axis
 * @param z_offset [m] Offset along the direction of maser's z axis
 * @param strength_z_offset [m] Offset along the direction of maser's z axis
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_formation_set_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t target_system,int32_t team_id,int32_t node_type,int32_t x_offset,int32_t y_offset,int32_t z_offset,int32_t strength_z_offset)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_FORMATION_SET_LEN];
    _mav_put_int32_t(buf, 0, team_id);
    _mav_put_int32_t(buf, 4, node_type);
    _mav_put_int32_t(buf, 8, x_offset);
    _mav_put_int32_t(buf, 12, y_offset);
    _mav_put_int32_t(buf, 16, z_offset);
    _mav_put_int32_t(buf, 20, strength_z_offset);
    _mav_put_uint8_t(buf, 24, target_system);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_FORMATION_SET_LEN);
#else
    mavlink_formation_set_t packet;
    packet.team_id = team_id;
    packet.node_type = node_type;
    packet.x_offset = x_offset;
    packet.y_offset = y_offset;
    packet.z_offset = z_offset;
    packet.strength_z_offset = strength_z_offset;
    packet.target_system = target_system;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_FORMATION_SET_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_FORMATION_SET;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_FORMATION_SET_MIN_LEN, MAVLINK_MSG_ID_FORMATION_SET_LEN, MAVLINK_MSG_ID_FORMATION_SET_CRC);
}

/**
 * @brief Encode a formation_set struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param formation_set C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_formation_set_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_formation_set_t* formation_set)
{
    return mavlink_msg_formation_set_pack(system_id, component_id, msg, formation_set->target_system, formation_set->team_id, formation_set->node_type, formation_set->x_offset, formation_set->y_offset, formation_set->z_offset, formation_set->strength_z_offset);
}

/**
 * @brief Encode a formation_set struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param formation_set C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_formation_set_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_formation_set_t* formation_set)
{
    return mavlink_msg_formation_set_pack_chan(system_id, component_id, chan, msg, formation_set->target_system, formation_set->team_id, formation_set->node_type, formation_set->x_offset, formation_set->y_offset, formation_set->z_offset, formation_set->strength_z_offset);
}

/**
 * @brief Send a formation_set message
 * @param chan MAVLink channel to send the message
 *
 * @param target_system  System ID.
 * @param team_id  team id. Values:[0-8], 0: not a team
 * @param node_type  node type, master or slave
 * @param x_offset [m] Offset along the direction of maser's x axis
 * @param y_offset [m] Offset along the direction of maser's y axis
 * @param z_offset [m] Offset along the direction of maser's z axis
 * @param strength_z_offset [m] Offset along the direction of maser's z axis
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_formation_set_send(mavlink_channel_t chan, uint8_t target_system, int32_t team_id, int32_t node_type, int32_t x_offset, int32_t y_offset, int32_t z_offset, int32_t strength_z_offset)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_FORMATION_SET_LEN];
    _mav_put_int32_t(buf, 0, team_id);
    _mav_put_int32_t(buf, 4, node_type);
    _mav_put_int32_t(buf, 8, x_offset);
    _mav_put_int32_t(buf, 12, y_offset);
    _mav_put_int32_t(buf, 16, z_offset);
    _mav_put_int32_t(buf, 20, strength_z_offset);
    _mav_put_uint8_t(buf, 24, target_system);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FORMATION_SET, buf, MAVLINK_MSG_ID_FORMATION_SET_MIN_LEN, MAVLINK_MSG_ID_FORMATION_SET_LEN, MAVLINK_MSG_ID_FORMATION_SET_CRC);
#else
    mavlink_formation_set_t packet;
    packet.team_id = team_id;
    packet.node_type = node_type;
    packet.x_offset = x_offset;
    packet.y_offset = y_offset;
    packet.z_offset = z_offset;
    packet.strength_z_offset = strength_z_offset;
    packet.target_system = target_system;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FORMATION_SET, (const char *)&packet, MAVLINK_MSG_ID_FORMATION_SET_MIN_LEN, MAVLINK_MSG_ID_FORMATION_SET_LEN, MAVLINK_MSG_ID_FORMATION_SET_CRC);
#endif
}

/**
 * @brief Send a formation_set message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_formation_set_send_struct(mavlink_channel_t chan, const mavlink_formation_set_t* formation_set)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_formation_set_send(chan, formation_set->target_system, formation_set->team_id, formation_set->node_type, formation_set->x_offset, formation_set->y_offset, formation_set->z_offset, formation_set->strength_z_offset);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FORMATION_SET, (const char *)formation_set, MAVLINK_MSG_ID_FORMATION_SET_MIN_LEN, MAVLINK_MSG_ID_FORMATION_SET_LEN, MAVLINK_MSG_ID_FORMATION_SET_CRC);
#endif
}

#if MAVLINK_MSG_ID_FORMATION_SET_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_formation_set_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t target_system, int32_t team_id, int32_t node_type, int32_t x_offset, int32_t y_offset, int32_t z_offset, int32_t strength_z_offset)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_int32_t(buf, 0, team_id);
    _mav_put_int32_t(buf, 4, node_type);
    _mav_put_int32_t(buf, 8, x_offset);
    _mav_put_int32_t(buf, 12, y_offset);
    _mav_put_int32_t(buf, 16, z_offset);
    _mav_put_int32_t(buf, 20, strength_z_offset);
    _mav_put_uint8_t(buf, 24, target_system);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FORMATION_SET, buf, MAVLINK_MSG_ID_FORMATION_SET_MIN_LEN, MAVLINK_MSG_ID_FORMATION_SET_LEN, MAVLINK_MSG_ID_FORMATION_SET_CRC);
#else
    mavlink_formation_set_t *packet = (mavlink_formation_set_t *)msgbuf;
    packet->team_id = team_id;
    packet->node_type = node_type;
    packet->x_offset = x_offset;
    packet->y_offset = y_offset;
    packet->z_offset = z_offset;
    packet->strength_z_offset = strength_z_offset;
    packet->target_system = target_system;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FORMATION_SET, (const char *)packet, MAVLINK_MSG_ID_FORMATION_SET_MIN_LEN, MAVLINK_MSG_ID_FORMATION_SET_LEN, MAVLINK_MSG_ID_FORMATION_SET_CRC);
#endif
}
#endif

#endif

// MESSAGE FORMATION_SET UNPACKING


/**
 * @brief Get field target_system from formation_set message
 *
 * @return  System ID.
 */
static inline uint8_t mavlink_msg_formation_set_get_target_system(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  24);
}

/**
 * @brief Get field team_id from formation_set message
 *
 * @return  team id. Values:[0-8], 0: not a team
 */
static inline int32_t mavlink_msg_formation_set_get_team_id(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  0);
}

/**
 * @brief Get field node_type from formation_set message
 *
 * @return  node type, master or slave
 */
static inline int32_t mavlink_msg_formation_set_get_node_type(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  4);
}

/**
 * @brief Get field x_offset from formation_set message
 *
 * @return [m] Offset along the direction of maser's x axis
 */
static inline int32_t mavlink_msg_formation_set_get_x_offset(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  8);
}

/**
 * @brief Get field y_offset from formation_set message
 *
 * @return [m] Offset along the direction of maser's y axis
 */
static inline int32_t mavlink_msg_formation_set_get_y_offset(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  12);
}

/**
 * @brief Get field z_offset from formation_set message
 *
 * @return [m] Offset along the direction of maser's z axis
 */
static inline int32_t mavlink_msg_formation_set_get_z_offset(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  16);
}

/**
 * @brief Get field strength_z_offset from formation_set message
 *
 * @return [m] Offset along the direction of maser's z axis
 */
static inline int32_t mavlink_msg_formation_set_get_strength_z_offset(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  20);
}

/**
 * @brief Decode a formation_set message into a struct
 *
 * @param msg The message to decode
 * @param formation_set C-struct to decode the message contents into
 */
static inline void mavlink_msg_formation_set_decode(const mavlink_message_t* msg, mavlink_formation_set_t* formation_set)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    formation_set->team_id = mavlink_msg_formation_set_get_team_id(msg);
    formation_set->node_type = mavlink_msg_formation_set_get_node_type(msg);
    formation_set->x_offset = mavlink_msg_formation_set_get_x_offset(msg);
    formation_set->y_offset = mavlink_msg_formation_set_get_y_offset(msg);
    formation_set->z_offset = mavlink_msg_formation_set_get_z_offset(msg);
    formation_set->strength_z_offset = mavlink_msg_formation_set_get_strength_z_offset(msg);
    formation_set->target_system = mavlink_msg_formation_set_get_target_system(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_FORMATION_SET_LEN? msg->len : MAVLINK_MSG_ID_FORMATION_SET_LEN;
        memset(formation_set, 0, MAVLINK_MSG_ID_FORMATION_SET_LEN);
    memcpy(formation_set, _MAV_PAYLOAD(msg), len);
#endif
}
