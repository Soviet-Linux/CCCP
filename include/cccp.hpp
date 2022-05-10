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
    std::string SPM_DIR = MAIN_DIR + "spm/";
    //where we store tests and logs
    std::string LOG_DIR = MAIN_DIR + "log/";
    // Dir where built binaries are stored after making or after uncompressing
    std::string BUILD_DIR = WORK_DIR + "build/";
    // Dir where the package sources are downloaded and built
    std::string MAKE_DIR = WORK_DIR + "make/";
    //The file where a lot of data are stored
    std::string DATA_FILE = DATA_DIR + "packages.json";
    /*
    Here is a more detailed look of the default directory structure 
    / --> ROOT
    └── var
        └── cccp --> MAIN_DIR
            ├── data --> DATA_DIR
            ├── spm --> SPM_DIR
            ├── log --> LOG_DIR
            └── work --> WORK_DIR
                ├── build --> BUILD_DIR (also called $BUILD_ROOT)
                └── make --> MAKE_DIR

    */

    /*
        This is an attempt to organize this project with fancy object-oriented stuff 
        I dont know how to do it yet but inshallah i will learn
        The result may be cool !
    */
    enum types {src,bin};

    extern bool DEBUG;

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

            // main functions that will be called from main.cpp
            void remove(const std::string& PName,bool DEBUG);
            bool check(const std::string& PName,bool DEBUG);

            // idk why this is public , but i will leave it here
            void make (bool DEBUG);
            
            
        private :

            // this is putting the directory structure in in the class 
            //Its not pretty , its probably bad but i i havent found other solution
            // also , its not accidental string copying , we need it in case we need to change the dir structure at runtime
            const std::string MAIN_DIR = soviet::MAIN_DIR;
            const std::string WORK_DIR = soviet::WORK_DIR;
            const std::string PKG_DIR = soviet::PKG_DIR;
            const std::string DATA_DIR = soviet::DATA_DIR;
            const std::string SPM_DIR = soviet::SPM_DIR;
            const std::string LOG_DIR = soviet::LOG_DIR;
            const std::string BUILD_DIR = soviet::BUILD_DIR;
            const std::string MAKE_DIR = soviet::MAKE_DIR;
            const std::string DATA_FILE = soviet::DATA_FILE;


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

