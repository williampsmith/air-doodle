#include <stdio.h>
#include <unistd.h>
#include <mraa/i2c.h>

#define #I2C_ADDR 2

int main() {
	char message[6];
	int i;

	mraa_init();
	mraa_i2c_context i2c;

	i2c = mraa_i2c_init(0);
	mraa_i2c_address(i2c, I2C_ADDR);

	for (i = 0; i < 20; i++) {
		mraa_i2c_read(i2c, message, 6);
		printf("%.6s\n", message);
		sleep(1);
	}

	mraa_i2c_stop(i2c);
	return 0;
}