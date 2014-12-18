import serial
import datetime

ser = serial.Serial('/dev/ttyACM0',9600,timeout=2) 


while True:
	line = str(ser.readline().strip())
	if line:
		current_time = datetime.datetime.now()
		file = open('vacuum_chamber.log', 'a')
		print(current_time.isoformat() + " " + line) 
		file.write(current_time.isoformat() + " " + line + "\n") 
		file.close()
	pass




