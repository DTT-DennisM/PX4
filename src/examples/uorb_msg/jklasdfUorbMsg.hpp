#pragma once

#include <px4_platform_common/defines.h>
#include <px4_platform_common/module.h>
#include <px4_platform_common/module_params.h>
#include <px4_platform_common/posix.h>
#include <px4_platform_common/px4_work_queue/ScheduledWorkItem.hpp>
#include <uORB/PublicationMulti.hpp>
#include <uORB/Subscription.hpp>
#include <uORB/topics/video_monitor.h>

class UorbMsg : public ModuleBase<UorbMsg>, public ModuleParams, public px4::ScheduledWorkItem {
public:
    UorbMsg(double latitude = 29.6603018, double longitude = -82.3160500);

    ~UorbMsg() override = default;

    bool init();

private:
    static constexpr uint32_t SENSOR_INTERVAL_US{1000000 / 5};

    uORB::PublicationMulti<video_monitor_s> _video_monitor_pub{ORB_ID(video_monitor)};

    int32_t _latitude{296603018}; // lat in 1e-7
    int32_t _longitude{-823160500}; // lon in 1e-7
    int32_t getOut = 1;
    int32_t poll_ret = -1;
};
