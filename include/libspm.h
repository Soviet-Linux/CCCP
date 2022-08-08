#pragma once


#include "shared.h"
#include "globals.h"
#include "utils.h"

enum packageType {
    ARCHIVE,
    BINARY,
    SPM_FILE
};
struct cmd
{
    // Commands
    char* prepare;
    char* make;
    char* test;
    char* install;
    char* special;
    char* download;
};
struct package
{
    // Basic infos
    char* name;
    enum packageType type;
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

int readConfig(char* configFilePath);



