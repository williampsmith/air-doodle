import sys
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

SAMPLING_RATE = 200 # HZ
T = 200.0/SAMPLING_RATE # assume sampling period is 1 for now to avoid multiplying by small decimal

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
    input_matrix = []
    accel_matrix = []
    num_samples = 0

    for line in f_read:
        num_samples += 1 #running count of number of samples
        line = line[:-1]
        input_vector = line.split(',')
        input_vector = list(map(float, input_vector))
        input_matrix.append(input_vector)

    input_matrix = np.array(input_matrix)
    print('Number of samples: ', num_samples)
    print('Input Matrix:')
    print(input_matrix)

    # ######### Condition accelerometer data by removing gravity vector from all mesaurements ############
    # for i in range(num_samples):
    #   x, y, z, roll, pitch = input_matrix[i]
    #   roll = roll * np.pi / 180.0
    #   pitch = pitch * np.pi / 180.0
    #   accel_matrix[i] = np.array([x, y - np.sin(roll), z - np.cos(roll)]) # checkme

    # compute nominal gravity vector by averaging first 10 samples 
    # (assumes user holds device still for a second or so in the beginning)
    g_nominal = [0,0,0]
    for i in range(3):
        g_nominal += input_matrix[i, :3]
    g_nominal /= 3.0

    # subtract nominal gravity from all samples
    for i in range(num_samples):
        accel_matrix.append(input_matrix[i, :3] - g_nominal)
    accel_matrix = np.array(accel_matrix)
    print('Acceleration Matrix:')
    print(accel_matrix)

    ######### Populate the velocity data ############
    velocity_matrix = np.empty((num_samples, 3))
    print('velocity matrix dimensions: ', velocity_matrix.shape)
    velocity_matrix[0] = np.array([0,0,0]) # initial velocity. Assume starting at rest

    for i in range(1, num_samples):
        velocity_matrix[i] = np.array(velocity_matrix[i - 1] + accel_matrix[i - 1] * T )

    print('Velocity Matrix:')
    print(velocity_matrix)

    ######### Populate the position data ############
    position_matrix = np.empty((num_samples, 3))
    print('position matrix dimensions: ', position_matrix.shape)
    position_matrix[0] = np.array([0,0,0]) # initial position. Assume starting at origin

    for i in range(1, num_samples):
        #velocity_matrix[i - 1] * T + 0.5 * accel_matrix[i - 1] * T**2
        position_matrix[i] = np.array(position_matrix[i - 1] + 0.5 * accel_matrix[i - 1] * T**2)

    print('Position Matrix:')
    print(position_matrix)
    np.savetxt(sys.argv[2], position_matrix, delimiter=",") # write text to a file

    ######### Plot 3D position data ############
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.scatter(position_matrix[::,0], position_matrix[::,1], position_matrix[::,2], depthshade=True)
    plt.show()

    ######### Plot 2D hand projected position data ############
    plt.plot((np.sin(45)*position_matrix[:, 0]+np.cos(45)*position_matrix[:, 1]), position_matrix[:, 2])
    plt.show()

    ######### Plot 2D projected position data ############
    pos2D = project(position_matrix)
    plt.plot(pos2D[:,0], pos2D[:,1])
    plt.show()

    # assume y position is invariant and return [x,z]
    plt.plot(position_matrix[:, 0], position_matrix[:, 2])
    plt.show()
    return position_matrix[:, [0, 2]]

#   return result # 2D projection
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


