import json
#from dist_2_waypoints import calc_distance, calc_second_coord, calc_heading
from dist_2_waypoints import *

latLoc = 4
lonLoc = 5
#f = open('test.plan')
#data = json.load(f)
lats = []
lons = []

def get_latlon(index, data):
	items = data['mission']['items']
	try:
		if items[index]['type'] == "ComplexItem" and \
		(items[index]['complexItemType'] == "vtolLandingPattern" or \
		items[index]['complexItemType'] == "fwLandingPattern"):	# Landing
			lat = items[index]['landCoordinate'][0]
			lon = items[index]['landCoordinate'][1]
			return lat, lon
		elif isinstance(items[index]['params'][latLoc], float) and \
		items[index]['type'] == "SimpleItem":	# Normal waypoint
			lat = items[index]['params'][latLoc]
			lon = items[index]['params'][lonLoc]
			return lat, lon
	except:
		print("passing")

def rot_waypoints(file, angle):
	f = open(file,'r')
	data = json.load(f)
	oLat = data['mission']['plannedHomePosition'][0]
	oLon = data['mission']['plannedHomePosition'][1]
	#Loop through the waypoints
	for i in range(1,len(data['mission']['items'])):
		[lat,lon] = get_latlon(i, data)
		heading = calc_heading(oLat, oLon, lat, lon)
		[newLat, newLon] = calc_second_coord(oLat, oLon, heading + angle, calc_distance(oLat, oLon, lat, lon))
		try:
			data['mission']['items'][i]['params'][latLoc] = newLat
			data['mission']['items'][i]['params'][lonLoc] = newLon
		except:
			data['mission']['items'][i]['landCoordinate'][0] = newLat
			data['mission']['items'][i]['landCoordinate'][1] = newLon
			lanLat = data['mission']['items'][i]['landingApproachCoordinate'][0]
			lanLon = data['mission']['items'][i]['landingApproachCoordinate'][1]
			heading = calc_heading(oLat, oLon, lanLat, lanLon)
			[newLat, newLon] = calc_second_coord(oLat, oLon, heading + angle, calc_distance(oLat, oLon, lanLat, lanLon))
			data['mission']['items'][i]['landingApproachCoordinate'][0] = newLat
			data['mission']['items'][i]['landingApproachCoordinate'][1] = newLon
#	f = open(file,'w')
#	json.dump(data, f, indent=5)
#	print("written sisisi")
	return data

def mv_waypoints(file, nLat, nLon):
	f = open(file, 'r')
	data = json.load(f)
	oLat = data['mission']['plannedHomePosition'][0]
	oLon = data['mission']['plannedHomePosition'][1]
	moveDistance = calc_distance(oLat, oLon, nLat, nLon)
	moveHeading = calc_heading(oLat, oLon, nLat, nLon)
	for i in range(1,len(data['mission']['items'])):
		[lat, lon] = get_latlon(i, data)
		heading = calc_heading(oLat, oLon, lat, lon)
		dist = calc_distance(oLat, oLon, lat, lon)
		[newLat, newLon] = calc_second_coord(lat, lon, moveHeading, moveDistance)
		try:
			data['mission']['items'][i]['params'][latLoc] = newLat
			data['mission']['items'][i]['params'][lonLoc] = newLon
		except:
			data['mission']['items'][i]['landCoordinate'][0] = newLat
			data['mission']['items'][i]['landCoordinate'][1] = newLon
			lanLat = data['mission']['items'][i]['landingApproachCoordinate'][0]
			lanLon = data['mission']['items'][i]['landingApproachCoordinate'][1]
			heading = calc_heading(oLat, oLon, lanLat, lanLon)
			dist = calc_distance(oLat, oLon, lanLat, lanLon)
			[newLat, newLon] = calc_second_coord(lanLat, lanLon, moveHeading, moveDistance)
			data['mission']['items'][i]['landingApproachCoordinate'][0] = newLat
			data['mission']['items'][i]['landingApproachCoordinate'][1] = newLon

	[newLat, newLon] = calc_second_coord(oLat, oLon, moveHeading, moveDistance)
	data['mission']['plannedHomePosition'][0] = newLat
	data['mission']['plannedHomePosition'][1] = newLon
#	f = open(file,'w')
#	json.dump(data, f, indent=5)
#	print("written sisisi")
	return data

def move():
	file = input("File: ")
	heading = float(input("Heading: "))
	lat = float(input("Latitude: "))
	lon = float(input("Longitude: "))
	if heading != 0:
		rot_waypoints(file, heading)
	if -90 <= lat <= 90:
		mv_waypoints(file, lat, lon)

#move()



#rot_waypoints('test3.plan', 45)
#mv_waypoints('test3.plan', 52.236380, 6.845207)
