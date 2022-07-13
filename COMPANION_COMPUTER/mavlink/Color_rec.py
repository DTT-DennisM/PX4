import numpy as np
import cv2



def detect():
	cap = cv2.VideoCapture(0)

	while True:
		ret,frame = cap.read()
		width = int(cap.get(3))
		height = int(cap.get(4))

	#	frame = cv2.resize(frame, (0,0), fx = 0.5, fy = 0.5)
	#	frame = cv2.GaussianBlur(frame, (9,9), 0)
		hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

		lower_blue = np.array([110, 50, 50])
		upper_blue = np.array([130, 255, 255])

		mask = cv2.inRange(hsv, lower_blue, upper_blue)
		bluecnts = cv2.findContours(mask.copy(),
			cv2.RETR_EXTERNAL,
			cv2.CHAIN_APPROX_SIMPLE) [-2]

		if len(bluecnts) > 0:
	#		blue_area = max(bluecnts, key=cv2.contourArea)
	#		(xg,yg,wg,hg) = cv2.boundingRect(blue_area)
	#		cv2.rectangle(frame,(xg,yg),(xg+wg,yg+hg),(0,255,0),2)
			print("detected!!!!")

	#	cv2.imshow('frame',frame)
	#	cv2.imshow('mask',mask)

	#	result = cv2.bitwise_and(frame, frame, mask=mask)

	#	cv2.imshow('frame',result)
		if cv2.waitKey(1) == ord('q'):
			break

	cap.release()
	cv2.destroyAllWindows()


#BGR_COLOR = np.array([[[255,0,0]]])
#x = cv2.cvtColor([[[255,0,0],]], cv2.COLOR_BGR2HSV)
#print(x[0][0])
