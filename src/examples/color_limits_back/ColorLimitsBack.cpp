#include "ColorLimitsBack.hpp"
#include <stdlib.h>
#include <px4_platform_common/defines.h>
#include <px4_platform_common/module.h>
#include <px4_platform_common/module_params.h>
#include <px4_platform_common/posix.h>
#include <px4_platform_common/px4_work_queue/ScheduledWorkItem.hpp>
#include <uORB/PublicationMulti.hpp>
#include <uORB/Subscription.hpp>
#include <uORB/topics/video_monitor.h>

using namespace time_literals;

ColorLimits::ColorLimits() :
    ModuleParams(nullptr),
    ScheduledWorkItem(MODULE_NAME, px4::wq_configurations::lp_default)
//    _latitude(latitude_deg * 10e6),
//    _longitude(longitude_deg * 10e6)
{
}

bool ColorLimits::init() {
    ScheduleOnInterval(SENSOR_INTERVAL_US);
    return true;
}

void ColorLimits::Run() {
    if(should_exit()) {
        ScheduleClear();
        exit_and_cleanup();
        return;
    }
/*
    uint64_t timestamp;
	int32_t lat;
	int32_t lon;
	float confidence;
	uint16_t no_people;
	char info[11];
	uint8_t _padding0[7]; // required for logger
*/


	struct color_limits_s colorLim;
	memset(&colorLim, 0, sizeof(colorLim));

	orb_advert_t color_limits_pub = orb_advertise(ORB_ID(color_limits), &colorLim);

//	for(int i = 0; i < 20; i++) {
		char myStr[] = {"Salut !!"}; memcpy(colorLim.info, myStr, 9);
		colorLim.h_low = rand() % 50;
		colorLim.h_up = 150;
		colorLim.s_low = rand() % 250;
		colorLim.s_up = 100;
		colorLim.v_low = rand() % 150;
		colorLim.v_up = 50;

		colorLim.timestamp = hrt_absolute_time();

		orb_publish(ORB_ID(color_limits), color_limits_pub, &colorLim);
		usleep(2000000);

//	}
//	_video_monitor_pub.publish(video_monitor);
	return;
}

int ColorLimits::task_spawn(int argc, char *argv[]) {
    ColorLimits *instance = new ColorLimits();

    if(instance) {
        _object.store(instance);
        _task_id = task_id_is_work_queue;

        if(instance -> init()) {
            return PX4_OK;
        }
    } else {
        PX4_ERR("alloc failed");
    }

    delete instance;
    _object.store(nullptr);
    _task_id = -1;

    return PX4_ERROR;
}

int ColorLimits::custom_command(int argc, char *argv[]) {
    return print_usage("unknown command");
}

int ColorLimits::print_usage(const char *reason) {
    if(reason) {
        PX4_WARN("%s\n", reason);
    }

    PRINT_MODULE_DESCRIPTION(R"DESCR_STR(### Description)DESCR_STR");

    PRINT_MODULE_USAGE_NAME("color_limits", "driver");
    PRINT_MODULE_USAGE_COMMAND("start");
    PRINT_MODULE_USAGE_DEFAULT_COMMANDS();
    return 0;
}

extern "C" __EXPORT int color_limits_back_main(int argc, char *argv[]);
int color_limits_back_main(int argc, char *argv[]) {
	PX4_INFO("Hello, I am only a test program able to inject COLOR_LIMITS messages. blablabla");
	PX4_INFO("Hello, I am only a test program able to inject COLOR_LIMITS messages.");
/*
    // Declare structure to store data that will be sent
    struct video_monitor_s videoMon;

    // Clear the structure by filling it with 0s in memory
    memset(&videoMon, 0, sizeof(videoMon));

    // Create a uORB topic advertisement
    orb_advert_t video_monitor_pub = orb_advertise(ORB_ID(video_monitor), &videoMon);

    for (int i=0; i<40; i++)
        {
        char myStr[]={"Salut !!"}; memcpy(videoMon.info, myStr, 9);
        videoMon.timestamp = hrt_absolute_time();
        videoMon.lat  = i;
        videoMon.lon  = 12345678;
        videoMon.no_people  = i+5;
        videoMon.confidence  = 0.369;

        orb_publish(ORB_ID(video_monitor), video_monitor_pub, &videoMon);
        printf("lat= %ld | lon= %ld | no. people= %d | confidence= %1.3f | %s\n",
		videoMon.lat, videoMon.lon, videoMon.no_people,
		(double)videoMon.confidence, videoMon.info);
        //sleep for 2s
        usleep (2000000);
        }

    PX4_INFO("inject_myUORB finished!");

    return 0;
*/
    return ColorLimits::main(argc, argv);
}
