// Generic Libs
#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>
#include <string.h>
#include <errno.h>
#include <fstream>
#include <stdint.h>

// Sensor Lib
#include "I2Cdev.h"
#include "MPU6050.h"

// Interrupt lib
#include <wiringPi.h>

#define BUTTON0_PIN 0

int main(int argc, char* argv[]) {
	const char* let = "089";
	int samp = 3;
//	int samp = strlen(let);
//	for (int i = 1; i < strlen(let) + 1; i++) {
//		std::cout << argv[i] << "\t";
//		strcat(let, argv[i]);
//	}


	// Setup file
	std::ofstream ofs;
	ofs.open("testData.txt", std::ofstream::out | std::ofstream::app);

	// Setup wiringPi
	if (wiringPiSetup() < 0) {
		std::cout << "Unable to setup wiringPi: " << strerror(errno) << "\n";
		return EXIT_FAILURE;
  	}

	// Setup sensor
	MPU6050 mpu6050;
  	mpu6050 = MPU6050();
	mpu6050.initialize();
  	while (!mpu6050.testConnection()) {
		std::cout << "Oops, no sensor connected ... Check your wiring or I2C ADDR!\n";
		delay(500);
		std::cout << "Trying Again...\n";
	}
	std::cout << mpu6050.getRate() << "\n";

	// Setup button
	pinMode(BUTTON0_PIN, INPUT);

	int16_t ax, ay, az, gx, gy, gz = 0;
	int curr = 0;
	for (int num = 0; num < samp; num++) {
		int samps = 0;
		while (samps < 2) {
			if (digitalRead(BUTTON0_PIN) == 1) {
				std::cout << "Prepare to write: " << let[num] << "\n";
				ofs << "%" << let[num] << "\n";
				delay(3000);
				while (digitalRead(BUTTON0_PIN) == 1) {
					mpu6050.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
					std::cout << ax << "\t" << ay << "\t" << az << "\t" << gx << "\t" << gy << "\t" << gz << "\n";
					ofs << ax << " " << ay << " " << az << " " << gx << " " << gy << " " << gz << "\n";
					delay(50);
				}
				ofs << "\n";
				samps = samps + 1;
			}
			delay(100);
		}
	}

	ofs.close();

	return EXIT_SUCCESS;
}
