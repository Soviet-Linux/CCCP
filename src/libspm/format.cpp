#include <stdio.h>
#include <stdarg.h>
#include <iostream>

// class stuff
#include "../../include/cccp.hpp"

char* soviet::format( const char* strFmtString, ... )
{
    //declare char* strDest
    char* strDest;

    va_list args;
    //initialize va_list args
    va_start(args, strFmtString);
    //declare size_t size
    size_t size = vsnprintf(NULL, 0, strFmtString, args);
    //allocate memory for strDest
    strDest = new char[size + 1];
    //initialize va_list args
    va_start(args, strFmtString);
    //initialize vsnprintf
    vsnprintf(strDest, size + 1, strFmtString, args);
    //return strDest
    return strDest;

}