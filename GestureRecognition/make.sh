g++ -std=c++11 -c main.cpp -I/usr/local/include
g++ main.o -o main -I/usr/local/include -L/usr/local/lib -lgrt
./main
