#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>


// You may assume that lines are no longer than 1024 bytes
#define LINELEN 1024

static void usage (void);
static bool get_args (int, char **, char *, long *, char **);
static bool tokenize_and_print (char, long, char *);

int
main (int argc, char *argv[])
{
  char delimeter = ' ';
  long field_to_print = 1;
  char *file_name;

  if (!get_args (argc, argv, &delimeter, &field_to_print, &file_name))
    {
      usage ();
      exit (EXIT_FAILURE);
    }
  //printf("%s\n", file_name);
  // call function to tokenize line of input text and print out specified field
  if(!tokenize_and_print (delimeter, field_to_print, file_name))
    {
      usage ();
      exit(EXIT_FAILURE);
    }

  return EXIT_SUCCESS;
}

static bool
get_args (int argc, char **argv, char *delimeter, long *field_to_print, char **file_name)
{
  int ch = 0;
  while ((ch = getopt (argc, argv, "d:f:")) != -1)
  {
    switch (ch)
    {
      // -d flag specifies what delimeter lines should be split on
      case 'd':
        *delimeter = optarg[0];
        break;

      // -f flag specifies which field of the line of input text to print
      case 'f':
        *field_to_print = strtol (optarg, NULL, 10);
        break;
      
      default:
        return false;
    }

    // grab name of file passed after the flags
    if (optind < argc)
      {
        *file_name = argv[optind];
      }
  }

  // if no flags were passed, file name could be remaining argument
  if (optind == argc - 1 && *delimeter == ' ' && *field_to_print == 1)
    *file_name = argv[optind];

  return true;
}

static bool tokenize_and_print (char delimeter, long field_to_print, char *file_name)
{
  // read from standard input if no file is specified on the command line
  FILE *file;

  // check to make sure field_to_print field is valid
  if (field_to_print < 1)
    {
      return false;
    }
  
  // open file if it exists; otherwise return false
  if (file_name != NULL)
    {
      //printf("%s\n", file_name);
      file = fopen(file_name, "r");
      if (file == NULL)
        {
          fprintf(stderr, "Could not open file.");
          return false;
        }
    } else {
      file = stdin;
    }

  char file_line[LINELEN];
  while (fgets(file_line, sizeof(file_line), file) != NULL)
    {
      // tokenize line based on provided delimeter
      char *token = strtok (file_line, &delimeter);

      // loop through each token
      for (int currField = 1; token != NULL; currField++)
        {
          // print token if this is specified field
          if (currField == field_to_print)
            {
              printf("%s\n", token);
              break;
            }

          token = strtok (NULL, &delimeter);
        }
    }

  fclose (file);
  return true;
}

static void
usage (void)
{
  printf ("cut, splits each line based on a delimiter\n");
  printf ("usage: cut [FLAG] FILE\n");
  printf ("FLAG can be:\n");
  printf ("  -d C     split each line based on the character C (default ' ')\n");
  printf ("  -f N     print the Nth field (1 is first, default 1)\n");
  printf ("If no FILE specified, read from STDIN\n");
}
