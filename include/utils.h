#pragma once

#include "libspm.h"
#include <limits.h> 

enum level {
    DBG1,
    DBG2,
    DBG3,
    INFO,
    ERROR,
    WARNING,
    FATAL,
    DOWNLOAD
};
// like the rm -rf command
int rmrf(char *path);
// a little wrapper that make strcpy easier
//int strcpa(char** dest,const char* value);
// to split strings into arrays  with a delimiter
unsigned int splitm (char* string,char delim,char** dest,unsigned max);
#define split(string,delim,dest) splitm(string,delim,dest,INT_MAX)

// to count the number of occurences of a char in a string
unsigned int countc(char* string,char c);
// a tool to have cool terminal output
int msg(enum level msgLevel, const char* message,...);
// a small helper function to free an array
//int freearr(void*** arr,unsigned long count);
// read entire file safely
long rdfile(const char* filePath,char** buffer);
// write entire file safely
int wrfile(const char* filePath,char* buffer,long size);
//get position in array
int strinarr( char* val, char** arr,long arrsize);
// Make a curl progress bar
int progress_func(void* ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded);
// donwnload a file 
int downloadFile(const char* url,const char* file_path);
//download package from repo
int downloadRepo(const char* url_path,const char* file_path);
// find last occurence of a char in a string.
long findlast(char* str,char c);
// remove a char with a string index
void popcharn(char* s,long s_size,int pos);
// exec a command and return the output
char* exec(const char* cmd);
// get package locations 
long get_locations(char*** locations, char* loc_dir);
// check and install dependencies for a package
int check_dependencies (char ** dependencies,int dependenciesCount);
// soething for sql stuff
int callback(void *NotUsed, int argc, char **argv, char **azColName);
// move program file to /
void move_binaries(char** locations,long loc_size);
// build a package from source
int make (char* package_dir,struct package* pkg);
// execute post install scripts
int exec_special(char* cmd,char* package_dir);
// untar a binary package to another dir
int uncompress_binary(char* bin_path,char* dest_dir);
// get package name from binray file
int get_bin_name(char* bin_path,char* name);
// cretae a binary spm package from dir
int create_archive(char* DIR,char* out_path);
//free string better
int sfree(char** ptr);
//check if the db global version is the same as the local version
int check_repo_version(int V_LOCAL);
// get local db version
int get_data_version(char* VERSION_PATH);
//check if  a package is installed
bool is_installed(char* name);
// check if a dir exists
int xis_dir (const char *d);
// create dir recursivelty (similar to mkdir -p)
int pmkdir (const char *dir);
//  move a file and create the dir if it doesn't exist
int mvsp(char* old_path,char* new_path);
// LIST  file in a dir
char** ls(char* path);
// free a package struct
int free_pkg(struct package* pkg);
