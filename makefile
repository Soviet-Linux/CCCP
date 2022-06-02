CC := g++
CFLAGS := -Wall -g -std=c++2a -pedantic 

TARGET := cccp
LIB_TARGET := libspm.so

TEST := test

SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = obj
TEST_DIR = tests

SOVIET = soviet


# $(patsubst %.cpp,%.o,$(SRCS)): substitute all ".cpp" file name strings to ".o" file name strings

# TODO: Fuuck I hate makefiles



libspm: SRC_DIR = ${SRC_DIR}/libspm
cccp: SRC_DIR = ${SRC_DIR}/cccp
libspm: $(LIB_TARGET)
cccp: $(TARGET)




$(LIB_TARGET): $(patsubst %.cpp,%.o,$(subst src,obj,$(SRCS)))# I still don't know what this means	
	@echo ""
	$(CC) ${CFLAGS} -shared -o ${BIN_DIR}/$@ $^
# I should try to figure out what this is doing
$(TARGET): $(patsubst %.cpp,%.o,$(subst src,obj,$(SRCS)))
	$(CC) ${CFLAGS} -o ${BIN_DIR}/$@ $^
# This is to compile all source files listed in SRCS to .o files in OBJ_DIR
# The $(subst) thing is a little hacky I think
$(patsubst %.cpp,%.o,$(subst src,obj,$(SRCS))): $(wildcard ${SRC_DIR}/*.cpp)
	$(CC) $(CFLAGS) -c $< -o $@
# This is to clean stuff but i dont think it work
# Dont try it may break stuff

install:
	cp -p $(BIN_DIR)/${TARGET} ${SOVIET}/bin/cccp
direct: $(SRCS)
	$(CC) $(CFLAGS)  -o ${BIN_DIR}/${TARGET} $^
clean:
	rm -rf $(TARGET) *.o
.PHONY: all clean