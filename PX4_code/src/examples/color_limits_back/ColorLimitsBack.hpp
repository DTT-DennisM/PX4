#pragma once

#include <px4_platform_common/defines.h>
#include <px4_platform_common/module.h>
#include <px4_platform_common/module_params.h>
#include <px4_platform_common/posix.h>
#include <px4_platform_common/px4_work_queue/ScheduledWorkItem.hpp>
#include <uORB/PublicationMulti.hpp>
#include <uORB/Subscription.hpp>
#include <uORB/topics/color_limits.h>

class ColorLimits : public ModuleBase<ColorLimits>, public ModuleParams, public px4::ScheduledWorkItem {
public:
    ColorLimits();

    ~ColorLimits() override = default;

    static int task_spawn(int argc, char *argv[]);

    static int custom_command(int argc, char *argv[]);

    static int print_usage(const char *reason = nullptr);

    bool init();

private:
    static constexpr uint32_t SENSOR_INTERVAL_US{1000000 / 5};

    void Run() override;

    uORB::PublicationMulti<color_limits_s> _color_limits_pub{ORB_ID(color_limits)};

//    int32_t _latitude{296603018}; // lat in 1e-7
//    int32_t _longitude{-823160500}; // lon in 1e-7
};
