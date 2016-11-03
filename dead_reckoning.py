import sys

SAMPLING_RATE = 100 # HZ

def dead_reckoning():
	file_path = sys.argv[1]
	print('filepath:', file_path)
	f = open(file_path, 'r')

	accel_matrix = []
	for line in f:
		line = line[:-1]
		accel_vector = line.split(',')
		accel_vector = list(map(float, accel_vector))
		accel_matrix.append(accel_vector)

	print('Number of samples: ', len(accel_matrix))

	
dead_reckoning()


