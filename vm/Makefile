 
CXX := gcc
CXXFLAGS := -g -O3 -lm

TARGET_NAME := tvm

SRC := vm.c 
HDR := instructions.h mem_mac.h vm.h

CLEAN_LIST := $(TARGET_NAME) *.o *.gch

default:
	$(CXX) $(CXXFLAGS) $(HDR) $(SRC) main.c -o $(TARGET_NAME)

clean:
	rm -f $(CLEAN_LIST)

build_test:
	$(CXX) $(CXXFLAGS) $(HDR) $(SRC) test.c  -o $(TARGET_NAME)

test:
	make build_test
	@echo ""
	./$(TARGET_NAME) 
	@make -s clean

