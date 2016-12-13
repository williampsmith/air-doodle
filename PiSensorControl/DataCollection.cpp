// Generic Libs
#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string.h>
#include <errno.h>

// BNO055 Sesnor Libs
#include "Adafruit_BNO055_Pi.h"

// MPU6050 Sensor Libs
//#include "I2Cdev.h"
//#include "MPU6050.h"

// Button lib
#include <wiringPi.h>

#define BUTTON0_PIN 0

int main(int argc, char* argv[]) {
//	if (argc < 4) {
//		std::cout << "Not enough arguments! Only got " << argc-1 << " expected 3" << std::endl;
//		return EXIT_FAILURE;
//	}

	// Get data collection
	const char* let = argv[1];
	int numSamps = strtol(argv[2], NULL, 10);

	// Setup file
	std::ofstream ofs;
        ofs.rdbuf()->pubsetbuf(0, 0);
	ofs.open(argv[3], std::ofstream::out | std::ofstream::app);

	// Setup wiringPi
	if (wiringPiSetup() < 0) {
		std::cout << "Unable to setup wiringPi: " << strerror(errno) << std::endl;
		return EXIT_FAILURE;
  	}

  	// Setup button
	pinMode(BUTTON0_PIN, INPUT);

	// Setup Data collection on MPU6050 sensor
	Adafruit_BNO055 bno055;
 	bno055 = Adafruit_BNO055(-1, BNO055_ADDRESS, I2C_PI, false);
 	while (!bno055.begin()) {
		std::cout << "Oops, no sensor connected ... Check your wiring or ADDR!\n";
		delay(500);
		std::cout << "Trying Again...\n";
	}

	std::vector<double> vo;
	std::vector<double> va;
	int curr = 0;
	for (int num = 0; num < strlen(let); num++) {
		for (int curr = 0; curr < numSamps; curr++) {
			while (digitalRead(BUTTON0_PIN) != HIGH) {
				delay(100);
			}
			std::cout << "Prepare to write: " << let[num] << std::endl;
			ofs << "%" << let[num] << std::endl;
			delay(3000);
			while (digitalRead(BUTTON0_PIN) == HIGH) {
				vo = bno055.getVector(bno055.VECTOR_EULER);
				va = bno055.getVector(bno055.VECTOR_LINEARACCEL);
				std::cout << vo[0] << "\t" << vo[1] << "\t" << vo[2] << "\t" << va[0] << "\t" << va[1] << "\t" << va[2] << std::endl;
				ofs << vo[0] << " " << vo[1] << " " << vo[2] << " " << va[0] << " " << va[1] << " " << va[2] << std::endl;
				delay(50);
			}
			ofs << std::endl;
			delay(100);
		}
	}

	// // Setup Data collection on MPU6050 sensor
	// MPU6050 mpu6050;
 	// mpu6050 = MPU6050();
	// mpu6050.initialize();
 	// while (!mpu6050.testConnection()) {
	// 	std::cout << "Oops, no sensor connected ... Check your wiring or I2C ADDR!\n";
	// 	delay(500);
	// 	std::cout << "Trying Again...\n";
	// }
	// std::cout << mpu6050.getRate() << std::endl;

	// int16_t ax, ay, az, gx, gy, gz = 0;
	// int curr = 0;
	// for (int num = 0; num < strlen(let); num++) {
	// 	int curr = 0;
	// 	while (curr < numSamps) {
	// 		if (digitalRead(BUTTON0_PIN) == 1) {
	// 			std::cout << "Prepare to write: " << let[num] << std::endl;
	// 			ofs << "%" << let[num] << std::endl;
	// 			delay(3000);
	// 			while (digitalRead(BUTTON0_PIN) == 1) {
	// 				mpu6050.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
	// 				std::cout << ax << "\t" << ay << "\t" << az << "\t" << gx << "\t" << gy << "\t" << gz << std::endl;
	// 				ofs << ax << " " << ay << " " << az << " " << gx << " " << gy << " " << gz << std::endl;
	// 				delay(50);
	// 			}
	// 			ofs << std::endl;
	// 			curr = curr + 1;
	// 		}
	// 		delay(100);
	// 	}
	// }

	ofs.close();

	return EXIT_SUCCESS;
}
