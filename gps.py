import serial as s
import string
import os
import webbrowser
import geopy.distance

#ATTENTION FONCTIONNE AVEC LE CODE ARDUINO myGPS

ser = s.Serial()
ser.baudrate = 9600
ser.port = "COM4"
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
	latitude = float(z[2]) / 100
	longitude = float(z[4]) / 100
	lati = str(latitude)
	longi = str(longitude)
	url = "http://www.coordonnees-gps.fr/latitude-longitude/" + lati + "/" + longi + "/10/roadmap"
	webbrowser.open_new(url)


ser.close


# Calcul de distance entre un point A et un point B
coords_1 = (4336.9693 , 704.3612)
coords_2 = (4337.0442, 704.3317
)

print geopy.distance.vincenty(coords_1, coords_2).km