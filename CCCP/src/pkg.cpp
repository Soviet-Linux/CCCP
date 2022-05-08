/*
Hi guys , I hope you'll have fn reading all of this 
If you can help me on improving it it would be great
PS : all of this is a little hacky , i know , but i'm not sure how to make it better
PS2: All of these includes are not needed but im to lazy to remove them
*/

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <regex>

#include "../include/spm.h"
#include "../include/pkg.h"
#include "../include/data.h"




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
void rm_pkg (const std::string& PName,const std::string& DATA_DIR,const std::string& DATA_FILE,bool DEBUG)
{
    //All the block of code below is for getting the data from the spm file of the package
    // creating a string variable to hold the package's spm file path
    std::string PPath = DATA_DIR + PName + ".spm";
    //small message , its uselless but i'll leave it there
    std::cout << "Uninstalling package" << std::endl;
    //Printing the package's spm file path , if DEBUG is true
    // Obviously its for debugging purposes
    if (DEBUG) std::cout << PPath << std::endl;
    //paring spm file using open_spm()
    pkg_data data = open_spm(PPath);

    //remove all the files in the data["locations"]
    for (int i = 0; i < data.locations.size(); i++)
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
            if (DEBUG) std::cout << "Removing " << data.locations[i] << std::endl;
            std::filesystem::remove(data.locations[i].c_str());
        }
        catch (std::exception& e)
        {
            std::string str_e = e.what();
            if (DEBUG) std::cout << "Exception: " << str_e << '\n';

            /*
            Here we are chacking if the exception is the one that we sould catch ("Directory not empty") or another exception
            The way we are doing this is not very good for performance but i was not sure how to do it better
            TODO: find a better way 
            We could use error codes or smth like that
            But i dont know how
            */

            //checking if the excetion string we should catch is inside the catched exception 
            // I know it isn't clear (Again , DM me on discord if you dont understand  )
            //Or maybe read the docs (at the time i write this the docs are not yet good enough to understand this)
            if ( str_e.find("Directory not empty") != std::string::npos)
            {
                if(DEBUG) std::cout << "its good , no worry.\n";
            }
            else
            {
                std::cout << "This is BAD, terminating : " << str_e << '\n';
                exit(1);
            }

        }
        
    }
    //removing the entries in packages.json
    rm_pkg_data(DATA_FILE,PName);

    //remove the spm file from DATA_DIR
    std::string rm_spm_cmd = "rm -rf " + DATA_DIR + PName + ".spm";
    system((rm_spm_cmd).c_str());
}
//checking if package is installed an untouched
bool check_pkg (const std::string& PPath,const std::string& DATA_DIR,bool DEBUG)
{
    // Pasing data from the spm file
    pkg_data data = open_spm(PPath);
    // check if the data["location "] is not empty
    //it could be empty because of a failed install
    if (data.locations.empty())
    {
        if (DEBUG) std::cout << "It may be some serious shit in the package , do something !!\n";
        return false;
    }
    //check all the files in the data["locations"]
    for (int i = 0; i < data.locations.size(); i++)
    {
        /*
            Checking if every file installed by the package is still there 
            This is a bad way to do  this but i dont have a better idea 
            Again , if you have an idea DM me on discord 
        */
         if (DEBUG) std::cout << "Checking " << data.locations[i] << std::endl;
         if (std::filesystem::exists(data.locations[i])) 
         {
             if (DEBUG) std::cout << data.locations[i] << " exists !" << std::endl;
         }
         else {
                std::cout << data.locations[i] << " does not exist !" << std::endl;
                return false;
         }
    }
    return true;
}

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
// This function is downloading and building the packages
void make_pkg (const pkg_data& pkg,const std::string& MAKE_DIR,const std::string& BUILD_DIR,const std::string& LOG_DIR,bool TESTING)
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
    std::string package_dir = MAKE_DIR + pkg.name + "*";

    /*
        So in this part we are foramtting and executing the commands to configure , compile , test and install the package.
        The test command is executed if TESTING is set to true
        The "BUILD_ROOT" part is pass the BUILD_DIR path (Where we will be installing the packages) to the command.
        It is ment to be used in the install or configure command : make prefix=$BUILD_ROOT install or ./configure --prefix=$BUILD_ROOT or smth like that.
    */
    // TODO: find someone intelligent to ameliorate this code

    //checking is the command are used and formatting and executing them
    if (!pkg.prepare_info.empty())
    {
        //formatting the prepare command
        std::string prepare_cmd = "cd " + package_dir + ";" + pkg.prepare_info;
        //Printing the command to the terminal
        std::cout << prepare_cmd << std::endl;
        //executing the command
        system(prepare_cmd.c_str());
    }
    if (!pkg.configure_info.empty())
    {
        //Formating the commands
        std::string configure_cmd = "BUILD_ROOT="+ BUILD_DIR +"; cd " + package_dir + ";" + pkg.configure_info;
        // printing the command to standard output ( this is for devloppement , i'll remove it for the release )
        std::cout << configure_cmd << std::endl;
        //executing the commands
        system(configure_cmd.c_str());
    }
    if (!pkg.make_info.empty())
    {
        //Formating the command
        std::string make_cmd = "BUILD_ROOT="+ BUILD_DIR +"; cd " + package_dir + ";" + pkg.make_info;
        // printing the command to standard output 
        std::cout << make_cmd << std::endl;
        //executing the command
        system(make_cmd.c_str());
        
    }

    // executing the package test suite if TESTING is set to true and storing the tests results in the LOG_DIR
    if (TESTING) system(("cd "+ package_dir + ";" + pkg.test_info + " > "+ LOG_DIR + pkg.name + ".test").c_str());

    //installing the package in the build directory

    //formatting the install command
    std::string install_cmd = "BUILD_ROOT="+ BUILD_DIR +"; cd "+ package_dir + ";" + pkg.install_info;

    //printing , for debugging purposes
    std::cout << install_cmd << std::endl; 

    //And finally , executing the install command
    system(install_cmd.c_str());

    // cleaning up the build directory
    system(("rm -rf " + MAKE_DIR + pkg.name + "*").c_str());
}

