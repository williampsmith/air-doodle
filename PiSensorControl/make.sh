#!/bin/bash/

# COMPILATION => might need: -I/usr/local/include -Wall -Wno-long
g++ -std=c++11 -pthread Adafruit_BNO055_Pi.cpp AirDoodle.cpp -o AirDoodle -lwiringPi -lbluetooth -lgrt
