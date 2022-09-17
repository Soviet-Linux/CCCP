
struct ecmp_section {
    char *name;
    char *buff; 
};

int ecmp_package_parse(struct package* pkg,char* path);
unsigned int ecmp_parse_file(struct ecmp_section*** sections,char* path);