/*
 * Created by Mitchell Oleson for EECS149 AirDoodle Projectpthread_mutex_lock(&threads)
 */

// Generic Libs
#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>
#include <string>

// Multithreading libs
#include <pthread.h>
//#include <semaphore.h>

// Bluetooth & socket libs
#include <sys/socket.h>
#include <bluetooth/bluetooth.h> // Bluetooth dev lib written for Raspberry Pi
#include <bluetooth/rfcomm.h>  // More dev libs written for Raspberry Pi

// BNO055 Sensor lib
#include "Adafruit_BNO055_Pi.h"

// MPU6050 Sensor lib
//#include "I2Cdev.h"
//#include "MPU6050.h"

// Interrupt/Button lib
#include <wiringPi.h>

// Classification lib
#include <GRT/GRT.h>


// ---------- GLOBAL DEFINITIONS -----------

#define BUTTON0_PIN	0 // Button for interrupts
#define BUTTON1_PIN 1 // Button to exit

#define SERVER_CHANNEL	1 // Channel for the bluetooth connection
#define SERVER_BADDR_CHAR "B8:27:EB:DE:90:2A" // Bluetooth MAC address for the edison


// ---------- FUNCTION DEFINITIONS -----------

// Implicit First Stage Program States
//  -> Intialize (setup)
//	-> Wait (interrupt)
//	-> Log (createInput)
//	-> Split (multithread to parser for classification and sending while awaiting more input)

// Second Stage Thread Functions
void irq_handler();	// Handles interrupts and function calls
void* analyze(void* args);	// Parse input acceleration matrix
void send(uint8_t tNum, uint8_t gesture);	// Send character and position to waiting edison
void logInput();	// Response for button interrupt to start logging data
void decrementThreads();	// Decrements number of threads currently running

// Bluetooth global variables
int blue_sock, status;
sockaddr_rc blue_conn = {0};

// Sensor global variables
Adafruit_BNO055 bno055;
//MPU6050 mpu6050;

// Threaded globals
pthread_mutexattr_t mutex_attr;
pthread_attr_t thread_attr;
pthread_mutex_t blue;
pthread_mutex_t newData;
pthread_mutex_t threads;
uint8_t aliveThreads = 0;
uint8_t nThread = 0;
typedef struct {
	uint8_t threadNum;
	GRT::MatrixDouble matrix;
} thread_arg_struct;
