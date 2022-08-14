#pragma once



#define LIBSPM_VERSION 0.403


// package info


// shared function to be called by external programs

// This prints the version , its bad 
// TODO: Rework this
float version();

//# Package manipulation 

// install packages
//main exec function
/*-->*/int installSpmFile(char* spm_path,int as_dep);
/*-->*/int installSpmBinary();
// Remove packages
int uninstall();
// Check packages
int check (char* dataSpmPath, char* locations[], int locationsCount);

//# Other stuff
enum TypeInt {
    SRC = 1,
    BIN = 2,
};
//get a package
int get(char* p_name,char* out_path);

// update the system
int update();
// clean the work dirs
int clean();



