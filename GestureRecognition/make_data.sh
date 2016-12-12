g++ -std=c++11 -c generateFormattedDataFile.cpp -I/usr/local/include
g++ generateFormattedDataFile.o -o generateFormattedDataFile -I/usr/local/include -L/usr/local/lib -lgrt
./generateFormattedDataFile
