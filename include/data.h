// find a package from an all file in json
int find_data(char* DB_PATH,struct package* pkg,char* format);
// remove an entry from all.json
int remove_data (char* i_path,char* name);
// add an entry in all.json
int store_data(char* DB_PATH,struct package* a_pkg,int as_dep);
// create the database and the table
int init_data(char* DB_PATH);
// list everythin
int get_all_data(char * DB_PATH);