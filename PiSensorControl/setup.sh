#!/bin/bash/
# INSTALLATION

# Install library files for bluetooth control
sudo apt-get install bluetooth libbluetooth-dev bluez-utils blueman

# Install library files for i2c control
sudo apt-get install libi2c-dev

# TESTING

# Test i2c connection on Pi
ls -l /dev/i2c*
#i2cdetect -y 0...n

# Setting up and testing bluetooth on Pi
sudo bluetoothctl
# -> power on
# -> agent on
# -> scan on
# -> connect XX:XX:XX:XX:XX:XX