#!/bin/bash/

# COMPILATION => might need: -I/usr/local/include -Wall -Wno-long
g++ AirDoodle.cpp -o AirDoodle -lwiringPi -lbluetooth -lpthread -lgrt