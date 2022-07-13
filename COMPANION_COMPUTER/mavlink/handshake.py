"""
This script will:
 > Connect to the pixhawk
 > Perform a handshake with the pixhawk to open the data link between the devices
"""
from pymavlink import mavutil
import time
mavutil.set_dialect("video_monitor")

def perform(px4):
	print("Starting handshake")
	while True:
        # Send handshake message
		px4.mav.video_monitor_send(
		timestamp = int(time.time()*1e6),
		info = b'Shake!!',
		lat = 0,
		lon = 0,
		no_people = 0,
		confidence = 0)
        
        # Receive message, wait for a maximum of 3 seconds
		msg = px4.recv_match(type='VIDEO_MONITOR', blocking=True, timeout=3.0)
		try:
            # If it is an acknowledgment
			if msg.confidence == 1:
				break
		except:
			print("retransmit")
			continue
	print("Handshake completed!!")
