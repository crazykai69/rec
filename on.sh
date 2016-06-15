#!/bin/bash
cd /opt/rec

case "$(sudo /opt/vc/bin/tvservice -s | grep -c 0x120002)" in
1) echo "HDMI is off, turning on"
   sudo /opt/vc/bin/tvservice -p
   sleep 1
   ;;
*) echo "HDMI is already on"
   ;;
esac

touch /opt/rec/tvon

case "$(pidof hello_font.bin | wc -w)" in
0) /opt/rec/hello_font.bin > /dev/null 2>/dev/null &
   ;;
*) # all ok
   ;;
esac

case "$(pidof -x master.py | wc -w)" in
0) sudo /opt/rec/master.py > /dev/null 2>/dev/null &
   ;;
*) ;;
esac

