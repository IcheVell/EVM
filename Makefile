all: main.cpp
	g++ -o cam main.cpp `pkg-config --cflags --libs opencv4`