#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv)
{
   setuid(0);
   char *script_path = SCRIPTPATH;

   char **args = malloc(argc * sizeof(char));
   char *seperator = " ";

   // Calculate the sum the string lengths of all args
   int arglength = 0;
   for (int i = 1; i < argc; i++) {
      arglength = arglength + strlen(argv[i]);
   }

   // Allocate an appropriately sized buffer for our system call
   // char *buffer = malloc (strlen(script_path) * sizeof(char) + strlen(seperator) * argc * sizeof(char) + arglength * sizeof(char));
   char *buffer = malloc (strlen(script_path) + strlen(seperator) * argc + arglength );
   // Join the string and the arguments
   strcpy(buffer, script_path);
   for (int i = 1; i < argc; i++) {
      strcat(buffer, seperator);
      strcat(buffer, argv[i]);
   }

   system(buffer);

   return 0;
}
