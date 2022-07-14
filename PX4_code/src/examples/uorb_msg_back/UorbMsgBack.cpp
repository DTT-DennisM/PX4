#include "UorbMsgBack.hpp"
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

UorbMsg::UorbMsg(double latitude_deg, double longitude_deg) :
    ModuleParams(nullptr),
    ScheduledWorkItem(MODULE_NAME, px4::wq_configurations::lp_default),
    _latitude(latitude_deg * 10e6),
    _longitude(longitude_deg * 10e6)
{
}

bool UorbMsg::init() {
    ScheduleOnInterval(SENSOR_INTERVAL_US);
    return true;
}

void UorbMsg::Run() {
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
	int poll_ret;
	int getOut = 1;

	PX4_INFO("Hello, I am only a test program able to receive VIDEO_MONITOR messages.");

	// Subscirbe to "video_monitor", then set a polling interval of 200ms
	int video_sub_fd = orb_subscribe(ORB_ID(video_monitor));
	orb_set_interval(video_sub_fd, 200);

	// Configure a POSIX POLLIN system to sleep the current 
	//       thread until data appears on the topic
	px4_pollfd_struct_t fds_video;
	fds_video.fd = video_sub_fd;
	fds_video.events = POLLIN;

	while (getOut)
	{
	    poll_ret = px4_poll (&fds_video, 1, 2000);
	    if ( poll_ret == 0 ) {
	        PX4_ERR ("Got no data within a second !");
            }

	    // If it didn't return 0, we got data!
	    else {
	        // Double check that the data we recieved was in the 
	        //       right format (I think - need to check)
	    	if(fds_video.revents && POLLIN)	{
	            		// declare a video_monitor_s variable to store the data we will receive
	            		video_monitor_s videoMon{};

		        	// Copy the obtaned data into the struct
			        orb_copy(ORB_ID(video_monitor), video_sub_fd, &videoMon);
				orb_advert_t video_monitor_pub = orb_advertise(ORB_ID(video_monitor), &videoMon);
				orb_publish(ORB_ID(video_monitor), video_monitor_pub, &videoMon);
				PX4_INFO("RECEIVED DATAAAAAAAAAAAAAAA");
//	            		printf ("lat= %ld | long= %ld | no. people= %d | confidence= %1.3f | %s \n",
//	                     		videoMon.lat, videoMon.lon, videoMon.no_people,
//	                     		(double)videoMon.confidence, videoMon.info);
	        }

	    }

	}
	return;
}

int UorbMsg::task_spawn(int argc, char *argv[]) {
    UorbMsg *instance = new UorbMsg();

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

int UorbMsg::custom_command(int argc, char *argv[]) {
    return print_usage("unknown command");
}

int UorbMsg::print_usage(const char *reason) {
    if(reason) {
        PX4_WARN("%s\n", reason);
    }

    PRINT_MODULE_DESCRIPTION(R"DESCR_STR(### Description)DESCR_STR");

    PRINT_MODULE_USAGE_NAME("uorb_msg_back", "driver");
    PRINT_MODULE_USAGE_COMMAND("start");
    PRINT_MODULE_USAGE_DEFAULT_COMMANDS();
    return 0;
}

extern "C" __EXPORT int uorb_msg_back_main(int argc, char *argv[]);
int uorb_msg_back_main(int argc, char *argv[]) {
	PX4_INFO("Hello, I am only a test program able to inject VIDEO_MONITOR messages. blablabla");
	PX4_INFO("Hello, I am only a test program able to inject VIDEO_MONITOR messages.");
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
    return UorbMsg::main(argc, argv);
}
