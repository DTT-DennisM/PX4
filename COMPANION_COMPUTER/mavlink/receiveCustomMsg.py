import handshake
from pymavlink import mavutil
import time

# Set correct dialect to receive custom messages
mavutil.set_dialect("my_MAVLinkLib")

# Set serial port
px4 = mavutil.mavlink_connection("/dev/ttyACM0", baud=921600)

# Waiting for heartbeat from pixhawk
px4.wait_heartbeat()

# Handshake to test the connection
answer = input("Need handshake? ")
if answer == 'y':
	handshake.perform(px4)

# Function for sending an acknowledgment
def sendAck(msg):
	px4.mav.color_limits_send(
	timestamp = int(time.time() * 1e6),
	info = b'Ack!',
	h_low = msg.h_low,
	h_up = msg.h_up,
	s_low = msg.s_low,
	s_up = msg.s_up,
	v_low = msg.v_low,
	v_up = msg.v_up)

# Loop to receive messages
while (True):
    # Wait until message has been received
	msg = px4.recv_match(blocking=True)
	# If something went wrong and message is not valid
	if not msg:
		print('No msg!\n')
		continue
	elif msg.get_type() == "BAD DATA":
		if mavutil.all_printable(msg.data):
			sys.stdout.write(msg.data)
			sys.stdout.flush()
			
	# If VIDEO_MONITOR message has been received
	elif msg.get_type() == "VIDEO_MONITOR":
		print('Info: %s' % msg.info)
		print('Latitude: %d' % msg.lat)
		print('Longitude: %d' % msg.lon)
		print('No. people: %d' % msg.no_people)
		print('Confidence: %f' % msg.confidence)
		print('\n')
		
    # If COLOR_LIMITS message has been received
	elif msg.get_type() == "COLOR_LIMITS" and msg.info != "Ack!":
		print('Info: %s' % msg.info)
		print('h_low: %d' % msg.h_low)
		print('h_up: %d' % msg.h_up)
		print('s_low: %d' % msg.s_low)
		print('s_up: %d' % msg.s_up)
		print('v_low: %d' % msg.v_low)
		print('v_up: %d' % msg.v_up)
        # Send acknowledgment
		sendAck(msg)
		print('Sent Ack!')
		print('\n')
