"""
 This script will:
 > Receive message from pixhawk and display them
"""
from pymavlink import mavutil
import time

px4 = mavutil.mavlink_connection("/dev/ttyACM0", baud=9600)

print("Waiting for heartbeat")

px4.wait_heartbeat()

print("Received heartbeat from px4!!")

while True:
	try:
		print(px4.recv_match().to_dict())
	except:
		pass
	time.sleep(1.0)
