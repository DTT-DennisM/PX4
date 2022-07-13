#include <stdlib.h>
#include <px4_platform_common/defines.h>
#include <px4_platform_common/module.h>
#include <px4_platform_common/module_params.h>
#include <px4_platform_common/posix.h>
#include <px4_platform_common/px4_work_queue/ScheduledWorkItem.hpp>
#include <uORB/PublicationMulti.hpp>
#include <uORB/Subscription.hpp>
#include <uORB/topics/video_monitor.h>
#include <stdio.h>
#include <sys/select.h>
#include <termios.h>


using namespace time_literals;

extern "C" __EXPORT int handshake_main(int argc, char *argv[]) {
    int poll_ret;
    int getOut = 1;
    int counter = 0;
    PX4_INFO("Hello, I am handshake");
    int video_sub_fd = orb_subscribe(ORB_ID(video_monitor));
    orb_set_interval(video_sub_fd, 200);

    px4_pollfd_struct_t fds_video;
    fds_video.fd = video_sub_fd;
    fds_video.events = POLLIN;
    while(getOut) {
        poll_ret = px4_poll(&fds_video, 1, 2000);

        if(poll_ret == 0) {
            	PX4_ERR("Ono, got 0 data within a second :(");
		counter++;
		if(counter > 5) {
			getOut = 0;
		}
        } else {
	    counter = 0;
            if(fds_video.events && POLLIN) {
                video_monitor_s videoMon{};

                orb_copy(ORB_ID(video_monitor), video_sub_fd, &videoMon);
		if((double)videoMon.confidence < 1.0) {
			for(int i = 0; i < 2; i++) {
				printf("lat= %ld | long= %ld | no. people= %d | confidence= %1.3f | %s\n",
					videoMon.lat, videoMon.lon, videoMon.no_people,
					(double)videoMon.confidence, videoMon.info);
			}
			orb_advert_t video_monitor_pub = orb_advertise(ORB_ID(video_monitor), &videoMon);
			videoMon.confidence = 1.0;
			orb_publish(ORB_ID(video_monitor), video_monitor_pub, &videoMon);
			videoMon.lat++;
		}
            }
        }
    }
    return 0;
}
