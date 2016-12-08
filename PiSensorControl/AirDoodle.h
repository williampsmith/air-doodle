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
#include <bluetooth/bluetooth.h> // Bluez dev lib written for Raspberry Pi
#include <bluetooth/hci.h> // Bluez dev lib written for Raspberry Pi
#include <bluetooth/hci_lib.h> // Bluez dev lib written for Raspberry Pi

// Sensor lib
#include "Adafruit_BNO055_Pi.h" // Adafruit BNO055 9-DOF sensor

// Interrupt lib
#include <wiringPi.h>

// Classification Lib
#include <GRT/GRT.h>


// ---------- GLOBAL DEFINITIONS -----------

#define BUTTON0_PIN	0 // Button for interrupts
#define BUTTON1_PIN 1 // Button to exit

#define SERVER_CHANNEL	1 // Channel for the bluetooth connection
#define SERVER_BADDR_CHAR "58:A8:39:00:55:1A" // Bluetooth MAC address for the edison

#define I2C_BAUD 115200 // Baudrate for i2c


// ---------- FUNCTION DEFINITIONS -----------

// Implicit First Stage Program States
//  -> Intialize (setup)
//	-> Wait (interrupt)
//	-> Log (createInput)
//	-> Split (multithread to parser for classification and sending while awaiting more input)

// Second Stage Thread Functions
void irq_handler();	// Handles interrupts and function calls
void analyze(void* args);	// Parse input acceleration matrix
void send(uint8_t tNum, uint8_t gesture);	// Send character and position to waiting edison
void logInput();	// Response for button interrupt to start logging data

// Bluetooth global variables
int blue_sock, status;
struct sockaddr_rc blue_conn = {0};

// BNO055 global variables
Adafruit_BNO055 bno055;

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
	GRT::MatrixFloat matrix;
} thread_arg_struct;
