CC = g++
CFLAGS = -Wall -Wextra -Werror
SRC = test.cpp first_lab.cpp

build: test.cpp first_lab.cpp
	$(CC) $(CFLAGS) $(SRC) -o final

clean:
	rm -rf final

all: clean build
	