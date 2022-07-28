

// class stuff
#include "../../include/libspm.hpp"

soviet::package soviet::parseFileName (const std::string& Path)
{
    soviet::package pkg;

    pkg.packagePath = Path;

    std::string packageFile = pkg.packagePath.substr(pkg.packagePath.find_last_of("/")+1,pkg.packagePath.length());
    msg(level::DBG2, "Package file is %s", packageFile.c_str());

    if (pkg.packagePath.length() < 5) 
    {
        msg(level::FATAL, "Package path is too short; maybe it's not a package? Terminating...");
    }
    std::string extension = packageFile.substr(packageFile.find_first_of("."),packageFile.length());
    pkg.name = packageFile.substr(0,packageFile.find_first_of("."));
    //initialize variables
    
    pkg.dataSpmPath = vars.SPM_DIR + pkg.name + ".spm";

    // debug extension
    msg(level::DBG2, "Extension is %s", extension.c_str());
    
    if (extension == ".src.spm.tar.gz")
    {
        pkg.type = "src";
    }
    else if (extension == ".bin.spm.tar.gz") 
    {
        pkg.type = "bin";
    }
    else if (extension == ".spm")
    {
        pkg.type = "src";
    }
    else 
    {
        msg(level::FATAL, "Package is not a valid package! Terminating...");
    }
  
    return pkg;
}