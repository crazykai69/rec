## Startup
In /etc/rc.local:
```
# sudo -u pi -i /home/pi/temp1.sh &
sudo -u pi -i /opt/rec/run_hello_font_from_rc.local.sh &
```
In crontab:
```
0 4 * * * sudo shutdown -r now
10 4 * * * sudo /opt/vc/bin/tvservice -o
0 1 * * * sudo /opt/vc/bin/tvservice -o
0 6 * * * sudo /opt/vc/bin/tvservice -p
```
