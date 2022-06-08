#!/bin/bash

CXX=g++


CCCP_SRC_DIR=src/cccp

OBJ_DIR=obj
BIN_DIR=./bin

CFLAGS="-Wall -Wextra  -pedantic -O2 -g"
CXXFLAGS="$CFLAGS -std=c++17"

LIB_SRC_DIR=src/libspm
CCCP_SRC_DIR=src/cccp

LIBSPM_DIR=$BIN_DIR

# This is the path to a soviet chroot
SOVIET=soviet

function libspm()
{
    echo "Building lib..."
    SRCS=$(find $LIB_SRC_DIR -name *.cpp)
    echo $SRCS
    for FILE in $SRCS; do
        temp=$(echo $FILE | sed "s/\.cpp/.o/")
        OBJ_FILE="${temp/$LIB_SRC_DIR/$OBJ_DIR}"
        if [ $FILE -nt $OBJ_FILE ]; then
            echo "Compiling $FILE... to $OBJ_FILE"
            $CXX $CXXFLAGS -c -o $OBJ_FILE $FILE -fPIC
        fi
        OBJS="$OBJS $OBJ_FILE"
    done
    echo "Linking lib..."

    $CXX -shared -o $BIN_DIR/libspm.so $OBJS
    echo "Done."
}
function cccp()
{
    echo "Building cccp..."
    SRCS=$(find $CCCP_SRC_DIR -name *.cpp)
    echo $SRCS
    for FILE in $SRCS; do
        echo $FILE
        temp=$(echo $FILE | sed "s/\.cpp/.o/")
        OBJ_FILE="${temp/$CCCP_SRC_DIR/$OBJ_DIR}"
        if [ $FILE -nt $OBJ_FILE ]; then
            echo "Compiling $FILE... to $OBJ_FILE"
            $CXX $CXXFLAGS -c -o $OBJ_FILE $FILE 
        fi
        OBJS="$OBJS $OBJ_FILE"
    done
    echo "Linking cccp..."
    echo " $CXX $CXXFLAGS -o $BIN_DIR/cccp $OBJS -fPIC -lspm"
    $CXX -L$LIBSPM_DIR $CXXFLAGS -o $BIN_DIR/cccp $OBJS -fPIC -lspm
    echo "Done."
}

# Checking if args are given
if [ -z "$1" ]; then
    echo "Usage: ./make.sh [lib|cccp]"
    exit 1
else
    if [ $1 = "lib" ]; then
        libspm
    elif [ $1 = "cccp" ]; then
        cccp
    elif [ $1 = "all" ]; then
        libspm
        cccp
    elif [ $1 = "clean" ]; then
        rm -rf $OBJ_DIR/*
        rm -rf $BIN_DIR/*
    elif [ $1 = "chroot" ]; then
        sudo cp $BIN_DIR/libspm.so $SOVIET/usr/lib/libspm.so
        sudo cp $BIN_DIR/cccp $SOVIET/usr/bin/cccp
    elif [ $1 = "install" ]; then
        sudo cp $BIN_DIR/libspm.so /usr/lib/libspm.so
        sudo cp $BIN_DIR/cccp /usr/bin/cccp
    elif [ $1 = "uninstall" ]; then
        sudo rm /usr/lib/libspm.so
        sudo rm /usr/bin/cccp
    elif [ $1 = "test" ]; then
        $CXX $CXXFLAGS -o $BIN_DIR/test tests/test.cpp -lspm
        ./$BIN_DIR/test
    else
        echo "Usage: ./make.sh [lib|cccp]"
        exit 1
    fi
fi





