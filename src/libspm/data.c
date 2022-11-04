#include "stdio.h"
#include "string.h"



//class thing
#include "../../include/libspm.h"
#include "../../include/utils.h"

#include "sqlite3.h"
#include <malloc.h>
#include <stdlib.h>



/*
This file contains several functions that are used to store , remove and manipulate installed package data.
Currently the data is stored in a json file.
This is not optimal , in fact it is reaaallly bad , but I dont know database stuff.
So every package name and version is stored , also the number of packages
If someone wants to chnage it , you just have to rewrite theses functions, dont touch other files.
I you want to do somethingg , you can use sqlite3 or any other database.

I really dont like that code so i wont comment. 
*/






/*
 The Format arg can be nulled if you dont care
 If you need it , you should preallocate if.
*/

int find_data_installed(char* DB_PATH,struct package* pkg,int* as_dep)
{
    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    
    int rc = sqlite3_open(DB_PATH , &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return -1;
    }
    
    char* sql = calloc(256,sizeof(char));
    sprintf(sql,"SELECT Version, Type, AsDep FROM Packages WHERE Name = '%s'",pkg->name);
        
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) {
        
        sqlite3_bind_int(res, 1, 3);
    } else {
        
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    
    int step = sqlite3_step(res);
    
    if (step == SQLITE_ROW) {
        pkg->version = strdup((char*)sqlite3_column_text(res, 0));
        pkg->type = strdup((char*)sqlite3_column_text(res, 1));
        *as_dep = sqlite3_column_int(res, 2);
        
    } 

    sqlite3_finalize(res);
    sqlite3_close(db);
    free(sql);
    
    return 0;
}

int find_data_repo(char* DB_PATH,struct package* pkg,char** fileFmt)
{
    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;
    
    int rc = sqlite3_open(DB_PATH , &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return -1;
    }
    
    char* sql = calloc(256,sizeof(char));
    sprintf(sql,"SELECT Version, Format FROM Packages WHERE Name = '%s'",pkg->name);
        
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) {
        
        sqlite3_bind_int(res, 1, 3);
    } else {
        
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    
    int step = sqlite3_step(res);
    
    if (step == SQLITE_ROW) {
        pkg->version = strdup((char*)sqlite3_column_text(res, 0));
        if (fileFmt != NULL)
        {
            *fileFmt = strdup((char*)sqlite3_column_text(res, 1));
        }
        
    } 

    sqlite3_finalize(res);
    sqlite3_close(db);
    free(sql);
    return 0;
}

int store_data(char* DB_PATH,struct package* a_pkg,int as_dep)
{
    msg(INFO,"Storing data to database %s",DB_PATH);

    sqlite3 *db;
    char *err_msg = 0;
    
    int rc = sqlite3_open(DB_PATH, &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }
    
    char* sql = calloc(256,sizeof(char));
    sprintf(sql,"INSERT INTO Packages VALUES('%s','%s','%s',%d);",a_pkg->name,a_pkg->version,a_pkg->type,as_dep);


    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);        
        sqlite3_close(db);
        
        return 1;
    } 
    
    sqlite3_close(db);
    free(sql);
    return 0;
}
int remove_data (char* DB_PATH,char* name)
{
    sqlite3 *db;
    char *err_msg = 0;
    
    int rc = sqlite3_open(DB_PATH, &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }
    
    char* sql = calloc(256,sizeof(char));
    sprintf(sql,"DELETE FROM Packages WHERE Name = '%s'",name);


    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);        
        sqlite3_close(db);
        
        return 1;
    } 
    
    sqlite3_close(db);
    free(sql);
    return 0;
}

int get_all_data(char * DB_PATH) {
    
    sqlite3 *db;
    char *err_msg = 0;
    
    int rc = sqlite3_open(DB_PATH, &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", 
                sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }
    
    char *sql = "SELECT * FROM Packages";
        
    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
    
    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);
        
        return 1;
    } 
    
    sqlite3_close(db);
    
    return 0;
}

int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    
    NotUsed = 0;
    printf("%s",*azColName);
    printf("%s - [%s] => (%s)", argv[0], argv[1], argv[2]);
    
    printf("\n");
    
    return 0;
 
}

int init_data(char* DB_PATH)
{
    sqlite3 *db;
    char *err_msg = 0;
    
    int rc = sqlite3_open(DB_PATH, &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }
    
    char *sql = "CREATE TABLE Packages( Name TEXT,Version TEXT,Type TEXT,AsDep INT);";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);        
        sqlite3_close(db);
        
        return 1;
    } 
    
    sqlite3_close(db);
    return 0;
}

/*
 A little more detail of the database scheme :
    The installed database contains basic data for all installed packages , 
    and the all databse contains information for all packages in repo :
     - Name TEXT -> the name of the Package
     - Version TEXT -> the Version of the package (In special format)
     - Type TEXT -> The type of the package ("src" or "bin")
     - Format TEXT -> the format of the local (or remote for all.db) copy of the package.
    Installed DB specific :
     - AsDEP INT -> 0 is the package is installed by the user , and 1 if its installed by the system.
*/