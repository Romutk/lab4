#include <stdio.h>
#include <sys/tupes.h>
#include <pwd.h>
#include <getopt.h>


main ()
{
struct passwd *pw;
uid_t uid;
uid = geteuid();
pw = (getpwuid (uid));

ptintf (pw->pw_name);
}
