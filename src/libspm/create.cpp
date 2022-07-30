
#include <sys/stat.h>


//class stuff
#include "../../include/libspm.hpp"

void soviet::package::createArchive (const std::string& archive_path , std::vector<std::string> add_files )
{
    msg(INFO,"Creating package archive from %s",name.c_str());

    var_spm(packagePath);

    std::string tar_tmp_dir = vars.TMP_DIR +  "/tar.spm.tmp.d";
    if ( info.find("download") != info.end() ) {
        if (info["download"][0] != '\0')
        {
            std::string download_cmd = format("URL=%s && cd %s && %s",url.c_str(),tar_tmp_dir.c_str(),info["download"].c_str());
            system(download_cmd.c_str()); 
        }
    }
    else 
    {
        msg(WARNING,"[-] No download command found in spm file , the created package may be empty");

    }
    // creating the tmp directory
    mkdir(tar_tmp_dir.c_str(), 0777);
    std::filesystem::copy(packagePath.c_str(),format("%s/%s.spm",tar_tmp_dir.c_str(),name.c_str()));

    // copy all add_files to tar_tmp_dir
    for (auto &path : add_files)
    {
        std::string base_filename = path.substr(path.find_last_of("/\\") + 1);
        std::string final_path = format("%s/%s",tar_tmp_dir.c_str(),base_filename.c_str());
        msg(DBG3,"moving %s to %s",path.c_str(),final_path.c_str());
        std::filesystem::copy(path,final_path.c_str());

    }
    std::string tar_cmd = format("( cd %s && tar -czvf %s * )",tar_tmp_dir.c_str(),archive_path.c_str());

    msg(DBG3,"Creating tar command : %s",tar_cmd.c_str());

    system(tar_cmd.c_str());

    std::filesystem::remove_all(tar_tmp_dir);

}

// parsing data and installing package
void soviet::package::createBinary(const std::string& binPath)
{

    /* All these variables are bad.*/
    //location spm file in build dir
    std::string spm_make = soviet::format("%s/%s.spm",vars.MAKE_DIR.c_str(),name.c_str());

    if (FileType == SPM_ARCHIVE)
    {
        msg(INFO, "Uncompressing %s", packagePath.c_str());

        //comand to  uncompress the .src;spm.tar.gz archive to MAKE_DIR
        std::string cmd_uncompress = soviet::format("tar -xf %s -C %s ",packagePath.c_str(),vars.MAKE_DIR.c_str());
        // if debug is on , print the command
        msg(DBG3,"Uncompressing package with %s",cmd_uncompress.c_str());
        //uncompressing <PName>.src.spm.tar.gz in PKG_DIR
        system(cmd_uncompress.c_str());

        //debug
        msg(DBG3, "%s/%s.spm",vars.MAKE_DIR.c_str(),name.c_str());
        // Reading spm file in MAKE DIR
        var_spm(spm_make);
    }
    else 
    {
        msg(DBG2, "Reading spm file in %s", packagePath.c_str());
        getSources();
        std::filesystem::copy(packagePath.c_str(),spm_make.c_str());
    }
    //making package
    make(format("%s/%s-%s",vars.MAKE_DIR.c_str(),name.c_str(),version.c_str()));
    // fancy output
    std::cout << "☭ Package built! ☭"<< "\n";
    //changing type to bin 
    type = "bin";
    //Get package locations
    get_locations();
    // Storing package data
    // Adding the locations to the package files , and the packages files to BUILD_DIR
    store_spm(spm_make,soviet::format("%s/%s.spm",vars.BUILD_DIR.c_str(),name.c_str()));
    
    // Creating the tar.gz package archive
    std::string cmd_archive = soviet::format("( cd %s && tar -czf %s * )", vars.BUILD_DIR.c_str() , binPath.c_str());

    msg(DBG3, "Archiving package with %s", cmd_archive.c_str());

    system(cmd_archive.c_str());
    // cleaning build directory
    std::filesystem::remove_all(vars.BUILD_DIR);
    mkdir(vars.BUILD_DIR.c_str(), 0777);
}