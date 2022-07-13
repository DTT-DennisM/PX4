#pragma once
// MESSAGE COLOR_LIMITS PACKING

#define MAVLINK_MSG_ID_COLOR_LIMITS 368


typedef struct __mavlink_color_limits_t {
 uint64_t timestamp; /*<  time since system start (microseconds)*/
 int16_t h_low; /*<  Hue lower bound. If unknown set to INT16_MAX*/
 int16_t h_up; /*<  Hue upper bound. If unknown set to INT16_MAX*/
 int16_t s_low; /*<  Saturation lower bound. If unknown set to INT16_MAX*/
 int16_t s_up; /*<  Saturation upper bound. If unknown set to INT16_MAX*/
 int16_t v_low; /*<  Value lower bound. If unknown set to INT16_MAX*/
 int16_t v_up; /*<  Value upper bound. If unknown set to INT16_MAX*/
 char info[11]; /*<  General information (11 characters, null terminated, valid characters are A-Z, 0-9, " " only)*/
} mavlink_color_limits_t;

#define MAVLINK_MSG_ID_COLOR_LIMITS_LEN 31
#define MAVLINK_MSG_ID_COLOR_LIMITS_MIN_LEN 31
#define MAVLINK_MSG_ID_368_LEN 31
#define MAVLINK_MSG_ID_368_MIN_LEN 31

#define MAVLINK_MSG_ID_COLOR_LIMITS_CRC 130
#define MAVLINK_MSG_ID_368_CRC 130

#define MAVLINK_MSG_COLOR_LIMITS_FIELD_INFO_LEN 11

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_COLOR_LIMITS { \
    368, \
    "COLOR_LIMITS", \
    8, \
    {  { "timestamp", NULL, MAVLINK_TYPE_UINT64_T, 0, 0, offsetof(mavlink_color_limits_t, timestamp) }, \
         { "info", NULL, MAVLINK_TYPE_CHAR, 11, 20, offsetof(mavlink_color_limits_t, info) }, \
         { "h_low", NULL, MAVLINK_TYPE_INT16_T, 0, 8, offsetof(mavlink_color_limits_t, h_low) }, \
         { "h_up", NULL, MAVLINK_TYPE_INT16_T, 0, 10, offsetof(mavlink_color_limits_t, h_up) }, \
         { "s_low", NULL, MAVLINK_TYPE_INT16_T, 0, 12, offsetof(mavlink_color_limits_t, s_low) }, \
         { "s_up", NULL, MAVLINK_TYPE_INT16_T, 0, 14, offsetof(mavlink_color_limits_t, s_up) }, \
         { "v_low", NULL, MAVLINK_TYPE_INT16_T, 0, 16, offsetof(mavlink_color_limits_t, v_low) }, \
         { "v_up", NULL, MAVLINK_TYPE_INT16_T, 0, 18, offsetof(mavlink_color_limits_t, v_up) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_COLOR_LIMITS { \
    "COLOR_LIMITS", \
    8, \
    {  { "timestamp", NULL, MAVLINK_TYPE_UINT64_T, 0, 0, offsetof(mavlink_color_limits_t, timestamp) }, \
         { "info", NULL, MAVLINK_TYPE_CHAR, 11, 20, offsetof(mavlink_color_limits_t, info) }, \
         { "h_low", NULL, MAVLINK_TYPE_INT16_T, 0, 8, offsetof(mavlink_color_limits_t, h_low) }, \
         { "h_up", NULL, MAVLINK_TYPE_INT16_T, 0, 10, offsetof(mavlink_color_limits_t, h_up) }, \
         { "s_low", NULL, MAVLINK_TYPE_INT16_T, 0, 12, offsetof(mavlink_color_limits_t, s_low) }, \
         { "s_up", NULL, MAVLINK_TYPE_INT16_T, 0, 14, offsetof(mavlink_color_limits_t, s_up) }, \
         { "v_low", NULL, MAVLINK_TYPE_INT16_T, 0, 16, offsetof(mavlink_color_limits_t, v_low) }, \
         { "v_up", NULL, MAVLINK_TYPE_INT16_T, 0, 18, offsetof(mavlink_color_limits_t, v_up) }, \
         } \
}
#endif

/**
 * @brief Pack a color_limits message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param timestamp  time since system start (microseconds)
 * @param info  General information (11 characters, null terminated, valid characters are A-Z, 0-9, " " only)
 * @param h_low  Hue lower bound. If unknown set to INT16_MAX
 * @param h_up  Hue upper bound. If unknown set to INT16_MAX
 * @param s_low  Saturation lower bound. If unknown set to INT16_MAX
 * @param s_up  Saturation upper bound. If unknown set to INT16_MAX
 * @param v_low  Value lower bound. If unknown set to INT16_MAX
 * @param v_up  Value upper bound. If unknown set to INT16_MAX
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_color_limits_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint64_t timestamp, const char *info, int16_t h_low, int16_t h_up, int16_t s_low, int16_t s_up, int16_t v_low, int16_t v_up)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_COLOR_LIMITS_LEN];
    _mav_put_uint64_t(buf, 0, timestamp);
    _mav_put_int16_t(buf, 8, h_low);
    _mav_put_int16_t(buf, 10, h_up);
    _mav_put_int16_t(buf, 12, s_low);
    _mav_put_int16_t(buf, 14, s_up);
    _mav_put_int16_t(buf, 16, v_low);
    _mav_put_int16_t(buf, 18, v_up);
    _mav_put_char_array(buf, 20, info, 11);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_COLOR_LIMITS_LEN);
#else
    mavlink_color_limits_t packet;
    packet.timestamp = timestamp;
    packet.h_low = h_low;
    packet.h_up = h_up;
    packet.s_low = s_low;
    packet.s_up = s_up;
    packet.v_low = v_low;
    packet.v_up = v_up;
    mav_array_memcpy(packet.info, info, sizeof(char)*11);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_COLOR_LIMITS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_COLOR_LIMITS;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_COLOR_LIMITS_MIN_LEN, MAVLINK_MSG_ID_COLOR_LIMITS_LEN, MAVLINK_MSG_ID_COLOR_LIMITS_CRC);
}

/**
 * @brief Pack a color_limits message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param timestamp  time since system start (microseconds)
 * @param info  General information (11 characters, null terminated, valid characters are A-Z, 0-9, " " only)
 * @param h_low  Hue lower bound. If unknown set to INT16_MAX
 * @param h_up  Hue upper bound. If unknown set to INT16_MAX
 * @param s_low  Saturation lower bound. If unknown set to INT16_MAX
 * @param s_up  Saturation upper bound. If unknown set to INT16_MAX
 * @param v_low  Value lower bound. If unknown set to INT16_MAX
 * @param v_up  Value upper bound. If unknown set to INT16_MAX
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_color_limits_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint64_t timestamp,const char *info,int16_t h_low,int16_t h_up,int16_t s_low,int16_t s_up,int16_t v_low,int16_t v_up)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_COLOR_LIMITS_LEN];
    _mav_put_uint64_t(buf, 0, timestamp);
    _mav_put_int16_t(buf, 8, h_low);
    _mav_put_int16_t(buf, 10, h_up);
    _mav_put_int16_t(buf, 12, s_low);
    _mav_put_int16_t(buf, 14, s_up);
    _mav_put_int16_t(buf, 16, v_low);
    _mav_put_int16_t(buf, 18, v_up);
    _mav_put_char_array(buf, 20, info, 11);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_COLOR_LIMITS_LEN);
#else
    mavlink_color_limits_t packet;
    packet.timestamp = timestamp;
    packet.h_low = h_low;
    packet.h_up = h_up;
    packet.s_low = s_low;
    packet.s_up = s_up;
    packet.v_low = v_low;
    packet.v_up = v_up;
    mav_array_memcpy(packet.info, info, sizeof(char)*11);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_COLOR_LIMITS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_COLOR_LIMITS;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_COLOR_LIMITS_MIN_LEN, MAVLINK_MSG_ID_COLOR_LIMITS_LEN, MAVLINK_MSG_ID_COLOR_LIMITS_CRC);
}

/**
 * @brief Encode a color_limits struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param color_limits C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_color_limits_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_color_limits_t* color_limits)
{
    return mavlink_msg_color_limits_pack(system_id, component_id, msg, color_limits->timestamp, color_limits->info, color_limits->h_low, color_limits->h_up, color_limits->s_low, color_limits->s_up, color_limits->v_low, color_limits->v_up);
}

/**
 * @brief Encode a color_limits struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param color_limits C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_color_limits_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_color_limits_t* color_limits)
{
    return mavlink_msg_color_limits_pack_chan(system_id, component_id, chan, msg, color_limits->timestamp, color_limits->info, color_limits->h_low, color_limits->h_up, color_limits->s_low, color_limits->s_up, color_limits->v_low, color_limits->v_up);
}

/**
 * @brief Send a color_limits message
 * @param chan MAVLink channel to send the message
 *
 * @param timestamp  time since system start (microseconds)
 * @param info  General information (11 characters, null terminated, valid characters are A-Z, 0-9, " " only)
 * @param h_low  Hue lower bound. If unknown set to INT16_MAX
 * @param h_up  Hue upper bound. If unknown set to INT16_MAX
 * @param s_low  Saturation lower bound. If unknown set to INT16_MAX
 * @param s_up  Saturation upper bound. If unknown set to INT16_MAX
 * @param v_low  Value lower bound. If unknown set to INT16_MAX
 * @param v_up  Value upper bound. If unknown set to INT16_MAX
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_color_limits_send(mavlink_channel_t chan, uint64_t timestamp, const char *info, int16_t h_low, int16_t h_up, int16_t s_low, int16_t s_up, int16_t v_low, int16_t v_up)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_COLOR_LIMITS_LEN];
    _mav_put_uint64_t(buf, 0, timestamp);
    _mav_put_int16_t(buf, 8, h_low);
    _mav_put_int16_t(buf, 10, h_up);
    _mav_put_int16_t(buf, 12, s_low);
    _mav_put_int16_t(buf, 14, s_up);
    _mav_put_int16_t(buf, 16, v_low);
    _mav_put_int16_t(buf, 18, v_up);
    _mav_put_char_array(buf, 20, info, 11);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_COLOR_LIMITS, buf, MAVLINK_MSG_ID_COLOR_LIMITS_MIN_LEN, MAVLINK_MSG_ID_COLOR_LIMITS_LEN, MAVLINK_MSG_ID_COLOR_LIMITS_CRC);
#else
    mavlink_color_limits_t packet;
    packet.timestamp = timestamp;
    packet.h_low = h_low;
    packet.h_up = h_up;
    packet.s_low = s_low;
    packet.s_up = s_up;
    packet.v_low = v_low;
    packet.v_up = v_up;
    mav_array_memcpy(packet.info, info, sizeof(char)*11);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_COLOR_LIMITS, (const char *)&packet, MAVLINK_MSG_ID_COLOR_LIMITS_MIN_LEN, MAVLINK_MSG_ID_COLOR_LIMITS_LEN, MAVLINK_MSG_ID_COLOR_LIMITS_CRC);
#endif
}

/**
 * @brief Send a color_limits message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_color_limits_send_struct(mavlink_channel_t chan, const mavlink_color_limits_t* color_limits)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_color_limits_send(chan, color_limits->timestamp, color_limits->info, color_limits->h_low, color_limits->h_up, color_limits->s_low, color_limits->s_up, color_limits->v_low, color_limits->v_up);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_COLOR_LIMITS, (const char *)color_limits, MAVLINK_MSG_ID_COLOR_LIMITS_MIN_LEN, MAVLINK_MSG_ID_COLOR_LIMITS_LEN, MAVLINK_MSG_ID_COLOR_LIMITS_CRC);
#endif
}

#if MAVLINK_MSG_ID_COLOR_LIMITS_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_color_limits_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint64_t timestamp, const char *info, int16_t h_low, int16_t h_up, int16_t s_low, int16_t s_up, int16_t v_low, int16_t v_up)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint64_t(buf, 0, timestamp);
    _mav_put_int16_t(buf, 8, h_low);
    _mav_put_int16_t(buf, 10, h_up);
    _mav_put_int16_t(buf, 12, s_low);
    _mav_put_int16_t(buf, 14, s_up);
    _mav_put_int16_t(buf, 16, v_low);
    _mav_put_int16_t(buf, 18, v_up);
    _mav_put_char_array(buf, 20, info, 11);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_COLOR_LIMITS, buf, MAVLINK_MSG_ID_COLOR_LIMITS_MIN_LEN, MAVLINK_MSG_ID_COLOR_LIMITS_LEN, MAVLINK_MSG_ID_COLOR_LIMITS_CRC);
#else
    mavlink_color_limits_t *packet = (mavlink_color_limits_t *)msgbuf;
    packet->timestamp = timestamp;
    packet->h_low = h_low;
    packet->h_up = h_up;
    packet->s_low = s_low;
    packet->s_up = s_up;
    packet->v_low = v_low;
    packet->v_up = v_up;
    mav_array_memcpy(packet->info, info, sizeof(char)*11);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_COLOR_LIMITS, (const char *)packet, MAVLINK_MSG_ID_COLOR_LIMITS_MIN_LEN, MAVLINK_MSG_ID_COLOR_LIMITS_LEN, MAVLINK_MSG_ID_COLOR_LIMITS_CRC);
#endif
}
#endif

#endif

// MESSAGE COLOR_LIMITS UNPACKING


/**
 * @brief Get field timestamp from color_limits message
 *
 * @return  time since system start (microseconds)
 */
static inline uint64_t mavlink_msg_color_limits_get_timestamp(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint64_t(msg,  0);
}

/**
 * @brief Get field info from color_limits message
 *
 * @return  General information (11 characters, null terminated, valid characters are A-Z, 0-9, " " only)
 */
static inline uint16_t mavlink_msg_color_limits_get_info(const mavlink_message_t* msg, char *info)
{
    return _MAV_RETURN_char_array(msg, info, 11,  20);
}

/**
 * @brief Get field h_low from color_limits message
 *
 * @return  Hue lower bound. If unknown set to INT16_MAX
 */
static inline int16_t mavlink_msg_color_limits_get_h_low(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  8);
}

/**
 * @brief Get field h_up from color_limits message
 *
 * @return  Hue upper bound. If unknown set to INT16_MAX
 */
static inline int16_t mavlink_msg_color_limits_get_h_up(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  10);
}

/**
 * @brief Get field s_low from color_limits message
 *
 * @return  Saturation lower bound. If unknown set to INT16_MAX
 */
static inline int16_t mavlink_msg_color_limits_get_s_low(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  12);
}

/**
 * @brief Get field s_up from color_limits message
 *
 * @return  Saturation upper bound. If unknown set to INT16_MAX
 */
static inline int16_t mavlink_msg_color_limits_get_s_up(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  14);
}

/**
 * @brief Get field v_low from color_limits message
 *
 * @return  Value lower bound. If unknown set to INT16_MAX
 */
static inline int16_t mavlink_msg_color_limits_get_v_low(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  16);
}

/**
 * @brief Get field v_up from color_limits message
 *
 * @return  Value upper bound. If unknown set to INT16_MAX
 */
static inline int16_t mavlink_msg_color_limits_get_v_up(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  18);
}

/**
 * @brief Decode a color_limits message into a struct
 *
 * @param msg The message to decode
 * @param color_limits C-struct to decode the message contents into
 */
static inline void mavlink_msg_color_limits_decode(const mavlink_message_t* msg, mavlink_color_limits_t* color_limits)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    color_limits->timestamp = mavlink_msg_color_limits_get_timestamp(msg);
    color_limits->h_low = mavlink_msg_color_limits_get_h_low(msg);
    color_limits->h_up = mavlink_msg_color_limits_get_h_up(msg);
    color_limits->s_low = mavlink_msg_color_limits_get_s_low(msg);
    color_limits->s_up = mavlink_msg_color_limits_get_s_up(msg);
    color_limits->v_low = mavlink_msg_color_limits_get_v_low(msg);
    color_limits->v_up = mavlink_msg_color_limits_get_v_up(msg);
    mavlink_msg_color_limits_get_info(msg, color_limits->info);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_COLOR_LIMITS_LEN? msg->len : MAVLINK_MSG_ID_COLOR_LIMITS_LEN;
        memset(color_limits, 0, MAVLINK_MSG_ID_COLOR_LIMITS_LEN);
    memcpy(color_limits, _MAV_PAYLOAD(msg), len);
#endif
}
