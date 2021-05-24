all:
	g++ -std=c++11 usr/interpreter.cpp src/interpreter.cpp \-I headers/ -o bin/interpreter
