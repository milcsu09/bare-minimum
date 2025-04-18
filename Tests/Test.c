#include <stdio.h>

extern void __main (void);

void
print (double x)
{
  printf ("%g\n", x);
}

int
main (void)
{
  __main ();
  return 0;
}

