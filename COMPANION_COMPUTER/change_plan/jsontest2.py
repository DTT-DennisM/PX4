import json
from dist_2_waypoints import *

latLoc = 4
lonLoc = 5

def get_latlon(index, data):
	items = data['mission']['items']
	try:
		# Get latitude and longitude from complex waypoint
		if items[index]['type'] == "ComplexItem" and \
		(items[index]['complexItemType'] == "vtolLandingPattern" or \
		items[index]['complexItemType'] == "fwLandingPattern"):
			lat = items[index]['landCoordinate'][0]
			lon = items[index]['landCoordinate'][1]
			return lat, lon
		elif isinstance(items[index]['params'][latLoc], float) and \
		items[index]['type'] == "SimpleItem":
			# Get latitude and longitude from normal waypoint
			lat = items[index]['params'][latLoc]
			lon = items[index]['params'][lonLoc]
			return lat, lon
	except:
		print("passing")

def rot_waypoints(file, angle):
	#Open file
	f = open(file,'r')
	data = json.load(f)
	
	# Get the latitude and longitude of the starting location
	oLat = data['mission']['plannedHomePosition'][0]
	oLon = data['mission']['plannedHomePosition'][1]
	
	# Rotate every waypoint around starting point
	for i in range(1,len(data['mission']['items'])):
		
		# Get latitude and longitude of coordinate
		[lat,lon] = get_latlon(i, data)
		
		# Calculate rotational angle
		heading = calc_heading(oLat, oLon, lat, lon)
		
		# Calculate new coordinates
		[newLat, newLon] = calc_second_coord(oLat, oLon, heading + angle, calc_distance(oLat, oLon, lat, lon))
		
		# Set new coordinate
		try:
			# Normal waypoint
			data['mission']['items'][i]['params'][latLoc] = newLat
			data['mission']['items'][i]['params'][lonLoc] = newLon
		except:
			# Special waypoint, ONLY WORKS FOR VTOL RIGHT NOW
			data['mission']['items'][i]['landCoordinate'][0] = newLat
			data['mission']['items'][i]['landCoordinate'][1] = newLon
			lanLat = data['mission']['items'][i]['landingApproachCoordinate'][0]
			lanLon = data['mission']['items'][i]['landingApproachCoordinate'][1]
			heading = calc_heading(oLat, oLon, lanLat, lanLon)
			[newLat, newLon] = calc_second_coord(oLat, oLon, heading + angle, calc_distance(oLat, oLon, lanLat, lanLon))
			data['mission']['items'][i]['landingApproachCoordinate'][0] = newLat
			data['mission']['items'][i]['landingApproachCoordinate'][1] = newLon
	return data

def mv_waypoints(file, nLat, nLon):
	#Open file
	f = open(file, 'r')
	data = json.load(f)
	
	# Get the latitude and longitude of the starting location
	oLat = data['mission']['plannedHomePosition'][0]
	oLon = data['mission']['plannedHomePosition'][1]
	
	#Calculate distance and angle between original and new position
	moveDistance = calc_distance(oLat, oLon, nLat, nLon)
	moveHeading = calc_heading(oLat, oLon, nLat, nLon)
	
	# Move every coordinate with respect to the previously calculated distance and heading
	for i in range(1,len(data['mission']['items'])):
		
		# Get latitude and longitude of coordinate
		[lat, lon] = get_latlon(i, data)
		
		# Calculate distance and angle between original starting point and coordinate
		heading = calc_heading(oLat, oLon, lat, lon)
		dist = calc_distance(oLat, oLon, lat, lon)
		
		# Calculate new location of coordinate
		[newLat, newLon] = calc_second_coord(lat, lon, moveHeading, moveDistance)
		
		# Save new coordinate
		try:
			# Normal waypoint
			data['mission']['items'][i]['params'][latLoc] = newLat
			data['mission']['items'][i]['params'][lonLoc] = newLon
		except:
			# Special waypoint, ONLY WORKS FOR VTOL RIGHT NOW
			data['mission']['items'][i]['landCoordinate'][0] = newLat
			data['mission']['items'][i]['landCoordinate'][1] = newLon
			lanLat = data['mission']['items'][i]['landingApproachCoordinate'][0]
			lanLon = data['mission']['items'][i]['landingApproachCoordinate'][1]
			heading = calc_heading(oLat, oLon, lanLat, lanLon)
			dist = calc_distance(oLat, oLon, lanLat, lanLon)
			[newLat, newLon] = calc_second_coord(lanLat, lanLon, moveHeading, moveDistance)
			data['mission']['items'][i]['landingApproachCoordinate'][0] = newLat
			data['mission']['items'][i]['landingApproachCoordinate'][1] = newLon
			
	# Set starting position to new coordinates
	[newLat, newLon] = calc_second_coord(oLat, oLon, moveHeading, moveDistance)
	data['mission']['plannedHomePosition'][0] = newLat
	data['mission']['plannedHomePosition'][1] = newLon
	return data
