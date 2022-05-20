CC := g++
CFLAGS := -Wall -g -std=c++2a -pedantic

TARGET := main
TEST := test

SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = obj
TEST_DIR = tests

SOVIET = soviet

# $(wildcard *.cpp /xxx/xxx/*.cpp): get all .cpp files from the current directory and dir "/xxx/xxx/"
SRCS := $(wildcard ${SRC_DIR}/*.cpp)
# $(patsubst %.cpp,%.o,$(SRCS)): substitute all ".cpp" file name strings to ".o" file name strings
OBJS := $(patsubst %.cpp,%.o,$(subst src,obj,$(SRCS)))

all: $(TARGET)
# I should try to figure out what this is doing
$(TARGET): $(OBJS)
	$(CC) -o ${BIN_DIR}/$@ $(subst src,obj,$^)
# This is to compile all source files listed in SRCS to .o files in OBJ_DIR
# The $(subst) thing is a little hacky I think
${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	$(CC) $(CFLAGS) -c $< -o $(subst ${SRC_DIR},${OBJ_DIR},$@)
# This is to clean stuff but i dont think it work
# Dont try it may break stuff
clean:
	rm -rf $(TARGET) *.o
# I still don't know what this means	
.PHONY: all clean

direct: $(SRCS)
	$(CC) $(CFLAGS)  -o ${BIN_DIR}/${TARGET} $^

install:
	cp -p $(BIN_DIR)/${TARGET} ${SOVIET}/bin/cccp
test:
	$(CC) $(CFLAGS) ${TEST_DIR}/${TEST}.cpp -o ${BIN_DIR}/${TEST}
