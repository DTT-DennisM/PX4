#ifndef COLOR_LIM_HPP
#define COLOR_LIM_HPP

#include <uORB/topics/color_limits.h>  //placed in: build/nxp_fmuk66-v3_default/uORB/topics
#include <../../../../mavlink/include/mavlink/v2.0/video_monitor/mavlink.h>
#include "../../../../mavlink/include/mavlink/v2.0/video_monitor/mavlink_msg_color_limits.h"

class MavlinkStreamColorLimits : public MavlinkStream
{
public:
    static MavlinkStream *new_instance(Mavlink *mavlink)
    { return new MavlinkStreamColorLimits(mavlink); }

    // In a member function declaration or definition, override specifier ensures that
    // the function is virtual and is overriding a virtual function from a base class.
    const char *get_name() const override
    { return MavlinkStreamColorLimits::get_name_static(); }

    // The constexpr specifier declares that it is possible to 
    // evaluate the value of the function or variable at compile time.
    static constexpr const char *get_name_static()
    { return "COLOR_LIMITS";  }

    uint16_t get_id() override
    { return get_id_static(); }

    static constexpr uint16_t get_id_static()
    { return MAVLINK_MSG_ID_COLOR_LIMITS; }

    unsigned get_size() override
    { return MAVLINK_MSG_ID_COLOR_LIMITS_LEN + MAVLINK_NUM_NON_PAYLOAD_BYTES; }

private:
    uORB::Subscription _sub{ORB_ID(color_limits)};

    /* do not allow top copying this class */
    MavlinkStreamColorLimits(MavlinkStreamColorLimits &);
    MavlinkStreamColorLimits& operator = (const MavlinkStreamColorLimits &);

protected:
    explicit MavlinkStreamColorLimits(Mavlink *mavlink) : MavlinkStream(mavlink)
    {}

    bool send() override
    {
//	PX4_INFO("Start of sending the message :)");
        struct color_limits_s _color_limits;  //make sure color_limits_s is the TEST HIERO
                                                //definition of your uORB topic
//	PX4_INFO("Before if loop!!");
        if (_sub.update(&_color_limits))
        {
//		PX4_INFO("Sending in if loop!");
	        mavlink_color_limits_t _msg_color_limits;  // mavlink_color_limits_t is the
	                                                     // definition of your custom
	                                                     // MAVLink message
	        _msg_color_limits.timestamp  = _color_limits.timestamp;
	        _msg_color_limits.h_low  = _color_limits.h_low;
	        _msg_color_limits.h_up  = _color_limits.h_up;
	        _msg_color_limits.s_low  = _color_limits.s_low;
	        _msg_color_limits.s_up  = _color_limits.s_up;
	        _msg_color_limits.v_low  = _color_limits.v_low;
	        _msg_color_limits.v_up  = _color_limits.v_up;

	        for(int i=0; i<11; i++)
	            _msg_color_limits.info[i] = _color_limits.info[i];

	        mavlink_msg_color_limits_send_struct(_mavlink->get_channel(),
	                                              &_msg_color_limits);
//		if((double)_color_limits.confidence > 0.9999)
//		        PX4_WARN("uorb => mavlink - message was sent !!!!");

	        return true;
        }

    return false;
    }
};
#endif // COLOR_LIM_HPP
