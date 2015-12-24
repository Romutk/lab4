/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
<<<<<<< HEAD
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
=======
 *      This product includes software developed by the University of
 *      California, Berkeley and its contributors.
>>>>>>> master-vanilla
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
<<<<<<< HEAD
 * 1999-02-22 Arkadiusz Mi∂kiewicz <misiek@pld.ORG.PL>
 * - added Native Language Support
 *
 * 2000-07-30 Per Andreas Buer <per@linpro.no> - added "q"-option
 */

/*
 * script
 */
=======
 * 1999-02-22 Arkadiusz Mi≈õkiewicz <misiek@pld.ORG.PL>
 * - added Native Language Support
 *
 * 2000-07-30 Per Andreas Buer <per@linpro.no> - added "q"-option
 *
 * 2014-05-30 Csaba Kos <csaba.kos@gmail.com>
 * - fixed a rare deadlock after child termination
 */

>>>>>>> master-vanilla
#include <stdio.h>
#include <stdlib.h>
#include <paths.h>
#include <time.h>
#include <sys/stat.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <locale.h>
#include <stddef.h>
<<<<<<< HEAD

#include "nls.h"
#include "c.h"

#if HAVE_LIBUTIL && HAVE_PTY_H
#include <pty.h>
#endif

#ifdef HAVE_LIBUTEMPTER
#include <utempter.h>
#endif

#define DEFAULT_OUTPUT "typescript"

void finish(int);
void done(void);
void fail(void);
void resize(int);
void fixtty(void);
void getmaster(void);
void getslave(void);
void doinput(void);
void dooutput(FILE *timingfd);
void doshell(void);

char	*shell;
FILE	*fscript;
int	master = -1;
int	slave;
pid_t	child;
pid_t	subchild;
int	childstatus;
char	*fname;

struct	termios tt;
struct	winsize win;
int	lb;
int	l;
#if !HAVE_LIBUTIL || !HAVE_PTY_H
char	line[] = "/dev/ptyXX";
#endif
int	aflg = 0;
char	*cflg = NULL;
int	eflg = 0;
int	fflg = 0;
int	qflg = 0;
int	tflg = 0;
int	forceflg = 0;

int die;
int resized;

static void
die_if_link(char *fn) {
	struct stat s;

	if (forceflg)
		return;
	if (lstat(fn, &s) == 0 && (S_ISLNK(s.st_mode) || s.st_nlink > 1))
		errx(EXIT_FAILURE,
		     _("output file `%s' is a link\n"
		       "Use --force if you really want to use it.\n"
		       "Program not started."), fn);
}

static void __attribute__((__noreturn__))
usage(FILE *out)
{
	fputs(_("\nUsage:\n"), out);
	fprintf(out,
	      _(" %s [options] [file]\n"), program_invocation_short_name);

	fputs(_("\nOptions:\n"), out);
	fputs(_(" -a, --append            append the output\n"
		" -c, --command <command> run command rather than interactive shell\n"
		" -r, --return            return exit code of the child process\n"
=======
#include <sys/wait.h>
#include <poll.h>
#include <sys/signalfd.h>
#include <assert.h>
#include <inttypes.h>

#include "closestream.h"
#include "nls.h"
#include "c.h"
#include "ttyutils.h"
#include "all-io.h"
#include "monotonic.h"

#include "debug.h"

UL_DEBUG_DEFINE_MASK(script);
UL_DEBUG_DEFINE_MASKNAMES(script) = UL_DEBUG_EMPTY_MASKNAMES;

#define SCRIPT_DEBUG_INIT	(1 << 1)
#define SCRIPT_DEBUG_POLL	(1 << 2)
#define SCRIPT_DEBUG_SIGNAL	(1 << 3)
#define SCRIPT_DEBUG_IO		(1 << 4)
#define SCRIPT_DEBUG_MISC	(1 << 5)
#define SCRIPT_DEBUG_ALL	0xFFFF

#define DBG(m, x)       __UL_DBG(script, SCRIPT_DEBUG_, m, x)
#define ON_DBG(m, x)    __UL_DBG_CALL(script, SCRIPT_DEBUG_, m, x)

#if defined(HAVE_LIBUTIL) && defined(HAVE_PTY_H)
# include <pty.h>
#endif

#ifdef HAVE_LIBUTEMPTER
# include <utempter.h>
#endif

#define DEFAULT_TYPESCRIPT_FILENAME "typescript"

struct script_control {
	char *shell;		/* shell to be executed */
	char *command;		/* command to be executed */
	char *fname;		/* output file path */
	FILE *typescriptfp;	/* output file pointer */
	char *tname;		/* timing file path */
	FILE *timingfp;		/* timing file pointer */
	struct timeval oldtime;	/* previous write or command start time */
	int master;		/* pseudoterminal master file descriptor */
	int slave;		/* pseudoterminal slave file descriptor */
	int poll_timeout;	/* poll() timeout, used in end of execution */
	pid_t child;		/* child pid */
	int childstatus;	/* child process exit value */
	struct termios attrs;	/* slave terminal runtime attributes */
	struct winsize win;	/* terminal window size */
#if !HAVE_LIBUTIL || !HAVE_PTY_H
	char line *;		/* terminal line */
#endif
	unsigned int
	 append:1,		/* append output */
	 rc_wanted:1,		/* return child exit value */
	 flush:1,		/* flush after each write */
	 quiet:1,		/* suppress most output */
	 timing:1,		/* include timing file */
	 force:1,		/* write output to links */
	 isterm:1,		/* is child process running as terminal */
	 data_on_way:1,		/* sent data to master */
	 die:1;			/* terminate program */

	sigset_t sigset;	/* catch SIGCHLD and SIGWINCH with signalfd() */
	sigset_t sigorg;	/* original signal mask */
	int sigfd;		/* file descriptor for signalfd() */
};

static void script_init_debug(void)
{
	__UL_INIT_DEBUG(script, SCRIPT_DEBUG_, 0, SCRIPT_DEBUG);
}

/*
 * For tests we want to be able to control time output
 */
#ifdef TEST_SCRIPT
static inline time_t script_time(time_t *t)
{
	const char *str = getenv("SCRIPT_TEST_SECOND_SINCE_EPOCH");
	int64_t sec;

	if (!str || sscanf(str, "%"SCNi64, &sec) != 1)
		return time(t);
	if (t)
		*t = (time_t)sec;
	return (time_t)sec;
}
#else	/* !TEST_SCRIPT */
# define script_time(x) time(x)
#endif

static void __attribute__((__noreturn__)) usage(FILE *out)
{
	fputs(USAGE_HEADER, out);
	fprintf(out, _(" %s [options] [file]\n"), program_invocation_short_name);

	fputs(USAGE_SEPARATOR, out);
	fputs(_("Make a typescript of a terminal session.\n"), out);

	fputs(USAGE_OPTIONS, out);
	fputs(_(" -a, --append            append the output\n"
		" -c, --command <command> run command rather than interactive shell\n"
		" -e, --return            return exit code of the child process\n"
>>>>>>> master-vanilla
		" -f, --flush             run flush after each write\n"
		"     --force             use output file even when it is a link\n"
		" -q, --quiet             be quiet\n"
		" -t, --timing[=<file>]   output timing data to stderr (or to FILE)\n"
		" -V, --version           output version information and exit\n"
		" -h, --help              display this help and exit\n\n"), out);

<<<<<<< HEAD
	exit(out == stderr ? EXIT_FAILURE : EXIT_SUCCESS);
}

/*
 * script -t prints time delays as floating point numbers
 * The example program (scriptreplay) that we provide to handle this
 * timing output is a perl script, and does not handle numbers in
 * locale format (not even when "use locale;" is added).
 * So, since these numbers are not for human consumption, it seems
 * easiest to set LC_NUMERIC here.
 */

int
main(int argc, char **argv) {
	sigset_t block_mask, unblock_mask;
	struct sigaction sa;
	extern int optind;
	int ch;
	FILE *timingfd = stderr;

	enum { FORCE_OPTION = CHAR_MAX + 1 };

	static const struct option longopts[] = {
		{ "append",	no_argument,	   NULL, 'a' },
		{ "command",	required_argument, NULL, 'c' },
		{ "return",	no_argument,	   NULL, 'e' },
		{ "flush",	no_argument,	   NULL, 'f' },
		{ "force",	no_argument,	   NULL, FORCE_OPTION, },
		{ "quiet",	no_argument,	   NULL, 'q' },
		{ "timing",	optional_argument, NULL, 't' },
		{ "version",	no_argument,	   NULL, 'V' },
		{ "help",	no_argument,	   NULL, 'h' },
		{ NULL,		0, NULL, 0 }
	};

	setlocale(LC_ALL, "");
	setlocale(LC_NUMERIC, "C");	/* see comment above */
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);

	while ((ch = getopt_long(argc, argv, "ac:efqt::Vh", longopts, NULL)) != -1)
		switch(ch) {
		case 'a':
			aflg = 1;
			break;
		case 'c':
			cflg = optarg;
			break;
		case 'e':
			eflg = 1;
			break;
		case 'f':
			fflg = 1;
			break;
		case FORCE_OPTION:
			forceflg = 1;
			break;
		case 'q':
			qflg = 1;
			break;
		case 't':
			if (optarg)
				if ((timingfd = fopen(optarg, "w")) == NULL)
					err(EXIT_FAILURE, _("cannot open timing file %s"), optarg);
			tflg = 1;
			break;
		case 'V':
			printf(_("%s from %s\n"), program_invocation_short_name,
						  PACKAGE_STRING);
			exit(EXIT_SUCCESS);
			break;
		case 'h':
			usage(stdout);
			break;
		case '?':
		default:
			usage(stderr);
		}
	argc -= optind;
	argv += optind;

	if (argc > 0)
		fname = argv[0];
	else {
		fname = DEFAULT_OUTPUT;
		die_if_link(fname);
	}
	if ((fscript = fopen(fname, aflg ? "a" : "w")) == NULL) {
		warn(_("open failed: %s"), fname);
		fail();
	}

	shell = getenv("SHELL");
	if (shell == NULL)
		shell = _PATH_BSHELL;

	getmaster();
	if (!qflg)
		printf(_("Script started, file is %s\n"), fname);
	fixtty();

#ifdef HAVE_LIBUTEMPTER
	utempter_add_record(master, NULL);
#endif
	/* setup SIGCHLD handler */
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = finish;
	sigaction(SIGCHLD, &sa, NULL);

	/* init mask for SIGCHLD */
	sigprocmask(SIG_SETMASK, NULL, &block_mask);
	sigaddset(&block_mask, SIGCHLD);

	sigprocmask(SIG_SETMASK, &block_mask, &unblock_mask);
	child = fork();
	sigprocmask(SIG_SETMASK, &unblock_mask, NULL);

	if (child < 0) {
		warn(_("fork failed"));
		fail();
	}
	if (child == 0) {

		sigprocmask(SIG_SETMASK, &block_mask, NULL);
		subchild = child = fork();
		sigprocmask(SIG_SETMASK, &unblock_mask, NULL);

		if (child < 0) {
			warn(_("fork failed"));
			fail();
		}
		if (child)
			dooutput(timingfd);
		else
			doshell();
	} else {
		sa.sa_handler = resize;
		sigaction(SIGWINCH, &sa, NULL);
	}
	doinput();

	fclose(timingfd);
	return EXIT_SUCCESS;
}

void
doinput() {
	ssize_t cc;
	char ibuf[BUFSIZ];

	fclose(fscript);

	while (die == 0) {
		if ((cc = read(STDIN_FILENO, ibuf, BUFSIZ)) > 0) {
			ssize_t wrt = write(master, ibuf, cc);
			if (wrt < 0) {
				warn (_("write failed"));
				fail();
			}
		}
		else if (cc < 0 && errno == EINTR && resized)
			resized = 0;
		else
			break;
	}

	done();
}

#include <sys/wait.h>

void
finish(int dummy __attribute__ ((__unused__))) {
	int status;
	pid_t pid;

	while ((pid = wait3(&status, WNOHANG, 0)) > 0)
		if (pid == child) {
			childstatus = status;
			die = 1;
		}
}

void
resize(int dummy __attribute__ ((__unused__))) {
	resized = 1;
	/* transmit window change information to the child */
	ioctl(STDIN_FILENO, TIOCGWINSZ, (char *)&win);
	ioctl(slave, TIOCSWINSZ, (char *)&win);
}

/*
 * Stop extremely silly gcc complaint on %c:
 *  warning: `%c' yields only last 2 digits of year in some locales
 */
static void
my_strftime(char *buf, size_t len, const char *fmt, const struct tm *tm) {
	strftime(buf, len, fmt, tm);
}

void
dooutput(FILE *timingfd) {
	ssize_t cc;
	time_t tvec;
	char obuf[BUFSIZ];
	struct timeval tv;
	double oldtime=time(NULL), newtime;
	int flgs = 0;
	ssize_t wrt;
	ssize_t fwrt;

	close(STDIN_FILENO);
#ifdef HAVE_LIBUTIL
	close(slave);
#endif
	tvec = time((time_t *)NULL);
	my_strftime(obuf, sizeof obuf, "%c\n", localtime(&tvec));
	fprintf(fscript, _("Script started on %s"), obuf);

	do {
		if (die && flgs == 0) {
			/* ..child is dead, but it doesn't mean that there is
			 * nothing in buffers.
			 */
			flgs = fcntl(master, F_GETFL, 0);
			if (fcntl(master, F_SETFL, (flgs | O_NONBLOCK)) == -1)
				break;
		}
		if (tflg)
			gettimeofday(&tv, NULL);

		errno = 0;
		cc = read(master, obuf, sizeof (obuf));

		if (die && errno == EINTR && cc <= 0)
			/* read() has been interrupted by SIGCHLD, try it again
			 * with O_NONBLOCK
			 */
			continue;
		if (cc <= 0)
			break;
		if (tflg) {
			newtime = tv.tv_sec + (double) tv.tv_usec / 1000000;
			fprintf(timingfd, "%f %zd\n", newtime - oldtime, cc);
			oldtime = newtime;
		}
		wrt = write(STDOUT_FILENO, obuf, cc);
		if (wrt < 0) {
			warn (_("write failed"));
			fail();
		}
		fwrt = fwrite(obuf, 1, cc, fscript);
		if (fwrt < cc) {
			warn (_("cannot write script file"));
			fail();
		}
		if (fflg)
			fflush(fscript);
	} while(1);

	if (flgs)
		fcntl(master, F_SETFL, flgs);
	done();
}

void
doshell() {
	char *shname;

#if 0
	int t;

	t = open(_PATH_DEV_TTY, O_RDWR);
	if (t >= 0) {
		ioctl(t, TIOCNOTTY, (char *)0);
		close(t);
	}
#endif

	getslave();
	close(master);
	fclose(fscript);
	dup2(slave, STDIN_FILENO);
	dup2(slave, STDOUT_FILENO);
	dup2(slave, STDERR_FILENO);
	close(slave);

	master = -1;

	shname = strrchr(shell, '/');
	if (shname)
		shname++;
	else
		shname = shell;

	if (cflg)
		execl(shell, shname, "-c", cflg, NULL);
	else
		execl(shell, shname, "-i", NULL);

	warn(_("failed to execute %s"), shell);
	fail();
}

void
fixtty() {
	struct termios rtt;

	rtt = tt;
=======
	fprintf(out, USAGE_MAN_TAIL("script(1)"));
	exit(out == stderr ? EXIT_FAILURE : EXIT_SUCCESS);
}

static void die_if_link(const struct script_control *ctl)
{
	struct stat s;

	if (ctl->force)
		return;
	if (lstat(ctl->fname, &s) == 0 && (S_ISLNK(s.st_mode) || s.st_nlink > 1))
		errx(EXIT_FAILURE,
		     _("output file `%s' is a link\n"
		       "Use --force if you really want to use it.\n"
		       "Program not started."), ctl->fname);
}

static void __attribute__((__noreturn__)) done(struct script_control *ctl)
{
	DBG(MISC, ul_debug("done!"));

	if (ctl->isterm)
		tcsetattr(STDIN_FILENO, TCSADRAIN, &ctl->attrs);
	if (!ctl->quiet)
		printf(_("Script done, file is %s\n"), ctl->fname);
#ifdef HAVE_LIBUTEMPTER
	if (ctl->master >= 0)
		utempter_remove_record(ctl->master);
#endif
	kill(ctl->child, SIGTERM);	/* make sure we don't create orphans */

	if (ctl->timingfp)
		fclose(ctl->timingfp);
	fclose(ctl->typescriptfp);

	if (ctl->rc_wanted) {
		if (WIFSIGNALED(ctl->childstatus))
			exit(WTERMSIG(ctl->childstatus) + 0x80);
		else
			exit(WEXITSTATUS(ctl->childstatus));
	}
	exit(EXIT_SUCCESS);
}

static void __attribute__((__noreturn__)) fail(struct script_control *ctl)
{
	DBG(MISC, ul_debug("fail!"));
	kill(0, SIGTERM);
	done(ctl);
}

static void wait_for_child(struct script_control *ctl, int wait)
{
	int status;
	pid_t pid;
	int options = wait ? 0 : WNOHANG;

	DBG(MISC, ul_debug("waiting for child"));

	while ((pid = wait3(&status, options, NULL)) > 0)
		if (pid == ctl->child)
			ctl->childstatus = status;
}

static void write_output(struct script_control *ctl, char *obuf,
			    ssize_t bytes)
{
	DBG(IO, ul_debug(" writing output"));

	if (ctl->timing && ctl->timingfp) {
		struct timeval now, delta;

		DBG(IO, ul_debug("  writing timing info"));

		gettime_monotonic(&now);
		timersub(&now, &ctl->oldtime, &delta);
		fprintf(ctl->timingfp, "%ld.%06ld %zd\n", delta.tv_sec, delta.tv_usec, bytes);
		if (ctl->flush)
			fflush(ctl->timingfp);
		ctl->oldtime = now;
	}

	DBG(IO, ul_debug("  writing to script file"));

	if (fwrite_all(obuf, 1, bytes, ctl->typescriptfp)) {
		warn(_("cannot write script file"));
		fail(ctl);
	}
	if (ctl->flush)
		fflush(ctl->typescriptfp);

	DBG(IO, ul_debug("  writing to output"));

	if (write_all(STDOUT_FILENO, obuf, bytes)) {
		DBG(IO, ul_debug("  writing output *failed*"));
		warn(_("write failed"));
		fail(ctl);
	}

	DBG(IO, ul_debug("  writing output *done*"));
}

static void wait_for_empty_fd(int fd)
{
	struct pollfd fds[] = {
	           { .fd = fd, .events = POLLIN }
	};

	while (poll(fds, 1, 10) == 1) {
		DBG(IO, ul_debug("  data to read"));
	}
}

/*
 * The script(1) is usually faster than shell, so it's good idea to wait until
 * the previous message is has been already read by shell from slave before we
 * wrate to master. This is necessary expecially for EOF situation when we can
 * send EOF to master before shell is fully initialized, to workaround this
 * problem we wait until slave is empty. For example:
 *
 *   echo "date" | script
 */
static int write_to_shell(struct script_control *ctl, char *buf, size_t bufsz)
{
	int rc;

	if (ctl->data_on_way) {
		wait_for_empty_fd(ctl->slave);
		ctl->data_on_way = 0;
	}
	rc = write_all(ctl->master, buf, bufsz);
	if (rc == 0)
		ctl->data_on_way = 1;
	return rc;

}

static void write_eof_to_shell(struct script_control *ctl)
{
	char c = DEF_EOF;

	DBG(IO, ul_debug(" sending EOF to master"));
	write_to_shell(ctl, &c, sizeof(char));
}

static void handle_io(struct script_control *ctl, int fd, int *eof)
{
	char buf[BUFSIZ];
	ssize_t bytes;

	DBG(IO, ul_debug("%d FD active", fd));
	*eof = 0;

	/* read from active FD */
	bytes = read(fd, buf, sizeof(buf));
	if (bytes < 0) {
		if (errno == EAGAIN || errno == EINTR)
			return;
		fail(ctl);
	}

	if (bytes == 0) {
		*eof = 1;
		return;
	}

	/* from stdin (user) to command */
	if (fd == STDIN_FILENO) {
		DBG(IO, ul_debug(" stdin --> master %zd bytes", bytes));

		if (write_to_shell(ctl, buf, bytes)) {
			warn(_("write failed"));
			fail(ctl);
		}
		/* without sync write_output() will write both input &
		 * shell output that looks like double echoing */
		fdatasync(ctl->master);

	/* from command (master) to stdout */
	} else if (fd == ctl->master) {
		DBG(IO, ul_debug(" master --> stdout %zd bytes", bytes));
		write_output(ctl, buf, bytes);
	}
}

static void handle_signal(struct script_control *ctl, int fd)
{
	struct signalfd_siginfo info;
	ssize_t bytes;

	DBG(SIGNAL, ul_debug("signal FD %d active", fd));

	bytes = read(fd, &info, sizeof(info));
	if (bytes != sizeof(info)) {
		if (bytes < 0 && (errno == EAGAIN || errno == EINTR))
			return;
		fail(ctl);
	}

	switch (info.ssi_signo) {
	case SIGCHLD:
		wait_for_child(ctl, 0);
		ctl->poll_timeout = 10;
		return;
	case SIGWINCH:
		if (ctl->isterm) {
			ioctl(STDIN_FILENO, TIOCGWINSZ, (char *)&ctl->win);
			ioctl(ctl->slave, TIOCSWINSZ, (char *)&ctl->win);
		}
		break;
	case SIGTERM:
		/* fallthrough */
	case SIGINT:
		/* fallthrough */
	case SIGQUIT:
		fprintf(stderr, _("\nSession terminated.\n"));
		/* Child termination is going to generate SIGCHILD (see above) */
		kill(ctl->child, SIGTERM);
		return;
	default:
		abort();
	}
}

static void do_io(struct script_control *ctl)
{
	int ret, ignore_stdin = 0, eof = 0;
	time_t tvec = script_time((time_t *)NULL);
	char buf[128];
	enum {
		POLLFD_SIGNAL = 0,
		POLLFD_MASTER,
		POLLFD_STDIN	/* optional; keep it last, see ignore_stdin */

	};
	struct pollfd pfd[] = {
		[POLLFD_SIGNAL] = { .fd = ctl->sigfd,   .events = POLLIN | POLLERR | POLLHUP },
		[POLLFD_MASTER] = { .fd = ctl->master,  .events = POLLIN | POLLERR | POLLHUP },
		[POLLFD_STDIN]	= { .fd = STDIN_FILENO, .events = POLLIN | POLLERR | POLLHUP }
	};


	if ((ctl->typescriptfp = fopen(ctl->fname, ctl->append ? "a" : "w")) == NULL) {
		warn(_("cannot open %s"), ctl->fname);
		fail(ctl);
	}
	if (ctl->timing) {
		if (!ctl->tname) {
			if (!(ctl->timingfp = fopen("/dev/stderr", "w")))
				err(EXIT_FAILURE, _("cannot open %s"), "/dev/stderr");
		} else if (!(ctl->timingfp = fopen(ctl->tname, "w")))
			err(EXIT_FAILURE, _("cannot open %s"), ctl->tname);
	}


	strftime(buf, sizeof buf, "%c\n", localtime(&tvec));
	fprintf(ctl->typescriptfp, _("Script started on %s"), buf);
	gettime_monotonic(&ctl->oldtime);

	while (!ctl->die) {
		size_t i;
		int errsv;

		DBG(POLL, ul_debug("calling poll()"));

		/* wait for input or signal */
		ret = poll(pfd, ARRAY_SIZE(pfd) - ignore_stdin, ctl->poll_timeout);
		errsv = errno;
		DBG(POLL, ul_debug("poll() rc=%d", ret));

		if (ret < 0) {
			if (errsv == EAGAIN)
				continue;
			warn(_("poll failed"));
			fail(ctl);
		}
		if (ret == 0) {
			DBG(POLL, ul_debug("setting die=1"));
			ctl->die = 1;
			break;
		}

		for (i = 0; i < ARRAY_SIZE(pfd) - ignore_stdin; i++) {
			if (pfd[i].revents == 0)
				continue;

			DBG(POLL, ul_debug(" active pfd[%s].fd=%d %s %s %s",
						i == POLLFD_STDIN  ? "stdin" :
						i == POLLFD_MASTER ? "master" :
						i == POLLFD_SIGNAL ? "signal" : "???",
						pfd[i].fd,
						pfd[i].revents & POLLIN  ? "POLLIN" : "",
						pfd[i].revents & POLLHUP ? "POLLHUP" : "",
						pfd[i].revents & POLLERR ? "POLLERR" : ""));
			switch (i) {
			case POLLFD_STDIN:
			case POLLFD_MASTER:
				/* data */
				if (pfd[i].revents & POLLIN)
					handle_io(ctl, pfd[i].fd, &eof);
				/* EOF maybe detected by two ways:
				 *	A) poll() return POLLHUP event after close()
				 *	B) read() returns 0 (no data) */
				if ((pfd[i].revents & POLLHUP) || eof) {
					DBG(POLL, ul_debug(" ignore FD"));
					pfd[i].fd = -1;
					/* according to man poll() set FD to -1 can't be used to ignore
					 * STDIN, so let's remove the FD from pool at all */
					if (i == POLLFD_STDIN) {
						ignore_stdin = 1;
						write_eof_to_shell(ctl);
					}
				}
				continue;
			case POLLFD_SIGNAL:
				handle_signal(ctl, pfd[i].fd);
				break;
			}
		}
	}

	DBG(POLL, ul_debug("poll() done"));

	if (!ctl->die)
		wait_for_child(ctl, 1);
	if (!ctl->quiet && ctl->typescriptfp) {
		tvec = script_time((time_t *)NULL);
		strftime(buf, sizeof buf, "%c\n", localtime(&tvec));
		fprintf(ctl->typescriptfp, _("\nScript done on %s"), buf);
	}
	done(ctl);
}

static void getslave(struct script_control *ctl)
{
#ifndef HAVE_LIBUTIL
	ctl->line[strlen("/dev/")] = 't';
	ctl->slave = open(ctl->line, O_RDWR);
	if (ctl->slave < 0) {
		warn(_("cannot open %s"), ctl->line);
		fail(ctl);
	}
	if (ctl->isterm) {
		tcsetattr(ctl->slave, TCSANOW, &ctl->attrs);
		ioctl(ctl->slave, TIOCSWINSZ, (char *)&ctl->win);
	}
#endif
	setsid();
	ioctl(ctl->slave, TIOCSCTTY, 0);
}

/* don't use DBG() stuff here otherwise it will be in  the typescript file */
static void __attribute__((__noreturn__)) do_shell(struct script_control *ctl)
{
	char *shname;

	getslave(ctl);

	/* close things irrelevant for this process */
	close(ctl->master);
	close(ctl->sigfd);

	dup2(ctl->slave, STDIN_FILENO);
	dup2(ctl->slave, STDOUT_FILENO);
	dup2(ctl->slave, STDERR_FILENO);
	close(ctl->slave);

	ctl->master = -1;

	shname = strrchr(ctl->shell, '/');
	if (shname)
		shname++;
	else
		shname = ctl->shell;

	sigprocmask(SIG_SETMASK, &ctl->sigorg, NULL);

	/*
	 * When invoked from within /etc/csh.login, script spawns a csh shell
	 * that spawns programs that cannot be killed with a SIGTERM. This is
	 * because csh has a documented behavior wherein it disables all
	 * signals when processing the /etc/csh.* files.
	 *
	 * Let's restore the default behavior.
	 */
	signal(SIGTERM, SIG_DFL);

	if (access(ctl->shell, X_OK) == 0) {
		if (ctl->command)
			execl(ctl->shell, shname, "-c", ctl->command, NULL);
		else
			execl(ctl->shell, shname, "-i", NULL);
	} else {
		if (ctl->command)
			execlp(shname, "-c", ctl->command, NULL);
		else
			execlp(shname, "-i", NULL);
	}
	warn(_("failed to execute %s"), ctl->shell);
	fail(ctl);
}

static void fixtty(struct script_control *ctl)
{
	struct termios rtt;

	if (!ctl->isterm)
		return;

	rtt = ctl->attrs;
>>>>>>> master-vanilla
	cfmakeraw(&rtt);
	rtt.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &rtt);
}

<<<<<<< HEAD
void
fail() {

	kill(0, SIGTERM);
	done();
}

void
done() {
	time_t tvec;

	if (subchild) {
		if (!qflg) {
			char buf[BUFSIZ];
			tvec = time((time_t *)NULL);
			my_strftime(buf, sizeof buf, "%c\n", localtime(&tvec));
			fprintf(fscript, _("\nScript done on %s"), buf);
		}
		fclose(fscript);
		close(master);

		master = -1;
	} else {
		tcsetattr(STDIN_FILENO, TCSADRAIN, &tt);
		if (!qflg)
			printf(_("Script done, file is %s\n"), fname);
#ifdef HAVE_LIBUTEMPTER
		if (master >= 0)
			utempter_remove_record(master);
#endif
	}

	if(eflg) {
		if (WIFSIGNALED(childstatus))
			exit(WTERMSIG(childstatus) + 0x80);
		else
			exit(WEXITSTATUS(childstatus));
	}
	exit(EXIT_SUCCESS);
}

void
getmaster() {
#if HAVE_LIBUTIL && HAVE_PTY_H
	tcgetattr(STDIN_FILENO, &tt);
	ioctl(STDIN_FILENO, TIOCGWINSZ, (char *)&win);
	if (openpty(&master, &slave, NULL, &tt, &win) < 0) {
		warn(_("openpty failed"));
		fail();
	}
#else
	char *pty, *bank, *cp;
	struct stat stb;

	pty = &line[strlen("/dev/ptyp")];
	for (bank = "pqrs"; *bank; bank++) {
		line[strlen("/dev/pty")] = *bank;
		*pty = '0';
		if (stat(line, &stb) < 0)
			break;
		for (cp = "0123456789abcdef"; *cp; cp++) {
			*pty = *cp;
			master = open(line, O_RDWR);
			if (master >= 0) {
				char *tp = &line[strlen("/dev/")];
=======
static void getmaster(struct script_control *ctl)
{
#if defined(HAVE_LIBUTIL) && defined(HAVE_PTY_H)
	int rc;

	ctl->isterm = isatty(STDIN_FILENO);

	if (ctl->isterm) {
		if (tcgetattr(STDIN_FILENO, &ctl->attrs) != 0)
			err(EXIT_FAILURE, _("failed to get terminal attributes"));
		ioctl(STDIN_FILENO, TIOCGWINSZ, (char *)&ctl->win);
		rc = openpty(&ctl->master, &ctl->slave, NULL, &ctl->attrs, &ctl->win);
	} else
		rc = openpty(&ctl->master, &ctl->slave, NULL, NULL, NULL);

	if (rc < 0) {
		warn(_("openpty failed"));
		fail(ctl);
	}
#else
	char *pty, *bank, *cp;

	ctl->isterm = isatty(STDIN_FILENO);

	pty = &ctl->line[strlen("/dev/ptyp")];
	for (bank = "pqrs"; *bank; bank++) {
		ctl->line[strlen("/dev/pty")] = *bank;
		*pty = '0';
		if (access(ctl->line, F_OK) != 0)
			break;
		for (cp = "0123456789abcdef"; *cp; cp++) {
			*pty = *cp;
			ctl->master = open(ctl->line, O_RDWR);
			if (ctl->master >= 0) {
				char *tp = &ctl->line[strlen("/dev/")];
>>>>>>> master-vanilla
				int ok;

				/* verify slave side is usable */
				*tp = 't';
<<<<<<< HEAD
				ok = access(line, R_OK|W_OK) == 0;
				*tp = 'p';
				if (ok) {
					tcgetattr(STDIN_FILENO, &tt);
					ioctl(STDIN_FILENO, TIOCGWINSZ,
						(char *)&win);
					return;
				}
				close(master);
				master = -1;
			}
		}
	}
	master = -1;
	warn(_("out of pty's"));
	fail();
#endif /* not HAVE_LIBUTIL */
}

void
getslave() {
#ifndef HAVE_LIBUTIL
	line[strlen("/dev/")] = 't';
	slave = open(line, O_RDWR);
	if (slave < 0) {
		warn(_("open failed: %s"), line);
		fail();
	}
	tcsetattr(slave, TCSANOW, &tt);
	ioctl(slave, TIOCSWINSZ, (char *)&win);
#endif
	setsid();
	ioctl(slave, TIOCSCTTY, 0);
=======
				ok = access(ctl->line, R_OK | W_OK) == 0;
				*tp = 'p';
				if (ok) {
					if (ctl->isterm) {
						tcgetattr(STDIN_FILENO, &ctl->attrs);
						ioctl(STDIN_FILENO, TIOCGWINSZ, (char *)&ctl->win);
					}
					return;
				}
				close(ctl->master);
				ctl->master = -1;
			}
		}
	}
	ctl->master = -1;
	warn(_("out of pty's"));
	fail(ctl);
#endif				/* not HAVE_LIBUTIL */

	DBG(IO, ul_debug("master fd: %d", ctl->master));
}

int main(int argc, char **argv)
{
	struct script_control ctl = {
#if !HAVE_LIBUTIL || !HAVE_PTY_H
		.line = "/dev/ptyXX",
#endif
		.master = -1,
		.poll_timeout = -1,
		0
	};
	int ch;

	enum { FORCE_OPTION = CHAR_MAX + 1 };

	static const struct option longopts[] = {
		{"append", no_argument, NULL, 'a'},
		{"command", required_argument, NULL, 'c'},
		{"return", no_argument, NULL, 'e'},
		{"flush", no_argument, NULL, 'f'},
		{"force", no_argument, NULL, FORCE_OPTION,},
		{"quiet", no_argument, NULL, 'q'},
		{"timing", optional_argument, NULL, 't'},
		{"version", no_argument, NULL, 'V'},
		{"help", no_argument, NULL, 'h'},
		{NULL, 0, NULL, 0}
	};

	setlocale(LC_ALL, "");
	/*
	 * script -t prints time delays as floating point numbers.  The example
	 * program (scriptreplay) that we provide to handle this timing output
	 * is a perl script, and does not handle numbers in locale format (not
	 * even when "use locale;" is added).  So, since these numbers are not
	 * for human consumption, it seems easiest to set LC_NUMERIC here.
	 */
	setlocale(LC_NUMERIC, "C");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);
	atexit(close_stdout);

	script_init_debug();

	while ((ch = getopt_long(argc, argv, "ac:efqt::Vh", longopts, NULL)) != -1)
		switch (ch) {
		case 'a':
			ctl.append = 1;
			break;
		case 'c':
			ctl.command = optarg;
			break;
		case 'e':
			ctl.rc_wanted = 1;
			break;
		case 'f':
			ctl.flush = 1;
			break;
		case FORCE_OPTION:
			ctl.force = 1;
			break;
		case 'q':
			ctl.quiet = 1;
			break;
		case 't':
			if (optarg)
				ctl.tname = optarg;
			ctl.timing = 1;
			break;
		case 'V':
			printf(UTIL_LINUX_VERSION);
			exit(EXIT_SUCCESS);
			break;
		case 'h':
			usage(stdout);
			break;
		default:
			usage(stderr);
		}
	argc -= optind;
	argv += optind;

	if (argc > 0)
		ctl.fname = argv[0];
	else {
		ctl.fname = DEFAULT_TYPESCRIPT_FILENAME;
		die_if_link(&ctl);
	}

	ctl.shell = getenv("SHELL");
	if (ctl.shell == NULL)
		ctl.shell = _PATH_BSHELL;

	getmaster(&ctl);
	if (!ctl.quiet)
		printf(_("Script started, file is %s\n"), ctl.fname);
	fixtty(&ctl);

#ifdef HAVE_LIBUTEMPTER
	utempter_add_record(ctl.master, NULL);
#endif
	/* setup signal handler */
	sigemptyset(&ctl.sigset);
	sigaddset(&ctl.sigset, SIGCHLD);
	sigaddset(&ctl.sigset, SIGWINCH);
	sigaddset(&ctl.sigset, SIGTERM);
	sigaddset(&ctl.sigset, SIGINT);
	sigaddset(&ctl.sigset, SIGQUIT);

	/* block signals used for signalfd() to prevent the signals being
	 * handled according to their default dispositions */
	sigprocmask(SIG_BLOCK, &ctl.sigset, &ctl.sigorg);

	if ((ctl.sigfd = signalfd(-1, &ctl.sigset, 0)) < 0)
		err(EXIT_FAILURE, _("cannot set signal handler"));

	DBG(SIGNAL, ul_debug("signal fd=%d", ctl.sigfd));

	fflush(stdout);
	ctl.child = fork();

	switch (ctl.child) {
	case -1: /* error */
		warn(_("fork failed"));
		fail(&ctl);
		break;
	case 0: /* child */
		do_shell(&ctl);
		break;
	default: /* parent */
		do_io(&ctl);
		break;
	}

	/* should not happen, all used functions are non-return */
	return EXIT_FAILURE;
>>>>>>> master-vanilla
}
