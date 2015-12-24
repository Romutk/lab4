/* tailf.c -- tail a log file and then follow it
 * Created: Tue Jan  9 15:49:21 1996 by faith@acm.org
 * Copyright 1996, 2003 Rickard E. Faith (faith@acm.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
<<<<<<< HEAD
 *
 * less -F and tail -f cause a disk access every five seconds.  This
 * program avoids this problem by waiting for the file size to change.
 * Hence, the file is not accessed, and the access time does not need to be
 * flushed back to disk.  This is sort of a "stealth" tail.
 */

=======
 */

/*
 * This command is deprecated.  The utility is in maintenance mode,
 * meaning we keep them in source tree for backward compatibility
 * only.  Do not waste time making this command better, unless the
 * fix is about security or other very critical issue.
 *
 * See Documentation/deprecated.txt for more information.
 */


>>>>>>> master-vanilla
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>
#include <getopt.h>
<<<<<<< HEAD
=======
#include <sys/mman.h>

>>>>>>> master-vanilla
#ifdef HAVE_INOTIFY_INIT
#include <sys/inotify.h>
#endif

#include "nls.h"
#include "xalloc.h"
<<<<<<< HEAD
#include "usleep.h"
#include "strutils.h"
#include "c.h"

#define DEFAULT_LINES  10

static void
tailf(const char *filename, int lines)
{
	char *buf, *p;
	int  head = 0;
	int  tail = 0;
	FILE *str;
	int  i;

	if (!(str = fopen(filename, "r")))
		err(EXIT_FAILURE, _("cannot open \"%s\" for read"), filename);

	buf = xmalloc((lines ? lines : 1) * BUFSIZ);
	p = buf;
	while (fgets(p, BUFSIZ, str)) {
		if (++tail >= lines) {
			tail = 0;
			head = 1;
		}
		p = buf + (tail * BUFSIZ);
	}

	if (head) {
		for (i = tail; i < lines; i++)
			fputs(buf + (i * BUFSIZ), stdout);
		for (i = 0; i < tail; i++)
			fputs(buf + (i * BUFSIZ), stdout);
	} else {
		for (i = head; i < tail; i++)
			fputs(buf + (i * BUFSIZ), stdout);
	}

	fflush(stdout);
	free(buf);
	fclose(str);
}

static void
roll_file(const char *filename, off_t *size)
=======
#include "strutils.h"
#include "c.h"
#include "closestream.h"

#define DEFAULT_LINES  10

/* st->st_size has to be greater than zero! */
static void tailf(const char *filename, size_t lines, struct stat *st)
{
	int fd;
	size_t i;
	char *data;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		err(EXIT_FAILURE, _("cannot open %s"), filename);
	data = mmap(0, st->st_size, PROT_READ, MAP_SHARED, fd, 0);
	i = (size_t) st->st_size - 1;

	/* humans do not think last new line in a file should be counted,
	 * in that case do off by one from counter point of view */
	if (data[i] == '\n')
		lines++;
	while (i) {
		if (data[i] == '\n') {
			if (--lines == 0) {
				i++;
				break;
			}
		}
		i--;
	}

	fwrite(data + i, st->st_size - i, 1, stdout);

	munmap(data, st->st_size);
	close(fd);
	fflush(stdout);
}

static void roll_file(const char *filename, struct stat *old)
>>>>>>> master-vanilla
{
	char buf[BUFSIZ];
	int fd;
	struct stat st;
	off_t pos;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
<<<<<<< HEAD
		err(EXIT_FAILURE, _("cannot open \"%s\" for read"), filename);

	if (fstat(fd, &st) == -1)
		err(EXIT_FAILURE, _("cannot stat \"%s\""), filename);

	if (st.st_size == *size) {
=======
		err(EXIT_FAILURE, _("cannot open %s"), filename);

	if (fstat(fd, &st) == -1)
		err(EXIT_FAILURE, _("stat of %s failed"), filename);

	if (st.st_size == old->st_size) {
>>>>>>> master-vanilla
		close(fd);
		return;
	}

<<<<<<< HEAD
	if (lseek(fd, *size, SEEK_SET) != (off_t)-1) {
=======
	if (lseek(fd, old->st_size, SEEK_SET) != (off_t)-1) {
>>>>>>> master-vanilla
		ssize_t rc, wc;

		while ((rc = read(fd, buf, sizeof(buf))) > 0) {
			wc = write(STDOUT_FILENO, buf, rc);
			if (rc != wc)
				warnx(_("incomplete write to \"%s\" (written %zd, expected %zd)\n"),
					filename, wc, rc);
		}
		fflush(stdout);
	}

	pos = lseek(fd, 0, SEEK_CUR);

	/* If we've successfully read something, use the file position, this
	 * avoids data duplication. If we read nothing or hit an error, reset
	 * to the reported size, this handles truncated files.
	 */
<<<<<<< HEAD
	*size = (pos != -1 && pos != *size) ? pos : st.st_size;
=======
	old->st_size = (pos != -1 && pos != old->st_size) ? pos : st.st_size;
>>>>>>> master-vanilla

	close(fd);
}

<<<<<<< HEAD
static void
watch_file(const char *filename, off_t *size)
{
	do {
		roll_file(filename, size);
		usleep(250000);
=======
static void watch_file(const char *filename, struct stat *old)
{
	do {
		roll_file(filename, old);
		xusleep(250000);
>>>>>>> master-vanilla
	} while(1);
}


#ifdef HAVE_INOTIFY_INIT

#define EVENTS		(IN_MODIFY|IN_DELETE_SELF|IN_MOVE_SELF|IN_UNMOUNT)
#define NEVENTS		4

<<<<<<< HEAD
static int
watch_file_inotify(const char *filename, off_t *size)
=======
static int watch_file_inotify(const char *filename, struct stat *old)
>>>>>>> master-vanilla
{
	char buf[ NEVENTS * sizeof(struct inotify_event) ];
	int fd, ffd, e;
	ssize_t len;

	fd = inotify_init();
	if (fd == -1)
		return 0;

	ffd = inotify_add_watch(fd, filename, EVENTS);
	if (ffd == -1) {
		if (errno == ENOSPC)
			errx(EXIT_FAILURE, _("%s: cannot add inotify watch "
				"(limit of inotify watches was reached)."),
				filename);

		err(EXIT_FAILURE, _("%s: cannot add inotify watch."), filename);
	}

	while (ffd >= 0) {
		len = read(fd, buf, sizeof(buf));
		if (len < 0 && (errno == EINTR || errno == EAGAIN))
			continue;
		if (len < 0)
			err(EXIT_FAILURE,
				_("%s: cannot read inotify events"), filename);

		for (e = 0; e < len; ) {
			struct inotify_event *ev = (struct inotify_event *) &buf[e];

			if (ev->mask & IN_MODIFY)
<<<<<<< HEAD
				roll_file(filename, size);
=======
				roll_file(filename, old);
>>>>>>> master-vanilla
			else {
				close(ffd);
				ffd = -1;
				break;
			}
			e += sizeof(struct inotify_event) + ev->len;
		}
	}
	close(fd);
	return 1;
}

#endif /* HAVE_INOTIFY_INIT */

static void __attribute__ ((__noreturn__)) usage(FILE *out)
{
<<<<<<< HEAD
	fprintf(out,
		_("\nUsage:\n"
		  " %s [option] file\n"),
		program_invocation_short_name);

	fprintf(out, _(
		"\nOptions:\n"
		" -n, --lines NUMBER  output the last NUMBER lines\n"
		" -NUMBER             same as `-n NUMBER'\n"
		" -V, --version       output version information and exit\n"
		" -h, --help          display this help and exit\n\n"));
=======
	fputs(USAGE_HEADER, out);
	fprintf(out, _(" %s [option] <file>\n"), program_invocation_short_name);

	fputs(USAGE_SEPARATOR, out);
	fputs(_("Follow the growth of a log file.\n"), out);

	fputs(USAGE_OPTIONS, out);
	fputs(_(" -n, --lines <number>   output the last <number> lines\n"), out);
	fputs(_(" -<number>              same as '-n <number>'\n"), out);

	fputs(USAGE_SEPARATOR, out);
	fputs(USAGE_HELP, out);
	fputs(USAGE_VERSION, out);
	fprintf(out, USAGE_MAN_TAIL("tailf(1)"));
	fputs(_("Warning: use of 'tailf' is deprecated, use 'tail -f' instead.\n"), out);
>>>>>>> master-vanilla

	exit(out == stderr ? EXIT_FAILURE : EXIT_SUCCESS);
}

/* parses -N option */
<<<<<<< HEAD
long old_style_option(int *argc, char **argv)
{
	int i = 1, nargs = *argc;
	long lines = -1;

	while(i < nargs) {
		if (argv[i][0] == '-' && isdigit(argv[i][1])) {
			lines = strtol_or_err(argv[i] + 1,
					_("failed to parse number of lines"));
			nargs--;
=======
static long old_style_option(int *argc, char **argv, size_t *lines)
{
	int i = 1, nargs = *argc, ret = 0;

	while(i < nargs) {
		if (argv[i][0] == '-' && isdigit(argv[i][1])) {
			*lines = strtoul_or_err(argv[i] + 1,
					_("failed to parse number of lines"));
			nargs--;
			ret = 1;
>>>>>>> master-vanilla
			if (nargs - i)
				memmove(argv + i, argv + i + 1,
						sizeof(char *) * (nargs - i));
		} else
			i++;
	}
	*argc = nargs;
<<<<<<< HEAD
	return lines;
=======
	return ret;
>>>>>>> master-vanilla
}

int main(int argc, char **argv)
{
	const char *filename;
<<<<<<< HEAD
	long lines;
	int ch;
	struct stat st;
	off_t size = 0;

	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);
=======
	size_t lines;
	int ch;
	struct stat st;
>>>>>>> master-vanilla

	static const struct option longopts[] = {
		{ "lines",   required_argument, 0, 'n' },
		{ "version", no_argument,	0, 'V' },
		{ "help",    no_argument,	0, 'h' },
		{ NULL,      0, 0, 0 }
	};

<<<<<<< HEAD
	lines = old_style_option(&argc, argv);
	if (lines < 0)
		lines = DEFAULT_LINES;

	while ((ch = getopt_long(argc, argv, "n:N:Vh", longopts, NULL)) != -1)
		switch((char)ch) {
		case 'n':
		case 'N':
			lines = strtol_or_err(optarg,
					_("failed to parse number of lines"));
			break;
		case 'V':
			printf(_("%s from %s\n"), program_invocation_short_name,
						  PACKAGE_STRING);
=======
	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);
	atexit(close_stdout);

	if (!old_style_option(&argc, argv, &lines))
		lines = DEFAULT_LINES;

	while ((ch = getopt_long(argc, argv, "n:N:Vh", longopts, NULL)) != -1)
		switch ((char)ch) {
		case 'n':
		case 'N':
			lines = strtoul_or_err(optarg,
					_("failed to parse number of lines"));
			break;
		case 'V':
			printf(UTIL_LINUX_VERSION);
>>>>>>> master-vanilla
			exit(EXIT_SUCCESS);
		case 'h':
			usage(stdout);
		default:
			usage(stderr);
		}

	if (argc == optind)
		errx(EXIT_FAILURE, _("no input file specified"));

	filename = argv[optind];

	if (stat(filename, &st) != 0)
<<<<<<< HEAD
		err(EXIT_FAILURE, _("cannot stat \"%s\""), filename);

	size = st.st_size;;
	tailf(filename, lines);

#ifdef HAVE_INOTIFY_INIT
	if (!watch_file_inotify(filename, &size))
#endif
		watch_file(filename, &size);
=======
		err(EXIT_FAILURE, _("stat of %s failed"), filename);
	if (!S_ISREG(st.st_mode))
		errx(EXIT_FAILURE, _("%s: is not a file"), filename);
	if (st.st_size)
		tailf(filename, lines, &st);

#ifdef HAVE_INOTIFY_INIT
	if (!watch_file_inotify(filename, &st))
#endif
		watch_file(filename, &st);
>>>>>>> master-vanilla

	return EXIT_SUCCESS;
}
