#pragma once


#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>

//json lib 
#include "../lib/nlohmann/json.hpp"
using nlohmann::json;

namespace soviet {

    extern std::map<std::string,std::string> configMap;

    // The filesystem root
    extern std::string ROOT;
    // For real use it must be set to "/"

    // main dir where all the files and packages will be stored and where we will work
    extern std::string MAIN_DIR;
    // the dir where we will be building the packages and downoading the sources
    //  TODO: proper dir names , but i dont have time to do it
    extern std::string WORK_DIR;
    // the dir where the data is stored
    extern std::string DATA_DIR;
    // where spm files of installed packages are stored
    extern std::string SPM_DIR;
    //where we store tests and logs
    extern std::string LOG_DIR;
    // Dir where built binaries are stored after making or after uncompressing
    extern std::string BUILD_DIR;
    // Dir where the package sources are downloaded and built
    extern std::string MAKE_DIR;
    //The file where a lot of data are stored
    extern std::string INSTALLED_FILE;
    extern std::string ALL_FILE;
    // configuraton file
    extern std::string CONFIG_FILE;
    
    extern std::vector<std::string> REPOS;
    /*
        This is an attempt to organize this project with fancy object-oriented stuff 
        I dont know how to do it yet but inshallah i will learn
        The result may be cool !
    */
    // declaring here the DEBUG and TESTING variables
    // I know @stefan wanted to use weird macros but i dont know how to do it
    // so i will use a global variable
    // But if one day you see that this is not enough, you can use the macros
    // Good Night
    extern bool DEBUG;
    extern bool TESTING;

  /*
    How the package class and the soviet function are designed :

    The package class:
       The package class is ment to create package object , to manipulate packages.
       This class contains :
        The package informations : 
        - The package name : package::name
	- The package version : package::version
	- The package type : package::type (It can be either "src" or "bin")
	- A vector containing all the dependencies of the package : package::dependencies
	- A vector contaning all the location the the files installed by the packages : package::locations
	The commands used to build the package :
	- the command used to prepare the dire where the package will be built : package::prepare_info
	- The command used to compile the package from source : package::build_info
	- the command used to test the package : package::test_info
	- the command used to install the package to the temporary root dir (BUILD_DIR or $BUILD_ROOT) : package::install_info
	- the commands the need to be ran after the installation of the package : package::special_info
	The path of the package files :
	 - The path of the package archive (.spm.tar.gz) : package::packagePath
	 - The path of the file contaning all the informations about the package when its installed : package::data:SpmPath
	Some function that need to be called :
	- The function used to install a package from a package archive file (.spm.tar.gz) : package::installFile
	- The function used to download a package from the repo and install it : package::installFromRepo
	- The function used to remove a package from the system : package::uninstall
	- The function used to check if a package is installed and not corrupted : package::check
	- The function used to create a binary package archive from a source package archive : package::createBinary
  Some private functions : 
  - TODO: finish this documentation
	
  */
      class package 
      {
        public:
            package();

            // Package name , used to identify the package
            std::string name;
            // Package version , used to identify the package
            std::string version;
            // At first i used an enum but it caused some problems
            std::string type;
            // Package dependencies
            std::vector<std::string> dependencies;
            // Package location (Post install)
            /*
            Guys , im thinking about a better way to do this
            We could find a system to transform this long list of file locations
            into a list of directory used by the package 
            you understand ?
            It's not very useful , but it may be better for performance
            */
            std::vector<std::string> locations;

            // commands to prepare , build , test and install the package 
            // It can be a script 
            std::string prepare_info;
            std::string build_info;
            std::string test_info;
            std::string install_info;

            std::string special_info;

            //Where the package is stored, This is used just for installing package 
            std::string packagePath;
            // where the spm file in data is stored
            std::string dataSpmPath ;

            
            // main functions that will be called from main.cpp
            void get();
            void uninstall();
            bool check();
            void installFile();
            void installFromRepo();
            void createBinary(const std::string& binPath);
            
        private :

            //utility functions that will be used for other things
            void get_locations();
            int check_dependencies ();
            void move_binaries();
            int make ();

            // Set of function that manipulate spm files
            nlohmann::json open_spm (const std::string& spm_path );
            void store_spm (const std::string &spm_path,const std::string& spm_out);
            int var_spm(const std::string& spm_path);

            int add_data ();
            int remove_data ();


                  
    };
    // I spend hours on this one , but it works !!
    char* format( const char* strFmtString, ... );
    int init_data ();
    std::string exec(const char* cmd);

    void help () ;
    void listPkgs ();
    int readConfig(const std::string& configFilePath);
    void init();
    void sync();
    void clean();
    
}

