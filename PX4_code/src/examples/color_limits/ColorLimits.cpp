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

//using namespace time_literals;


extern "C" __EXPORT int color_limits_main(int argc, char *argv[]);

int color_limits_main(int argc, char *argv[]) {
  PX4_INFO("Hello, I am only a test program able to inject COLOR_LIMITS messages.");
  PX4_INFO("Enter all values: h_low, h_up, s_low, s_up, v_low, v_up");
  int h_low = -1, h_up = -1, s_low = -1, s_up = -1, v_low = -1, v_up = -1;
  scanf("%d%d%d%d%d%d", &h_low, &h_up, &s_low, &s_up, &v_low, &v_up);

  // if needed to exit!

  if (h_low < -1)
    return 0;

  // Declare structure to store data that will be sent
  PX4_INFO("Thanks for inserting!");
  struct color_limits_s colorLim;

  // Clear the structure by filling it with 0s in memory
  memset(&colorLim, 0, sizeof(colorLim));

  // Create a uORB topic advertisement
  orb_advert_t color_limits_pub = orb_advertise(ORB_ID(color_limits), &colorLim);


  // USER INPUT
  char myStr[] = {"Salut !!"}; memcpy(colorLim.info, myStr, 9);
  bool retransmit = true;
  // run this as long as no acknowledgment has been received
  while(retransmit) {

    // Set all values and transmit the message
    colorLim.timestamp = hrt_absolute_time();
    colorLim.h_low = h_low;
    colorLim.h_up = h_up;
    colorLim.s_low = s_low;
    colorLim.s_up = s_up;
    colorLim.v_low = v_low;
    colorLim.v_up = v_up;
    orb_publish(ORB_ID(color_limits), color_limits_pub, &colorLim);

    // To get out of loop when acknowledgment has been received
    int getOut = 1;

    // Set the communication address and the interval to 1 second
    int color_sub_fd = orb_subscribe(ORB_ID(color_limits));

    // Check now many seconds have been passed since transmission
    int counter = 0;

    // Set variables for polling
    int poll_ret;
    orb_set_interval(color_sub_fd, 200);
    px4_pollfd_struct_t fds_color;
    fds_color.fd = color_sub_fd;
    fds_color.events = POLLIN;

    // Waiting for acknowledgment
    while(getOut) {
      poll_ret = px4_poll(&fds_color, 1, 2000);

      if(poll_ret == 0) {

        // Antoher second without an acknowledgment
        counter++;
        if(counter > 5) {
          // 5 seconds have been passed without acknowledgment
          retransmit = true;
          PX4_ERR("It has been 5 seconds, retransmitting now");
          break;
        }
      }

      else {
      // Message received
        counter = 0;
        if(fds_color.events && POLLIN) {

          orb_copy(ORB_ID(color_limits), color_sub_fd, &colorLim);
          if(colorLim.info[0] == 'A') {
	          // The message is an acknowledgment
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
