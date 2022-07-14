from math import *
R = 6371000 # Radius of earth in m (approx)
toRad = pi/180 # Convert degrees to radians
toDeg = 180/pi # Convert radians to degrees

# Calculate distance between 2 coordinates
def calc_distance(lat1, lon1, lat2, lon2):

	phi1 = lat1 * toRad
	phi2 = lat2 * toRad
	dphi = (lat2-lat1) * toRad
	dlam = (lon2-lon1) * toRad

	a = sin(dphi/2) * sin(dphi/2) + \
	cos(phi1) * cos(phi2) * \
	sin(dlam/2) * sin(dlam/2)

	c = 2 * atan2(sqrt(a), sqrt(1-a))

	return R * c

# Calculate second coordinate based on first coordinate, heading and distance to second point
def calc_second_coord(latitude, longitude, heading, d):
	head = heading * toRad

	lat1 = latitude * toRad
	lon1 = longitude * toRad

	lat2 = asin(sin(lat1) * cos(d/R) + \
	cos(lat1) * sin(d/R) * cos(head))

	lon2 = lon1 + atan2(sin(head) * sin(d/R) * cos(lat1), \
	cos(d/R) - sin(lat1) * sin(lat2))

	lat2 = lat2 * toDeg
	lon2 = lon2 * toDeg

	return lat2, lon2

# Calculate the angle between two coordinates
def calc_heading(lat1, lon1, lat2, lon2):
	dLon = (lon2 - lon1) * toRad
	lat1 = lat1 * toRad
	lon1 = lon1 * toRad
	lat2 = lat2 * toRad
	lon2 = lon2 * toRad
	y = sin(dLon) * cos(lat2)
	x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dLon)

	heading = ((atan2(y, x)) * toDeg + 360) % 360
	return heading
