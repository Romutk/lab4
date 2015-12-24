/*
 * ionice: set or get process io scheduling class and priority
 *
 * Copyright (C) 2005 Jens Axboe <jens@axboe.dk>
 *
 * Released under the terms of the GNU General Public License version 2
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <ctype.h>

#include "nls.h"
#include "strutils.h"
#include "c.h"
<<<<<<< HEAD
=======
#include "closestream.h"
>>>>>>> master-vanilla

static int tolerant;

static inline int ioprio_set(int which, int who, int ioprio)
{
	return syscall(SYS_ioprio_set, which, who, ioprio);
}

static inline int ioprio_get(int which, int who)
{
	return syscall(SYS_ioprio_get, which, who);
}

enum {
	IOPRIO_CLASS_NONE,
	IOPRIO_CLASS_RT,
	IOPRIO_CLASS_BE,
	IOPRIO_CLASS_IDLE,
};

enum {
	IOPRIO_WHO_PROCESS = 1,
	IOPRIO_WHO_PGRP,
	IOPRIO_WHO_USER,
};

#define IOPRIO_CLASS_SHIFT	(13)
#define IOPRIO_PRIO_MASK	((1UL << IOPRIO_CLASS_SHIFT) - 1)

#define IOPRIO_PRIO_CLASS(mask)	((mask) >> IOPRIO_CLASS_SHIFT)
#define IOPRIO_PRIO_DATA(mask)	((mask) & IOPRIO_PRIO_MASK)
#define IOPRIO_PRIO_VALUE(class, data)	(((class) << IOPRIO_CLASS_SHIFT) | data)

const char *to_prio[] = {
	[IOPRIO_CLASS_NONE] = "none",
	[IOPRIO_CLASS_RT]   = "realtime",
	[IOPRIO_CLASS_BE]   = "best-effort",
	[IOPRIO_CLASS_IDLE] = "idle"
};

static int parse_ioclass(const char *str)
{
	size_t i;

	for (i = 0; i < ARRAY_SIZE(to_prio); i++)
		if (!strcasecmp(str, to_prio[i]))
			return i;
	return -1;
}

<<<<<<< HEAD
static void ioprio_print(int pid)
{
	int ioprio = ioprio_get(IOPRIO_WHO_PROCESS, pid);
=======
static void ioprio_print(int pid, int who)
{
	int ioprio = ioprio_get(who, pid);
>>>>>>> master-vanilla

	if (ioprio == -1)
		err(EXIT_FAILURE, _("ioprio_get failed"));
	else {
		int ioclass = IOPRIO_PRIO_CLASS(ioprio);
		const char *name = _("unknown");

<<<<<<< HEAD
		if (ioclass > 0 && (size_t) ioclass < ARRAY_SIZE(to_prio))
			name = to_prio[ioclass];

		if (ioclass != IOPRIO_CLASS_IDLE)
			printf("%s: prio %lu\n", name,
					IOPRIO_PRIO_DATA(ioprio));
=======
		if (ioclass >= 0 && (size_t) ioclass < ARRAY_SIZE(to_prio))
			name = to_prio[ioclass];

		if (ioclass != IOPRIO_CLASS_IDLE)
			printf(_("%s: prio %lu\n"), name,
			       IOPRIO_PRIO_DATA(ioprio));
>>>>>>> master-vanilla
		else
			printf("%s\n", name);
	}
}

<<<<<<< HEAD
static void ioprio_setpid(pid_t pid, int ioclass, int data)
{
	int rc = ioprio_set(IOPRIO_WHO_PROCESS, pid,
=======
static void ioprio_setid(int which, int ioclass, int data, int who)
{
	int rc = ioprio_set(who, which,
>>>>>>> master-vanilla
			    IOPRIO_PRIO_VALUE(ioclass, data));

	if (rc == -1 && !tolerant)
		err(EXIT_FAILURE, _("ioprio_set failed"));
}

static void __attribute__ ((__noreturn__)) usage(FILE * out)
{
<<<<<<< HEAD
	fprintf(out,
	       _("\n"
		 "%1$s - sets or gets process io scheduling class and priority.\n"
		 "\n"
		 "Usage:\n"
		 "  %1$s [OPTION] -p PID [PID...]\n"
		 "  %1$s [OPTION] COMMAND\n"
		 "\n"
		 "Options:\n"
		 "  -c, --class <class>   scheduling class name or number\n"
		 "                           0: none, 1: realtime, 2: best-effort, 3: idle\n"
		 "  -n, --classdata <num> scheduling class data\n"
		 "                           0-7 for realtime and best-effort classes\n"
		 "  -p, --pid=PID         view or modify already running process\n"
		 "  -t, --ignore          ignore failures\n"
		 "  -V, --version         output version information and exit\n"
		 "  -h, --help            display this help and exit\n\n"),
		program_invocation_short_name);
=======
	fputs(USAGE_HEADER, out);
	fprintf(out,  _(" %1$s [options] -p <pid>...\n"
			" %1$s [options] -P <pgid>...\n"
			" %1$s [options] -u <uid>...\n"
			" %1$s [options] <command>\n"), program_invocation_short_name);

	fputs(USAGE_SEPARATOR, out);
	fputs(_("Show or change the I/O-scheduling class and priority of a process.\n"), out);

	fputs(USAGE_OPTIONS, out);
	fputs(_(" -c, --class <class>    name or number of scheduling class,\n"
		"                          0: none, 1: realtime, 2: best-effort, 3: idle\n"), out);
	fputs(_(" -n, --classdata <num>  priority (0..7) in the specified scheduling class,\n"
		"                          only for the realtime and best-effort classes\n"), out);
	fputs(_(" -p, --pid <pid>...     act on these already running processes\n"), out);
	fputs(_(" -P, --pgid <pgrp>...   act on already running processes in these groups\n"), out);
	fputs(_(" -t, --ignore           ignore failures\n"), out);
	fputs(_(" -u, --uid <uid>...     act on already running processes owned by these users\n"), out);

	fputs(USAGE_SEPARATOR, out);
	fputs(USAGE_HELP, out);
	fputs(USAGE_VERSION, out);

	fprintf(out, USAGE_MAN_TAIL("ionice(1)"));
>>>>>>> master-vanilla

	exit(out == stderr ? EXIT_FAILURE : EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
	int data = 4, set = 0, ioclass = IOPRIO_CLASS_BE, c;
<<<<<<< HEAD
	pid_t pid = 0;
=======
	int which = 0, who = 0;
	const char *invalid_msg = NULL;
>>>>>>> master-vanilla

	static const struct option longopts[] = {
		{ "classdata", required_argument, NULL, 'n' },
		{ "class",     required_argument, NULL, 'c' },
		{ "help",      no_argument,       NULL, 'h' },
		{ "ignore",    no_argument,       NULL, 't' },
		{ "pid",       required_argument, NULL, 'p' },
<<<<<<< HEAD
=======
		{ "pgid",      required_argument, NULL, 'P' },
		{ "uid",       required_argument, NULL, 'u' },
>>>>>>> master-vanilla
		{ "version",   no_argument,       NULL, 'V' },
		{ NULL, 0, NULL, 0 }
	};

	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);
<<<<<<< HEAD

	while ((c = getopt_long(argc, argv, "+n:c:p:tVh", longopts, NULL)) != EOF)
		switch (c) {
		case 'n':
			data = strtol_or_err(optarg, _("failed to parse class data"));
=======
	atexit(close_stdout);

	while ((c = getopt_long(argc, argv, "+n:c:p:P:u:tVh", longopts, NULL)) != EOF)
		switch (c) {
		case 'n':
			data = strtos32_or_err(optarg, _("invalid class data argument"));
>>>>>>> master-vanilla
			set |= 1;
			break;
		case 'c':
			if (isdigit(*optarg))
<<<<<<< HEAD
				ioclass = strtol_or_err(optarg,
						_("failed to parse class"));
=======
				ioclass = strtos32_or_err(optarg,
						_("invalid class argument"));
>>>>>>> master-vanilla
			else {
				ioclass = parse_ioclass(optarg);
				if (ioclass < 0)
					errx(EXIT_FAILURE,
						_("unknown scheduling class: '%s'"),
						optarg);
			}
			set |= 2;
			break;
		case 'p':
<<<<<<< HEAD
			pid = strtol_or_err(optarg, _("failed to parse pid"));
=======
			if (who)
				errx(EXIT_FAILURE,
				     _("can handle only one of pid, pgid or uid at once"));
			invalid_msg = _("invalid PID argument");
			which = strtos32_or_err(optarg, invalid_msg);
			who = IOPRIO_WHO_PROCESS;
			break;
		case 'P':
			if (who)
				errx(EXIT_FAILURE,
				     _("can handle only one of pid, pgid or uid at once"));
			invalid_msg = _("invalid PGID argument");
			which = strtos32_or_err(optarg, invalid_msg);
			who = IOPRIO_WHO_PGRP;
			break;
		case 'u':
			if (who)
				errx(EXIT_FAILURE,
				     _("can handle only one of pid, pgid or uid at once"));
			invalid_msg = _("invalid UID argument");
			which = strtos32_or_err(optarg, invalid_msg);
			who = IOPRIO_WHO_USER;
>>>>>>> master-vanilla
			break;
		case 't':
			tolerant = 1;
			break;
		case 'V':
<<<<<<< HEAD
			printf(_("%s from %s\n"),
				program_invocation_short_name, PACKAGE_STRING);
=======
			printf(UTIL_LINUX_VERSION);
>>>>>>> master-vanilla
			return EXIT_SUCCESS;
		case 'h':
			usage(stdout);
		default:
			usage(stderr);
		}

	switch (ioclass) {
		case IOPRIO_CLASS_NONE:
			if ((set & 1) && !tolerant)
				warnx(_("ignoring given class data for none class"));
			data = 0;
			break;
		case IOPRIO_CLASS_RT:
		case IOPRIO_CLASS_BE:
			break;
		case IOPRIO_CLASS_IDLE:
			if ((set & 1) && !tolerant)
				warnx(_("ignoring given class data for idle class"));
			data = 7;
			break;
		default:
			if (!tolerant)
				warnx(_("unknown prio class %d"), ioclass);
			break;
	}

<<<<<<< HEAD
	if (!set && !pid && optind == argc)
		/*
		 * ionice without options, print the current ioprio
		 */
		ioprio_print(0);

	else if (!set && pid) {
		/*
		 * ionice -p PID [PID ...]
		 */
		ioprio_print(pid);

		for(; argv[optind]; ++optind) {
			pid = strtol_or_err(argv[optind], _("failed to parse pid"));
			ioprio_print(pid);
		}
	} else if (set && pid) {
		/*
		 * ionice -c CLASS -p PID [PID ...]
		 */
		ioprio_setpid(pid, ioclass, data);

		for(; argv[optind]; ++optind) {
			pid = strtol_or_err(argv[optind], _("failed to parse pid"));
			ioprio_setpid(pid, ioclass, data);
=======
	if (!set && !which && optind == argc)
		/*
		 * ionice without options, print the current ioprio
		 */
		ioprio_print(0, IOPRIO_WHO_PROCESS);
	else if (!set && who) {
		/*
		 * ionice -p|-P|-u ID [ID ...]
		 */
		ioprio_print(which, who);

		for(; argv[optind]; ++optind) {
			which = strtos32_or_err(argv[optind], invalid_msg);
			ioprio_print(which, who);
		}
	} else if (set && who) {
		/*
		 * ionice -c CLASS -p|-P|-u ID [ID ...]
		 */
		ioprio_setid(which, ioclass, data, who);

		for(; argv[optind]; ++optind) {
			which = strtos32_or_err(argv[optind], invalid_msg);
			ioprio_setid(which, ioclass, data, who);
>>>>>>> master-vanilla
		}
	} else if (argv[optind]) {
		/*
		 * ionice [-c CLASS] COMMAND
		 */
<<<<<<< HEAD
		ioprio_setpid(0, ioclass, data);
		execvp(argv[optind], &argv[optind]);
		err(EXIT_FAILURE, _("executing %s failed"), argv[optind]);
=======
		ioprio_setid(0, ioclass, data, IOPRIO_WHO_PROCESS);
		execvp(argv[optind], &argv[optind]);
		err(EXIT_FAILURE, _("failed to execute %s"), argv[optind]);
>>>>>>> master-vanilla
	} else
		usage(stderr);


	return EXIT_SUCCESS;
}
