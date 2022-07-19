#pragma once

#include <string>
#include <vector>

struct configs
{
    int DEBUG;
    bool TESTING;

    bool OVERWRITE;
    bool QUIET;
    // The filesystem root
    std::string ROOT;
    // For real use it must be set to "/"

    // main dir where all the files and packages will be stored and where we will work
    std::string MAIN_DIR;
    // the dir where we will be building the packages and downoading the sources
    //  TODO: proper dir names , but i dont have time to do it
    std::string WORK_DIR;
    // the dir where the data is stored
    std::string DATA_DIR;
    // where spm files of installed packages are stored
    std::string SPM_DIR;
    //where we store tests and logs
    std::string LOG_DIR;
    // Dir where built binaries are stored after making or after uncompressing
    std::string BUILD_DIR;
    // Dir where the package sources are downloaded and built
    std::string MAKE_DIR;
    // temp dir
    std::string TMP_DIR;

    //The file where a lot of data are stored
    std::string INSTALLED_FILE;
    std::string ALL_FILE;
    // configuraton file
    std::string CONFIG_FILE;
    
    std::vector<std::string> REPOS;

};

/*
parameters are parameters
option is cast to an enum : enum actionList {INSTALL_LOCAL,INSTALL_FROM_REPO,CHECK,LIST,REMOVE,CREATE,GET};
*/
enum actionList {INSTALL_LOCAL,INSTALL_FROM_REPO,CHECK,LIST,REMOVE,CREATE,HELP,UPDATE,CLEAN,SYNC,PRINT};
// shared function to be called by external programs
int cccp(int actionInt , std::vector<std::string> parameters, configs spmConfig);
float version();