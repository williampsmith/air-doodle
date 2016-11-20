import sys
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

SAMPLING_RATE = 200 # HZ
T = 100 # assume sampling period is 1 for now to avoid multiplying by small decimal

# IMPORTANT NOTE: THIS ALGORITHM WAS TESTED ON THE WIIMOTE USING A MODEL THAT 
# PRE-ADJUSTS FOR BIAS AND SENSITIVITY IN THE SENSOR. THEREFORE, NO CALIBRATION 
# NEED BE PERFORMED. IMPLEMENTATION FOR OTHER SENSORS MUST DO ADDITIONAL WORK TO 
# CALIBRATE. DATA IS ALSO ALREADY LOW-PASS FILTERED. THIS WILL ALSO NEED TO BE 
# IMPLEMENTED IN THE MORE GENERAL ALGORITHM.
def dead_reckoning():
	if len(sys.argv) < 2:
		print('Error: Usage. Please provide data source as argument.')
		return 

	f_read = open(sys.argv[1], 'r')
	#f_write = open(sys.argv[2], 'w')

	######### Populate the accel data ############
	#[x, y, z, roll, pitch]
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

	# ######### Condition accelerometer data by removing gravity vector from all mesaurements ############
	# for i in range(num_samples):
	# 	x, y, z, roll, pitch = accel_matrix[i]
	# 	roll = roll * np.pi / 180.0
	# 	pitch = pitch * np.pi / 180.0
	# 	accel_matrix[i] = np.array([x, y - np.sin(roll), z - np.cos(roll), roll, pitch]) # checkme

	# compute nominal gravity vector by averaging first 10 samples 
	# (assumes user holds device still for a second or so in the beginning)
	g_nominal = [0,0,0]
	for i in range(10):
		g_nominal += accel_matrix[i, :3]
	g_nominal /= 10.0

	# subtract nominal gravity from all samples
	for i in range(num_samples):
		x, y, z, roll, pitch = accel_matrix[i]
		g_x, g_y, g_z = g_nominal
		accel_matrix[i] = np.array([x - g_x, y - g_y, z - g_z] + [roll, pitch])


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
	np.savetxt(sys.argv[2], position_matrix, delimiter=",") # write text to a file

	######### Plot 3D position data ############
	fig = plt.figure()
	ax = fig.add_subplot(111, projection='3d')
	ax.scatter(position_matrix[::,0], position_matrix[::,1], position_matrix[::,2], depthshade=True)
	plt.show()

	######### Plot 2D projected position data ############
	pos2D = project(position_matrix)
	plt.plot(pos2D[:,0], pos2D[:,1])
	plt.show()

	# assume y position is invariant and return [x,z]
	plt.plot(position_matrix[:, [0, 2]])
	plt.show()
	return position_matrix[:, [0,2]]


# performs SVD and PCA on matrix to project onto 2D writing surface
# test case: position_matrix = np.array([[1,2,0],[3,4,0],[5,9,0],[2,5,0]])
# def project(position_matrix):
# 	# subtract the mean from each dimension
# 	dimension_len, vector_len = np.size(position_matrix)
# 	for column in range(vector_len):
# 		v = position_matrix[:, column]
# 		mean = v.sum() / float(dimension_len)
# 		print(mean)
# 		v = v - mean
# 		position_matrix[:, column] = v

# 	U,S,V = np.linalg.svd(position_matrix)
# 	result = V[:, :2].T.dot(position_matrix.T)
# 	result = result.T

# 	return result # 2D projection
def project(position_matrix):
	# subtract the mean from each dimension
	dimension_len, vector_len = np.shape(position_matrix)
	for column in range(vector_len):
		v = position_matrix[:, column]
		mean = v.sum() / float(dimension_len)
		print(mean)
		v = v - mean
		position_matrix[:, column] = v

	U,S,V = np.linalg.svd(position_matrix)
	result = V[:, :2].T.dot(position_matrix.T)
	result = result.T

	return result # 2D projection

dead_reckoning()


