#!/bin/bash
cd /home/pi
while 1
do
  /home/pi/hello_font.bin >> /home/pi/hello_font.log
  sleep 2
done