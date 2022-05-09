#include <iostream>
#include <string>
#include <vector>

#include "../include/format.h"


int main()
{
    
    std::string str = "%s better than %s";
    std::cout << format(str.c_str(), "C++", "Python") << std::endl;
}