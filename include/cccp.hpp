#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

//json lib 
#include "../lib/nlohmann/json.hpp"
using nlohmann::json;

namespace soviet {

    // The filesystem root
    std::string ROOT = "/";
    // For real use it must be set to "/"

    // main dir where all the files and packages will be stored and where we will work
    std::string MAIN_DIR = ROOT + "var/cccp/";
    // the dir where we will be building the packages and downoading the sources
    //  TODO: proper dir names , but i dont have time to do it
    std::string WORK_DIR = MAIN_DIR + "work/";
    // the dir where the package file are storeds
    std::string PKG_DIR = MAIN_DIR + "pkg/";
    // the dir where the data is stored
    std::string DATA_DIR = MAIN_DIR + "data/";
    //where we store tests and logs
    std::string LOG_DIR = MAIN_DIR + "log/";
    // where the sources are stored for local packages
    std::string SRC_DIR = MAIN_DIR + "src/";
    // where the binaries are stored
    std::string BIN_DIR = MAIN_DIR + "bin/";
    // Dir where built binaries are stored after making or after uncompressing
    std::string BUILD_DIR = WORK_DIR + "build/";
    // Dir where the package sources are downloaded and built
    std::string MAKE_DIR = WORK_DIR + "sources/";
    //dir where special scripts are stored
    std::string SPECIAL_DIR = MAIN_DIR + "special/";
    //The file where a lot of data are stored
    std::string DATA_FILE = DATA_DIR + "packages.json";
    /*
    Here is a more detailed look of the default directory structure 
    / --> ROOT
    └── var
        └── cccp --> MAIN_DIR
            ├── bin --> BIN_DIR
            ├── data --> DATA_DIR
            ├── log --> LOG_DIR
            ├── pkg --> PKG_DIR
            ├── special --> SPECIAL_DIR
            ├── src --> SRC_DIR
            └── work --> WORK_DIR
                ├── build --> BUILD_DIR
                └── sources --> MAKE_DIR

    */
    /*
        This is an attempt to organize this project with fancy object-oriented stuff 
        I dont know how to do it yet but inshallah i will learn
        The result may be cool !
    */
    enum types {src,bin};

    bool DEBUG;

    class package 
    {
        public:
            std::string name;
            std::string version;
            types type;

            std::vector<std::string> dependencies;
            std::vector<std::string> locations;

            // commands to prepare , build , test and install the package 
            // It can be a script 
            std::string prepare_info;
            std::string build_info;
            std::string test_info;
            std::string install_info;

            std::string special_info;

            // idk why this is public , but i will leave it here
            void make (bool DEBUG);
            bool check_pkg (const std::string& PPath,bool DEBUG);
            
        private :

            std::string MAIN_DIR = soviet::MAIN_DIR;

            //utility functions that will be used for other things
            std::vector<std::string> get_locations(const std::string &PATH);
            int check_dependencies (const std::vector<std::string>& dependencies, const std::string& DATA_DIR);
            void move_binaries( std::vector<std::string> files ,std::string ROOT);
            // This function will open the spm file , parse the json and return the data
            int open_spm (const std::string& PPath);
            void store_spm (const std::string& PPath,const std::string& out_path);
                  
    };
    // I spend hours on this one , but it works !!
    char* format( const char* strFmtString, ... );
    
}

