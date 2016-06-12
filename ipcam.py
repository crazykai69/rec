#!/usr/bin/python
import subprocess
import time

while(1):
	subprocess.call( ["/usr/bin/omxplayer", "--live", "--blank", "rtsp://192.168.178.210:554/mpeg4"] )
	time.sleep(2)
