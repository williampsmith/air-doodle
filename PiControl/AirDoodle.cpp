/*
 *
 */

#include "AirDoodle.h"


// ---------- INTERRUPT FUNCTION -----------

// Handler for the button interrupt
void irq_handler() {
	// Attempt to acquire newData lock
	if (pthread_mutex_trylock(&newData) == 0) {
		// Begin logging new data
		logInput();
		// Release newData lock
		pthread_mutex_unlock(&newData);
	}
}


// ---------- HELPER FUNCTIONS -----------

// Transmit gesture classification via the bluetooth connection
void send(int num, unsigned char* gesture) {
	// Acquire blue lock
	pthread_mutex_lock(&blue);

	// Send thread number for ordering
	status = write(pi_sock, (char *) &num);
	while (status < 0) {
		printf("Error sending threadNum to server\n");
		delay(50);
		printf("Trying again...\n");
		status = write(pi_sock, (char *) &num)
	}

	// Send byte of the char
	status = write(pi_sock, gesture);
	while (status < 0) {
		printf("Error sending gesture to server\n");
		delay(50);
		printf("Trying again...\n");
		status = write(pi_sock, num)
	}

	// Release blue lock
	pthread_mutex_unlock(&blue);
}

// Drop in function for new thread
void analyze(void* args) {
	unsigned char* gesture;
	struct thread_arg_struct *inputs = args;

	// Classify input acceletaion data matrix
	gesture = classify(inputs.matrix); // Replace with final function calls based on input data

	// Send threadNum and recognized gesture to bluetooth function
	send(inputs.threadNum, gesture);
}

// Logging function
void logInput() {
	float input_matrix[][];
	float next[3];
	int move = 10;

	// Read new data until movemnet stops
	while (move > 1.1) {
		next = i2c.read();
		matrix.append(next);
		move = norm(next);
	}

	// Create structs for new thread
	pthread_t pth;
	struct thread_arg_struct inputs;
	inputs.threadNum = nThread;
	inputs.matrix = input_matrix;

	// Split off into new thread for analysis
	pthread_create(&pth, &attr, analyze, (void *) &inputs);
	nThread += 1;
}


// ---------- MAIN FUNCTION -----------

int main(int argc, char **argv) {
	// Setup wiringPi
	if (wiringPiSetup() < 0) {
      printf("Unable to setup wiringPi: %s\n", strerror(errno));
      exit(1);
  	}

  	// Setup button
  	pinMode(BUTTON0_PIN, INPUT);

  	// Setup sensor
  	pi_bno055 = new Adafruit_BNO055(BNO055_ID, BNO055_ADDRESS);
  	pi_bno055.begin(OPERATION_MODE_ACCGYRO);

    // Setup and connect to Edison
    pi_sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	pi_conn.rc_family = AF_BLUETOOTH;
	pi_conn.rc_channel = (uint8_t) EDISON_CHANNEL;
	str2ba(EDISON_BADDR_CHAR, &pi_conn.rc_bdaddr);
	status = connect(s, (struct sockaddr *)&pi_conn, sizeof(pi_conn));
	if (status < 0) {
		printf("Error connecting to server\n");
		exit(1);
	}

	// Setup threaded environment & mutexes
	pthread_mutex_init(&blue)
	pthread_mutex_init(&newData)
	if (pthread_attr_init(&attr); != 0) {
		printf("Error in pthread_attr_init\n", );
	}
	if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0) {
		printf("Error in pthread_attr_setdetachstate\n", );
	}
	
	// Setup interrupts
	if (wiringPiISR(BUTTON0_PIN, INT_EDGE_FALLING, &irq_handler) < 0 ) {
    	printf("Unable to setup ISR: %s\n", strerror (errno));
      	exit(1);
  	}

  	while (true) {
  		;
  	}

	// Cleanup and close running processes
	close(pi_sock);

	// Exit
	return 0;
}