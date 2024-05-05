#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#define MAXLENGTH 100

static void usage (void);
static bool get_args (int, char **, char **, bool *, bool *);
static bool list_files (char *, bool, bool);

int
main (int argc, char *argv[])
{
  char *directory_path = ".";
  
  bool list_hidden_files = false;
  bool list_file_sizes = false;

  if (!get_args (argc, argv, &directory_path, &list_hidden_files, &list_file_sizes))
    {
      //usage ();
      exit (EXIT_FAILURE);
    }
  
  list_files (directory_path, list_hidden_files, list_file_sizes);
  return EXIT_SUCCESS;
}

// Parse command-line arguments for flags that may be included.
static bool
get_args (int argc, char **argv, char **directory_path, bool *list_hidden, bool *list_sizes)
{
  int ch = 0;
  while ((ch = getopt (argc, argv, "as")) != -1)
  {
    switch (ch)
    {
      case 'a':
        *list_hidden = true;
        break;
      
      case 's':
        *list_sizes = true;
        break;

      default:
        return false;
    }
  }

  // get directory path
  if (optind < argc)
    *directory_path = argv[optind];

  return true;
}

// function lists all files in specified directory; if no DIR is 
// specified, it uses the current working directory
static bool list_files (char *directory_path, bool list_hidden, bool list_sizes)
{
  DIR *directory;
  struct dirent *dir_stream;
  struct stat file_info;
  char path_buffer[MAXLENGTH];
  int bytes;

  // if directory path is ".", use current working directory
  if (!strcmp (directory_path, "."))
    {
      getcwd (directory_path, strlen (directory_path));
    }

  // open directory with provided path name and account for directory being NULL
  directory = opendir (directory_path);
  if (directory == NULL) {
    fprintf (stderr, "Cannot access '%s': No such file or directory\n", directory_path);
    usage ();
    return EXIT_FAILURE;
  }

  // traverse directory and print its contents
  while ((dir_stream = readdir (directory)) != NULL)
  {
    // skip over entries for current directory and parent directory
    if (!list_hidden && ((dir_stream->d_name[0] == '.' && dir_stream->d_name[1] == '\0') 
        || (dir_stream->d_name[1] == '.' && dir_stream->d_name[2] == '\0'))) {
      continue;
    }

    // don't list hidden file if -a flag not set
    if (dir_stream->d_name[0] == '.' && !list_hidden)
      continue;

    // construct full path of file in directory
    bytes = snprintf (path_buffer, sizeof(path_buffer), "%s/%s", directory_path, dir_stream->d_name);
    if (bytes >= sizeof (path_buffer))
      {
        fprintf (stderr, "Full directory path cannot be printed due to large size: %s/%s\n", directory_path
        , dir_stream->d_name);
        continue;
      }

    // get file info via stat()
    if (stat (path_buffer, &file_info) == -1)
      {
        fprintf (stderr, "Failed to get file info for '%s'\n", path_buffer);
        continue;
      }

    // if -s flag is set, print file size
    if (list_sizes)
      {
        printf ("%ld ", (long) file_info.st_size);
      }

    // print file name in directory
    printf ("%s\n", dir_stream -> d_name);
  }
  closedir (directory);
  return EXIT_SUCCESS;
}


static void
usage (void)
{
  printf ("ls, list directory contents\n");
  printf ("usage: ls [FLAG ...] [DIR]\n");
  printf ("FLAG is one or more of:\n");
  printf ("  -a       list all files (even hidden ones)\n");
  printf ("  -s       list file sizes\n");
  printf ("If no DIR specified, list current directory contents\n");
}
