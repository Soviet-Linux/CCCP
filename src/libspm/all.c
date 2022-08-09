#include "string.h"

#include "../../include/libspm.h"
#include "../../include/json.h"
#include "../../include/utils.h"

#include "../../lib/jsmn.h"
#include <stdlib.h>




int findall(char *ALL_PATH,struct package* a_pkg) // i  could just use the ALL_FILE global , but if i want to load from solething else , this system is easier
{
    // This specifies how an object in the all file is formatted
    // (It can seem a little wierd , but it is not)
    enum ALL_FORMAT {
        K_NAME = 1, // ===> "name" :
        NAME = 2, // ===> "<value>_of_name"
        K_VERSION = 3, // ===> "version" :
        VERSION = 4, // ===> "<value_of_version>"
        K_TYPE = 5, // ===> "type" :
        TYPE = 6, // ====> "<ssf|src|bin>"
    };


    char* a_str;
    long a_size = rdfile(ALL_PATH, &a_str) ;

    //TODO: IMPORTANT 
    //Find a way to calculate or at least aproximate the number of json tokens    
    int t_max = 128;

    //parse json
    //parsing json data
    jsmn_parser p;
    jsmntok_t t[t_max]; 

    jsmn_init(&p);
    int r = jsmn_parse(&p, a_str, a_size, t, 128);

    // First find the good object :
    // To do that find the name

    int i;
    int obj_index = -1;
    // In the code below we are looping through all string in the json 
    // and checking is they are a name key and if their value is the name of our package
    for (i = 1; i < r; i++) {
        if (
            t[i].type == JSMN_OBJECT && t[i].size == 3 && // Here we are checking if its an object and if its size is 3 (for K_NAME,K_VERSION and K_TYPE)
            strcmp(jstrval(t[i + K_NAME],a_str),"name") == 0 && // We are checking if t[i + K_NAME] = "name"
            strcmp(jstrval(t[i + NAME],a_str),a_pkg->name) == 0 // Here we are cheking the value of the NAME
           )
            obj_index = i;
    }

    // Now we need to parse this object
    a_pkg->version = jstrval(t[obj_index + VERSION],a_str);
    a_pkg->type = strtype(jstrval(t[obj_index + TYPE],a_str));
    
    // I could have optimised more the code above,
    // but it would grealty affect readability for a minimal performance boost 
    // So i prefer to keep it like that

    return 0;

}