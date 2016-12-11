// Generic Libs
#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>

// Sensor Lib
#include "I2Cdev.h"
#include "MPU6050.h"

// Interrupt lib
#include <wiringPi.h>

#define BUTTON0_PIN 0

int main(int argc, char **argv) {
	char* let[argc-1];
	int samp = argc - 1;
	for (int i = 1; i < argc; i++) {
		let[i-1] = argv[i];
	}

	// Setup file
	std::ofstream ofs;
	ofs.open("testData.txt", std::ofstream::out | std::ofstream::app);

	// Setup wiringPi
	if (wiringPiSetup() < 0) {
		std::cout << "Unable to setup wiringPi: " << strerror(errno) << "\n";
		return EXIT_FAILURE;
  	}

  	// Setup buttons
  	pinMode(BUTTON0_PIN, INPUT);

	// Setup sensor
  	mpu6050 = MPU6050();
	mpu6050.initialize();
  	while (!mpu6050.testConnection()) {
		std::cout << "Oops, no sensor connected ... Check your wiring or I2C ADDR!\n";
		delay(500);
		std::cout << "Trying Again...\n";
	}

	// Setup button
	pinMode(BUTTON0_PIN, INPUT);

	int16_t ax, ay, az, gx, gy, gz = 0;
	int num = 0;
	while (num < samp*20) {
		if (digitalRead(BUTTON0_PIN) == 1) {
			curr = num % samp;
			std::cout << "Prepare to write: " << let[curr];
			ofs << "%" << let[curr] << "\n";
			delay(3000);
			while (digitalRead(BUTTON0_PIN) == 1) {
				mpu6050.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
				std::cout << ax << "\t" << ay << "\t" << az << "\t" << gx << "\t" << gy << "\t" << gz << "\n";
				ofs << ax << " " << ay << " " << az << " " << gx << " " << gy << " " << gz << "\n";
			}
			ofs << "\n";
			num = num + 1;
		}
	}

	ofs.close();

	return EXIT_SUCCESS;
}