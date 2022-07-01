#!/bin/bash

# C++ compiler
CXX=g++


CCCP_SRC_DIR=src/cccp

OBJ_DIR=obj
BIN_DIR=./bin

CFLAGS="-Wall -Wextra  -pedantic -O2 -g "
CXXFLAGS="$CFLAGS -std=c++17"

LIB_SRC_DIR=src/libspm
CCCP_SRC_DIR=src/cccp

LIBSPM_DIR=$BIN_DIR

# This is the path to a soviet chroot
SOVIET=soviet

function libspm()
{
    echo "Building lib..."
    LIB_SRCS=$(find $LIB_SRC_DIR -name *.cpp)
    echo $LIB_SRCS
    for LIB_FILE in $LIB_SRCS; do
        temp=$(echo $LIB_FILE | sed "s/\.cpp/.o/")
        LIB_OBJ_FILE="${temp/$LIB_SRC_DIR/$OBJ_DIR}"
        if [ $LIB_FILE -nt $LIB_OBJ_FILE ]; then
            echo "Compiling $LIB_FILE... to $LIB_OBJ_FILE"
            echo "$CXX $CXXFLAGS -c -o $CCCP_OBJ_FILE $LIB_FILE -fPIC "
            $CXX $CXXFLAGS -c -o $LIB_OBJ_FILE $LIB_FILE -fPIC -lcurl
        fi
        LIB_OBJS="$LIB_OBJS $LIB_OBJ_FILE"
    done
    echo "Linking lib..."

    $CXX -shared -o $BIN_DIR/libspm.so $LIB_OBJS -lcurl
    echo "Done."
}
function cccp()
{
    echo "Building cccp..."
    CCCP_SRCS=$(find $CCCP_SRC_DIR -name *.cpp)
    echo $CCCP_SRCS
    for CCCP_FILE in $CCCP_SRCS; do
        echo $CCCP_FILE
        temp=$(echo $CCCP_FILE | sed "s/\.cpp/.o/")
        CCCP_OBJ_FILE="${temp/$CCCP_SRC_DIR/$OBJ_DIR}"
        if [ $CCCP_FILE -nt $CCCP_OBJ_FILE ]; then
            echo "Compiling $CCCP_FILE... to $CCCP_OBJ_FILE"
            $CXX $CXXFLAGS -c -o $CCCP_OBJ_FILE $CCCP_FILE 
        fi
        CCCP_OBJS="$CCCP_OBJS $CCCP_OBJ_FILE"
    done
    echo "Linking cccp..."
    echo " $CXX $CXXFLAGS -o $BIN_DIR/cccp $CCCP_OBJS  -lspm "
    $CXX -L$LIBSPM_DIR $CXXFLAGS -o $BIN_DIR/cccp $CCCP_OBJS -lspm -lcurl
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
        cp $BIN_DIR/libspm.so $SOVIET/usr/lib/libspm.so
        cp $BIN_DIR/cccp $SOVIET/usr/bin/cccp
	echo "REPOS=http://our.sovietlinux.ml" >> $SOVIET/etc/cccp.conf
    elif [ $1 = "install" ]; then
        cp $BIN_DIR/libspm.so /usr/lib/libspm.so
        cp $BIN_DIR/cccp /usr/bin/cccp
	echo "REPOS=http://our.sovietlinux.ml" >> /etc/cccp.conf
    elif [ $1 = "uninstall" ]; then
        rm /usr/lib/libspm.so
        rm /usr/bin/cccp
    elif [ $1 = "test" ]; then
        $CXX $CXXFLAGS -o $BIN_DIR/test tests/test.cpp -lspm -lcurl
        ./$BIN_DIR/test
    else
        echo "Usage: ./make.sh [lib|cccp]"
        exit 1
    fi
fi





