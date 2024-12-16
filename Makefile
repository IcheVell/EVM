all: clean build

build: main.cpp
	g++ main.cpp -o main


clean:
	rm -rf main