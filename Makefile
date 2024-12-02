all: clean build

build: main.c	
	gcc main.c -o main

clean:
	rm -rf main