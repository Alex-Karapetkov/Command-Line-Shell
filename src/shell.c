#include <stdio.h>
#include <string.h>

#include "builtins.h"
#include "hash.h"
#include "process.h"

// No command line can be more than 100 characters
#define MAXLENGTH 100

void
shell (FILE *input)
{
  hash_init (100);
  hash_insert ("?", "0");
  char buffer[MAXLENGTH];
  while (1)
    {
      // print the cursor
      printf ("$ ");

      // Print the cursor and get the next command entered
      memset (buffer, 0, sizeof (buffer));
      if (fgets (buffer, MAXLENGTH, input) == NULL)
        break;
      if (input != stdin)
        printf ("%s", buffer);
      // tokenizes command from STDIN
      char *token = strtok (buffer, " ");

      if (token != NULL && !strncmp (token, "echo", strlen ("echo")))
        {
          token = strtok (NULL, " ");
          // keep sending strings to echo until none are left from input line
          while (token != NULL)
            {
              echo (token);
              token = strtok (NULL, " ");
              if (token != NULL)
                printf (" ");
            }
        }
      else if (token != NULL && !strncmp (token, "pwd", strlen ("pwd")))
        {
          pwd ();
        }

      else if (token != NULL && !strncmp (token, "cd", strlen ("cd")))
        {
          char *path = strtok (NULL, " ");
          if (cd (path) == 1)
            {
              printf ("dukesh: cd: %sNo such file or directory\n", path);
            }
        }

      else if (token != NULL && !strncmp (token, "which", strlen ("which")))
        {
          char *command = strtok (NULL, " ");
          if (which (command) == 1)
            {
              printf ("No command passed and/or no location found.\n");
            }
        }
      else if (token != NULL && !strncmp (token, "quit", strlen ("quit")))
        {
          printf ("\n");
          quit ();
        }
    }
  printf ("\n");
  hash_destroy ();
}