/*
 * Copyright (C) 2009 Karel Zak <kzak@redhat.com>
 *
 * This file may be redistributed under the terms of the GNU Public
 * License.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <blkid.h>

#include "nls.h"
<<<<<<< HEAD
#include "c.h"

static void __attribute__((__noreturn__)) usage(int rc)
{
	const char *p = program_invocation_short_name;

	if (!p)
		p = "findfs";

	fprintf(stderr, _("Usage: %s LABEL=<label>|UUID=<uuid>\n"), p);
=======
#include "closestream.h"
#include "c.h"
#include "exitcodes.h"

/* Exit codes used by findfs. */
#define FINDFS_SUCCESS		0	/* no errors */
#define FINDFS_NOT_FOUND	1	/* label or uuid cannot be found */
#define FINDFS_USAGE_ERROR	2	/* user did something unexpected */

static void __attribute__((__noreturn__)) usage(int rc)
{
	FILE *out = rc ? stderr : stdout;
	fputs(USAGE_HEADER, out);
	fprintf(out, _(" %s [options] {LABEL,UUID,PARTUUID,PARTLABEL}=<value>\n"),
		program_invocation_short_name);

	fputs(USAGE_SEPARATOR, out);
	fputs(_("Find a filesystem by label or UUID.\n"), out);

	fputs(USAGE_OPTIONS, out);
	fputs(USAGE_HELP, out);
	fputs(USAGE_VERSION, out);
	fprintf(out, USAGE_MAN_TAIL("findfs(8)"));
>>>>>>> master-vanilla
	exit(rc);
}

int main(int argc, char **argv)
{
<<<<<<< HEAD
	char	*dev, *tk, *vl;
=======
	char	*dev;
>>>>>>> master-vanilla

	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);
<<<<<<< HEAD
=======
	atexit(close_stdout);
>>>>>>> master-vanilla

	if (argc != 2)
		/* we return '2' for backward compatibility
		 * with version from e2fsprogs */
<<<<<<< HEAD
		usage(2);

	if (!strncmp(argv[1], "LABEL=", 6)) {
		tk = "LABEL";
		vl = argv[1] + 6;
	} else if (!strncmp(argv[1], "UUID=", 5)) {
		tk = "UUID";
		vl = argv[1] + 5;
	} else if (!strcmp(argv[1], "-h") == 0 ||
		   !strcmp(argv[1], "--help") == 0) {
		usage(EXIT_SUCCESS);
	} else
		usage(2);

	dev = blkid_evaluate_tag(tk, vl, NULL);
	if (!dev)
		errx(EXIT_FAILURE, _("unable to resolve '%s'"),	argv[1]);

	puts(dev);
	exit(EXIT_SUCCESS);
=======
		usage(FINDFS_USAGE_ERROR);

	if (strcmp(argv[1], "-V") == 0 ||
		   strcmp(argv[1], "--version") == 0) {
		printf(UTIL_LINUX_VERSION);
		return FINDFS_SUCCESS;
	} else if (strcmp(argv[1], "-h") == 0 ||
		   strcmp(argv[1], "--help") == 0) {
		usage(FINDFS_SUCCESS);
	} else if (argv[1][0] == '-')
		usage(FINDFS_USAGE_ERROR);

	dev = blkid_evaluate_tag(argv[1], NULL, NULL);
	if (!dev)
		errx(FINDFS_NOT_FOUND, _("unable to resolve '%s'"), argv[1]);

	puts(dev);
	return FINDFS_SUCCESS;
>>>>>>> master-vanilla
}

