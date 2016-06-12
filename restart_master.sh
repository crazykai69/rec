#!/bin/bash
cd /opt/rec

case "$(pidof -x master.py | wc -w)" in
0) sudo /opt/rec/master.py > mpy.log 2>mpy.err &
   ;;
*) ;;
esac

