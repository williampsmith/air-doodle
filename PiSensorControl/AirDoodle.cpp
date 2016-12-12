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
		std::cout << "Trying again..." << std::endl;
		status = write(blue_sock, (char *) &tNum, 1);
	}

	delay(500);
	// Send gesture label
	status = write(blue_sock, (char *) &gesture, 1);
	while (status < 0) {
		std::cout << "Error sending " << gesture << " to server... ";
		delay(50);
		std::cout << "Trying again..." << std::endl;
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

	std::cout << "Size: " << inputs->matrix.getSize() << std::endl;
	std::cout << "Rows: " << inputs->matrix.getNumRows() << std::endl;
	std::cout << "Cols: " << inputs->matrix.getNumCols() << std::endl;


	//Setup a custom recognition pipeline
  	GRT::GestureRecognitionPipeline pipeline;
  	if (!pipeline.load("Pi_DTW_Pipeline_Model.txt")) {
  		std::cout << "Failed to load the classifier model" << std::endl;
		decrementThreads();
  		return NULL;
  	}

	// Predict gesture using the classifier
	if (!pipeline.predict(inputs->matrix)) {
		std::cout << "Failed to perform prediction for thread " << inputs->threadNum << inputs->threadNum << std::endl;
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
	std::cout << "Entered logInput" << std::endl;
	delay(500);

	GRT::MatrixDouble input_matrix;
	GRT::VectorDouble input_vector(5);
	double move = 2;

	// Read BNO055 data until movemnet stops
	std::vector<double> vo;
	std::vector<double> va;
	while (move > 0.2) {
		vo = bno055.getVector(bno055.VECTOR_EULER);
		va = bno055.getVector(bno055.VECTOR_LINEARACCEL);
		//input_vector[0] = (float) vo[0]; // REMOVE BAD FOR CLASS
		input_vector[0] = vo[1];
		input_vector[1] = vo[2];
		input_vector[2] = va[0];
		input_vector[3] = va[1];
		input_vector[4] = va[2];
		input_matrix.push_back(input_vector);
		move = std::sqrt(va[0]*va[0] + va[1]*va[1] + va[2]*va[2]);
		std::cout << input_vector[0] << " " << input_vector[1] << " " << input_vector[2] << " " << input_vector[3] << " " << input_vector[4] << std::endl;
		std::cout << vo[0] << " " << vo[1] << " " << vo[2] << " " << va[0] << " " << va[1] << " " << va[2] << std::endl;
		std::cout << std::endl;
		delay(50);
	}

	// Read MPU6050 data until movemnet stops
	// int16_t ax, ay, az, gx, gy, gz;
	// while (move > 1.1) {
	// 	mpu6050.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
	// 	input_vector[0] = (float) ax;
	// 	input_vector[1] = (float) ay;
	// 	input_vector[2] = (float) az;
	// 	input_vector[3] = (float) gx;
	// 	input_vector[4] = (float) gy;
	// 	input_vector[5] = (float) gz;
	// 	input_matrix.push_back(input_vector);
	// 	move = (double) std::sqrt(ax*ax + ay*ay + az*az);
	// }

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
		std::cout << "Unable to setup wiringPi: " << strerror(errno) << std::endl;
		return EXIT_FAILURE;
  	}

  	// Setup buttons
  	pinMode(BUTTON1_PIN, INPUT);

  	// Setup BNO055 sensor
 	bno055 = Adafruit_BNO055(-1, BNO055_ADDRESS, I2C_PI, false);
 	//bno055 = Adafruit_BNO055(); // Needs more work (uart not connecting)
 	while (!bno055.begin()) {
		std::cout << "Oops, no sensor connected ... Check your wiring or ADDR! ";
		delay(500);
		std::cout << "Trying Again..." << std::endl;
	}

	// Test code for BNO055 sensor
	// std::vector<double> vo;
	// std::vector<double> va;
	// while (true) {
	// 	vo = bno055.getVector(bno055.VECTOR_EULER);
	// 	va = bno055.getVector(bno055.VECTOR_LINEARACCEL);
	// 	std::cout << vo[0] << "\t" << vo[1] << "\t" << vo[2] << "\t" << va[0] << "\t" << va[1] << "\t" << va[2] << std::endl;
 	// 	fflush(stdout);
 	// 	delay(10);
	// }

  	// Setup MPU6050 sensor
 	//  mpu6050 = MPU6050();
	// mpu6050.initialize();
 	//  while (!mpu6050.testConnection()) {
	// 	std::cout << "Oops, no sensor connected ... Check your wiring or I2C ADDR! ";
	// 	delay(500);
	// 	std::cout << "Trying Again..." << std::endl;
	// }

	// Test code for MPU6050 sensor
	// int16_t ax, ay, az, gx, gy, gz = 0;
	// while (true) {
	// 	mpu6050.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
	// 	std::cout << ax << "\t" << ay << "\t" << az << "\t" << gx << "\t" << gy << "\t" << gz << std::endl;
 	//  fflush(stdout);
 	//  delay(100);
	// }

        // Setup and connect via bluetooth to display unit
	blue_sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	blue_conn.rc_family = AF_BLUETOOTH;
	blue_conn.rc_channel = (uint8_t) SERVER_CHANNEL;
	str2ba(SERVER_BADDR_CHAR, &blue_conn.rc_bdaddr);
	while (connect(blue_sock, (struct sockaddr *) &blue_conn, sizeof(blue_conn)) < 0) {
		std::cout << "Error connecting to server... ";
		delay(500);
		std::cout << "Trying again..." << std::endl;
	}

	// Setup threaded environment & mutexes
	if (pthread_mutexattr_init(&mutex_attr) != 0) {
		std::cout << "Error initializing mutex attr." << std::endl;
		return EXIT_FAILURE;
	}
	if (pthread_mutex_init(&blue, &mutex_attr) != 0) {
		std::cout << "Error creating blue mutex." << std::endl;
		return EXIT_FAILURE;
	}
	if (pthread_mutex_init(&newData, &mutex_attr) != 0) {
		std::cout << "Error creating newData mutex." << std::endl;
		return EXIT_FAILURE;
	}
	if (pthread_mutex_init(&threads, &mutex_attr) != 0) {
		std::cout << "Error creating threads mutex." << std::endl;
		return EXIT_FAILURE;
	}
	if (pthread_attr_init(&thread_attr) != 0) {
		std::cout << "Error in pthread_attr_init" << std::endl;
		return EXIT_FAILURE;
	}
	if (pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED) != 0) {
		std::cout << "Error in pthread_attr_setdetachstate" << std::endl;
		return EXIT_FAILURE;
	}

	// Setup interrupts
	if (wiringPiISR(BUTTON0_PIN, INT_EDGE_FALLING, &irq_handler) < 0 ) {
    	std::cout << "Unable to setup ISR: " << strerror(errno) << std::endl;
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

	// Lock bluetooth mutex for sending
	pthread_mutex_lock(&blue);

	// Send end command to matrix controller
	status = write(blue_sock, "", 1);
	while (status < 0) {
		std::cout << "Error sending end command to server... ";
		delay(50);
		std::cout << "Trying again..." << std::endl;
		status = write(blue_sock, "", 1);
	}

	// Close the bluetooth socket
	close(blue_sock);

	// Exit
	return EXIT_SUCCESS;
}
