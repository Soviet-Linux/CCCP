#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <stdarg.h>

// class stuff
#include "../../include/libspm.hpp"

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

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
        // different debug levels
        case level::DBG1:
            if (vars.DEBUG >= 1) std::cout << BOLDGREEN << " DEBUG: " << RESET << GREEN << FmtMsg << RESET << std::endl;
            break;
        case level::DBG2:
            if (vars.DEBUG >= 2) std::cout << BOLDGREEN << " DEBUG: " << RESET << GREEN << FmtMsg << RESET << std::endl;
            break;
        case level::DBG3:
            if (vars.DEBUG >= 3) std::cout << BOLDGREEN << " DEBUG: " << RESET << GREEN << FmtMsg << RESET << std::endl;
            break;
        

        case level::INFO:
            std::cout << BOLDBLUE << "INFO: " << RESET  << BLUE << FmtMsg << RESET << std::endl;
            break;
        case level::WARNING:
            std::cout << BOLDYELLOW << "WARNING: " << RESET  << YELLOW << FmtMsg << RESET << std::endl;
            break;
        case level::ERROR:
            std::cout << BOLDMAGENTA << "ERROR: " << RESET  << MAGENTA << FmtMsg << RESET << std::endl;
            break;
        case level::FATAL:
            std::cout << BOLDRED << "FATAL: " << RESET  << RED << FmtMsg <<  RESET << std::endl;
            exit(1);
        default:
            std::cout << "UNKNOWN: " << FmtMsg << std::endl;
            break;
    }
    return 0;
}
