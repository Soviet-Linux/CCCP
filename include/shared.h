#pragma once


#define PATTERN "{\"name\":\"\",\"type\":\"\",\"version\":\"\",\"license\":\"\",\"maintainer\":\"\",\"description\":\"\",\"dependencies\":[],\"makedependencies\":[],\"optionaldependencies\":[],\"url\":\"\",\"info\":{\"download\":\"\",\"prepare\":\"\",\"make\":\"\",\"test\":\"\",\"install\":\"\",\"special\":\"\"},\"locations\":[]}"
#define VERSION 0.401


// package info


// shared function to be called by external programs

// This prints the version , its bad 
// TODO: Rework this
float version();

//# Package manipulation 

// install packages
int install();
/*-->*/int installSpm();
     /*-->*/int installSpmFile();
     /*-->*/int installSpmArchive();
// Remove packages
int uninstall();
// Check packages
int check (char* dataSpmPath, char* locations[], int locationsCount);

//# Other stuff

// update the system
int update();
// clean the work dirs
int clean();



