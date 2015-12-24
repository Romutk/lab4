#ifndef UTIL_LINUX_MANGLE_H
#define UTIL_LINUX_MANGLE_H

/*
 * Functions for \oct encoding used in mtab/fstab/swaps/etc.
 */

extern char *mangle(const char *s);

extern void unmangle_to_buffer(const char *s, char *buf, size_t len);
<<<<<<< HEAD
=======
void unhexmangle_to_buffer(const char *s, char *buf, size_t len);

>>>>>>> master-vanilla
extern char *unmangle(const char *s, char **end);

static inline void unmangle_string(char *s)
{
	unmangle_to_buffer(s, s, strlen(s) + 1);
}

<<<<<<< HEAD
=======
static inline void unhexmangle_string(char *s)
{
	unhexmangle_to_buffer(s, s, strlen(s) + 1);
}

>>>>>>> master-vanilla
#endif /* UTIL_LINUX_MANGLE_H */

