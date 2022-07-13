"""
This script will:
 > Connect to the pixhawk
 > Send a custom VIDEO_MONITOR message
 > Wait for an acknowledgment before sending another message
 > Retransmit if no acknowledgment has been received
"""
from pymavlink import mavutil
import time

# Set the dialect to receive the correct messages
mavutil.set_dialect("video_monitor")

# Connect to the serial port of the pixhawk
px4 = mavutil.mavlink_connection("/dev/ttyACM0", baud=921600)

# Waiting for a heartbeat
px4.wait_heartbeat()

print("Heartbeat from system (system %u component %u)" %(px4.target_system, px4.target_component))

counter = 0
retransmit = False
while True:
#	if not retransmit:
    # Send a custom message
	px4.mav.video_monitor_send(
	timestamp = int(time.time()*1e6),
	info = b'Salut!',
	lat = counter,
	lon = 231234567,
	no_people = 6,
	confidence = 0.357)
	print("The custom message with the number %u was sent!" %(counter))
	retransmit = False
    # Wait for acknowledgment before sending new message
	while True:
        # Wait for acknowledgment for a maximum of 3 seconds
		msg = px4.recv_match(type='VIDEO_MONITOR', blocking=True, timeout=3.0)
		try:
            # If acknowledgment has been received
			if msg.confidence == 1 and msg.lat == counter:
				break
		except:
            # No acknowledgment received, going to retransmit message
			print("No response, retransmission!!")
			retransmit = True
			break

	counter += 1
	time.sleep(4.0)

