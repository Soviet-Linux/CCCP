#include <string>
#include <unistd.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

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
            /*
                To uninstall packages , we'll use the location data ( all the files that were installed by the program )
                If you want more detail go to store_spm() in spm.cpp
                So we have all list of all thoses file in a json array inside the package's spm file in DATA_DIR
                (each package have a spm file in the DATA_DIR , and each spm file contains a json array of all the files that were installed by the program)
                And we will cycle through all the files in the json array and remove them from the system.

                This is simple . It could be simpler but really i dont know how .
                So dont touch this , except if there are a critical bug or an important missing feature.
            */
            // this function is for uninstaling packages
            void purge(const std::string& PName,bool DEBUG)
            {
                // creating a string variable to hold the package's spm file path
                std::string PPath = DATA_DIR + PName + ".spm";
                //small message , its uselless but i'll leave it there
                std::cout << "Uninstalling package " << PName << std::endl;
                //paring spm file using open_spm()
                open_spm(PPath);

                //remove all the files in the data["locations"]
                for (int i = 0; i < locations.size(); i++)
                {
                    /*
                        We had a small issue here earlier with the folders ,
                        because some essential non empty system folder (like /usr ) where removed
                        I fixed it by using std::filesystem::remove() , because it can't remove non empty folders
                        but this created another problem because it would drop an exception and kill the program 
                        so i had to use a try catch block to catch the exception and continue the program
                        But the try catch block is not the best way to do it , but i had to do it because i was not sure
                        More on that later
                    */
                    try {
                        std::cout << "Removing " << locations[i] << std::endl;
                        remove(locations[i].c_str());
                    }
                    catch (std::exception& e)
                    {
                        std::string str_e = e.what();

                        /*
                        Here we are checking if the exception is the one that we sould catch ("Directory not empty") or another exception
                        The way we are doing this is not very good for performance but i was not sure how to do it better
                        TODO: find a better way 
                        We could use error codes or smth like that
                        But i dont know how
                        */

                        //checking if the excetion string we should catch is inside the catched exception 
                        // I know it isn't clear (Again , DM me on discord if you dont understand  )
                        //Or maybe read the docs (at the time i write this the docs are not yet good enough to understand this)
                        if (!(str_e.find("Directory not empty") != std::string::npos))
                        {
                            std::cout << "This is BAD, terminating : " << str_e << '\n';
                            exit(1);
                        }

                    }
                    
                }
                //removing the entries in packages.json
                remove_data(DATA_FILE,PName);

                //remove the spm file from DATA_DIR
                std::string rm_spm_cmd = "rm -rf " + DATA_DIR + PName + ".spm";
                system((rm_spm_cmd).c_str());
            }

            //checking if package is installed and untouched
            bool check_pkg (const std::string& PName,bool DEBUG)
            {
                std::string PPath = DATA_DIR + PName + ".spm";
                // Pasing data from the spm file
                open_spm(PPath);
                // check if the data["location "] is not empty
                //it could be empty because of a failed install
                if (locations.empty())
                {
                    if (DEBUG) std::cout << "It may be some serious shit in the package , do something !!\n";
                    return false;
                }
                //check all the files in the data["locations"]
                for (int i = 0; i < locations.size(); i++)
                {
                    /*
                        Checking if every file installed by the package is still there 
                        This is a bad way to do  this but i dont have a better idea 
                        Again , if you have an idea DM me on discord 
                    */
                    if (DEBUG) std::cout << "Checking " << locations[i] << std::endl;
                    if (access(locations[i].c_str(),F_OK))
                    {
                        if (DEBUG) std::cout << locations[i] << " exists !" << std::endl;
                    }
                    else {
                            std::cout << locations[i] << " does not exist !" << std::endl;
                            return false;
                    }
                }
                return true;
            }

            // idk why this is public , but i will leave it here
            /*
                All hte complexity in this function and really in this entire project if just because we need to track  files installed by a makefile
                For now we juste install in a separate directory (BUILD_DIR) and then move it to the correct location (The real filesystem)
                I tried many solutions to do this like installwatch , checkinstall , but none of them worked for me.
                If you have an idea PLEASE , I beg you tell me or write it directly here .
                IDEAS :
                - 
                -
                -
                I prefer simple solution that deosnt include to write a timestamp based install-log thing from scratch
                (I tried , but its not good enough)

            */
            void make (bool DEBUG)
            {
                /*
                    We have some problems here , because some complex packages require advanced options to be installed 
                    (like glibc that wants a separate build dir )
                    For nom i resolved that by adding more complexity to the build commands , but its not pretty.
                    So i dont know i we should implement these options directly here and drop a lot of performance juste for a small number of packages 
                    or if we leave like that with some ugly command in the .spm file
                    If you come across this you can vote :

                    change -> 0
                    keep like that -> 1

                    Also this code is very bad so if you dont understand it dm me on discord : PKD667#9879 .
                */
                //TODO: resolve this issue (the original TODO was "fix this shit" , but i think "resolve this issue is better")

                // this solution with the '*' is very bad i think
                std::string package_dir = MAKE_DIR + name + "*";

                /*
                    So in this part we are foramtting and executing the commands to configure , compile , test and install the package.
                    The test command is executed if TESTING is set to true
                    The "BUILD_ROOT" part is pass the BUILD_DIR path (Where we will be installing the packages) to the command.
                    It is ment to be used in the install or configure command : make prefix=$BUILD_ROOT install or ./configure --prefix=$BUILD_ROOT or smth like that.
                */
                // TODO: find someone intelligent to ameliorate this code

                //checking is the command are used and formatting and executing them
                if (!prepare_info.empty())
                {
                    //formatting the prepare command
                    std::string prepare_cmd = "cd " + package_dir + " && " + prepare_info;
                    //Printing the command to the terminal
                    std::cout << prepare_cmd << std::endl;
                    //executing the command
                    system(prepare_cmd.c_str());
                }
                if (!build_info.empty())
                {
                    //Formating the command
                    std::string make_cmd = "BUILD_ROOT="+ BUILD_DIR +"; ( cd " + package_dir + " && "  + build_info + " )";
                    // printing the command to standard output 
                    std::cout << make_cmd << std::endl;
                    //executing the command
                    system(make_cmd.c_str());
                    
                }

                // executing the package test suite if TESTING is set to true and storing the tests results in the LOG_DIR
                if ( !test_info.empty()) system(("( cd "+ package_dir + " && " + test_info + " > "+ LOG_DIR + name + ".test )").c_str());

                //installing the package in the build directory

                //formatting the install command
                std::string install_cmd = "BUILD_ROOT="+ BUILD_DIR +"; ( cd "+ package_dir + " && " + install_info + " )";

                //printing , for debugging purposes
                std::cout << install_cmd << std::endl; 

                //And finally , executing the install command
                system(install_cmd.c_str());

                // cleaning up the build directory
                system(("rm -rf " + package_dir).c_str());
            }
            
            
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
            std::vector<std::string> get_locations(const std::string &PATH)
            {
                //declaring output vector
                std::vector<std::string> locations;

                std::string temp_file = "/tmp/temp_loc.txt";
                //Get package file location
                std::string location_cmd = "( cd " + PATH + " && find . -type f | cut -c2- > " + temp_file + " && find . -type d | cut -c2- | tac  | sed '/^$/d' >> " + temp_file + " )";
                //std::cout << location_cmd << std::endl;
                system(location_cmd.c_str());
                //also the temp.txt file is a little hacky i think
                //Add the package locations
                std::string line;
                std::ifstream data_file ((temp_file).c_str());
                //adding the location the the location list
                if (data_file.is_open())
                {
                    //reading the command output from a file
                    while ( getline (data_file,line) )
                    {
                        locations.push_back(line);
                    }
                    data_file.close();
                }
                return locations;
            }
            // This function will check if all dependencies of a package are installed
            int check_dependencies (const std::vector<std::string>& dependencies, const std::string& DATA_DIR) 
            {
                //TODO : check if the dependencies are installed
                //
                //Print the dependencies
                if (dependencies[0] == "base")
                {
                    std::cout << "this packages requires the base system , no dependencies checking.\n";
                    return 1;
                }
                else {
                    for (int i = 0; i < dependencies.size(); i++)
                    {
                        // TODO: implement vesrion checking
                        // is use the C function because my clangd server doesnt recognize the fucking std::filesystem thing and it bothered me 
                        if (access((DATA_DIR + dependencies[i] + ".spm").c_str(),F_OK))
                        {
                            std::cout << "dependency " << dependencies[i] << " is installed.\n";
                        }
                        else {
                            std::cout << "dependency " << dependencies[i] << " is not installed.\n";
                            //install automatically the needed dependency
                            //To do this run install_package
                            //install_package(dependencies[i]);
                        }
                    }
                }
            
                return 1;
            }
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
            // This function will open the spm file , parse the json and return the data
            int open_spm (const std::string& PPath)
            {
                std::ifstream file_spm((PPath).c_str(), std::ios::in);
                std::stringstream buffer;
                buffer << file_spm.rdbuf();
                //parsing json data
                auto pkg_info = json::parse(buffer.str());

                name = pkg_info["name"];
                type = pkg_info["type"];
                version = pkg_info["version"];
                special_info = pkg_info["info"]["special"];
                for (int i = 0; i < pkg_info["dependencies"].size(); i++)
                {
                    dependencies.push_back(pkg_info["dependencies"][i]);
                }

                if (type == src)
                {
                    prepare_info = pkg_info["info"]["prepare"];
                    build_info = pkg_info["info"]["make"];
                    test_info = pkg_info["info"]["test"];
                    install_info = pkg_info["info"]["install"];
                    
                }
                else if (type == bin)
                {
                    for (int i = 0; i < pkg_info["locations"].size(); i++)
                    {
                    locations.push_back(pkg_info["locations"][i]);
                    }
                }
                return 1;
                
            }
            //This fucntion is very important , it will store the install location data to the "DB"
            void store_spm (const std::string& PPath,const std::string& out_path)
            {
                std::string temp_file = "/tmp/temp_loc.txt";
                std::cout << "Storing location in spm file" << std::endl;
                std::ifstream file_spm((PPath).c_str(), std::ios::in);
                std::stringstream buffer;
                buffer << file_spm.rdbuf();
                file_spm.close();
                //parsing json data
                auto pkg_info = json::parse(buffer.str());
                //change package type if its a binary
                if (type == bin)
                {
                    pkg_info["type"] = "bin";
                }
                //also the temp.txt file is a little hacky i think
                //Add the package locations
                std::string line;
                std::ifstream data_file ((temp_file).c_str());
                //adding the location the the location list
                if (data_file.is_open())
                {
                    //reading the command output from a file
                    while ( getline (data_file,line) )
                    {
                        pkg_info["locations"].push_back(line);
                    }
                    data_file.close();
                }
                //removing temp file 
                // TODO: comment this better
                system(("rm "+ temp_file).c_str());
                //Writing the data to a file 
                std::ofstream file_spm_out((out_path).c_str(), std::ios::out);
                file_spm_out << pkg_info.dump(4);
                file_spm_out.close();
            }
            int remove_data (const std::string& data_path, const std::string& pkg_name)
            {
                std::ifstream file_spm((data_path).c_str(), std::ios::in);
                std::stringstream buffer;
                buffer << file_spm.rdbuf();
                file_spm.close();
                //parsing json data
                auto pkg_info = json::parse(buffer.str());
                for (int i = 0; i < pkg_info["package_list"].size(); i++)
                {
                    std::string name = pkg_info["package_list"][i]["name"];
                    if (name == pkg_name)
                    {
                        pkg_info["package_list"].erase(pkg_info["package_list"].begin() + i);
                        pkg_info["packages"] = int(pkg_info["packages"]) - 1;
                        break;
                    }
                }
                std::ofstream file_data_new((data_path).c_str(), std::ios::out);
                file_data_new << pkg_info.dump(4);
                file_data_new.close();
                return 0;
            }
            int add_data (const std::string& data_path, const std::string& pkg_name, const std::string& pkg_version)
            {
                std::ifstream file_spm((data_path).c_str(), std::ios::in);
                std::stringstream buffer;
                buffer << file_spm.rdbuf();
                file_spm.close();
                //parsing json data
                auto pkg_info = json::parse(buffer.str());
                json o;
                o["name"] = pkg_name;
                o["version"] = pkg_version;
                pkg_info["package_list"].push_back(o);
                pkg_info["packages"] = int(pkg_info["packages"]) + 1;
                std::ofstream file_data_new((data_path).c_str(), std::ios::out);
                file_data_new << pkg_info.dump(4);
                file_data_new.close();
                return 0;
            }
            int init_data (const std::string& data_path)
            {
                auto basic_json = json::parse("{\"package_list\" :[],\"packages\" : 0}");
                std::cout << basic_json.dump(4) << std::endl;
                //write the json to data_path
                std::ofstream file_spm((data_path).c_str(), std::ios::out);
                file_spm << basic_json.dump(4);
                file_spm.close();

                return 0;
            }
                  
    };
    // I spend hours on this one , but it works !!
    char* format( const char* strFmtString, ... );
    
}
