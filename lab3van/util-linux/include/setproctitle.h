<<<<<<< HEAD

void initproctitle (int argc, char **argv);
#if 0
void setproctitle (const char *fmt, ...);
#else
void setproctitle (const char *prog, const char *txt);
=======
#ifndef UTIL_LINUX_SETPROCTITLE_H
#define UTIL_LINUX_SETPROCTITLE_H

extern void initproctitle (int argc, char **argv);
extern void setproctitle (const char *prog, const char *txt);

>>>>>>> master-vanilla
#endif
