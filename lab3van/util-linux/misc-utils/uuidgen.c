/*
 * gen_uuid.c --- generate a DCE-compatible uuid
 *
 * Copyright (C) 1999, Andreas Dilger and Theodore Ts'o
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Public
 * License.
 * %End-Header%
 */

#include <stdio.h>
<<<<<<< HEAD
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_GETOPT_H
#include <getopt.h>
#else
extern int getopt(int argc, char * const argv[], const char *optstring);
extern char *optarg;
extern int optind;
#endif
=======
#include <stdlib.h>
#include <getopt.h>
>>>>>>> master-vanilla

#include "uuid.h"
#include "nls.h"
#include "c.h"
<<<<<<< HEAD

#define DO_TYPE_TIME	1
#define DO_TYPE_RANDOM	2

static void __attribute__ ((__noreturn__)) usage(FILE * out)
{
	fputs(_("\nUsage:\n"), out);
	fprintf(out,
	      _(" %s [options]\n"), program_invocation_short_name);

	fputs(_("\nOptions:\n"), out);
=======
#include "closestream.h"

static void __attribute__ ((__noreturn__)) usage(FILE * out)
{
	fputs(USAGE_HEADER, out);
	fprintf(out,
	      _(" %s [options]\n"), program_invocation_short_name);

	fputs(USAGE_SEPARATOR, out);
	fputs(_("Create a new UUID value.\n"), out);

	fputs(USAGE_OPTIONS, out);
>>>>>>> master-vanilla
	fputs(_(" -r, --random     generate random-based uuid\n"
		" -t, --time       generate time-based uuid\n"
		" -V, --version    output version information and exit\n"
		" -h, --help       display this help and exit\n\n"), out);

<<<<<<< HEAD
=======
	fprintf(out, USAGE_MAN_TAIL("uuidgen(1)"));
>>>>>>> master-vanilla
	exit(out == stderr ? EXIT_FAILURE : EXIT_SUCCESS);
}

int
main (int argc, char *argv[])
{
	int    c;
	int    do_type = 0;
	char   str[37];
	uuid_t uu;

	static const struct option longopts[] = {
<<<<<<< HEAD
		{"random", required_argument, NULL, 'r'},
=======
		{"random", no_argument, NULL, 'r'},
>>>>>>> master-vanilla
		{"time", no_argument, NULL, 't'},
		{"version", no_argument, NULL, 'V'},
		{"help", no_argument, NULL, 'h'},
		{NULL, 0, NULL, 0}
	};

	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);
<<<<<<< HEAD
=======
	atexit(close_stdout);
>>>>>>> master-vanilla

	while ((c = getopt_long(argc, argv, "rtVh", longopts, NULL)) != -1)
		switch (c) {
		case 't':
<<<<<<< HEAD
			do_type = DO_TYPE_TIME;
			break;
		case 'r':
			do_type = DO_TYPE_RANDOM;
			break;
		case 'V':
			printf(_("%s from %s\n"),
				program_invocation_short_name,
				PACKAGE_STRING);
=======
			do_type = UUID_TYPE_DCE_TIME;
			break;
		case 'r':
			do_type = UUID_TYPE_DCE_RANDOM;
			break;
		case 'V':
			printf(UTIL_LINUX_VERSION);
>>>>>>> master-vanilla
			return EXIT_SUCCESS;
		case 'h':
			usage(stdout);
		default:
			usage(stderr);
		}

	switch (do_type) {
<<<<<<< HEAD
	case DO_TYPE_TIME:
		uuid_generate_time(uu);
		break;
	case DO_TYPE_RANDOM:
=======
	case UUID_TYPE_DCE_TIME:
		uuid_generate_time(uu);
		break;
	case UUID_TYPE_DCE_RANDOM:
>>>>>>> master-vanilla
		uuid_generate_random(uu);
		break;
	default:
		uuid_generate(uu);
		break;
	}

	uuid_unparse(uu, str);

	printf("%s\n", str);

	return EXIT_SUCCESS;
}
