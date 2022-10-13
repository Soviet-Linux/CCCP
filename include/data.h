
int find_data_installed(char* DB_PATH,struct package* pkg,int* as_dep);
int find_data_repo(char* DB_PATH,struct package* pkg,char** fileFmt);
// remove an entry from all.json
int remove_data (char* i_path,char* name);
// add an entry in all.json
int store_data(char* DB_PATH,struct package* a_pkg,int as_dep);
// create the database and the table
int init_data(char* DB_PATH);
// list everythin
int get_all_data(char * DB_PATH);