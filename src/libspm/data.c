#include "stdio.h"
#include "../../lib/jsmn.h"
#include "string.h"



//class thing
#include "../../include/libspm.h"
#include "../../include/utils.h"
#include "../../include/json.h"
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

// This is the basic pattern of a json object insid the all file
#define ALL_OBJ_PATTERN "{\"name\":\"%s\",\"version\":\"%s\",\"type\":\"%s\"}"

// This specifies how an object in the all file is formatted
// (It can seem a little wierd , but it is not)
enum ALL_OBJ_FORMAT {
    K_NAME = 1, // ===> "name" :
    NAME = 2, // ===> "<value>_of_name"
    K_TYPE = 3, // ===> "type" :
    TYPE = 4, // ====> "<ssf|src|bin>"
    K_VERSION = 5, // ===> "version" :
    VERSION = 6 // ===> "<value_of_version>"
};

long all_find_obj_by_name(char* str,jsmntok_t t[],unsigned int t_num,char* name)
{
    // First find the good object :
    // To do that find the name

    int i;
    int obj_index = -1;
    // In the code below we are looping through all string in the json 
    // and checking is they are a name key and if their value is the name of our package
    for (i = 0; i < t_num; i++)  {
        if (
            t[i].type == JSMN_OBJECT && t[i].size == 3 && // Here we are checking if its an object and if its size is 3 (for K_NAME,K_VERSION and K_TYPE)
            strcmp(jstrval(t[i + K_NAME],str),"name") == 0 && // We are checking if t[i + K_NAME] = "name"
            strcmp(jstrval(t[i + NAME],str),name) == 0 // Here we are cheking the value of the NAME
           )
            obj_index = i;
    }
    return obj_index;
}

int find_data(char *ALL_PATH,struct package* a_pkg) // i  could just use the ALL_FILE global , but if i want to load from solething else , this system is easier
{



    char* a_str;
    long a_size = rdfile(ALL_PATH, &a_str) ;

    jsmntok_t *t;
    unsigned long j_size = jparse(&t,a_str,a_size);

    long obj_index = all_find_obj_by_name(a_str,t,j_size,a_pkg->name);

    if (obj_index < 1) return -1;

    // Now we need to parse this object
    a_pkg->version = jstrval(t[obj_index + VERSION],a_str);
    a_pkg->type = strtype(jstrval(t[obj_index + TYPE],a_str));
    
    // I could have optimised more the code above,
    // but it would grealty affect readability for a minimal performance boost 
    // So i prefer to keep it like that

    return 0;

}
int add_data (char* i_path,struct package* a_pkg)
{
    char * new_data; 
    char* data_str; // content of the old data fle
    long data_size = rdfile(i_path, &data_str);

    /*
    This part is kina a hack 
    Basically we are removing the ending '}' and ']'
    And the we append the json object 
    and finally we replace the chars we removed at the beginning.
    */


    char* new_obj = format(ALL_OBJ_PATTERN,a_pkg->name,a_pkg->version,typestr(a_pkg->type));


    popchar(data_str,data_size,findlast(data_str,'}'));
    popchar(data_str,data_size,findlast(data_str,']'));

    
    //allocating precisely all the memory needed by this fucking string
    new_data = calloc(data_size + strlen(new_obj) + 1,sizeof(char));

    strcat(new_data,",");
    strcat(new_data,new_obj);
    strcat(new_data,"]}");
    
    // write new data to file
    wrfile(i_path, new_data, strlen(new_data));

    free(new_data);
    free(data_str);
    free(new_obj);
    return 0;
}
int remove_data (char* i_path,char* name)
{
    char* data_str;
    long data_size = rdfile(i_path, &data_str);


    jsmntok_t *t; 

    unsigned long j_size = jparse(&t,data_str,data_size);

    long obj_index = all_find_obj_by_name(data_str, t,j_size,name);

    if (obj_index < 1) return -1;

    int i;
    for (i = t[obj_index].start; i >= t[obj_index].end;i++)
    {
        popchar(data_str,strlen(data_str),i);
    }
    return 0;
}

long listpkgs(char * FILE) {
    //open DATA_FILE
    char* data;
    int d_size = rdfile(FILE,&data);

    long package_count = 0;

    jsmntok_t *t; 
    unsigned long t_max = jparse(&t,data,d_size);

    // rint the name , versionand type of each packages
    int i;
    for (i = 0; i < t_max; i++) {
        if (t[i].type == JSMN_OBJECT && t[i].size == 3) {
            printf("=> %s  version=(%s) from [%s]\n",jstrval(t[i + NAME],data),jstrval(t[i + VERSION],data),jstrval(t[i + TYPE],data));
            package_count++;
        }
    }
    return package_count;
}