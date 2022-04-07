#include <iostream>
#include <vector>
#include <fstream>
#include <memory>
#include <string>
#include <stdexcept>

#include "../include/nlohmann/json.hpp"

using nlohmann::json;

#include "../include/misc.h"
#include "../include/make.h"

std::vector<std::string> split(std::string str, const std::string token) { // TODO: either rewrite this in a better way or use builtin functions
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

const std::vector<std::string> open_spm (const std::string& PPath, const std::string& PKG_DIR)
{
    std::streampos size;
    char * memblock;
    std::ifstream file_spm((PKG_DIR + PPath).c_str(), std::ios::in);
    std::stringstream buffer;
    buffer << file_spm.rdbuf();
    auto pkg_info = json::parse(buffer.str());
    std::cout << pkg_info.dump(4) << "\n";

}

template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )
{
    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf( new char[ size ] );
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}
