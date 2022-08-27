#include "stdio.h"
#include "stdlib.h"
#include "string.h"

// class stuff
#include "../../include/globals.h"
#include "../../include/libspm.h"
#include "../../include/utils.h"

long get_locations(char ***locations, char *loc_dir) {
  // Get package file location
  char *files_location_cmd =
      format("( cd %s && find . -type f | cut -c2- ) ", BUILD_DIR);
  /*
  This way of getting locations is pretty bad , i should wite a proper way to do
  it But it works so its ok for now
  */
  msg(DBG2, "Getting files locations with %s ", files_location_cmd);

  // execute the commands
  char* res = exec(files_location_cmd);
  
  // print the locations
  int count;
  *locations = split(res, '\n', &count);

  /* WARNING : This is a bad way to do it , i should use a proper way to do it
   */
  count -= 1;
  /* ARBITRARY NUMBER , i should find a better way to do it */


  
  // free the memory
  free(res);

  return count;
}