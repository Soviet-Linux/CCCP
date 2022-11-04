#include "stdio.h"
#include "stdlib.h"
#include "string.h"

// class stuff
#include "../../include/globals.h"
#include "../../include/libspm.h"
#include "../../include/utils.h"
#include <linux/limits.h>
#include <stdlib.h>

long get_locations(char ***locations, char *loc_dir) {
  // Get package file location
  char files_location_cmd[PATH_MAX + 64];
  sprintf(files_location_cmd,"( cd %s && find . -type f | cut -c2- ) ", BUILD_DIR);
  /*
  This way of getting locations is pretty bad , i should wite a proper way to do
  it But it works so its ok for now
  */
  msg(DBG2, "Getting files locations with %s ", files_location_cmd);

  // execute the commands
  // WARING : this is a memory leak
  char* res = exec(files_location_cmd);

  msg(DBG2, "Got locations !");
  
  // print the locations
  unsigned int max_count = countc(res, '\n');
  int count = splitm(res, '\n', *locations, max_count);


  printf("Got %d locations\n", count);

  /* WARNING : This is a bad way to do it , i should use a proper way to do it
   */
  //count -= 1;
  /* ARBITRARY NUMBER , i should find a better way to do it */
  // THIS ABOVE IS SOME REAL SHIT

  return count;
}