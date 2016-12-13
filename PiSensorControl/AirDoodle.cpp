/*
 * Created by Mitchell Oleson for EECS149 AirDoodle Project
 * pthread_mutex_lock(&threads)
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
void send(uint8_t* data, uint8_t len) {
	// Acquire blue lock
	pthread_mutex_lock(&blue);

	// Send [tNum gesture] to awating pi
	status = write(blue_sock, (char*) data, len);
	while (status < 0) {
		std::cout << "Error sending thread " << data[0] << " to server... ";
		delay(100);
		std::cout << "Trying again..." << std::endl;
		status = write(blue_sock, data, len);
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
void *analyze(void* args) {
	thread_arg_struct* inputs = (thread_arg_struct*) args;
	uint8_t data[3];

	//std::cout << "Size: " << inputs->matrix.getSize() << std::endl;
	//std::cout << "Rows: " << inputs->matrix.getNumRows() << std::endl;
	//std::cout << "Cols: " << inputs->matrix.getNumCols() << std::endl;

	//Setup a custom recognition pipeline
  	GRT::GestureRecognitionPipeline pipeline;
  	if (!pipeline.load("AirDoodle_DTW_Pipeline_Model.txt")) {
  		std::cout << "Failed to load the classifier model" << std::endl;
		std::cout << std::endl;
		decrementThreads();
  		return NULL;
  	}

	// Predict gesture using the classifier
	if (!pipeline.predict(inputs->matrix)) {
		std::cout << "Failed to perform prediction for thread " << inputs->threadNum << std::endl;
		std::cout << std::endl;
		decrementThreads();
		return NULL;
	}
	data[0] = inputs->threadNum;
	data[1] = pipeline.getPredictedClassLabel();
	data[2] = (uint8_t) (pipeline.getMaximumLikelihood()*100);
	//if (data[2] < 40) {
	//	data[1] = 0;
	//}
	std::cout << "Thread Num: " << unsigned(data[0]) << ", Gesture: " << unsigned(data[1]) << ", Likelihood: " << unsigned(data[2]) << std::endl;
	std::cout << std::endl;

	// Send threadNum and recognized gesture to bluetooth function
	send(data, 3);

	// End of thread run
	decrementThreads();
}

// Logging function
void logInput() {
	std::cout << "Entered logInput" << std::endl;
	//delay(500);

	GRT::VectorDouble input_vector(3);
	thread_arg_struct* inputs = new thread_arg_struct;
	inputs->threadNum = nThread;

	//std::cout << "Collecting Data..." << std::endl;

	// Read BNO055 data until second button press
	//std::vector<double> vo;
	std::vector<double> va;
	digitalWrite(PIN1_LED, HIGH);
	while (digitalRead(PIN0_BUTTON) != LOW) {
		//vo = bno055.getVector(bno055.VECTOR_EULER);
		va = bno055.getVector(bno055.VECTOR_LINEARACCEL);
		//input_vector[0] = (float) vo[0]; // REMOVE: THROUGHS OFF CLASSIFICATION
		//input_vector[1] = vo[1];
		//input_vector[2] = vo[2];
		//input_vector[0] = va[0];
		//input_vector[1] = va[1];
		//input_vector[2] = va[2];
		//inputs->matrix.push_back(input_vector);
		inputs->matrix.push_back(va);
		//std::cout << vo[1] << " " << vo[2] << " " << va[0] << " " << va[1] << " " << va[2] << std::endl;
		std::cout << va[0] << " " << va[1] << " " << va[2] << std::endl;
		//std::cout << input_vector[0] << " " << input_vector[1] << " " << input_vector[2] << " " << input_vector[3] << " " << input_vector[4] << std::endl;
		//std::cout << std::endl;
		delay(50);
	}
	digitalWrite(PIN1_LED, LOW);

	// Read MPU6050 data until second button press
	// int16_t ax, ay, az, gx, gy, gz;
	// while (collect) {
	// 	mpu6050.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
	// 	input_vector[0] = (float) ax;
	// 	input_vector[1] = (float) ay;
	// 	input_vector[2] = (float) az;
	// 	input_vector[3] = (float) gx;
	// 	input_vector[4] = (float) gy;
	// 	input_vector[5] = (float) gz;
	// 	input_matrix.push_back(input_vector);
	// }

	//std::cout << "Creating thread..." << std::endl;
	if (inputs->matrix.getNumRows() < 10) {
		std::cout << "Not enough data (only " << inputs->matrix.getNumRows() << " rows)" << std::endl;
		std::cout << std::endl;
		return;
	}

	// Create thread struct for split
	pthread_t pth;

	// Split off into new thread for analysis
	pthread_create(&pth, &thread_attr, &analyze, (void *) inputs);

	// Increment thread counts
	pthread_mutex_lock(&threads);
	aliveThreads = aliveThreads + 1;
	nThread = nThread + 1;
	pthread_mutex_unlock(&threads);

	std::cout << "Leaving logInput" << std::endl;
	std::cout << std::endl;
}


// ---------- MAIN FUNCTION -----------

int main(int argc, char **argv) {
	// Setup wiringPi
	if (wiringPiSetup() < 0) {
		std::cout << "Unable to setup wiringPi: " << strerror(errno) << std::endl;
		return EXIT_FAILURE;
  	}

  	// Setup buttons
  	pinMode(PIN1_LED, OUTPUT);
  	pinMode(PIN2_BUTTON, INPUT);

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
	while (wiringPiISR(PIN0_BUTTON, INT_EDGE_RISING, &irq_handler) < 0 ) {
    		std::cout << "Unable to setup ISR: " << strerror(errno) << " ... ";
		delay(500);
		std::cout << "Trying again..." << std::endl;
  	}

  	for (;;) {
  		if (digitalRead(PIN2_BUTTON) == 1) {
  			break;
  		} else {
  			delay(500);
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
