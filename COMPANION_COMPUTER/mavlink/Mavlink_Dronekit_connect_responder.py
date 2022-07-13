
"""
This script will:
 > Connect to the pixhawk
 > When disarmed, able to receive message from ground station to change color recognition boundaries
 > When armed, switch to color recognition algorithm and detect color
"""

from pymavlink import mavutil
from dronekit import connect, VehicleMode
import time
import json
import numpy as np
import cv2
import math

# Set dialect to receive custom messages
mavutil.set_dialect("my_MAVLinkLib")

#use USB serial connection so set connection_string
connection_string = "/dev/ttyACM0"

#set baud rate to correspond to the set value on Pixhawk
baud_rate = 115200
h_low = -1
h_up = -1
s_low = -1
s_up = -1
v_low = -1
v_up = -1

# Send the acknowledgment
def sendAck(px4, msg):
	px4.mav.color_limits_send(
	timestamp = int(time.time() * 1e6),
	info = b'Ack!',
	h_low = msg.h_low,
	h_up = msg.h_up,
	s_low = msg.s_low,
	s_up = msg.s_up,
	v_low = msg.v_low,
	v_up = msg.v_up)

# Set the search boundaries
def set_boundaries(msg):
	global h_low, h_up, s_low, s_up, v_low, v_up
	h_low = msg.h_low
	h_up = msg.h_up
	s_low = msg.s_low
	s_up = msg.s_up
	v_low = msg.v_low
	v_up = msg.v_up
	
def getData():
    global h_low
    try: # USB
        path = open('/media/usb/color_limits.json','r')
        print("USB")
    except: # Pre-written script
        path = open('/home/pi/Documents/DTT4.0/limits.json','r')
        print("File")
    # Get data from file and return it
    data = json.load(path)
    return [data[0], data[1], data[2]]
            

# Pymavlink connection
def px():
    # Create connection and wait for heartbeat
	px4 = mavutil.mavlink_connection(connection_string, baud=921600)
	print("waiting for hb")
	px4.wait_heartbeat()
	print("done hb")
	
	#TODO: when attempting to use armed/disarmed functions, comment this statement
	# Counter to exit the loops when not using the arm/disarm functionalities
	counter = 0
	
	# Stay in loop until the drone is armed
	while True:
		msg = px4.recv_match(type='COLOR_LIMITS')
		if msg:
			if msg.info != 'Ack!':
				print(msg.to_dict())

				# Send an acknowledgment
				sendAck(px4, msg)

				# Set the boundaries
				set_boundaries(msg)

				#TODO: when attempting to use armed/disarmed functions, comment this statement
				counter += 1
        # If the motors are armed, switch to dronekit connection
		if px4.motors_armed():
			print("drone armed!")
			break
		#TODO: when attempting to use armed/disarmed functions, comment this statement
		elif counter > 0:
			counter = 0
			break

# Dronekit connection
def dk():
	global h_low, h_up, s_low, s_up, v_low, v_up
	# Retrieve boundaries, Script -> USB -> Pre-existing boundaries on Pi
	# If no boundaries have been written from the script
	if h_low == -1:
        #Try to get data from USB, if no USB is present, use the pre-written data
		[[h_low,h_up],[s_low,s_up],[v_low,v_up]] = getData()
		#print("h_low: %d | h_up: %d | s_low: %d | s_up: %d | v_low: %d | v_up: %d"% (h_low, h_up, s_low, s_up, v_low, v_up))

    # Create dronekit connection and for a connection message
	px4 = connect(connection_string, wait_ready=False)

	px4.wait_ready('autopilot_version')
	counter = 0
	cap = cv2.VideoCapture(0)
	while True: # TODO: DO DRONEKIT STUFF HERE
        
        # Get width and height
		ret,frame = cap.read()
		width = int(cap.get(3))
		height = int(cap.get(4))

        # Convert colorspace from BGR to HSV
		hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        
        # Set the upper and lower bounds
		lower_bound = np.array([h_low, s_low, v_low])
		upper_bound = np.array([h_up, s_up, v_up])

        # Create a mask and apply it to the incoming video signal
		mask = cv2.inRange(hsv, lower_bound, upper_bound)
		cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE) [-2]
		
		# If pixels within the boundaries have been detected
		if len(cnts) > 0:
			areaCol = max(cnts, key=cv2.contourArea)
			(xg,yg,wg,hg) = cv2.boundingRect(areaCol)
			area = wg * hg
            # Check if the area of the pixels is big enough to be recognized as white rectangle
			if 2527 < area < 480000:
                # Draw rectangle around area and calculate the center
				cv2.rectangle(frame,(xg,yg),(xg+wg,yg+hg),(0,255,0),2)
				centerx = xg + (wg/2)
				centery = yg + (hg/2)
				center = [centerx,centery]
				print(center)
#				cv2.circle(frame,(xg,yg),radius=10,color=(255,255,255),thickness=-1)

# angle attempt
"""
				focal = 2714.3 / 4
#				cx = 959.5 / 2
#				cy = 539.5 / 2
				cx = (width-1) / 2
				cy = (height-1) / 2

				v_x = (centerx-cx) / focal
				v_y = (centery-cy) / focal

				a_rad = math.atan(v_x)
				a_deg = math.atan(v_x) * 180/math.pi
#				print(a_rad)
				h = 40
				x_dist = math.tan(a_rad)*h
				print(wg)
				print(hg)
"""
#end angle attempt

        # Show the frame
		cv2.imshow('frame',frame)
        
        #TODO: when attempting to use armed/disarmed functions, uncomment this part
        # Check if the drone is disarmed
        #if not px4.armed:
        #    break
        
        # Exit when 'q' has been pressed
		if cv2.waitKey(1) == ord('q'):
			break
	print("Out of loop!")
	# Stop the video connection and close the dronekit connection
	cap.release()
	cv2.destroyAllWindows()
	px4.close()


# Switch between pymavlink and dronekit
while True:
	print("to px")
	px()
	print("to dk")
	dk()
