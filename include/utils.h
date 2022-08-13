#pragma once

#include "libspm.h"

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

// format string
char* format( const char* strFmtString, ... );
// like the rm -rf command
int rmrf(char *path);
// a little wrapper that make strcpy easier
int strcpa(char** dest,const char* value);
// to split strings into arrays  with a delimiter
char** split (const char* string,char delim,int* returnCount);
// a tool to have cool terminal output
int msg(enum level msgLevel, const char* message,...);
// a small helper function to free an array
int freearr(void*** arr,unsigned long count);
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
// remove a char with astring index
void popchar(char* s,long s_size,int pos);
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
