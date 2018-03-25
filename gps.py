import serial as s
import string
import os
import webbrowser
import geopy.distance


ser = s.Serial()
ser.baudrate = 9600
ser.port = "COM5"
ser.timeout=5
ser.open()


while 1 :
	x = ser.readline()
	print("valeur du serial = " + x)
	if x.find('$GPGGA'):
		y = 0
	else:
		y = x
		break

if y != 0:
    z = string.split(y, ',')
else:
    z = 0

if z != 0:
	latitude = z[2]
	longitude = z[4]
	print("latitude " + latitude + " longitude " + longitude)
	url = "http://www.coordonnees-gps.fr/latitude-longitude/" + latitude + "/" + longitude + "/10/roadmap"
	webbrowser.open_new(url)


ser.close


# Calcul de distance entre un point A et un point B
coords_1 = (52.2296756, 21.0122287)
coords_2 = (52.406374, 16.9251681)

print geopy.distance.vincenty(coords_1, coords_2).km