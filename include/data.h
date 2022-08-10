#include "../lib/jsmn.h"

// find a package from an all file in json
int find_data(char *ALL_PATH,struct package* a_pkg);
// find an object with his name in all.json
long all_find_obj_by_name(char* str,jsmntok_t t[],int t_num,char* name);
// remove an entry from all.json
int remove_data (char* i_path,char* name);
// add an entry in all.json
int add_data (char* i_path,struct package* a_pkg);