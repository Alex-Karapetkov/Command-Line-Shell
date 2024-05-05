#ifndef __cs361_builtins__
#define __cs361_builtins__

#include <stdbool.h>

int echo (char *);
int export(char *);
int pwd (void);
int unset (char *);
int which (char *);
int cd (char *);
void quit (void);
int can_execute (char *filepath);

#endif