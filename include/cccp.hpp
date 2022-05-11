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
    extern std::string ROOT;
    // For real use it must be set to "/"

    // main dir where all the files and packages will be stored and where we will work
    extern std::string MAIN_DIR;
    // the dir where we will be building the packages and downoading the sources
    //  TODO: proper dir names , but i dont have time to do it
    extern std::string WORK_DIR;
    // the dir where the package file are storeds
    extern std::string PKG_DIR;
    // the dir where the data is stored
    extern std::string DATA_DIR;
    extern std::string SPM_DIR;
    //where we store tests and logs
    extern std::string LOG_DIR;
    // Dir where built binaries are stored after making or after uncompressing
    extern std::string BUILD_DIR;
    // Dir where the package sources are downloaded and built
    extern std::string MAKE_DIR;
    //The file where a lot of data are stored
    extern std::string DATA_FILE;
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
    
    enum action {INSTALL,CHECK,REMOVE,CREATE,LIST};

    class package 
    {
        public:
            std::string name;
            std::string version;
            // At first i used an enum but it caused some problems
            std::string type;

            std::vector<std::string> dependencies;
            std::vector<std::string> locations;

            // commands to prepare , build , test and install the package 
            // It can be a script 
            std::string prepare_info;
            std::string build_info;
            std::string test_info;
            std::string install_info;

            std::string special_info;

            //Where the package is stored
            std::string packagePath;
            // where the spm file in data is stored
            std::string dataSpmPath = DATA_DIR + name + ".spm";


            // main functions that will be called from main.cpp
            void purge();
            bool check();
            void install();
            void createBinary(const std::string& binPath );

            // idk why this is public , but i will leave it here
            void make ();
            
            // declaring here the DEBUG and TESTING variables
            bool DEBUG = false;
            bool TESTING = false;
            
        private :

            //utility functions that will be used for other things
            void get_locations();
            int check_dependencies ();
            void move_binaries();

            // Set of function that manipulate spm files
            nlohmann::json open_spm (const std::string& spm_path);
            void store_spm (const std::string &spm_path,const std::string& out_path);
            int var_spm(const std::string& spm_path);

            int add_data ();
            int remove_data ();


                  
    };
    // I spend hours on this one , but it works !!
    char* format( const char* strFmtString, ... );
    int init_data (const std::string& data_path);
    std::vector<action> parse(int argc, char *argv[]);

    
}

