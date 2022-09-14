#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "malloc.h"

#include "../include/utils.h"
#include "../include/libspm.h"


struct ecmp_section {
    char *name;
    char *buff; 
};

unsigned int ecmp_parse_file(struct ecmp_section** sections,char* path);



int ecmp_package_parse(struct package* pkg,char* path)
{
    struct ecmp_section** sections = NULL;

    // get sections from file
    // WARNING: this function allocates memory for sections
    unsigned int section_count = ecmp_parse_file(sections,path);

    // parse sections
    for (int i = 0; i < section_count; i++)
    {
        if (strcmp(sections[i]->name,"info") == 0)
        {
            // parse info section
            int line_count;
            char** lines = split(sections[i]->buff,'\n',&line_count);
            for (int j = 0; j < line_count; j++)
            {
                char** line = split(lines[j],'=',2);
                if (strcmp(line[0],"name") == 0)
                {
                    pkg->name = line[1];
                }
                else if (strcmp(line[0],"type") == 0)
                {
                    pkg->type = line[1];
                }
                else if (strcmp(line[0],"version") == 0)
                {
                    pkg->version = line[1];
                }
                else if (strcmp(line[0],"license") == 0)
                {
                    pkg->license = line[1];
                }
                else if (strcmp(line[0],"url") == 0)
                {
                    pkg->url = line[1];
                }
                else
                {
                    printf("Unknown info key: %s\n",line[0]);
                }
            }

   
                
        }
        else if (strcmp(sections[i]->name,"makedeps") == 0)
        {
            // parse files section
            int line_count;
            
        }
        else if (strcmp(sections[i]->name,"dependencies") == 0)
        {
            // parse dependencies section
        }
        else if (strcmp(sections[i]->name,"locations") == 0)
        {
            // parse scripts section
        }
        else if (strcmp(sections[i]->name,"description") == 0)
        {
            // parse scripts section
        }
        else if (strcmp(sections[i]->name,"install") == 0)
        {
            // parse scripts section
        }
        else if (strcmp(sections[i]->name,"special") == 0)
        {
            // parse scripts section
        }
        else {
            // unknown section
        }

    }

}





unsigned int ecmp_parse_file(struct ecmp_section** sections,char* path)
{
    unsigned int sec_count = 0;
    sections = calloc(32,sizeof(struct ecmp_section*));

    // read file line by line
    FILE* fp = fopen(path,"r");
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    while((read = getline(&line,&len,fp)) != -1)
    {
        // ignore comments
        if(line[0] == '#') continue;
        
        if (line[0] == '[') {
            // section name
            for (int i = 1; i < strlen(line); i++) {
                if (line[i] == ']') {
                    sections[sec_count]->name[i] = '\0';
                    break;
                }
                else {
                    sections[sec_count]->name[i] = line[i];
                }
            }
            printf("section name: %s",sections[sec_count]->name);
            sec_count++;
            continue;
        }

        // check if buff is null
        if (sec_count == 0) {
            continue;
        }

        add_line:

        // append line to buffer
        // check if buffer is full
        if ((strlen(sections[sec_count]->buff) + strlen(line)) * sizeof(char) > malloc_usable_size(sections[sec_count]->buff)) {
            printf("buffer full");
            // reallocate buffer
            sections[sec_count]->buff = realloc(sections[sec_count]->buff, malloc_usable_size(sections[sec_count]->buff) + 512);
            goto add_line;
        }
        strcat(sections[sec_count]->buff,line);
    }

    fclose(fp);
    return sec_count;
}