#include <stdio.h>
#include <sys/types.h>
#include <getopt.h>
#include <unistd.h>

int main ()
{
  char *cp;
  cp = getlogin ();
  puts (cp);
}
