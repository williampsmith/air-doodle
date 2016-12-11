import pdb

files = ['0', '8', '9']
for file in files:
    old_file = open('trainingData/training_data_' + file + '.txt', 'r')
    new_file = open('trainingData/preformattedData', 'a')

    for line in old_file:
        new_line = ''
        if line == '' or line[0] == '%':
            new_file.write(line)
        else:
            data = line.split(',')
            for datum in data:
                new_line = new_line + datum + ' '
        new_file.write(new_line)
        #pdb.set_trace()

    old_file.close()
    new_file.close()
