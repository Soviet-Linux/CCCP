#include "stdbool.h"

extern int DEBUG;
extern bool TESTING;

extern bool OVERWRITE;
extern bool QUIET;
// The filesystem root
extern char* ROOT;
// For real use it must be set to "/"

// main dir where all the files and packages will be stored and where we will work
extern char* MAIN_DIR;
// the dir where we will be building the packages and downoading the sources
//  TODO: proper dir names , but i dont have time to do it
extern char* WORK_DIR;
// the dir where the data is stored
extern char* DATA_DIR;
// where spm files of installed packages are stored
extern char* SPM_DIR;
//where we store tests and logs
extern char* LOG_DIR;
// Dir where built binaries are stored after making or after uncompressing
extern char* BUILD_DIR;
// Dir where the package sources are downloaded and built
extern char* MAKE_DIR;
// temp dir
extern char* TMP_DIR;

//The file where a lot of data are stored
extern char* INSTALLED_FILE;
extern char* ALL_FILE;
// configuraton file
extern char* CONFIG_FILE;
// This is a fucking string array..., i know
extern char** REPOS;
extern int REPO_COUNT;

extern char* TEST_LOG;
