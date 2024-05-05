#include <assert.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "hash.h"
#include "shell.h"

static bool get_args (int, char **, FILE **);
static void usage (void);

int
main (int argc, char *argv[])
{
  FILE *script = NULL;
  // call usage function and exit if get_args fails
  if (!get_args (argc, argv, &script))
    {
      usage ();
      exit (EXIT_FAILURE);
    }
  fclose (script);
  return EXIT_SUCCESS;
}

/* Parse the command-line arguments. Sets the client/server variables to
   point to a file name (typically in the data/ directory). Can also set
   the bot variable if a second file is used to interact with the
   client/server. If -d was passed, turn on debugging mode to print
   information about state transitions. */
static bool
get_args (int argc, char **argv, FILE **script)
{
  int ch = 0;
  while ((ch = getopt (argc, argv, "b:h")) != -1)
    {
      switch (ch)
        {
        // if dukesh is run with -b, read the commands from the file instead of
        // STDIN
        case 'b':
          *script = fopen (optarg, "r");
          shell (*script);
          break;
        case 'h':
          usage ();
          exit (EXIT_FAILURE);
        default:
          return false;
        }
    }
  shell (stdin);
  return true;
}

static void
usage (void)
{
  printf ("dukesh, a simple command shell\n");
  printf ("usage: dukesh [-b FILE]\n");
  printf ("  -b FILE    use FILE as a shell script to execute\n");
  printf ("If no script is passed, then the shell should be interactive,\n");
  printf ("processing one command at a time from STDIN.\n");
}
