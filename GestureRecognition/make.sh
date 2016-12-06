g++ -std=c++11 -c dtwPipeline.cpp -I/usr/local/include
g++ dtwPipeline.o -o dtwPipeline -I/usr/local/include -L/usr/local/lib -lgrt
./dtwPipeline
