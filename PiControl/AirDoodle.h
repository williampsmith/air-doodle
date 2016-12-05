// Generic Libs
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Multithreading libs
#include <pthread.h>
#include <semaphore.h>

// Bluetooth & socket libs
#include <sys/socket.h>
#include <bluetooth/bluetooth.h> // Bluez dev lib written for Raspberry Pi
#include <bluetooth/hci.h> // Bluez dev lib written for Raspberry Pi
#include <bluetooth/hci_lib.h> // Bluez dev lib written for Raspberry Pi

// Sensor lib
#include <Adafruit_BNO055.h> // Adafruit BNO055 9-DOF sensor

// Interrupt lib
#include <wiringPi.h>


// ---------- GLOBAL DEFINITIONS -----------

#define BUTTON0_PIN	0 // Button for Interrupts

#define EDISON_CHANNEL	1 // Channel for the bluetooth connection
#define EDISON_BADDR_CHAR "58:A8:39:00:55:1A" // Bluetooth MAC address for the edison

#define I2C_BAUD 38400 // Baudrate for i2c


// ---------- MAIN FUNCTION -----------

// Implicit First Stage Program States
//  -> Intialize (setup)
//	-> Wait (interrupt)
//	-> Log (createInput)
//	-> Split (multithread to parser for classification and sending while awaiting more input)

// Second Stage Thread Functions
void irq_handler();	// Handles interrupts and function calls
void analyze(void* args);	// Parse input acceleration matrix
void send(int num, unsigned char* gesture);		// Send character and position to waiting edison
void logInput();	// Response for button interrupt to start logging data

// Bluetooth global variables
int pi_sock, status;
struct sockaddr_rc pi_conn = {0};

// BNO055 global variables
Adafruit_BNO055 pi_bno055;

// Threaded globals
mutex_t blue;
mutex_t newData;
pthread_attr_t attr;
int nThread = 0;
typedef struct {
	int threadNum;
	float matrix[][];
} thread_arg_struct;