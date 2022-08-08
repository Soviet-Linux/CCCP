#pragma once

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

