CXX = arm-none-eabi-g++
CXXFLAGS = -march=armv7-a -mfpu=vfpv3-d16 -mfloat-abi=hard -specs=nosys.specs
LDFLAGS = -lc -lnosys

SRC = first_lab.cpp test.cpp
TARGET = lab4

all: clean lab4_with_O0 lab4_with_O1 lab4_with_Ofast

lab4_with_O0: $(SRC)
	$(CXX) -O0 $(CXXFLAGS) $(SRC) -o lab4_with_O0 $(LDFLAGS)

lab4_with_O1: $(SRC)
	$(CXX) -O1 $(CXXFLAGS) $(SRC) -o lab4_with_O1 $(LDFLAGS)

lab4_with_Ofast: $(SRC)
	$(CXX) -Ofast $(CXXFLAGS) $(SRC) -o lab4_with_Ofast $(LDFLAGS)

clean:
	rm -rf lab4*
