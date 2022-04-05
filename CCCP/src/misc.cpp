#include <iostream>
#include <vector>

#include "misc.h"

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

