

// find a package from an all file in json
int find_data(char *ALL_PATH,struct package* a_pkg);
// remove an entry from all.json
int remove_data (char* i_path,char* name);
// add an entry in all.json
int add_data (char* i_path,struct package* a_pkg);