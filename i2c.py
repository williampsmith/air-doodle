I2C_ADDR = 2

import mraa
from time import sleep

i2c = mraa.I2c(0)
i2c.address(I2C_ADDR)

# read 6 bytes from the arduino
for i in range(20):
	message = i2c.read(5)
	print(message)
	sleep(1) # delay for 1 second