/*
 * mountpoint(1) - see if a directory is a mountpoint
 *
 * This is libmount based reimplementation of the mountpoit(1)
 * from sysvinit project.
 *
 *
 * Copyright (C) 2011 Red Hat, Inc. All rights reserved.
 * Written by Karel Zak <kzak@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it would be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
<<<<<<< HEAD
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
=======
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
>>>>>>> master-vanilla
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <libmount.h>

#include "nls.h"
#include "xalloc.h"
#include "c.h"
<<<<<<< HEAD

static int quiet;

static dev_t dir_to_device(const char *spec)
{
	struct libmnt_table *tb = mnt_new_table_from_file("/proc/self/mountinfo");
	struct libmnt_fs *fs;
	struct libmnt_cache *cache;
	dev_t res = 0;
=======
#include "closestream.h"
#include "pathnames.h"

struct mountpoint_control {
	char *path;
	dev_t dev;
	struct stat st;
	unsigned int
		dev_devno:1,
		fs_devno:1,
		quiet:1;
};

static int dir_to_device(struct mountpoint_control *ctl)
{
	struct libmnt_table *tb = mnt_new_table_from_file(_PATH_PROC_MOUNTINFO);
	struct libmnt_fs *fs;
	struct libmnt_cache *cache;
	int rc = -1;
>>>>>>> master-vanilla

	if (!tb) {
		/*
		 * Fallback. Traditional way to detect mountpoints. This way
		 * is independent on /proc, but not able to detect bind mounts.
		 */
<<<<<<< HEAD
		struct stat pst, st;
		char buf[PATH_MAX], *cn;
		int len;

		if (stat(spec, &st) != 0)
			return 0;

		cn = mnt_resolve_path(spec, NULL);	/* canonicalize */

		len = snprintf(buf, sizeof(buf), "%s/..", cn ? cn : spec);
		free(cn);

		if (len < 0 || (size_t) len + 1 > sizeof(buf))
			return 0;
		if (stat(buf, &pst) !=0)
			return 0;

		if ((st.st_dev != pst.st_dev) ||
		    (st.st_dev == pst.st_dev && st.st_ino == pst.st_ino))
			return st.st_dev;

		return 0;
=======
		struct stat pst;
		char buf[PATH_MAX], *cn;
		int len;

		cn = mnt_resolve_path(ctl->path, NULL);	/* canonicalize */

		len = snprintf(buf, sizeof(buf), "%s/..", cn ? cn : ctl->path);
		free(cn);

		if (len < 0 || (size_t) len + 1 > sizeof(buf))
			return -1;
		if (stat(buf, &pst) !=0)
			return -1;

		if ((ctl->st.st_dev != pst.st_dev) ||
		    (ctl->st.st_dev == pst.st_dev && ctl->st.st_ino == pst.st_ino)) {
			ctl->dev = ctl->st.st_dev;
			return 0;
		}

		return -1;
>>>>>>> master-vanilla
	}

	/* to canonicalize all necessary paths */
	cache = mnt_new_cache();
	mnt_table_set_cache(tb, cache);
<<<<<<< HEAD

	fs = mnt_table_find_target(tb, spec, MNT_ITER_BACKWARD);
	if (fs && mnt_fs_get_target(fs))
		res = mnt_fs_get_devno(fs);

	mnt_free_table(tb);
	mnt_free_cache(cache);
	return res;
}

static int print_devno(const char *devname, struct stat *st)
{
	struct stat stbuf;

	if (!st && stat(devname, &stbuf) == 0)
		st = &stbuf;
	if (!st)
		return -1;
	if (!S_ISBLK(st->st_mode)) {
		if (!quiet)
			warnx(_("%s: not a block device"), devname);
		return -1;
	}
	printf("%u:%u\n", major(st->st_rdev), minor(st->st_rdev));
=======
	mnt_unref_cache(cache);

	fs = mnt_table_find_target(tb, ctl->path, MNT_ITER_BACKWARD);
	if (fs && mnt_fs_get_target(fs)) {
		ctl->dev = mnt_fs_get_devno(fs);
		rc = 0;
	}

	mnt_unref_table(tb);
	return rc;
}

static int print_devno(const struct mountpoint_control *ctl)
{
	if (!S_ISBLK(ctl->st.st_mode)) {
		if (!ctl->quiet)
			warnx(_("%s: not a block device"), ctl->path);
		return -1;
	}
	printf("%u:%u\n", major(ctl->st.st_rdev), minor(ctl->st.st_rdev));
>>>>>>> master-vanilla
	return 0;
}

static void __attribute__((__noreturn__)) usage(FILE *out)
{
<<<<<<< HEAD
	fputs(_("\nUsage:\n"), out);
=======
	fputs(USAGE_HEADER, out);
>>>>>>> master-vanilla
	fprintf(out,
	      _(" %1$s [-qd] /path/to/directory\n"
		" %1$s -x /dev/device\n"), program_invocation_short_name);

<<<<<<< HEAD
	fputs(_("\nOptions:\n"), out);
	fputs(_(" -q, --quiet        quiet mode - don't print anything\n"
		" -d, --fs-devno     print maj:min device number of the filesystem\n"
		" -x, --devno        print maj:min device number of the block device\n"
		" -h, --help         this help\n"), out);

	fprintf(out, _("\nFor more information see mountpoint(1).\n"));
=======
	fputs(USAGE_SEPARATOR, out);
	fputs(_("Check whether a directory or file is a mountpoint.\n"), out);

	fputs(USAGE_OPTIONS, out);
	fputs(_(" -q, --quiet        quiet mode - don't print anything\n"
		" -d, --fs-devno     print maj:min device number of the filesystem\n"
		" -x, --devno        print maj:min device number of the block device\n"), out);
	fputs(USAGE_SEPARATOR, out);
	fputs(USAGE_HELP, out);
	fputs(USAGE_VERSION, out);
	fprintf(out, USAGE_MAN_TAIL("mountpoint(1)"));
>>>>>>> master-vanilla

	exit(out == stderr ? EXIT_FAILURE : EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
<<<<<<< HEAD
	int c, fs_devno = 0, dev_devno = 0, rc = 0;
	char *spec;
	struct stat st;
=======
	int c;
	struct mountpoint_control ctl = { 0 };
>>>>>>> master-vanilla

	static const struct option longopts[] = {
		{ "quiet", 0, 0, 'q' },
		{ "fs-devno", 0, 0, 'd' },
		{ "devno", 0, 0, 'x' },
		{ "help", 0, 0, 'h' },
<<<<<<< HEAD
=======
		{ "version", 0, 0, 'V' },
>>>>>>> master-vanilla
		{ NULL, 0, 0, 0 }
	};

	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);
<<<<<<< HEAD

	mnt_init_debug(0);

	while ((c = getopt_long(argc, argv, "qdxh", longopts, NULL)) != -1) {

		switch(c) {
		case 'q':
			quiet = 1;
			break;
		case 'd':
			fs_devno = 1;
			break;
		case 'x':
			dev_devno = 1;
=======
	atexit(close_stdout);

	mnt_init_debug(0);

	while ((c = getopt_long(argc, argv, "qdxhV", longopts, NULL)) != -1) {

		switch(c) {
		case 'q':
			ctl.quiet = 1;
			break;
		case 'd':
			ctl.fs_devno = 1;
			break;
		case 'x':
			ctl.dev_devno = 1;
>>>>>>> master-vanilla
			break;
		case 'h':
			usage(stdout);
			break;
<<<<<<< HEAD
=======
		case 'V':
			printf(UTIL_LINUX_VERSION);
			return EXIT_SUCCESS;
>>>>>>> master-vanilla
		default:
			usage(stderr);
			break;
		}
	}

	if (optind + 1 != argc)
		usage(stderr);

<<<<<<< HEAD
	spec = argv[optind++];

	if (stat(spec, &st)) {
		if (!quiet)
			err(EXIT_FAILURE, "%s", spec);
		return EXIT_FAILURE;
	}
	if (dev_devno)
		rc = print_devno(spec, &st);
	else {
		dev_t src;

		if (!S_ISDIR(st.st_mode)) {
			if (!quiet)
				errx(EXIT_FAILURE, _("%s: not a directory"), spec);
			return EXIT_FAILURE;
		}
		src = dir_to_device(spec);
		if (!src) {
			if (!quiet)
				printf(_("%s is not a mountpoint\n"), spec);
			return EXIT_FAILURE;
		}
		if (fs_devno)
			printf("%u:%u\n", major(src), minor(src));
		else if (!quiet)
			printf(_("%s is a mountpoint\n"), spec);
	}

	return rc ? EXIT_FAILURE : EXIT_SUCCESS;
=======
	ctl.path = argv[optind];

	if (stat(ctl.path, &ctl.st)) {
		if (!ctl.quiet)
			err(EXIT_FAILURE, "%s", ctl.path);
		return EXIT_FAILURE;
	}
	if (ctl.dev_devno)
		return print_devno(&ctl) ? EXIT_FAILURE : EXIT_SUCCESS;
	if (dir_to_device(&ctl)) {
		if (!ctl.quiet)
			printf(_("%s is not a mountpoint\n"), ctl.path);
		return EXIT_FAILURE;
	}
	if (ctl.fs_devno)
		printf("%u:%u\n", major(ctl.dev), minor(ctl.dev));
	else if (!ctl.quiet)
		printf(_("%s is a mountpoint\n"), ctl.path);
	return EXIT_SUCCESS;
>>>>>>> master-vanilla
}
