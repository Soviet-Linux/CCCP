#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>



//json lib 
#include "../lib/nlohmann/json.hpp"
using nlohmann::json;

#include "shared.h"



namespace soviet {

    /*
        This is an attempt to organize this project with fancy object-oriented stuff 
        I dont know how to do it yet but inshallah i will learn
        The result may be cool !
    */

    // idk if it is a good solution or not 
    extern std::string cwd;

    extern configs vars;

    // this is used by some function to know if its a package archive ar an spm file
    enum spmType {
    SPM_ARCHIVE,
    SPM_FILE
    };

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
	- The function used to install a package from a package archive file (.spm.tar.gz) : package::installArchive
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
            // make dependencies
            std::vector<std::string> makedependencies;
            // opt dependencies
            std::vector<std::string> optionaldependencies;

            // Package location (Post install)
            /*
            Guys , im thinking about a better way to do this
            We could find a system to transform this long list of file locations
            into a list of directory used by the package 
          sed -i 's/old-text/new-text/g'  you understand ?
            It's not very useful , but it may be better for performance

            NEW ME :
            I think its areally bad idea , 
            the current way of doing it is better
            for more advanced functionality
            also most packages mont need it 
            its a useless optimization for the moment
            
            If i realize that the current way of doing is too slow i'll try other systems
            but i think all distro do this 
            */
            std::vector<std::string> locations;

            // commands to prepare , build , test and install the package 
            // It can be a script 
            std::map <std::string, std::string> info;


             // optional field : url to dowload sources
            std::string url;



            //Where the package is stored, This is used just for installing package 
            std::string packagePath;
            // where the spm file in data is stored
            std::string dataSpmPath ;

            spmType FileType;
            
            // main functions that will be called from main.cpp
            void get();
            bool check();
            
            void install();
            void uninstall();

            void createBinary(const std::string& binPath);
            void createArchive (const std::string& archive_path , std::vector<std::string> add_files );

            int make (const std::string& package_dir);
            
        private :

            //utility functions that will be used for other things
            void get_locations();
            int check_dependencies ();
            void move_binaries();

            // Set of function that manipulate spm files
            nlohmann::json open_spm (const std::string& spm_path );
            void store_spm (const std::string &spm_path,const std::string& spm_out);
            int var_spm(const std::string& spm_path);


            int add_data ();
            int remove_data ();

            void getSources ();


                  
    };
    // I spend hours on this one , but it works !!
    char* format( const char* strFmtString, ... );
    int init_data ();
    std::string exec(const char* cmd);
    package parseFileName (const std::string& Path);

    std::string arch2spm (const std::string& arch_file,const std::string& arch_download);
    json arch2json(const std::string& PKGBUILD);

    int installArch(const std::string PKGBUILD);
    int installCompatible(const std::string& file_path,const std::string& OptDownload = "");
    int installAur(const std::string& pkg_name);

    void help () ;
    void listPkgs ();
    int readConfig(const std::string& configFilePath);
    void init();
    void sync();
    void clean();

    enum level {
        DBG1,
        DBG2,
        DBG3,
        INFO,
        WARNING,
        ERROR,
        FATAL,
        DOWNLOAD
    };

    int msg(level msgLevel,const std::string& msg,...);

    int downloadRepo(const std::string& url_path,const std::string& file_path);
    int downloadFile(const std::string& url,const std::string file_path);

    
}

