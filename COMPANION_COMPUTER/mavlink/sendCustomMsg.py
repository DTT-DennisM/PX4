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
#	counter += 1
#	time.sleep(1.0)
    # Wait for acknowledgment before sending new message
	while True:
        # Wait for acknowledgment for a maximum of 3 seconds
		msg = px4.recv_match(type='VIDEO_MONITOR', blocking=True, timeout=3.0)
		try:
            # If acknowledgment has been received
			if msg.confidence == 1 and msg.lat == counter:
#				print('gud confidence: %f' % msg.confidence)
				break
#			else:
#				print('no gud confidence: %f' % msg.confidence)
		except:
            # No acknowledgment received, going to retransmit message
			print("No response, retransmission!!")
			retransmit = True
			break
"""
	if not msg:
		print('No msg!\n')
		continue
	if msg.get_type() == "BAD_DATA":
		if mavutil.all_printable(msg.data):
			sys.stdout.write(msg.data)
			sys.stdout.flush()
"""
#	else:
#		print('Info: %s | Latitude: %d | Longitude: %d | No. people: %d | Confidence: %f\n' % (msg.info, msg.lat, msg.lon, msg.no_people, msg.confidence))
#		print('Latitude: %d' % msg.lat)
#		print('Longitude: %d' % msg.lon)
#		print('No. people: %d' % msg.no_people)
#		print('Confidence: %f' % msg.confidence)
#		print('\n')

	counter += 1
	time.sleep(4.0)

