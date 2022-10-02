#pragma once


#include "globals.h"

#define LIBSPM_VERSION 0.404

#define SOURCE "src"
#define BINARY "bin"

#define SPM 0xf
#define ECMP 0xe

#define DEFAULT_FORMAT SPM;

struct cmd
{
    // Commands
    char* make;
    char* test;
    char* prepare;
    char* install;
    char* special;
    char* download;
};
struct package
{
    // Basic infos
    char* name;
    char* type; // for the type at first i used an enum but im lazy and its stupid;
    char* version;
    char* license;
    char* url;

    char** dependencies;
    int dependenciesCount;

    char** makedependencies;
    int makedependenciesCount;

    char** optionaldependencies;
    int optionaldependenciesCount;

    char ** locations;
    int locationsCount;

    // cmds
    struct cmd info;

};

// package info


// shared function to be called by external programs

// This prints the version , its bad 
// TODO: Rework this
float version();

//# Package manipulation 

// install packages
//main exec function
/*-->*/int installSpmFile(char* spm_path,int as_dep);
/*-->*/int installSpmBinary(char* archivePath,int as_dep);
// Remove packages
int uninstall(char* name);
// Check packages
int check (char* dataSpmPath, char* locations[], int locationsCount);


//get a package
char* get(char* p_name,char* out_path);

// update the system
int update();
// clean the work dirs
int clean();

// init the system
void init();

int readConfig(char* configFilePath);


//open a pkg file (can be spm or ecmp)
int open_pkg(char* path, struct package* pkg);
int create_pkg(char* path,struct package* pkg);



