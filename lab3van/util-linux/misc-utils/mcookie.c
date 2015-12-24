/* mcookie.c -- Generates random numbers for xauth
 * Created: Fri Feb  3 10:42:48 1995 by faith@cs.unc.edu
 * Revised: Fri Mar 19 07:48:01 1999 by faith@acm.org
 * Public Domain 1995, 1999 Rickard E. Faith (faith@acm.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * 
 * This program gathers some random bits of data and used the MD5
 * message-digest algorithm to generate a 128-bit hexadecimal number for
 * use with xauth(1).
 *
 * NOTE: Unless /dev/random is available, this program does not actually
 * gather 128 bits of random information, so the magic cookie generated
 * will be considerably easier to guess than one might expect.
 *
<<<<<<< HEAD
 * 1999-02-22 Arkadiusz Mi∂kiewicz <misiek@pld.ORG.PL>
=======
 * 1999-02-22 Arkadiusz Mi≈õkiewicz <misiek@pld.ORG.PL>
>>>>>>> master-vanilla
 * - added Native Language Support
 * 1999-03-21 aeb: Added some fragments of code from Colin Plumb.
 *
 */

#include "c.h"
#include "md5.h"
#include "nls.h"
<<<<<<< HEAD
=======
#include "closestream.h"
#include "randutils.h"
#include "strutils.h"
#include "xalloc.h"
#include "all-io.h"

>>>>>>> master-vanilla
#include <fcntl.h>
#include <getopt.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

<<<<<<< HEAD
#define BUFFERSIZE	4096

struct rngs {
	const char *path;
	int minlength, maxlength;
} rngs[] = {
	{"/dev/random",		16, 16},  /* 16 bytes = 128 bits suffice */
	{"/proc/interrupts",	 0,  0},
	{"/proc/slabinfo",	 0,  0},
	{"/proc/stat",		 0,  0},
	{"/dev/urandom",	32, 64},
};

#define RNGS (sizeof(rngs)/sizeof(struct rngs))

/* The basic function to hash a file */
static off_t hash_file(struct MD5Context *ctx, int fd)
{
	off_t count = 0;
	ssize_t r;
	unsigned char buf[BUFFERSIZE];

	while ((r = read(fd, buf, sizeof(buf))) > 0) {
		MD5Update(ctx, buf, r);
=======
enum {
	BUFFERSIZE = 4096,
	RAND_BYTES = 128
};

struct mcookie_control {
	struct	MD5Context ctx;
	char	**files;
	size_t	nfiles;
	uint64_t maxsz;

	unsigned int verbose:1;
};

/* The basic function to hash a file */
static uint64_t hash_file(struct mcookie_control *ctl, int fd)
{
	unsigned char buf[BUFFERSIZE];
	uint64_t wanted, count;

	wanted = ctl->maxsz ? ctl->maxsz : sizeof(buf);

	for (count = 0; count < wanted; ) {
		size_t rdsz = sizeof(buf);
		ssize_t r;

		if (wanted - count < rdsz)
			rdsz = wanted - count;

		r = read_all(fd, (char *) buf, rdsz);
		if (r < 0)
			break;
		MD5Update(&ctl->ctx, buf, r);
>>>>>>> master-vanilla
		count += r;
	}
	/* Separate files with a null byte */
	buf[0] = '\0';
<<<<<<< HEAD
	MD5Update(ctx, buf, 1);
=======
	MD5Update(&ctl->ctx, buf, 1);
>>>>>>> master-vanilla
	return count;
}

static void __attribute__ ((__noreturn__)) usage(FILE * out)
{
<<<<<<< HEAD
	fputs(_("\nUsage:\n"), out);
	fprintf(out,
	      _(" %s [options]\n"), program_invocation_short_name);

	fputs(_("\nOptions:\n"), out);
	fputs(_(" -f, --file <file> use file as a cookie seed\n"
		" -v, --verbose     explain what is being done\n"
		" -V, --version     output version information and exit\n"
		" -h, --help        display this help and exit\n\n"), out);
=======
	fputs(USAGE_HEADER, out);
	fprintf(out, _(" %s [options]\n"), program_invocation_short_name);

	fputs(USAGE_SEPARATOR, out);
	fputs(_("Generate magic cookies for xauth.\n"), out);

	fputs(USAGE_OPTIONS, out);
	fputs(_(" -f, --file <file>     use file as a cookie seed\n"), out);
	fputs(_(" -m, --max-size <num>  limit how much is read from seed files\n"), out);
	fputs(_(" -v, --verbose         explain what is being done\n"), out);

	fputs(USAGE_SEPARATOR, out);
	fputs(USAGE_HELP, out);
	fputs(USAGE_VERSION, out);
	fprintf(out, USAGE_MAN_TAIL("mcookie(1)"));
>>>>>>> master-vanilla

	exit(out == stderr ? EXIT_FAILURE : EXIT_SUCCESS);
}

<<<<<<< HEAD
int main(int argc, char **argv)
{
	size_t i;
	struct MD5Context ctx;
	unsigned char digest[MD5LENGTH];
	unsigned char buf[BUFFERSIZE];
	int fd;
	int c;
	pid_t pid;
	char *file = NULL;
	int verbose = 0;
	int r;
	struct timeval tv;
	struct timezone tz;

	static const struct option longopts[] = {
		{"file", required_argument, NULL, 'f'},
=======
static void randomness_from_files(struct mcookie_control *ctl)
{
	size_t i;

	for (i = 0; i < ctl->nfiles; i++) {
		const char *fname = ctl->files[i];
		size_t count;
		int fd;

		if (*fname == '-' && !*(fname + 1))
			fd = STDIN_FILENO;
		else
			fd = open(fname, O_RDONLY);

		if (fd < 0) {
			warn(_("cannot open %s"), fname);
		} else {
			count = hash_file(ctl, fd);
			if (ctl->verbose)
				fprintf(stderr,
					P_("Got %zu byte from %s\n",
					   "Got %zu bytes from %s\n", count),
					count, fname);

			if (fd != STDIN_FILENO)
				if (close(fd))
					err(EXIT_FAILURE,
					    _("closing %s failed"), fname);
		}
	}
}

int main(int argc, char **argv)
{
	struct mcookie_control ctl = { .verbose = 0 };
	size_t i;
	unsigned char digest[MD5LENGTH];
	unsigned char buf[RAND_BYTES];
	int c;

	static const struct option longopts[] = {
		{"file", required_argument, NULL, 'f'},
		{"max-size", required_argument, NULL, 'm'},
>>>>>>> master-vanilla
		{"verbose", no_argument, NULL, 'v'},
		{"version", no_argument, NULL, 'V'},
		{"help", no_argument, NULL, 'h'},
		{NULL, 0, NULL, 0}
	};

	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);
<<<<<<< HEAD

	while ((c =
		getopt_long(argc, argv, "f:vVh", longopts, NULL)) != -1)
		switch (c) {
		case 'v':
			verbose = 1;
			break;
		case 'f':
			file = optarg;
			break;
		case 'V':
			printf(_("%s from %s\n"),
			       program_invocation_short_name,
			       PACKAGE_STRING);
=======
	atexit(close_stdout);

	while ((c = getopt_long(argc, argv, "f:m:vVh", longopts, NULL)) != -1) {
		switch (c) {
		case 'v':
			ctl.verbose = 1;
			break;
		case 'f':
			if (!ctl.files)
				ctl.files = xmalloc(sizeof(char *) * argc);
			ctl.files[ctl.nfiles++] = optarg;
			break;
		case 'm':
			ctl.maxsz = strtosize_or_err(optarg,
						     _("failed to parse length"));
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
<<<<<<< HEAD

	MD5Init(&ctx);
	gettimeofday(&tv, &tz);
	MD5Update(&ctx, (unsigned char *) &tv, sizeof(tv));

	pid = getppid();
	MD5Update(&ctx, (unsigned char *) &pid, sizeof(pid));
	pid = getpid();
	MD5Update(&ctx, (unsigned char *) &pid, sizeof(pid));

	if (file) {
		int count = 0;

		if (file[0] == '-' && !file[1])
			fd = STDIN_FILENO;
		else
			fd = open(file, O_RDONLY);

		if (fd < 0) {
			warn(_("Could not open %s"), file);
		} else {
			count = hash_file(&ctx, fd);
			if (verbose)
				fprintf(stderr,
					_("Got %d bytes from %s\n"), count,
					file);

			if (fd != STDIN_FILENO)
				if (close(fd))
					err(EXIT_FAILURE,
					    _("closing %s failed"), file);
		}
	}

	for (i = 0; i < RNGS; i++) {
		if ((fd = open(rngs[i].path, O_RDONLY | O_NONBLOCK)) >= 0) {
			int count = sizeof(buf);

			if (rngs[i].maxlength && count > rngs[i].maxlength)
				count = rngs[i].maxlength;
			r = read(fd, buf, count);
			if (r > 0)
				MD5Update(&ctx, buf, r);
			else
				r = 0;
			close(fd);
			if (verbose)
				fprintf(stderr,
					_("Got %d bytes from %s\n"), r,
					rngs[i].path);
			if (rngs[i].minlength && r >= rngs[i].minlength)
				break;
		} else if (verbose)
			warn(_("Could not open %s"), rngs[i].path);
	}

	MD5Final(digest, &ctx);
=======
	}

	if (ctl.maxsz && ctl.nfiles == 0)
		warnx(_("--max-size ignored when used without --file"));

	randomness_from_files(&ctl);
	free(ctl.files);

	random_get_bytes(&buf, RAND_BYTES);
	MD5Update(&ctl.ctx, buf, RAND_BYTES);
	if (ctl.verbose)
		fprintf(stderr, P_("Got %d byte from %s\n",
				   "Got %d bytes from %s\n", RAND_BYTES),
				RAND_BYTES, random_tell_source());

	MD5Final(digest, &ctl.ctx);
>>>>>>> master-vanilla
	for (i = 0; i < MD5LENGTH; i++)
		printf("%02x", digest[i]);
	putchar('\n');

<<<<<<< HEAD
	/*
	 * The following is important for cases like disk full,
	 * so shell scripts can bomb out properly rather than
	 * think they succeeded.
	 */
	if (fflush(stdout) < 0 || fclose(stdout) < 0)
		return EXIT_FAILURE;

=======
>>>>>>> master-vanilla
	return EXIT_SUCCESS;
}
