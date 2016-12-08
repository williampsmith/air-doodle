/*
 * Created by Mitchell Oleson for EECS149 AirDoodle Projectpthread_mutex_lock(&threads)
 */

#include "AirDoodle.h"


// ---------- INTERRUPT FUNCTION -----------

// Handler for the button interrupt
void irq_handler() {
	// Attempt to acquire newData lock (if fails we are already collecting so ignore)
	if (pthread_mutex_trylock(&newData) == 0) {
		// Begin logging new data
		logInput();
		// Release newData lock
		pthread_mutex_unlock(&newData);
	}
}


// ---------- HELPER FUNCTIONS -----------

// Transmit gesture classification via the bluetooth connection
void send(uint8_t tNum, uint8_t gesture) {
	// Acquire blue lock
	pthread_mutex_lock(&blue);

	// Send thread number for ordering
	status = write(blue_sock, (char *) &tNum, 1);
	while (status < 0) {
		std::cout << "Error sending " << tNum << " to server... ";
		delay(50);
		std::cout << "Trying again...\n";
		status = write(blue_sock, (char *) &tNum, 1);
	}

	// Send gesture label
	status = write(blue_sock, (char *) &gesture, 1);
	while (status < 0) {
		std::cout << "Error sending " << gesture << " to server... ";
		delay(50);
		std::cout << "Trying again...\n";
		status = write(blue_sock, (char *) &gesture, 1);
	}

	// Release blue lock
	pthread_mutex_unlock(&blue);
}

// Decrement alive threads
void decrementThreads() {
	pthread_mutex_lock(&threads);
	aliveThreads = aliveThreads - 1;
	pthread_mutex_unlock(&threads);
}

// Drop in function for new thread
void* analyze(void* args) {
	thread_arg_struct* inputs = (thread_arg_struct*) args;

	//Setup a custom recognition pipeline
  	GRT::GestureRecognitionPipeline pipeline;
  	if (!pipeline.load("DTW_Pipeline_Model.txt")) {
  		std::cout << "Failed to load the classifier model\n";
		decrementThreads();
  		return NULL;
  	}

	// Predict gesture using the classifier
	if (!pipeline.predict(inputs->matrix)) {
		std::cout << "Failed to perform prediction for thread " << inputs->threadNum << "\n";
		decrementThreads();
		return NULL;
	}
	uint8_t gesture = pipeline.getPredictedClassLabel();

	// Send threadNum and recognized gesture to bluetooth function
	send(inputs->threadNum, gesture);

	// End of thread run
	decrementThreads();
	return NULL;
}

// Logging function
void logInput() {
	GRT::MatrixFloat input_matrix;
	GRT::VectorFloat input_vector(6);
	std::vector<double> accel;
	std::vector<double> gyro;
	double move = 2;

	// Read new data until movemnet stops
	while (move > 1.1) {
		accel = bno055.getVector(bno055.VECTOR_LINEARACCEL);
		gyro = bno055.getVector(bno055.VECTOR_EULER);
		input_vector[0] = (float) gyro[0];
		input_vector[1] = (float) gyro[1];
		input_vector[2] = (float) gyro[2];
		input_vector[3] = (float) accel[0];
		input_vector[4] = (float) accel[1];
		input_vector[5] = (float) accel[2];
		input_matrix.push_back(input_vector);
		move = std::sqrt(accel[0]*accel[0] + accel[1]*accel[1] + accel[2]*accel[2]);
	}

	// Create structs for new thread
	pthread_t pth;
	thread_arg_struct inputs;
	inputs.threadNum = nThread;
	inputs.matrix = input_matrix;

	// Split off into new thread for analysis
	pthread_create(&pth, &thread_attr, &analyze, &inputs);

	// Increment thread counts
	pthread_mutex_lock(&threads);
	aliveThreads = aliveThreads + 1;
	nThread = nThread + 1;
	pthread_mutex_unlock(&threads);
}


// ---------- MAIN FUNCTION -----------

int main(int argc, char **argv) {
	// Setup wiringPi
	if (wiringPiSetup() < 0) {
		std::cout << "Unable to setup wiringPi: " << strerror(errno) << "\n";
	      	return EXIT_FAILURE;
  	}

  	// Setup buttons
  	pinMode(BUTTON1_PIN, INPUT);

  	// Setup sensor
  	bno055 = Adafruit_BNO055(55);
  	while (!bno055.begin()) {
		std::cout << "Oops, no BNO055 detected ... Check your wiring or I2C ADDR!\n";
		delay(500);
		std::cout << "Trying Again...\n";
	}
  	bno055.setExtCrystalUse(true);

	// Test code for bno055
	while (true) {
		std::vector<double> tmp;
		tmp = bno055.getVector(bno055.VECTOR_LINEARACCEL);
		std::cout << "Acceleration => x: " << tmp[0] << " y: " << tmp[1] << " z: " << tmp[2] << "\n";
		tmp = bno055.getVector(bno055.VECTOR_EULER);
		std::cout << "Orientation => x: " << tmp[0] << " y: " << tmp[1] << " z: " << tmp[2] << "\n";
		std::cout << "\n";
		delay(100);
	}

    	// Setup and connect via bluetooth to display unit
	blue_sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	blue_conn.rc_family = AF_BLUETOOTH;
	blue_conn.rc_channel = (uint8_t) SERVER_CHANNEL;
	str2ba(SERVER_BADDR_CHAR, &blue_conn.rc_bdaddr);
	status = connect(blue_sock, (struct sockaddr *) &blue_conn, sizeof(blue_conn));
	if (status < 0) {
		std::cout << "Error connecting to server\n";
		return EXIT_FAILURE;
	}

	// Setup threaded environment & mutexes
	if (pthread_mutexattr_init(&mutex_attr) != 0) {
		std::cout << "Error initializing mutex attr.\n";
		return EXIT_FAILURE;
	}
	if (pthread_mutex_init(&blue, &mutex_attr) != 0) {
		std::cout << "Error creating blue mutex.\n";
		return EXIT_FAILURE;
	}
	if (pthread_mutex_init(&newData, &mutex_attr) != 0) {
		std::cout << "Error creating newData mutex.\n";
		return EXIT_FAILURE;
	}
	if (pthread_mutex_init(&threads, &mutex_attr) != 0) {
		std::cout << "Error creating threads mutex.\n";
		return EXIT_FAILURE;
	}
	if (pthread_attr_init(&thread_attr) != 0) {
		std::cout << "Error in pthread_attr_init\n";
		return EXIT_FAILURE;
	}
	if (pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED) != 0) {
		std::cout << "Error in pthread_attr_setdetachstate\n";
		return EXIT_FAILURE;
	}

	// Setup interrupts
	if (wiringPiISR(BUTTON0_PIN, INT_EDGE_FALLING, &irq_handler) < 0 ) {
    	std::cout << "Unable to setup ISR: " << strerror(errno) << "\n";
      	return EXIT_FAILURE;
  	}

  	for (;;) {
  		if (digitalRead(BUTTON1_PIN) == 1) {
  			break;
  		} else {
  			delay(50);
  		}
  	}

  	// Block data collection (or stall till last sample is done being collected)
  	pthread_mutex_lock(&newData);

  	// Wait for all threads to finish and exit
  	while (aliveThreads > 0) {
		delay(50);
	}

	// Cleanup and close running processes
	close(blue_sock);

	// Exit
	return EXIT_SUCCESS;
}
