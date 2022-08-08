#include "libspm.h"

// parse json from spm file
int open_spm (const char* PPath,struct package* pkg);
//write package data to file
int create_spm(const char* newPath,struct package* pkg);