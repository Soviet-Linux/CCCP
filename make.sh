#!/bin/sh

CXX=g++


CCCP_SRC_DIR=src/cccp

OBJ_DIR=obj
BIN_DIR=./bin

CFLAGS="-Wall -Wextra  -pedantic -O2 -g"
CXXFLAGS="$CFLAGS -std=c++17"

LIB_SRC_DIR=src/libspm
CCCP_SRC_DIR=src/cccp

LIBSPM_DIR=bin
if [ -z "$1" ]; then
    echo "Usage: ./make.sh [lib|cccp]"
else
    if [ $1 = "lib" ]; then
        echo "Building lib..."
        SRCS=$(find $LIB_SRC_DIR -name *.cpp)
        echo $SRCS
        for FILE in $SRCS; do
            temp=$(echo $FILE | sed "s/\.cpp/.o/")
            OBJ_FILE="${temp/$LIB_SRC_DIR/$OBJ_DIR}"
            echo "Compiling $FILE... to $OBJ_FILE"
            $CXX $CXXFLAGS -c -o $OBJ_FILE $FILE -fPIC
            OBJS="$OBJS $OBJ_FILE"
        done
        echo "Linking lib..."

        $CXX -shared -o $BIN_DIR/libspm.so $OBJS
        echo "Done."
    elif [ $1 = "cccp" ]; then
        echo "Building cccp..."
        SRCS=$(find $CCCP_SRC_DIR -name *.cpp)
        echo $SRCS
        for FILE in $SRCS; do
            echo $FILE
            temp=$(echo $FILE | sed "s/\.cpp/.o/")
            OBJ_FILE="${temp/$CCCP_SRC_DIR/$OBJ_DIR}"
            echo "Compiling $FILE... to $OBJ_FILE"
            $CXX  $CXXFLAGS -c -o $OBJ_FILE $FILE 
            OBJS="$OBJS $OBJ_FILE"
        done
        echo "Linking cccp..."
        echo " $CXX $CXXFLAGS -o $BIN_DIR/cccp $OBJS -fPIC -lspm"
        $CXX -L$LIBSPM_DIR $CXXFLAGS -o $BIN_DIR/cccp $OBJS -fPIC -lspm
        echo "Done."
    else
    echo "Usage: ./make.sh [lib|cccp]"
    fi
fi