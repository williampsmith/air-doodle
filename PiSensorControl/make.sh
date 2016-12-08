#!/bin/bash/

# COMPILATION => might need: -I/usr/local/include -Wall -Wno-long
g++ -std=c++11 -pthread AirDoodle.cpp -o AirDoodle -lwiringPi -lbluetooth -lgrt
