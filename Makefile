start : main.cpp
	g++ -lcurl -std=c++11 main.cpp

run : start
	./start
