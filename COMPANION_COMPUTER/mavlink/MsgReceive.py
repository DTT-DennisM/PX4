from pymavlink import mavutil

mavutil.set_dialect("video_monitor")
#mavutil.set_dialect("my_MAVLinkLib")

px4 = mavutil.mavlink_connection("/dev/ttyACM0", baud=921600)

px4.wait_heartbeat()

print("heartbeat from system %u component %u" % (px4.target_system, px4.target_component))

while True:
	msg = px4.recv_match(blocking=True)
	try:
		if((msg.get_type() == 'SCALED_IMU') or
		(msg.get_type() == 'SCALED_IMU2') or
		(msg.get_type() == 'HIGHRES_IMU') or
		(msg.get_type() == 'ATTITUDE') or
		(msg.get_type() == 'ALTITUDE') or
		(msg.get_type() == 'HEARTBEAT') or
		(msg.get_type() == 'VFR_HUD') or
		(msg.get_type() == 'ACTUATOR_CONTROL_TARGET') or
		(msg.get_type() == 'ODOMETRY') or
		(msg.get_type() == 'CURRENT_EVENT_SEQUENCE') or
		(msg.get_type() == 'ATTITUDE_QUATERNION') or
		(msg.get_type() == 'TIMESYNC') or
		(msg.get_type() == 'ATTITUDE_TARGET') or
		(msg.get_type() == 'ESTIMATOR_STATUS') or
		(msg.get_type() == 'EXTENDED_SYS_STATE') or
		(msg.get_type() == 'LINK_NODE_STATUS') or
		(msg.get_type() == 'SYS_STATUS') or
		(msg.get_type() == 'SYSTEM_TIME') or
		(msg.get_type() == 'BATTERY_STATUS') or
		(msg.get_type() == 'PING') or
		(msg.get_type() == 'VIBRATION')):
			continue

	except:
		print("nothin")
	if not msg:
		print('no msg!')
	elif msg.get_type() == 'BAD_DATA':
		if mavutil.all_printable(msg.data):
			sys.stdout.write(msg.data)
			sys.stdout.flush()

	elif msg.get_type() == "VIDEO_MONITOR":
		print(msg.to_dict())
	elif msg.get_type() == "COLOR_LIMITS":
		print(msg.to_dict())
	print(msg.to_dict())
