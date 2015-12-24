/*
 * fsfreeze.c -- Filesystem freeze/unfreeze IO for Linux
 *
 * Copyright (C) 2010 Hajime Taira <htaira@redhat.com>
 *                    Masatake Yamato <yamato@redhat.com>
 *
 * This program is free software.  You can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation: either version 1 or
 * (at your option) any later version.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>

<<<<<<< HEAD
#include "blkdev.h"
#include "nls.h"
#include "c.h"
=======
#include "c.h"
#include "blkdev.h"
#include "nls.h"
#include "closestream.h"
#include "optutils.h"

enum fs_operation {
	NOOP,
	FREEZE,
	UNFREEZE
};
>>>>>>> master-vanilla

static int freeze_f(int fd)
{
	return ioctl(fd, FIFREEZE, 0);
}

static int unfreeze_f(int fd)
{
	return ioctl(fd, FITHAW, 0);
}

static void __attribute__((__noreturn__)) usage(FILE *out)
{
<<<<<<< HEAD
	fputs(_("\nUsage:\n"), out);
	fprintf(out,
	      _(" %s [options] <mount point>\n"), program_invocation_short_name);

	fputs(_("\nOptions:\n"), out);
	fputs(_(" -h, --help        this help\n"
		" -f, --freeze      freeze the filesystem\n"
		" -u, --unfreeze    unfreeze the filesystem\n"), out);

	fputs(_("\nFor more information see fsfreeze(8).\n"), out);
=======
	fprintf(out, USAGE_HEADER);
	fprintf(out,
	      _(" %s [options] <mountpoint>\n"), program_invocation_short_name);

	fputs(USAGE_SEPARATOR, out);
	fputs(_("Suspend access to a filesystem (ext3/4, ReiserFS, JFS, XFS).\n"), out);

	fputs(USAGE_OPTIONS, out);
	fputs(_(" -f, --freeze      freeze the filesystem\n"), out);
	fputs(_(" -u, --unfreeze    unfreeze the filesystem\n"), out);
	fprintf(out, USAGE_SEPARATOR);
	fprintf(out, USAGE_HELP);
	fprintf(out, USAGE_VERSION);
	fprintf(out, USAGE_MAN_TAIL("fsfreeze(8)"));
>>>>>>> master-vanilla

	exit(out == stderr ? EXIT_FAILURE : EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
	int fd = -1, c;
<<<<<<< HEAD
	int freeze = -1, rc = EXIT_FAILURE;
=======
	int action = NOOP, rc = EXIT_FAILURE;
>>>>>>> master-vanilla
	char *path;
	struct stat sb;

	static const struct option longopts[] = {
	    { "help",      0, 0, 'h' },
	    { "freeze",    0, 0, 'f' },
	    { "unfreeze",  0, 0, 'u' },
<<<<<<< HEAD
	    { NULL,        0, 0, 0 }
	};

	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);

	while ((c = getopt_long(argc, argv, "hfu", longopts, NULL)) != -1) {
=======
	    { "version",   0, 0, 'V' },
	    { NULL,        0, 0, 0 }
	};

	static const ul_excl_t excl[] = {       /* rows and cols in in ASCII order */
		{ 'f','u' },			/* freeze, unfreeze */
		{ 0 }
	};
	int excl_st[ARRAY_SIZE(excl)] = UL_EXCL_STATUS_INIT;

	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);
	atexit(close_stdout);

	while ((c = getopt_long(argc, argv, "hfuV", longopts, NULL)) != -1) {

		err_exclusive_options(c, longopts, excl, excl_st);

>>>>>>> master-vanilla
		switch(c) {
		case 'h':
			usage(stdout);
			break;
		case 'f':
<<<<<<< HEAD
			freeze = TRUE;
			break;
		case 'u':
			freeze = FALSE;
			break;
=======
			action = FREEZE;
			break;
		case 'u':
			action = UNFREEZE;
			break;
		case 'V':
			printf(UTIL_LINUX_VERSION);
			exit(EXIT_SUCCESS);
>>>>>>> master-vanilla
		default:
			usage(stderr);
			break;
		}
	}

<<<<<<< HEAD
	if (freeze == -1)
		errx(EXIT_FAILURE, _("no action specified"));
=======
	if (action == NOOP)
		errx(EXIT_FAILURE, _("neither --freeze or --unfreeze specified"));
>>>>>>> master-vanilla
	if (optind == argc)
		errx(EXIT_FAILURE, _("no filename specified"));
	path = argv[optind++];

	if (optind != argc) {
		warnx(_("unexpected number of arguments"));
		usage(stderr);
	}

	fd = open(path, O_RDONLY);
	if (fd < 0)
<<<<<<< HEAD
		err(EXIT_FAILURE, _("%s: open failed"), path);

	if (fstat(fd, &sb) == -1) {
		warn(_("%s: fstat failed"), path);
=======
		err(EXIT_FAILURE, _("cannot open %s"), path);

	if (fstat(fd, &sb) == -1) {
		warn(_("stat of %s failed"), path);
>>>>>>> master-vanilla
		goto done;
	}

	if (!S_ISDIR(sb.st_mode)) {
		warnx(_("%s: is not a directory"), path);
		goto done;
	}

<<<<<<< HEAD
	if (freeze) {
=======
	switch (action) {
	case FREEZE:
>>>>>>> master-vanilla
		if (freeze_f(fd)) {
			warn(_("%s: freeze failed"), path);
			goto done;
		}
<<<<<<< HEAD
	} else {
=======
		break;
	case UNFREEZE:
>>>>>>> master-vanilla
		if (unfreeze_f(fd)) {
			warn(_("%s: unfreeze failed"), path);
			goto done;
		}
<<<<<<< HEAD
=======
		break;
	default:
		abort();
>>>>>>> master-vanilla
	}

	rc = EXIT_SUCCESS;
done:
	if (fd >= 0)
		close(fd);
	return rc;
}

