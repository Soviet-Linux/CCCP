#include <iostream>


#define RELEASE 0.25


extern "C" float version();

// Main function
int main(void)
{

    std::cout << "[+] CCCP C++ Frontend v" << RELEASE << std::endl;
    std::cout << "[+] Libspm C++ Library v" << version() << std::endl;
    std::cout << "Copyright (C) 2022  Paul Kronlund-Drouault" << std::endl;
}
