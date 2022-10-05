#include <stdio.h>
int print_hello() {
   printf("Hello World\n");
   return 0;
}
int edit_list(char** list,int count) {
   list[0]  = "China";
   printf("count is %d\n",count);
   list[count] = "China";
   count++;
   printf("count is %d\n",count);
   return count;
}
