#include <px4_platform_common/px4_config.h>
#include <px4_platform_common/posix.h>
#include <px4_platform_common/defines.h>
#include <px4_platform_common/module.h>
#include <px4_platform_common/module_params.h>
#include <px4_platform_common/px4_work_queue/ScheduledWorkItem.hpp>

#include <unistd.h>
#include <stdio.h>
#include <poll.h>
#include <string.h>

#include <uORB/PublicationMulti.hpp>
#include <uORB/Subscription.hpp>
#include <uORB/uORB.h>
#include <uORB/topics/color_limits.h>

using namespace time_literals;


extern "C" __EXPORT int color_limits_main(int argc, char *argv[]);

int color_limits_main(int argc, char *argv[])
{
    PX4_INFO("Hello, I am only a test program able to inject COLOR_LIMITS messages.");
//-----------------------------------------------------------------------------
    PX4_INFO("Enter all ze numbers: h_low, h_up, s_low, s_up, v_low, v_up");
    int h_low = -1, h_up = -1, s_low = -1, s_up = -1, v_low = -1, v_up = -1;
//    int input[6] = {-1,-1,-1,-1,-1,-1};
    scanf("%d%d%d%d%d%d", &h_low, &h_up, &s_low, &s_up, &v_low, &v_up);

// if needed to exit!

    if (h_low < -1) {
	return 0;
    }
// ------------------

/*
   for(int i = 0; i < 6; i++) {
	PX4_INFO("Enter new number!");
	while(input[i] == -1 || input[i] == 0) {
		scanf("%d", &input[i]);
	}
    }
    h_low = input[0];
    h_up = input[1];
    s_low = input[2];
    s_up = input[3];
    v_low = input[4];
    v_up = input[5];
*/
//-----------------------------------------------------------------------------

// while(h_low == -1 || h_up == -1 || s_low == -1 || s_up == -1 || v_low == -1 || v_up == -1) {
//    	scanf("%d%d%d%d%d%d", &h_low, &h_up, &s_low, &s_up, &v_low, &v_up);
//    }
//    printf("a: %d | b: %d | c: %d | d: %d | e: %d | f: %d", a,b,c,d,e,f);
    // Declare structure to store data that will be sent
	PX4_INFO("Thanks for inserting!");
    struct color_limits_s colorLim;

    // Clear the structure by filling it with 0s in memory
    memset(&colorLim, 0, sizeof(colorLim));

    // Create a uORB topic advertisement
    orb_advert_t color_limits_pub = orb_advertise(ORB_ID(color_limits), &colorLim);
/*
    for (int i=0; i<40; i++) { // TODO: REMOVE WHEN NEEDED
        char myStr[]={"Salut !!"}; memcpy(colorLim.info, myStr, 9);
        colorLim.timestamp = hrt_absolute_time();
        colorLim.h_low  = rand() % 100;
        colorLim.h_up  = 100;
	colorLim.s_low  = rand() % 250;
        colorLim.s_up  = 250;
	colorLim.v_low  = rand() % 150;
        colorLim.v_up  = 150;

        orb_publish(ORB_ID(color_limits), color_limits_pub, &colorLim);
	printf("h_low: %d | h_up: %d | s_low: %d | s_up: %d | v_low: %d | v_up: %d | info: %s\n",
		colorLim.h_low, colorLim.h_up, colorLim.s_low, colorLim.s_up,
		colorLim.v_low, colorLim.v_up, colorLim.info);
        //sleep for 2s
       usleep (2000000);	// TODO: REMOVE WHEN NEEDED
    }				// TODO: REMOVE WHEN NEEDED
*/

// USER INPUT
//    int h_low = rand()%256, h_up = rand()%256, s_low = rand()%256, s_up = rand()%256, v_low = rand()%256, v_up = rand()%256;
    char myStr[] = {"Salut !!"}; memcpy(colorLim.info, myStr, 9);
    bool retransmit = true;
	while(retransmit) {
	    colorLim.timestamp = hrt_absolute_time();
	    colorLim.h_low = h_low;
	    colorLim.h_up = h_up;
	    colorLim.s_low = s_low;
	    colorLim.s_up = s_up;
	    colorLim.v_low = v_low;
	    colorLim.v_up = v_up;
	    orb_publish(ORB_ID(color_limits), color_limits_pub, &colorLim);


	    int poll_ret;
	    int getOut = 1;
	    int counter = 0;
	    int color_sub_fd = orb_subscribe(ORB_ID(color_limits));
	    orb_set_interval(color_sub_fd, 200);
	    px4_pollfd_struct_t fds_color;
	    fds_color.fd = color_sub_fd;
	    fds_color.events = POLLIN;

	    while(getOut) {
		poll_ret = px4_poll(&fds_color, 1, 2000);

	        if(poll_ret == 0) {
			counter++;
			if(counter > 5) {
				retransmit = true;
				PX4_ERR("It has been 5 seconds, retransmitting now");
				break;
			}
	        } else {
		    	counter = 0;
			if(fds_color.events && POLLIN) {
	//			color_limits_s colorLim{};
				orb_copy(ORB_ID(color_limits), color_sub_fd, &colorLim);
				if(colorLim.info[0] == 'A') {
					for(int i = 0; i < 2; i++) {
						printf("Ack received containing values: h_low: %d | h_up: %d | s_low: %d | s_up: %d | v_low: %d | v_up: %d\n",
							colorLim.h_low, colorLim.h_up, colorLim.s_low,
							colorLim.s_up, colorLim.v_low, colorLim.v_up);
						retransmit = false;
						getOut = 0;
					}
				}
			}
		}
	    }
	}

    PX4_INFO("color_limits finished!");

    return 0;
}
