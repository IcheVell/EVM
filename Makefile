all: clean build

build: main.cpp	
	g++ -O1 main.cpp -o main

clean:
	rm -rf main