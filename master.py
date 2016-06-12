#!/usr/bin/python

import os.path, time, subprocess

onTime = 320
timePassed = 0
newTime = 0
turnedOff = 0

while True:
	newTime = os.path.getmtime("/opt/rec/tvon")
	timePassed = int(time.time()) - newTime
	if turnedOff == 0 and timePassed > onTime:
		print "Time over"
		try:
			subprocess.call(["/opt/rec/off.sh"])
		except:
			print "Unexpected exception"
		turnedOff = 1
	elif timePassed <= onTime:
		turnedOff = 0
	time.sleep(1)
