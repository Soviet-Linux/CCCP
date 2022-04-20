#include <fstream>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <vector>
#include <algorithm>
#include <filesystem>

// Json parsing library
#include "../include/nlohmann/json.hpp"

//using json = nlohmann::json;
using nlohmann::json;

//including header file
#include "../include/make.h"

// This function is downloading and building the packages
void make_pkg (const pkg_data& pkg,const std::string& MAKE_DIR,const std::string& BUILD_DIR)
{
    // this solution with the '*' is very bad i think
    std::string package_dir = MAKE_DIR + pkg.name + "*";

    std::string prepare_cmd = "( cd "+ package_dir + " && " + pkg.prepare_info + " )";
    std::string make_cmd = "( cd " + package_dir + " && " + pkg.configure_info + " && " + pkg.make_info + " )";
    std::string test_cmd = "( cd "+ package_dir + " && " + pkg.test_info + " )";
    std::string install_cmd = "BUILD_ROOT="+ BUILD_DIR +"; ( cd "+ package_dir + " && " + pkg.install_info + " )";
    std::string special_cmd = "( cd "+ package_dir + " && " + pkg.special_info + " )";

    std::cout << make_cmd << std::endl;
    std::cout << test_cmd << std::endl;
    std::cout << install_cmd << std::endl;
    std::cout << special_cmd << std::endl;

    
    system(make_cmd.c_str());
    system(test_cmd.c_str());
    system(install_cmd.c_str());
    system(special_cmd.c_str());

    // cleaning up the build directory
    system(("rm -rf " + MAKE_DIR + pkg.name + "*").c_str());
}
// This function is moving the binaries to the correct locations
void move_binaries(const std::string& BUILD_DIR ,const std::string& ROOT)
{
    //moving binaries to their install location on the system
    std::string move_cmd = "cp -rfvl " + BUILD_DIR + "* " + ROOT + "\n ";
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
    for (int i = 0; i < pkg_info["locations"].size(); i++)
    {
        data.locations.push_back(pkg_info["locations"][i]);
    }
    if (data.type == "src")
    {
        data.download_info = pkg_info["info"]["download"];
        data.configure_info = pkg_info["info"]["configure"];
        data.make_info = pkg_info["info"]["make"];
        data.test_info = pkg_info["info"]["test"];
        data.install_info = pkg_info["info"]["install"];
        data.special_info = pkg_info["info"]["special"];
    }
    else if (data.type == "local")
    {
        data.configure_info = pkg_info["info"]["configure"];
        data.make_info = pkg_info["info"]["make"];
        data.install_info = pkg_info["info"]["install"];
        data.test_info = pkg_info["info"]["test"];
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
    std::string temp_file = "/tmp/temp_loc.txt";
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
    std::string location_cmd = "( cd " + BUILD_DIR + " && find . -type f | cut -c2- > " + temp_file + " )";
    std::cout << location_cmd << std::endl;
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
            std::cout << line << '\n';
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
// this function is for uninstaling packages
void rm_pkg (const std::string& PPath,const std::string& DATA_DIR)
{
    std::cout << "Uninstalling package" << std::endl;
    std::cout << PPath << std::endl;
    pkg_data data = open_spm(PPath);
    //remove all the files in the data["locations"]
    for (int i = 0; i < data.locations.size(); i++)
    {
        //Fuuuck it leaves the folders
        // TODO: find someone intelligent enough to fix this
        //
         std::string rm_cmd = "rm -rf " + data.locations[i];
         std::cout << rm_cmd << std::endl;
         system(rm_cmd.c_str());
    }
    //remove the spm file from DATA
    std::string rm_spm_cmd = "rm -rf " + DATA_DIR + data.name + ".spm";
    system((rm_spm_cmd).c_str());
}
bool check_pkg (const std::string& PPath,const std::string& DATA_DIR)
{
    std::cout << "checking package" << std::endl;
    std::cout << PPath << std::endl;
    pkg_data data = open_spm(PPath);
    //remove all the files in the data["locations"]
    for (int i = 0; i < data.locations.size(); i++)
    {
        //Fuuuck it leaves the folders
        // TODO: find someone intelligent enough to fix this
        //
         if (std::filesystem::exists(data.locations[i])) 
         {
             std::cout << data.locations[i] << " exists !" << std::endl;
         }
         else {
                std::cout << data.locations[i] << " does not exist !" << std::endl;
                return false;
         }
    }
    return true;
}
