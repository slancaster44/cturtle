 
CXX := clang
CXXFLAGS := -g -lm

TARGET_NAME := hash_test

SRC := hash.c
HDR := hash.h

CLEAN_LIST := $(TARGET_NAME) *.o *.gch *.out

default:
	@cp ../common/* .
	$(CXX) $(CXXFLAGS) $(HDR) $(SRC) main.c 
	@rm -rf *.o *.gch
	@mv a.out $(TARGET_NAME)

clean:
	rm -f $(CLEAN_LIST)

build_test:
	@cp ../common/* .
	$(CXX) $(CXXFLAGS) $(HDR) $(SRC) test.c 
	@rm -rf *.o *.gch
	@mv a.out $(TARGET_NAME)

test:
	make build_test
	@echo ""
	./$(TARGET_NAME) 
	@make -s clean

