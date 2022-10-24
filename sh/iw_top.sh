#!/bin/sh
while [ 1 ]
do
iw dev wlan0 station dump |grep "signal avg"
sleep 1
done
