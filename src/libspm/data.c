#include "stdio.h"
#include "string.h"



//class thing
#include "../../include/libspm.h"
#include "../../include/utils.h"
#include "../../include/json.h"

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




int get_data_version(char* VERSION_PATH)
{
    char* V_STR;
    rdfile(VERSION_PATH,&V_STR); 
    
    return atoi(V_STR);
}

int find_data(char* DB_PATH,struct package* pkg)
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
    
    char *sql = format("SELECT Version, Type FROM Packages WHERE Name = '%s'",pkg->name);
        
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) {
        
        sqlite3_bind_int(res, 1, 3);
    } else {
        
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    
    int step = sqlite3_step(res);
    
    if (step == SQLITE_ROW) {
        strcpa(&pkg->version,(char*)sqlite3_column_text(res, 0));
        strcpa(&pkg->type,(char*)sqlite3_column_text(res, 1));
        
    } 

    sqlite3_finalize(res);
    sqlite3_close(db);
    
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
    
    char *sql = format("INSERT INTO Packages VALUES('%s','%s','%s',%d);",a_pkg->name,a_pkg->version,a_pkg->type,as_dep);


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
    
    char *sql = format("DELETE FROM Packages WHERE Name = '%s'",name);


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