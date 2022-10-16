
struct ecmp_section {
    char *name;
    char *buff; 
};
unsigned int ecmp_parse_file(struct ecmp_section*** sections,char* path);