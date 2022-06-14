start : main.cpp
	g++ -lcurl -std=c++11 main.cpp -o start

run : start
	./start
