#!/bin/sh
PERIOD=5

while true
do
    echo "Starting!"
    result=`./tcptron 213.3.30.106 9999 montanalow ./build/zug_zug`
    echo $result
    echo "----"
    sleep 1
    echo "1"
    sleep 1
    echo "2"
    sleep 1
    echo "3"
    sleep 1
    echo "4"
    sleep 1
    echo "5"
    sleep 1
done
