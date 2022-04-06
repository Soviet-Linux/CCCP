#include <iostream>
#include <vector>
#include <fstream>


#include "../include/misc.h"
#include "../include/make.h"

std::vector<std::string> split(std::string str, std::string token){
    std::vector<std::string>result;
    while(str.size()){
        int index = str.find(token);
        if(index!=std::string::npos){
            result.push_back(str.substr(0,index));
            str = str.substr(index+token.size());
            if(str.size()==0)result.push_back(str);
        }else{
            result.push_back(str);
            str = "";
        }
    }
    return result;
}
std::vector<std::string> open_spm (std::string PPath,std::string PKG_DIR)
{
    std::streampos size;
    char * memblock;
    PPath = PKG_DIR + PPath;
    std::cout << PPath << "\n";
    std::ifstream file_spm(PPath.c_str(), std::ios::in);
    std::string line;
    std::vector<std::string> pkg_info;
    if (file_spm.is_open())
    {
        std::cout << "file opened " << "\n";
        while ( getline (file_spm,line) )
        {
        pkg_info.push_back(line);
        }
        file_spm.close();
        return pkg_info;
    }

    else {
        std::cout << "Unable to open file \n";
        return std::vector<std::string> () ;
    }

    

}

