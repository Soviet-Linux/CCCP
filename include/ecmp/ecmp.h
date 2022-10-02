
struct ecmp_section {
    char *name;
    char *buff; 
};

int open_ecmp(char* path,struct package* pkg);
unsigned int ecmp_parse_file(struct ecmp_section*** sections,char* path);
int create_ecmp(char* path,struct package* pkg);