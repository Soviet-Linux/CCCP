# Explaining 
The libspm is the core of the Soviet Linux package manager. It is a library that is used to install packages and manage them.
Its ment to be called by any other program that needs to install packages.

The main frontend is the CCCP , the soviet llinux command line package manager.
The CCCP is written in rust , and is used by the user to install and manager package.
The CCCP will have to parse user input and call the libspm.


# Important functions 

The CCCP will have to call the following functions:

### Install functions 

 - `int installSpmFile(char* spm_path,int as_dep)` ==> This function will install a package from a source spm file.
 - `int installSpmBinary(char* archivePath,int as_dep)` ==> This function will install a package from a binary spm file.

### Remove function
 - `int uninstall(char* name)` ==> This function will uninstall a package.

### Update function

 - `int updatePkg(char* name) ` ==> This function will update a package.  
 _Note: This function is not yet implemented._


### Binary function
 - `int createBinary(char* spm_path,char* bin_path)` ==> This function will create a binary spm file from a source spm file.

### Misc
 - `int get(char* p_name,char* out_path)` ==> This function will dwnload a package from the repositories. (The _OUR_)
 - `void sync ()` ==> This function will sync packqge database with the repositories. (The _OUR_)
 - `int msg(enum level msgLevel, const char* message,...)` ==> This function will print a message to the user using the spm debugging style.
 - `void init()` ==> This function will initialize the libspm.

## _Note_

This is a work in progress. I may add other function or modify the existing ones.

### Future additions
 - `int get_all_data(char * DB_PATH)` ==> This function will get the data (type name version)from all packages in the package database.   
 _Note: This function is not yet implemented. The ptototype will change._

