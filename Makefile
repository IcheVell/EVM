CC = g++
CFLAGS = -Wall -Wextra -Werror
SRC = test.cpp first_lab.cpp

build: test.cpp first_lab.cpp
	$(CC) $(CFLAGS) $(SRC) -o final

build_zero: test.cpp first_lab.cpp
	$(CC) $(CFLAGS) -O0 $(SRC) -o final_with_o_zero

build_one: test.cpp first_lab.cpp
	$(CC) $(CFLAGS) -O1 $(SRC) -o final_with_o_one

build_two: test.cpp first_lab.cpp
	$(CC) $(CFLAGS) -O2 $(SRC) -o final_with_o_two

build_three: test.cpp first_lab.cpp
	$(CC) $(CFLAGS) -O3 $(SRC) -o final_with_o_three		

build_s: test.cpp first_lab.cpp
	$(CC) $(CFLAGS) -Os $(SRC) -o final_with_o_s

build_fast: test.cpp first_lab.cpp
	$(CC) $(CFLAGS) -Ofast $(SRC) -o final_with_o_fast

build_g: test.cpp first_lab.cpp
	$(CC) $(CFLAGS) -Og $(SRC) -o final_with_o_g		

clean:
	rm -rf final*

lab: build build_zero build_one build_two build_three build_s build_fast build_g

all: clean build
	