g++ -std=c++11 -c dtw.cpp -I/usr/local/include
g++ dtw.o -o dtw -I/usr/local/include -L/usr/local/lib -lgrt
./dtw
