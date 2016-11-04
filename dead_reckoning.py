import sys
import numpy as np

SAMPLING_RATE = 200 # HZ
T = 1 # assume sampling period is 1 for now to avoid multiplying by small decimal

# IMPORTANT NOTE: THIS ALGORITHM WAS TESTED ON THE WIIMOTE USING A MODEL THAT 
# PRE-ADJUSTS FOR BIAS AND SENSITIVITY IN THE SENSOR. THEREFORE, NO CALIBRATION 
# NEED BE PERFORMED. IMPLEMENTATION FOR OTHER SENSORS MUST DO ADDITIONAL WORK TO CALIBRATE.
def dead_reckoning():
	f_read = open(sys.argv[1], 'r')
	#f_write = open(sys.argv[2], 'w')

	######### Populate the accel data ############
	#[x, y, z, pitch, roll]
	accel_matrix = []
	num_samples = 0

	for line in f_read:
		num_samples += 1 #running count of number of samples
		line = line[:-1]
		accel_vector = line.split(',')
		accel_vector = list(map(float, accel_vector))
		accel_matrix.append(accel_vector)

	accel_matrix = np.array(accel_matrix)
	print('Number of samples: ', num_samples)
	print('Acceleration Matrix:')
	print(accel_matrix)

	######### Populate the velocity data ############
	velocity_matrix = np.empty((num_samples, 3))
	print('velocity matrix dimensions: ', velocity_matrix.shape)
	velocity_matrix[0] = np.array([0,0,0]) # initial velocity. Assume starting at rest

	for i in range(1, num_samples):
		velocity_vector = velocity_matrix[i - 1] + accel_matrix[i - 1, :3] + np.abs(((accel_matrix[i, :3] - accel_matrix[i - 1, :3])) / 2.0) * T 
		velocity_matrix[i] = np.array(velocity_vector)

	print('Velocity Matrix:')
	print(velocity_matrix)

	######### Populate the position data ############
	position_matrix = np.empty((num_samples, 3))
	print('position matrix dimensions: ', position_matrix.shape)
	position_matrix[0] = np.array([0,0,0]) # initial position. Assume starting at origin

	for i in range(1, num_samples):
		position_vector = position_matrix[i - 1] + velocity_matrix[i - 1] + np.abs(((velocity_matrix[i] - velocity_matrix[i - 1])) / 2.0) * T 
		position_matrix[i] = np.array(position_vector)

	print('Position Matrix:')
	print(position_matrix)

	np.savetxt(sys.argv[2], position_matrix) # write text to a file

dead_reckoning()


