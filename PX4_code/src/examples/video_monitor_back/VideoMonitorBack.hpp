#pragma once

#include <px4_platform_common/defines.h>
#include <px4_platform_common/module.h>
#include <px4_platform_common/module_params.h>
#include <px4_platform_common/posix.h>
#include <px4_platform_common/px4_work_queue/ScheduledWorkItem.hpp>
#include <uORB/PublicationMulti.hpp>
#include <uORB/Subscription.hpp>
#include <uORB/topics/video_monitor.h>

class VideoMonitor : public ModuleBase<VideoMonitor>, public ModuleParams, public px4::ScheduledWorkItem {
public:
    VideoMonitor(double latitude = 29.6603018, double longitude = -82.3160500);

    ~VideoMonitor() override = default;

    static int task_spawn(int argc, char *argv[]);

    static int custom_command(int argc, char *argv[]);

    static int print_usage(const char *reason = nullptr);

    bool init();

private:
    static constexpr uint32_t SENSOR_INTERVAL_US{1000000 / 5};

    void Run() override;

    uORB::PublicationMulti<video_monitor_s> _video_monitor_pub{ORB_ID(video_monitor)};

    int32_t _latitude{296603018}; // lat in 1e-7
    int32_t _longitude{-823160500}; // lon in 1e-7
};
