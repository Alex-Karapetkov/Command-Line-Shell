#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

// No command line can be more than 100 characters
#define MAXLENGTH 100

// Given a message as input, print it to the screen followed by a
// newline ('\n'). If the message contains the two-byte escape sequence
// "\\n", print a newline '\n' instead. No other escape sequence is
// allowed. If the sequence contains a '$', it must be an environment
// variable or the return code variable ("$?"). Environment variable
// names must be wrapped in curly braces (e.g., ${PATH}).
//
// Returns 0 for success, 1 for errors (invalid escape sequence or no
// curly braces around environment variables).
int
echo (char *message)
{
  char *token = message;

  // check for '\\n'
  while ((token = strstr (token, "\\n")) != NULL)
    {
      // print portion of message before newline
      printf ("%.*s", (int)(token - message), message);

      // move past newline (2 bytes)
      token += 2;

      // print newline
      printf ("\n");

      message = token;
    }

  printf ("%s", message);
  return 0;
}

// Given a key-value pair string (e.g., "alpha=beta"), insert the mapping
// into the global hash table (hash_insert ("alpha", "beta")).
//
// Returns 0 on success, 1 for an invalid pair string (kvpair is NULL or
// there is no '=' in the string).
int export(char *kvpair) { return 0; }

// Prints the current working directory (see getcwd()). Returns 0.
int
pwd (void)
{
  char *buffer = calloc (MAXLENGTH, sizeof (char));
  if (getcwd (buffer, MAXLENGTH) == NULL)
    exit (EXIT_FAILURE);

  printf ("%s\n", buffer);
  free (buffer);
  return 0;
}

// Removes a key-value pair from the global hash table.
// Returns 0 on success, 1 if the key does not exist.
int
unset (char *key)
{
  return 0;
}

// helper function to check if passed file is executable
int
can_execute (const char *path)
{
  struct stat st;
  if (stat (path, &st) == 0)
    {
      return (st.st_mode & S_IXUSR) != 0;
    }
  return 0;
}

// Given a string of commands, find their location(s) in the $PATH global
// variable. If the string begins with "-a", print all locations, not just
// the first one.
//
// Returns 0 if at least one location is found, 1 if no commands were
// passed or no locations found.
int
which (char *cmdline)
{

  // check if the command is a built-in
  if (!strncmp (cmdline, "echo", strlen ("echo"))
      || !strncmp (cmdline, "export", strlen ("export"))
      || !strncmp (cmdline, "pwd", strlen ("pwd"))
      || !strncmp (cmdline, "unset", strlen ("unset"))
      || !strncmp (cmdline, "which", strlen ("which"))
      || !strncmp (cmdline, "cd", strlen ("cd"))
      || !strncmp (cmdline, "quit", strlen ("quit")))
    {
      // get rid of null terminator at end of string
      char *command = strtok (cmdline, "\n");
      printf ("%s: dukesh built-in command\n", command);
      return 0;
    }

  // check if command starts with ./
  if (strstr (cmdline, "./") == cmdline)
    {
      // check with function whether file can be executed
      int length = strlen (cmdline);
      // get rid of newline
      cmdline[length - 1] = '\0';
      if (can_execute (cmdline) == 1)
        {
          printf ("%s\n", cmdline);
          return 0;
        }
    }

  // traverse through $PATH for an executable file that matches the command
  char *path = getenv ("PATH");
  char *token = strtok (path, ":");
  while (token != NULL)
    {
      char filepath[255];
      int length = strlen (token) + strlen (cmdline);
      snprintf (filepath, sizeof (filepath), "%s/%s", token, cmdline);

      filepath[length] = '\0';

      if (can_execute (filepath) == 1)
        {
          printf ("%s\n", filepath);
          return 0;
        }

      token = strtok (NULL, ":");
    }
  return 1;
}

// change the current working directory to relative path that is passed
// returns 0 for success or 1 for errors
int
cd (char *path)
{
  int path_len = strlen (path);
  path[path_len - 1] = '\0';

  if (chdir (path) != 0)
    return 1;
  return 0;
}

// quit built-in command exits out of the shell when called
void
quit (void)
{
  exit (EXIT_SUCCESS);
}
