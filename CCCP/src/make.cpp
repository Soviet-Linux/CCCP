#include <fstream>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <vector>
#include <algorithm>

// Json parsing library
#include "../include/nlohmann/json.hpp"

//using json = nlohmann::json;
using nlohmann::json;

//including header file
#include "../include/make.h"

// This function is downloading and building the packages
void make_pkg (const std::string& PName, const std::string& build_info, const std::string& MAKE_DIR,const std::string& BUILD_DIR)
{

    const std::string& build_cmd = "BUILD_ROOT="+ BUILD_DIR +"\n( cd "+ MAKE_DIR + PName +"* && "+ build_info +" )";
    std::cout << build_cmd << std::endl;
    
    system(build_cmd.c_str());
    system(("rm -rf " + MAKE_DIR + PName + "*").c_str());
}
void download_pkg (const std::string& download_info, const std::string& MAKE_DIR)
{
    const std::string& download_cmd = "( cd "+ MAKE_DIR +" && "+download_info+" )";

    std::cout << download_cmd << "\n";
    system(download_cmd.c_str());
}
// This function is moving the binaries to the correct locations
void move_binaries(const std::string& BUILD_DIR ,const std::string& ROOT)
{
    //moving binaries to their install location on the system
    std::string move_cmd = "cp -rvl " + BUILD_DIR + "* " + ROOT + "\n ";
    std::cout << move_cmd << "\n";
    system(move_cmd.c_str());
    system(("rm -rf " + BUILD_DIR + "*").c_str());
}
// This function will check if all dependencies of a package are installed
int check_dependencies (const std::vector<std::string>& dependencies, const std::string& DATA_DIR) 
{
    //TODO : check if the dependencies are installed
    //
    //Print the dependencies
    std::cout << "Dependencies : ";
    for (int i = 0; i < dependencies.size(); i++)
    {
        std::cout << dependencies[i] << " ";
    }
    std::cout << std::endl;
    return 1;
}
// This function will open the spm file , parse the json and return the data
pkg_data open_spm (const std::string& PPath)
{
    std::ifstream file_spm((PPath).c_str(), std::ios::in);
    std::stringstream buffer;
    buffer << file_spm.rdbuf();
    //parsing json data
    auto pkg_info = json::parse(buffer.str());
    pkg_data data;
    data.name = pkg_info["name"];
    data.type = pkg_info["type"];
    data.version = pkg_info["version"];
    data.special_info = pkg_info["info"]["special"];
    for (int i = 0; i < pkg_info["dependencies"].size(); i++)
    {
        data.dependencies.push_back(pkg_info["dependencies"][i]);
    }
    if (data.type == "src")
    {
        data.download_info = pkg_info["info"]["download"];
        data.build_info = pkg_info["info"]["build"];
    }
    else if (data.type == "local")
    {
        data.build_info = pkg_info["info"]["build"];
    }
    return data;
}
// changing source spm file to bin spm file
void bin_spm (const std::string& in_path , const std::string& out_path)
{

    std::ifstream file_spm((in_path).c_str(), std::ios::in);
    std::stringstream buffer;
    buffer << file_spm.rdbuf();
    file_spm.close();
    //parsing json data
    auto pkg_info = json::parse(buffer.str());
    pkg_info["type"] = "bin";
    pkg_info["info"]["build"] = "";
    pkg_info["info"]["download"] = "";
    std::ofstream file_spm_bin(out_path, std::ios::out);
    file_spm_bin << pkg_info.dump(4);
    file_spm_bin.close();
    std::cout << "Bin spm file created" << std::endl;

    
    
}
//This fucntion is very important , it will store the install location data to the "DB"
void store_spm (const std::string& PPath,const std::string& BUILD_DIR,const std::string& out_path)
{
    std::cout << "Storing location in spm file" << std::endl;
    std::ifstream file_spm((PPath).c_str(), std::ios::in);
    std::stringstream buffer;
    buffer << file_spm.rdbuf();
    file_spm.close();
    //parsing json data
    auto pkg_info = json::parse(buffer.str());
    //Get package file location
    //this work is a little outside the scope of this function , but its ok
    // TODO: change this
    std::string location_cmd = "( cd " + BUILD_DIR + " && find . -type f | cut -c2- > temp.txt )";
    std::cout << location_cmd << std::endl;
    system(location_cmd.c_str());
    //also the temp.txt file is a little hacky i think
    //Add the package locations
    std::string line;
    std::ifstream data_file ((BUILD_DIR + "temp.txt").c_str());
    //adding the location the the location list
    if (data_file.is_open())
    {
        //reading the command output from a file
        while ( getline (data_file,line) )
        {
            std::cout << line << '\n';
            pkg_info["locations"].push_back(line);
        }
        data_file.close();
    }
    //removing temp file 
    // TODO: comment this better
    system(("rm "+ BUILD_DIR + "temp.txt").c_str());
    //Writing the data to a file 
    std::ofstream file_spm_out((out_path).c_str(), std::ios::out);
    file_spm_out << pkg_info.dump(4);
    file_spm_out.close();
}

