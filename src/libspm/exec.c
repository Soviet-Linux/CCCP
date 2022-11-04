#include "stdio.h"
#include "stdlib.h"
#include "malloc.h"
// class stuff
#include "../../include/libspm.h"
#include "../../include/utils.h"
#include <string.h>

char* exec(const char* cmd) 
{
  FILE *fp;
  char path[PATH_MAX];
  char* result = (char*)calloc(2048,sizeof(char));

  /* Open the command for reading. */
  fp = popen(cmd, "r");
  if (fp == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }

  /* Read the output a line at a time - output it. */
  while (fgets(path, sizeof(path), fp) != NULL) {
    if (strlen(result) + strlen(path) > malloc_usable_size(result) / sizeof(char)) {
      result = (char*)realloc(result, strlen(result) + strlen(path) + 1);
    }
    strcat(result,path);
  }

  /* close */
  pclose(fp);

  return result;
}