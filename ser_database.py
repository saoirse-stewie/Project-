import MySQLdb
import serial

ser = serial.Serial("/dev/ttyAMA0")
ser.baudrate = 9600

db = MySQLdb.connect("localhost", "root" , "root", "framedata")

cursor = db.cursor()

while 1:
	data = ser.read(1)
	print(data)
	ser.write("received")
	if data == 'h':
		print(data)
		sql_find_particular = 'SELECT CL_LP FROM STARTUP'
		print ("%s",sql_find_particular)
		try:
			cursor.execute(sql_find_particular)
			results = cursor.fetchone()
			
			for result in results:
				print results
		except:
			print "error: unable to fetch data"
			
db.close()
ser.close()
