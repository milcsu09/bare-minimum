#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void __main (int, char **);

int
main (int argc, char **argv)
{
  __main (argc, argv);
  return 0;
}

