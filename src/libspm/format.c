#include <stdio.h>
#include <stdlib.h>


// class stuff
#include "../../include/libspm.h"

char* format( const char* strFmtString, ... )
{
    //declare char* strDest
    char* strDest;

    va_list args;
    //initialize va_list args
    va_start(args, strFmtString);
    //declare size_t size
    size_t size = vsnprintf(NULL, 0, strFmtString, args);
    //allocate memory for strDest
    strDest = malloc(size + 1);
    //initialize va_list args
    va_start(args, strFmtString);
    //initialize vsnprintf
    vsnprintf(strDest, size + 1, strFmtString, args);
    //return strDest
    return strDest;

}