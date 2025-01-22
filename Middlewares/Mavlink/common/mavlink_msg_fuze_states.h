#pragma once
// MESSAGE FUZE_STATES PACKING

#define MAVLINK_MSG_ID_FUZE_STATES 12932


typedef struct __mavlink_fuze_states_t {
 uint32_t delay_ms; /*<  delay ms*/
 uint8_t device_id; /*<  device id*/
 uint8_t safe; /*<  safe*/
 uint8_t armed; /*<  armed*/
 uint8_t deep_armed; /*<  deep_armed*/
 uint8_t self_destroy; /*<  self_destroy*/
 uint8_t touch_explosion; /*<  touch_explosion*/
 uint8_t countdown; /*<  countdown*/
 uint8_t lock; /*<  lock*/
} mavlink_fuze_states_t;

#define MAVLINK_MSG_ID_FUZE_STATES_LEN 12
#define MAVLINK_MSG_ID_FUZE_STATES_MIN_LEN 12
#define MAVLINK_MSG_ID_12932_LEN 12
#define MAVLINK_MSG_ID_12932_MIN_LEN 12

#define MAVLINK_MSG_ID_FUZE_STATES_CRC 194
#define MAVLINK_MSG_ID_12932_CRC 194



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_FUZE_STATES { \
    12932, \
    "FUZE_STATES", \
    9, \
    {  { "delay_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_fuze_states_t, delay_ms) }, \
         { "device_id", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_fuze_states_t, device_id) }, \
         { "safe", NULL, MAVLINK_TYPE_UINT8_T, 0, 5, offsetof(mavlink_fuze_states_t, safe) }, \
         { "armed", NULL, MAVLINK_TYPE_UINT8_T, 0, 6, offsetof(mavlink_fuze_states_t, armed) }, \
         { "deep_armed", NULL, MAVLINK_TYPE_UINT8_T, 0, 7, offsetof(mavlink_fuze_states_t, deep_armed) }, \
         { "self_destroy", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_fuze_states_t, self_destroy) }, \
         { "touch_explosion", NULL, MAVLINK_TYPE_UINT8_T, 0, 9, offsetof(mavlink_fuze_states_t, touch_explosion) }, \
         { "countdown", NULL, MAVLINK_TYPE_UINT8_T, 0, 10, offsetof(mavlink_fuze_states_t, countdown) }, \
         { "lock", NULL, MAVLINK_TYPE_UINT8_T, 0, 11, offsetof(mavlink_fuze_states_t, lock) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_FUZE_STATES { \
    "FUZE_STATES", \
    9, \
    {  { "delay_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_fuze_states_t, delay_ms) }, \
         { "device_id", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_fuze_states_t, device_id) }, \
         { "safe", NULL, MAVLINK_TYPE_UINT8_T, 0, 5, offsetof(mavlink_fuze_states_t, safe) }, \
         { "armed", NULL, MAVLINK_TYPE_UINT8_T, 0, 6, offsetof(mavlink_fuze_states_t, armed) }, \
         { "deep_armed", NULL, MAVLINK_TYPE_UINT8_T, 0, 7, offsetof(mavlink_fuze_states_t, deep_armed) }, \
         { "self_destroy", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_fuze_states_t, self_destroy) }, \
         { "touch_explosion", NULL, MAVLINK_TYPE_UINT8_T, 0, 9, offsetof(mavlink_fuze_states_t, touch_explosion) }, \
         { "countdown", NULL, MAVLINK_TYPE_UINT8_T, 0, 10, offsetof(mavlink_fuze_states_t, countdown) }, \
         { "lock", NULL, MAVLINK_TYPE_UINT8_T, 0, 11, offsetof(mavlink_fuze_states_t, lock) }, \
         } \
}
#endif

/**
 * @brief Pack a fuze_states message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param delay_ms  delay ms
 * @param device_id  device id
 * @param safe  safe
 * @param armed  armed
 * @param deep_armed  deep_armed
 * @param self_destroy  self_destroy
 * @param touch_explosion  touch_explosion
 * @param countdown  countdown
 * @param lock  lock
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_fuze_states_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t delay_ms, uint8_t device_id, uint8_t safe, uint8_t armed, uint8_t deep_armed, uint8_t self_destroy, uint8_t touch_explosion, uint8_t countdown, uint8_t lock)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_FUZE_STATES_LEN];
    _mav_put_uint32_t(buf, 0, delay_ms);
    _mav_put_uint8_t(buf, 4, device_id);
    _mav_put_uint8_t(buf, 5, safe);
    _mav_put_uint8_t(buf, 6, armed);
    _mav_put_uint8_t(buf, 7, deep_armed);
    _mav_put_uint8_t(buf, 8, self_destroy);
    _mav_put_uint8_t(buf, 9, touch_explosion);
    _mav_put_uint8_t(buf, 10, countdown);
    _mav_put_uint8_t(buf, 11, lock);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_FUZE_STATES_LEN);
#else
    mavlink_fuze_states_t packet;
    packet.delay_ms = delay_ms;
    packet.device_id = device_id;
    packet.safe = safe;
    packet.armed = armed;
    packet.deep_armed = deep_armed;
    packet.self_destroy = self_destroy;
    packet.touch_explosion = touch_explosion;
    packet.countdown = countdown;
    packet.lock = lock;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_FUZE_STATES_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_FUZE_STATES;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_FUZE_STATES_MIN_LEN, MAVLINK_MSG_ID_FUZE_STATES_LEN, MAVLINK_MSG_ID_FUZE_STATES_CRC);
}

/**
 * @brief Pack a fuze_states message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param delay_ms  delay ms
 * @param device_id  device id
 * @param safe  safe
 * @param armed  armed
 * @param deep_armed  deep_armed
 * @param self_destroy  self_destroy
 * @param touch_explosion  touch_explosion
 * @param countdown  countdown
 * @param lock  lock
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_fuze_states_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t delay_ms,uint8_t device_id,uint8_t safe,uint8_t armed,uint8_t deep_armed,uint8_t self_destroy,uint8_t touch_explosion,uint8_t countdown,uint8_t lock)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_FUZE_STATES_LEN];
    _mav_put_uint32_t(buf, 0, delay_ms);
    _mav_put_uint8_t(buf, 4, device_id);
    _mav_put_uint8_t(buf, 5, safe);
    _mav_put_uint8_t(buf, 6, armed);
    _mav_put_uint8_t(buf, 7, deep_armed);
    _mav_put_uint8_t(buf, 8, self_destroy);
    _mav_put_uint8_t(buf, 9, touch_explosion);
    _mav_put_uint8_t(buf, 10, countdown);
    _mav_put_uint8_t(buf, 11, lock);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_FUZE_STATES_LEN);
#else
    mavlink_fuze_states_t packet;
    packet.delay_ms = delay_ms;
    packet.device_id = device_id;
    packet.safe = safe;
    packet.armed = armed;
    packet.deep_armed = deep_armed;
    packet.self_destroy = self_destroy;
    packet.touch_explosion = touch_explosion;
    packet.countdown = countdown;
    packet.lock = lock;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_FUZE_STATES_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_FUZE_STATES;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_FUZE_STATES_MIN_LEN, MAVLINK_MSG_ID_FUZE_STATES_LEN, MAVLINK_MSG_ID_FUZE_STATES_CRC);
}

/**
 * @brief Encode a fuze_states struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param fuze_states C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_fuze_states_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_fuze_states_t* fuze_states)
{
    return mavlink_msg_fuze_states_pack(system_id, component_id, msg, fuze_states->delay_ms, fuze_states->device_id, fuze_states->safe, fuze_states->armed, fuze_states->deep_armed, fuze_states->self_destroy, fuze_states->touch_explosion, fuze_states->countdown, fuze_states->lock);
}

/**
 * @brief Encode a fuze_states struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param fuze_states C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_fuze_states_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_fuze_states_t* fuze_states)
{
    return mavlink_msg_fuze_states_pack_chan(system_id, component_id, chan, msg, fuze_states->delay_ms, fuze_states->device_id, fuze_states->safe, fuze_states->armed, fuze_states->deep_armed, fuze_states->self_destroy, fuze_states->touch_explosion, fuze_states->countdown, fuze_states->lock);
}

/**
 * @brief Send a fuze_states message
 * @param chan MAVLink channel to send the message
 *
 * @param delay_ms  delay ms
 * @param device_id  device id
 * @param safe  safe
 * @param armed  armed
 * @param deep_armed  deep_armed
 * @param self_destroy  self_destroy
 * @param touch_explosion  touch_explosion
 * @param countdown  countdown
 * @param lock  lock
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_fuze_states_send(mavlink_channel_t chan, uint32_t delay_ms, uint8_t device_id, uint8_t safe, uint8_t armed, uint8_t deep_armed, uint8_t self_destroy, uint8_t touch_explosion, uint8_t countdown, uint8_t lock)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_FUZE_STATES_LEN];
    _mav_put_uint32_t(buf, 0, delay_ms);
    _mav_put_uint8_t(buf, 4, device_id);
    _mav_put_uint8_t(buf, 5, safe);
    _mav_put_uint8_t(buf, 6, armed);
    _mav_put_uint8_t(buf, 7, deep_armed);
    _mav_put_uint8_t(buf, 8, self_destroy);
    _mav_put_uint8_t(buf, 9, touch_explosion);
    _mav_put_uint8_t(buf, 10, countdown);
    _mav_put_uint8_t(buf, 11, lock);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FUZE_STATES, buf, MAVLINK_MSG_ID_FUZE_STATES_MIN_LEN, MAVLINK_MSG_ID_FUZE_STATES_LEN, MAVLINK_MSG_ID_FUZE_STATES_CRC);
#else
    mavlink_fuze_states_t packet;
    packet.delay_ms = delay_ms;
    packet.device_id = device_id;
    packet.safe = safe;
    packet.armed = armed;
    packet.deep_armed = deep_armed;
    packet.self_destroy = self_destroy;
    packet.touch_explosion = touch_explosion;
    packet.countdown = countdown;
    packet.lock = lock;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FUZE_STATES, (const char *)&packet, MAVLINK_MSG_ID_FUZE_STATES_MIN_LEN, MAVLINK_MSG_ID_FUZE_STATES_LEN, MAVLINK_MSG_ID_FUZE_STATES_CRC);
#endif
}

/**
 * @brief Send a fuze_states message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_fuze_states_send_struct(mavlink_channel_t chan, const mavlink_fuze_states_t* fuze_states)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_fuze_states_send(chan, fuze_states->delay_ms, fuze_states->device_id, fuze_states->safe, fuze_states->armed, fuze_states->deep_armed, fuze_states->self_destroy, fuze_states->touch_explosion, fuze_states->countdown, fuze_states->lock);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FUZE_STATES, (const char *)fuze_states, MAVLINK_MSG_ID_FUZE_STATES_MIN_LEN, MAVLINK_MSG_ID_FUZE_STATES_LEN, MAVLINK_MSG_ID_FUZE_STATES_CRC);
#endif
}

#if MAVLINK_MSG_ID_FUZE_STATES_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_fuze_states_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t delay_ms, uint8_t device_id, uint8_t safe, uint8_t armed, uint8_t deep_armed, uint8_t self_destroy, uint8_t touch_explosion, uint8_t countdown, uint8_t lock)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, delay_ms);
    _mav_put_uint8_t(buf, 4, device_id);
    _mav_put_uint8_t(buf, 5, safe);
    _mav_put_uint8_t(buf, 6, armed);
    _mav_put_uint8_t(buf, 7, deep_armed);
    _mav_put_uint8_t(buf, 8, self_destroy);
    _mav_put_uint8_t(buf, 9, touch_explosion);
    _mav_put_uint8_t(buf, 10, countdown);
    _mav_put_uint8_t(buf, 11, lock);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FUZE_STATES, buf, MAVLINK_MSG_ID_FUZE_STATES_MIN_LEN, MAVLINK_MSG_ID_FUZE_STATES_LEN, MAVLINK_MSG_ID_FUZE_STATES_CRC);
#else
    mavlink_fuze_states_t *packet = (mavlink_fuze_states_t *)msgbuf;
    packet->delay_ms = delay_ms;
    packet->device_id = device_id;
    packet->safe = safe;
    packet->armed = armed;
    packet->deep_armed = deep_armed;
    packet->self_destroy = self_destroy;
    packet->touch_explosion = touch_explosion;
    packet->countdown = countdown;
    packet->lock = lock;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FUZE_STATES, (const char *)packet, MAVLINK_MSG_ID_FUZE_STATES_MIN_LEN, MAVLINK_MSG_ID_FUZE_STATES_LEN, MAVLINK_MSG_ID_FUZE_STATES_CRC);
#endif
}
#endif

#endif

// MESSAGE FUZE_STATES UNPACKING


/**
 * @brief Get field delay_ms from fuze_states message
 *
 * @return  delay ms
 */
static inline uint32_t mavlink_msg_fuze_states_get_delay_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field device_id from fuze_states message
 *
 * @return  device id
 */
static inline uint8_t mavlink_msg_fuze_states_get_device_id(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  4);
}

/**
 * @brief Get field safe from fuze_states message
 *
 * @return  safe
 */
static inline uint8_t mavlink_msg_fuze_states_get_safe(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  5);
}

/**
 * @brief Get field armed from fuze_states message
 *
 * @return  armed
 */
static inline uint8_t mavlink_msg_fuze_states_get_armed(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  6);
}

/**
 * @brief Get field deep_armed from fuze_states message
 *
 * @return  deep_armed
 */
static inline uint8_t mavlink_msg_fuze_states_get_deep_armed(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  7);
}

/**
 * @brief Get field self_destroy from fuze_states message
 *
 * @return  self_destroy
 */
static inline uint8_t mavlink_msg_fuze_states_get_self_destroy(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  8);
}

/**
 * @brief Get field touch_explosion from fuze_states message
 *
 * @return  touch_explosion
 */
static inline uint8_t mavlink_msg_fuze_states_get_touch_explosion(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  9);
}

/**
 * @brief Get field countdown from fuze_states message
 *
 * @return  countdown
 */
static inline uint8_t mavlink_msg_fuze_states_get_countdown(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  10);
}

/**
 * @brief Get field lock from fuze_states message
 *
 * @return  lock
 */
static inline uint8_t mavlink_msg_fuze_states_get_lock(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  11);
}

/**
 * @brief Decode a fuze_states message into a struct
 *
 * @param msg The message to decode
 * @param fuze_states C-struct to decode the message contents into
 */
static inline void mavlink_msg_fuze_states_decode(const mavlink_message_t* msg, mavlink_fuze_states_t* fuze_states)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    fuze_states->delay_ms = mavlink_msg_fuze_states_get_delay_ms(msg);
    fuze_states->device_id = mavlink_msg_fuze_states_get_device_id(msg);
    fuze_states->safe = mavlink_msg_fuze_states_get_safe(msg);
    fuze_states->armed = mavlink_msg_fuze_states_get_armed(msg);
    fuze_states->deep_armed = mavlink_msg_fuze_states_get_deep_armed(msg);
    fuze_states->self_destroy = mavlink_msg_fuze_states_get_self_destroy(msg);
    fuze_states->touch_explosion = mavlink_msg_fuze_states_get_touch_explosion(msg);
    fuze_states->countdown = mavlink_msg_fuze_states_get_countdown(msg);
    fuze_states->lock = mavlink_msg_fuze_states_get_lock(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_FUZE_STATES_LEN? msg->len : MAVLINK_MSG_ID_FUZE_STATES_LEN;
        memset(fuze_states, 0, MAVLINK_MSG_ID_FUZE_STATES_LEN);
    memcpy(fuze_states, _MAV_PAYLOAD(msg), len);
#endif
}
