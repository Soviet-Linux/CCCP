#include <string>
#include <iostream>
#include <fstream>


#include "../lib/nlohmann/json.hpp"

//using json = nlohmann::json;
using nlohmann::json;

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
    }
    else if (data.type == "local" || data.type == "src")
    {
        data.prepare_info = pkg_info["info"]["prepare"];
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

    std::string location_cmd = "( cd " + BUILD_DIR + " && find . -type f | cut -c2- > " + temp_file + " && find . -type d | cut -c2- | tac  | sed '/^$/d' >> " + temp_file + " )";
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