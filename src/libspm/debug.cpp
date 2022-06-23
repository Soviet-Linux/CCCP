#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <stdarg.h>

// class stuff
#include "../../include/libspm.hpp"



int soviet::msg(level msgLevel, const std::string& message,...)
{
        //declare char* strDest
    char* strDest;

    va_list args;
    //initialize va_list args
    va_start(args, message.c_str());
    //declare size_t size
    size_t size = vsnprintf(NULL, 0, message.c_str(), args);
    //allocate memory for strDest
    strDest = new char[size + 1];
    //initialize va_list args
    va_start(args, message.c_str());
    //initialize vsnprintf
    vsnprintf(strDest, size + 1, message.c_str(), args);
    std::string FmtMsg = strDest;

    switch (msgLevel)
    {
        case level::DBG:
            if (DEBUG) std::cout << "DEBUG: " << FmtMsg << std::endl;
            break;
        case level::INFO:
            std::cout << "INFO: " << FmtMsg << std::endl;
            break;
        case level::WARNING:
            std::cout << "WARNING: " << FmtMsg << std::endl;
            break;
        case level::ERROR:
            std::cout << "ERROR: " << FmtMsg << std::endl;
            break;
        case level::FATAL:
            std::cout << "FATAL: " << FmtMsg << std::endl;
            exit(1);
        default:
            std::cout << "UNKNOWN: " << FmtMsg << std::endl;
            break;
    }
    return 0;
}
