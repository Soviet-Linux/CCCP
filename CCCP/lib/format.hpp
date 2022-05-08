
/* 
This fucking file is a mess if you try to code this project run away
*/
#include <stdio.h>
#include <string>
#include <stdarg.h>

int format(std::string s,std::string count, ...)
{
    char buffer [50];

    va_list list;
    va_start(list, count); 

    for(int j=0; j ; j++)
    {
        sprintf (buffer,s.c_str(),va_arg(list, int));
    }

    va_end(list);

    return count;
}