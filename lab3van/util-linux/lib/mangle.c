/*
 * Functions for \oct encoding used in mtab/fstab/swaps/etc.
 *
 * Based on code from mount(8).
 *
 * Copyright (C) 2010 Karel Zak <kzak@redhat.com>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "mangle.h"
#include "c.h"

<<<<<<< HEAD
#define isoctal(a) (((a) & ~7) == '0')

static unsigned char need_escaping[] = { ' ', '\t', '\n', '\\' };
=======
#define isoctal(a)		(((a) & ~7) == '0')

#define from_hex(c)		(isdigit(c) ? c - '0' : tolower(c) - 'a' + 10)

#define is_unwanted_char(x)	(strchr(" \t\n\\", (unsigned int) x) != NULL)

>>>>>>> master-vanilla

char *mangle(const char *s)
{
	char *ss, *sp;
<<<<<<< HEAD
	size_t n;
=======
>>>>>>> master-vanilla

	if (!s)
		return NULL;

<<<<<<< HEAD
	n = strlen(s);
	ss = sp = malloc(4*n+1);
	if (!sp)
		return NULL;
	while(1) {
		for (n = 0; n < sizeof(need_escaping); n++) {
			if (*s == need_escaping[n]) {
				*sp++ = '\\';
				*sp++ = '0' + ((*s & 0300) >> 6);
				*sp++ = '0' + ((*s & 070) >> 3);
				*sp++ = '0' + (*s & 07);
				goto next;
			}
		}
		*sp++ = *s;
		if (*s == 0)
			break;
	next:
=======
	ss = sp = malloc(4 * strlen(s) + 1);
	if (!sp)
		return NULL;
	while(1) {
		if (!*s) {
			*sp = '\0';
			break;
		}
		if (is_unwanted_char(*s)) {
			*sp++ = '\\';
			*sp++ = '0' + ((*s & 0300) >> 6);
			*sp++ = '0' + ((*s & 070) >> 3);
			*sp++ = '0' + (*s & 07);
		} else
			*sp++ = *s;
>>>>>>> master-vanilla
		s++;
	}
	return ss;
}

<<<<<<< HEAD
=======

>>>>>>> master-vanilla
void unmangle_to_buffer(const char *s, char *buf, size_t len)
{
	size_t sz = 0;

	if (!s)
		return;

	while(*s && sz < len - 1) {
<<<<<<< HEAD
		if (*s == '\\' && sz + 4 < len - 1 && isoctal(s[1]) &&
=======
		if (*s == '\\' && sz + 3 < len - 1 && isoctal(s[1]) &&
>>>>>>> master-vanilla
		    isoctal(s[2]) && isoctal(s[3])) {

			*buf++ = 64*(s[1] & 7) + 8*(s[2] & 7) + (s[3] & 7);
			s += 4;
			sz += 4;
		} else {
			*buf++ = *s++;
			sz++;
		}
	}
	*buf = '\0';
}

<<<<<<< HEAD
=======
void unhexmangle_to_buffer(const char *s, char *buf, size_t len)
{
	size_t sz = 0;

	if (!s)
		return;

	while(*s && sz < len - 1) {
		if (*s == '\\' && sz + 3 < len - 1 && s[1] == 'x' &&
		    isxdigit(s[2]) && isxdigit(s[3])) {

			*buf++ = from_hex(s[2]) << 4 | from_hex(s[3]);
			s += 4;
			sz += 4;
		} else {
			*buf++ = *s++;
			sz++;
		}
	}
	*buf = '\0';
}

>>>>>>> master-vanilla
static inline char *skip_nonspaces(const char *s)
{
	while (*s && !(*s == ' ' || *s == '\t'))
		s++;
	return (char *) s;
}

/*
 * Returns mallocated buffer or NULL in case of error.
 */
char *unmangle(const char *s, char **end)
{
	char *buf;
	char *e;
	size_t sz;

	if (!s)
		return NULL;

	e = skip_nonspaces(s);
	sz = e - s + 1;

	if (end)
		*end = e;
	if (e == s)
		return NULL;	/* empty string */

	buf = malloc(sz);
	if (!buf)
		return NULL;

	unmangle_to_buffer(s, buf, sz);
	return buf;
}

#ifdef TEST_PROGRAM
#include <errno.h>
int main(int argc, char *argv[])
{
<<<<<<< HEAD
	if (argc < 3) {
		fprintf(stderr, "usage: %s --mangle | --unmangle <string>\n",
=======
	char *p = NULL;
	if (argc < 3) {
		fprintf(stderr, "usage: %s --mangle|unmangle <string>\n",
>>>>>>> master-vanilla
						program_invocation_short_name);
		return EXIT_FAILURE;
	}

<<<<<<< HEAD
	if (!strcmp(argv[1], "--mangle"))
		printf("mangled: '%s'\n", mangle(argv[2]));
=======
	if (!strcmp(argv[1], "--mangle")) {
		p = mangle(argv[2]);
		printf("mangled: '%s'\n", p);
		free(p);
	}
>>>>>>> master-vanilla

	else if (!strcmp(argv[1], "--unmangle")) {
		char *x = unmangle(argv[2], NULL);

		if (x) {
			printf("unmangled: '%s'\n", x);
			free(x);
		}

		x = strdup(argv[2]);
		unmangle_to_buffer(x, x, strlen(x) + 1);

		if (x) {
			printf("self-unmangled: '%s'\n", x);
			free(x);
		}
	}

	return EXIT_SUCCESS;
}
#endif /* TEST_PROGRAM */
