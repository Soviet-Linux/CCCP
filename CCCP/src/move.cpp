#include <stdio.h>
#include <vector>
#include <string>

// This function is moving the binaries to the correct locations
void move_binaries( std::vector<std::string> files ,std::string ROOT)
{
    //moving binaries to their install location on the system
    //loop through the files and move them to the correct location
    for (int i = 0; i < files.size(); i++)
    {
        // C is better (Why this fucking function is called rename and not move ?!)
        rename(files[i].c_str(), (ROOT + files[i]).c_str());
        
    }
}