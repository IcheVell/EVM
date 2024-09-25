CC = g++
CFLAGS = -Wall -Wextra -Werror
SRC = first_lab.cpp

all: clean build
	
build: build_withO0 build_withO1 build_withOfast

build_withO0: first_lab.cpp
	$(CC) $(CFLAGS) -O0 -S $(SRC) -o O0

build_withO1: first_lab.cpp
	$(CC) $(CFLGAS) -O1 -S $(SRC) -o O1

build_withOfast: first_lab.cpp
	$(CC) $(CFLAGS) -Ofast -S $(SRC) -o Ofast

clean:
	rm -rf O*