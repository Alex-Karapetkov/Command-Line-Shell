#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <string.h>

// You may assume that lines are no longer than 1024 bytes
#define LINELEN 1024
#define CMDLINELEN 100
#define DEFAULTNUMLINES 5

static void usage (void);
static bool get_args (int, char **, long *, char **);
static bool print_lines (long , char *);

int
main (int argc, char *argv[])
{

  long num_lines = 5;
  char *file_name = NULL;

  if (!get_args (argc, argv, &num_lines, &file_name))
    {
      exit (EXIT_FAILURE);
    }

  print_lines (num_lines, file_name);

  return EXIT_SUCCESS;
}

// parse command-line arguments for possible flag and file name
static bool
get_args (int argc, char **argv, long *num_lines, char **file_name)
{
  int ch = 0;
  while ((ch = getopt (argc, argv, "n:")) != -1)
  {
    switch (ch)
    {
      // -n flag specifies how many lines of file to print
      case 'n':
        *num_lines = strtol (optarg, NULL, 10);
        break;

      default:
        return false;
    }
  }

  // grab name of file passed after the -n flag
  if (optind < argc)
    {
      *file_name = argv[optind];
    }

  // if -n flag was not passed, file name could be remaining argument
  if (optind < argc && *num_lines == 5)
    *file_name = argv[optind];

  return true;
}

// prints the first several lines of a file; the default is to print 5 lines; the -n flag
// can specify a different number. If no file is specified on the command line, read
// from standard input
static bool print_lines (long num_lines, char *file_name)
{
  char buffer [CMDLINELEN];
  FILE *file_ptr;
  char line_buffer [LINELEN];

  // if file name is NULL (no file name was passed), read from stdin
  if (file_name == NULL)
    {
      memset (buffer, 0, sizeof (buffer));
      if (fgets (buffer, sizeof (buffer), stdin) == NULL)
        {
          usage ();
          return EXIT_FAILURE;
        }
      file_name = buffer;
    }

    // open file for reading
    file_ptr = fopen (file_name, "r");
    if (file_ptr == NULL)
      {
        fprintf (stderr, "Cannot open file '%s'\n", file_name);
        return EXIT_FAILURE;
      }

    // print number of lines specified by num_lines from file stream
    while (line_buffer != NULL && num_lines != 0)
    {
      memset (line_buffer, 0, sizeof (line_buffer));
      fgets (line_buffer, sizeof (line_buffer), file_ptr);
      printf ("%s", line_buffer);
      num_lines -= 1;
    }
    return EXIT_SUCCESS;
}

static void
usage (void)
{
  printf ("head, prints the first few lines of a file\n");
  printf ("usage: head [FLAG] FILE\n");
  printf ("FLAG can be:\n");
  printf ("  -n N     show the first N lines (default 5)\n");
  printf ("If no FILE specified, read from STDIN\n");
}
