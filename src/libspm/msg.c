#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// class stuff
#include "../../include/libspm.h"
#include "../../include/utils.h"


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

int msg(enum level msgLevel, const char* message,...)
{
        //declare char* strDest
    char* strDest;

    va_list args;
    //initialize va_list args
    va_start(args, message);
    //declare size_t size
    size_t size = vsnprintf(NULL, 0, message, args);
    //allocate memory for strDest
    strDest = (char*) calloc(size +2 ,sizeof(char));
    //initialize va_list args
    va_start(args,message);
    //initialize vsnprintf
    vsnprintf(strDest, size+1, message, args);

    //initialize va_end
    va_end(args);


    switch (msgLevel)
    {
        // different debug levels
        case DBG1:
            if (DEBUG >=1 ) printf("%sDEBUG: %s%s%s%s\n",BOLDGREEN,GREEN,strDest,RESET,RESET);
            break;
        case DBG2:
            if (DEBUG >= 2) printf("%sDEBUG: %s%s%s%s\n",BOLDGREEN,RESET,BOLDBLUE,strDest,RESET);
            break;
        case DBG3:
            if (DEBUG >= 3) printf("%sDEBUG: %s%s%s%s\n", BOLDGREEN, RESET,GREEN,strDest,RESET);

            break;
        case INFO:
            printf("%sINFO: %s%s%s%s\n",BOLDBLUE,RESET,BLUE,strDest,RESET);
            break;
        case WARNING:
            printf("%sWARNING: %s%s%s%s\n",BOLDYELLOW,RESET,YELLOW,strDest,RESET);
            break;
        case ERROR:
            printf("%sERROR: %s%s%s%s\n",BOLDMAGENTA,RESET,MAGENTA,strDest,RESET);
            break;
        case FATAL:
            printf("%sFATAL: %s%s%s%s\n",BOLDBLUE,RESET,BLUE,strDest,RESET);
            free(strDest);
            quit(1);
        case DOWNLOAD:
            printf("%sDOWNLOAD: %s%s%s%s\r",BOLDBLUE,RESET,BLUE,strDest,RESET);
            fflush(stdin);
            break;
        default:
            printf("UNKNOWN: %s\n",strDest);
            break;
    }
    //free memory
    free(strDest);
    

    return 0;
}
