/* Original author unknown, may be "krishna balasub@cis.ohio-state.edu" */
/*
<<<<<<< HEAD

  Modified Sat Oct  9 10:55:28 1993 for 0.99.13

  Patches from Mike Jagdis (jaggy@purplet.demon.co.uk) applied Wed Feb
  8 12:12:21 1995 by faith@cs.unc.edu to print numeric uids if no
  passwd file entry.

  Patch from arnolds@ifns.de (Heinz-Ado Arnolds) applied Mon Jul 1
  19:30:41 1996 by janl@math.uio.no to add code missing in case PID:
  clauses.

  Patched to display the key field -- hy@picksys.com 12/18/96

  1999-02-22 Arkadiusz Mi∂kiewicz <misiek@pld.ORG.PL>
  - added Native Language Support

*/

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/shm.h>

#include "nls.h"
#include "c.h"

/*-------------------------------------------------------------------*/
/* SHM_DEST and SHM_LOCKED are defined in kernel headers,
   but inside #ifdef __KERNEL__ ... #endif */
#ifndef SHM_DEST
/* shm_mode upper byte flags */
#define SHM_DEST        01000   /* segment will be destroyed on last detach */
#define SHM_LOCKED      02000   /* segment will not be swapped */
#endif

/* For older kernels the same holds for the defines below */
#ifndef MSG_STAT
#define MSG_STAT	11
#define MSG_INFO	12
#endif

#ifndef SHM_STAT
#define SHM_STAT        13
#define SHM_INFO        14
struct shm_info {
     int   used_ids;
     ulong shm_tot; /* total allocated shm */
     ulong shm_rss; /* total resident shm */
     ulong shm_swp; /* total swapped shm */
     ulong swap_attempts;
     ulong swap_successes;
};
#endif

#ifndef SEM_STAT
#define SEM_STAT	18
#define SEM_INFO	19
#endif

/* Some versions of libc only define IPC_INFO when __USE_GNU is defined. */
#ifndef IPC_INFO
#define IPC_INFO        3
#endif
/*-------------------------------------------------------------------*/

/* The last arg of semctl is a union semun, but where is it defined?
   X/OPEN tells us to define it ourselves, but until recently
   Linux include files would also define it. */
#ifndef HAVE_UNION_SEMUN
/* according to X/OPEN we have to define it ourselves */
union semun {
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *__buf;
};
#endif

/* X/OPEN (Jan 1987) does not define fields key, seq in struct ipc_perm;
   glibc-1.09 has no support for sysv ipc.
   glibc 2 uses __key, __seq */
#if defined (__GLIBC__) && __GLIBC__ >= 2
#define KEY __key
#else
#define KEY key
#endif

#define LIMITS 1
#define STATUS 2
#define CREATOR 3
#define TIME 4
#define PID 5

void do_shm (char format);
void do_sem (char format);
void do_msg (char format);
void print_shm (int id);
void print_msg (int id);
void print_sem (int id);

static void
usage(int rc) {
	printf (_("Usage: %1$s [-asmq] [-t|-c|-l|-u|-p]\n"
	          "       %1$s [-s|-m|-q] -i id\n"
	          "       %1$s -h for help\n"),
		program_invocation_short_name);
	exit(rc);
}

static void
help (int rc) {
	printf (_("Usage: %1$s [resource]... [output-format]\n"
	          "       %1$s [resource] -i id\n\n"),
		program_invocation_short_name);

	printf (_("Provide information on IPC facilities for which you "
		  "have read access.\n\n"));

	printf (_(
	"    -h      display this help\n"
	"    -i id   print details on resource identified by id\n\n"));

	printf (_("Resource options:\n"
	"    -m      shared memory segments\n"
	"    -q      message queues\n"
	"    -s      semaphores\n"
	"    -a      all (default)\n\n"));

	printf (_("Output format:\n"
	"    -t      time\n"
	"    -p      pid\n"
	"    -c      creator\n"
	"    -l      limits\n"
	"    -u      summary\n"));
	exit(rc);
}

int
main (int argc, char **argv) {
	int opt, msg = 0, sem = 0, shm = 0, id=0, print=0;
	char format = 0;
	char options[] = "atcluphsmqi:";
=======
 * Modified Sat Oct  9 10:55:28 1993 for 0.99.13
 *
 * Patches from Mike Jagdis (jaggy@purplet.demon.co.uk) applied Wed Feb 8
 * 12:12:21 1995 by faith@cs.unc.edu to print numeric uids if no passwd file
 * entry.
 *
 * Patch from arnolds@ifns.de (Heinz-Ado Arnolds) applied Mon Jul 1 19:30:41
 * 1996 by janl@math.uio.no to add code missing in case PID: clauses.
 *
 * Patched to display the key field -- hy@picksys.com 12/18/96
 *
 * 1999-02-22 Arkadiusz Mi≈õkiewicz <misiek@pld.ORG.PL>
 * - added Native Language Support
 */

#include <errno.h>
#include <features.h>
#include <getopt.h>

#include "c.h"
#include "nls.h"
#include "closestream.h"

#include "ipcutils.h"

enum output_formats {
	NOTSPECIFIED,
	LIMITS,
	STATUS,
	CREATOR,
	TIME,
	PID
};
enum {
	OPT_HUMAN = CHAR_MAX + 1
};

static void do_shm (char format, int unit);
static void print_shm (int id, int unit);
static void do_sem (char format);
static void print_sem (int id);
static void do_msg (char format, int unit);
static void print_msg (int id, int unit);

/* we read time as int64_t from /proc, so cast... */
#define xctime(_x)	ctime((time_t *) (_x))

static void __attribute__ ((__noreturn__)) usage(FILE * out)
{
	fputs(USAGE_HEADER, out);
	fprintf(out, _(" %1$s [resource-option...] [output-option]\n"
		       " %1$s -m|-q|-s -i <id>\n"), program_invocation_short_name);

	fputs(USAGE_SEPARATOR, out);
	fputs(_("Show information on IPC facilities.\n"), out);

	fputs(USAGE_OPTIONS, out);
	fputs(_(" -i, --id <id>  print details on resource identified by <id>\n"), out);
	fputs(USAGE_HELP, out);
	fputs(USAGE_VERSION, out);

	fputs(USAGE_SEPARATOR, out);
	fputs(_("Resource options:\n"), out);
	fputs(_(" -m, --shmems      shared memory segments\n"), out);
	fputs(_(" -q, --queues      message queues\n"), out);
	fputs(_(" -s, --semaphores  semaphores\n"), out);
	fputs(_(" -a, --all         all (default)\n"), out);

	fputs(USAGE_SEPARATOR, out);
	fputs(_("Output options:\n"), out);
	fputs(_(" -t, --time        show attach, detach and change times\n"), out);
	fputs(_(" -p, --pid         show PIDs of creator and last operator\n"), out);
	fputs(_(" -c, --creator     show creator and owner\n"), out);
	fputs(_(" -l, --limits      show resource limits\n"), out);
	fputs(_(" -u, --summary     show status summary\n"), out);
	fputs(_("     --human       show sizes in human-readable format\n"), out);
	fputs(_(" -b, --bytes       show sizes in bytes\n"), out);
	fprintf(out, USAGE_MAN_TAIL("ipcs(1)"));

	exit(out == stderr ? EXIT_FAILURE : EXIT_SUCCESS);
}

int main (int argc, char **argv)
{
	int opt, msg = 0, shm = 0, sem = 0, id = 0, specific = 0;
	char format = NOTSPECIFIED;
	int unit = IPC_UNIT_DEFAULT;
	static const struct option longopts[] = {
		{"id", required_argument, NULL, 'i'},
		{"queues", no_argument, NULL, 'q'},
		{"shmems", no_argument, NULL, 'm'},
		{"semaphores", no_argument, NULL, 's'},
		{"all", no_argument, NULL, 'a'},
		{"time", no_argument, NULL, 't'},
		{"pid", no_argument, NULL, 'p'},
		{"creator", no_argument, NULL, 'c'},
		{"limits", no_argument, NULL, 'l'},
		{"summary", no_argument, NULL, 'u'},
		{"human", no_argument, NULL, OPT_HUMAN},
		{"bytes", no_argument, NULL, 'b'},
		{"version", no_argument, NULL, 'V'},
		{"help", no_argument, NULL, 'h'},
		{NULL, 0, NULL, 0}
	};
	char options[] = "i:qmsatpclubVh";
>>>>>>> master-vanilla

	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);
<<<<<<< HEAD

	while ((opt = getopt (argc, argv, options)) != -1) {
		switch (opt) {
		case 'i':
			id = atoi (optarg);
			print = 1;
=======
	atexit(close_stdout);

	while ((opt = getopt_long(argc, argv, options, longopts, NULL)) != -1) {
		switch (opt) {
		case 'i':
			id = atoi (optarg);
			specific = 1;
>>>>>>> master-vanilla
			break;
		case 'a':
			msg = shm = sem = 1;
			break;
		case 'q':
			msg = 1;
			break;
<<<<<<< HEAD
		case 's':
			sem = 1;
			break;
		case 'm':
			shm = 1;
			break;
=======
		case 'm':
			shm = 1;
			break;
		case 's':
			sem = 1;
			break;
>>>>>>> master-vanilla
		case 't':
			format = TIME;
			break;
		case 'c':
			format = CREATOR;
			break;
		case 'p':
			format = PID;
			break;
		case 'l':
			format = LIMITS;
			break;
		case 'u':
			format = STATUS;
			break;
<<<<<<< HEAD
		case 'h':
			help(EXIT_SUCCESS);
		case '?':
			usage(EXIT_SUCCESS);
		}
	}


	if  (print) {
	        if (shm)
			print_shm (id);
		if (sem)
			print_sem (id);
		if (msg)
			print_msg (id);
		if (!shm && !sem && !msg )
			usage (EXIT_FAILURE);
	} else {
		if ( !shm && !msg && !sem)
			msg = sem = shm = 1;
		printf ("\n");

		if (shm) {
			do_shm (format);
=======
		case OPT_HUMAN:
			unit = IPC_UNIT_HUMAN;
			break;
		case 'b':
			unit = IPC_UNIT_BYTES;
			break;
		case 'h':
			usage(stdout);
		case 'V':
			printf(UTIL_LINUX_VERSION);
			return EXIT_SUCCESS;
		default:
			usage(stderr);
		}
	}

	if (specific && (msg + shm + sem != 1))
		errx (EXIT_FAILURE,
		      _("when using an ID, a single resource must be specified"));
	if (specific) {
		if (msg)
			print_msg (id, unit);
		if (shm)
			print_shm (id, unit);
		if (sem)
			print_sem (id);
	} else {
		if (!msg && !shm && !sem)
			msg = shm = sem = 1;
		printf ("\n");
		if (msg) {
			do_msg (format, unit);
			printf ("\n");
		}
		if (shm) {
			do_shm (format, unit);
>>>>>>> master-vanilla
			printf ("\n");
		}
		if (sem) {
			do_sem (format);
			printf ("\n");
		}
<<<<<<< HEAD
		if (msg) {
			do_msg (format);
			printf ("\n");
		}
=======
>>>>>>> master-vanilla
	}
	return EXIT_SUCCESS;
}

<<<<<<< HEAD

static void
print_perms (int id, struct ipc_perm *ipcp) {
	struct passwd *pw;
	struct group *gr;

	printf ("%-10d %-10o", id, ipcp->mode & 0777);

	if ((pw = getpwuid(ipcp->cuid)))
		printf(" %-10s", pw->pw_name);
	else
		printf(" %-10u", ipcp->cuid);
	if ((gr = getgrgid(ipcp->cgid)))
		printf(" %-10s", gr->gr_name);
	else
		printf(" %-10u", ipcp->cgid);

	if ((pw = getpwuid(ipcp->uid)))
		printf(" %-10s", pw->pw_name);
	else
		printf(" %-10u", ipcp->uid);
	if ((gr = getgrgid(ipcp->gid)))
		printf(" %-10s\n", gr->gr_name);
	else
		printf(" %-10u\n", ipcp->gid);
}


void do_shm (char format)
{
	int maxid, shmid, id;
	struct shmid_ds shmseg;
	struct shm_info shm_info;
	struct shminfo shminfo;
	struct ipc_perm *ipcp = &shmseg.shm_perm;
	struct passwd *pw;

	maxid = shmctl (0, SHM_INFO, (struct shmid_ds *) (void *) &shm_info);
	if (maxid < 0) {
		printf (_("kernel not configured for shared memory\n"));
		return;
	}

	switch (format) {
	case LIMITS:
		printf (_("------ Shared Memory Limits --------\n"));
		if ((shmctl (0, IPC_INFO, (struct shmid_ds *) (void *) &shminfo)) < 0 )
			return;
		/* glibc 2.1.3 and all earlier libc's have ints as fields
		   of struct shminfo; glibc 2.1.91 has unsigned long; ach */
		printf (_("max number of segments = %lu\n"),
			(unsigned long) shminfo.shmmni);
		printf (_("max seg size (kbytes) = %lu\n"),
			(unsigned long) (shminfo.shmmax >> 10));
		printf (_("max total shared memory (kbytes) = %llu\n"),
			getpagesize() / 1024 * (unsigned long long) shminfo.shmall);
		printf (_("min seg size (bytes) = %lu\n"),
			(unsigned long) shminfo.shmmin);
		return;

	case STATUS:
		printf (_("------ Shared Memory Status --------\n"));
		/* TRANSLATORS: This output format is mantained for backward compatibility
		   as ipcs is used in scripts. For consistency with the rest, the translated
		   form can follow this model:

		   "segments allocated = %d\n"
		   "pages allocated = %ld\n"
		   "pages resident = %ld\n"
		   "pages swapped = %ld\n"
		   "swap performance = %ld attempts, %ld successes\n"
		*/
		printf (_("segments allocated %d\n"
		          "pages allocated %ld\n"
		          "pages resident  %ld\n"
		          "pages swapped   %ld\n"
		          "Swap performance: %ld attempts\t %ld successes\n"),
			shm_info.used_ids,
			shm_info.shm_tot,
			shm_info.shm_rss,
			shm_info.shm_swp,
			shm_info.swap_attempts, shm_info.swap_successes);
		return;

=======
static void do_shm (char format, int unit)
{
	struct passwd *pw;
	struct shm_data *shmds, *shmdsp;

	switch (format) {
	case LIMITS:
	{
		struct ipc_limits lim;

		printf (_("------ Shared Memory Limits --------\n"));
		if (ipc_shm_get_limits(&lim))
			return;
		printf (_("max number of segments = %ju\n"), lim.shmmni);
		ipc_print_size(unit == IPC_UNIT_DEFAULT ? IPC_UNIT_KB : unit,
			       _("max seg size"), lim.shmmax, "\n", 0);
		ipc_print_size(unit == IPC_UNIT_DEFAULT ? IPC_UNIT_KB : unit,
			       _("max total shared memory"),
			       (uint64_t) lim.shmall * getpagesize(), "\n", 0);
		ipc_print_size(unit == IPC_UNIT_DEFAULT ? IPC_UNIT_BYTES : unit,
			       _("min seg size"), lim.shmmin, "\n", 0);
		return;
	}
	case STATUS:
	{
		int maxid;
		struct shmid_ds shmbuf;
		struct shm_info *shm_info;

		maxid = shmctl (0, SHM_INFO, &shmbuf);
		shm_info =  (struct shm_info *) &shmbuf;
		if (maxid < 0) {
			printf (_("kernel not configured for shared memory\n"));
			return;
		}

		printf (_("------ Shared Memory Status --------\n"));
		/*
		 * TRANSLATORS: This output format is maintained for backward
		 * compatibility as ipcs is used in scripts. For consistency
		 * with the rest, the translated form can follow this model:
		 *
		 * "segments allocated = %d\n"
		 * "pages allocated = %ld\n"
		 * "pages resident = %ld\n"
		 * "pages swapped = %ld\n"
		 * "swap performance = %ld attempts, %ld successes\n"
		 */
		printf (_("segments allocated %d\n"
			  "pages allocated %ld\n"
			  "pages resident  %ld\n"
			  "pages swapped   %ld\n"
			  "Swap performance: %ld attempts\t %ld successes\n"),
			shm_info->used_ids,
			shm_info->shm_tot,
			shm_info->shm_rss,
			shm_info->shm_swp,
			shm_info->swap_attempts, shm_info->swap_successes);
		return;
	}

	/*
	 * Headers only
	 */
>>>>>>> master-vanilla
	case CREATOR:
		printf (_("------ Shared Memory Segment Creators/Owners --------\n"));
		printf ("%-10s %-10s %-10s %-10s %-10s %-10s\n",
			_("shmid"),_("perms"),_("cuid"),_("cgid"),_("uid"),_("gid"));
		break;

	case TIME:
		printf (_("------ Shared Memory Attach/Detach/Change Times --------\n"));
		printf ("%-10s %-10s %-20s %-20s %-20s\n",
			_("shmid"),_("owner"),_("attached"),_("detached"),
			_("changed"));
		break;

	case PID:
		printf (_("------ Shared Memory Creator/Last-op PIDs --------\n"));
		printf ("%-10s %-10s %-10s %-10s\n",
			_("shmid"),_("owner"),_("cpid"),_("lpid"));
		break;

	default:
		printf (_("------ Shared Memory Segments --------\n"));
		printf ("%-10s %-10s %-10s %-10s %-10s %-10s %-12s\n",
<<<<<<< HEAD
			_("key"),_("shmid"),_("owner"),_("perms"),_("bytes"),
=======
			_("key"),_("shmid"),_("owner"),_("perms"),
			unit == IPC_UNIT_HUMAN ? _("size") : _("bytes"),
>>>>>>> master-vanilla
			_("nattch"),_("status"));
		break;
	}

<<<<<<< HEAD
	for (id = 0; id <= maxid; id++) {
		shmid = shmctl (id, SHM_STAT, &shmseg);
		if (shmid < 0)
			continue;
		if (format == CREATOR)  {
			print_perms (shmid, ipcp);
			continue;
		}
		pw = getpwuid(ipcp->uid);
		switch (format) {
		case TIME:
			if (pw)
				printf ("%-10d %-10.10s", shmid, pw->pw_name);
			else
				printf ("%-10d %-10u", shmid, ipcp->uid);
			/* ctime uses static buffer: use separate calls */
			printf(" %-20.16s", shmseg.shm_atime
			       ? ctime(&shmseg.shm_atime) + 4 : _("Not set"));
			printf(" %-20.16s", shmseg.shm_dtime
			       ? ctime(&shmseg.shm_dtime) + 4 : _("Not set"));
			printf(" %-20.16s\n", shmseg.shm_ctime
			       ? ctime(&shmseg.shm_ctime) + 4 : _("Not set"));
			break;
		case PID:
			if (pw)
				printf ("%-10d %-10.10s", shmid, pw->pw_name);
			else
				printf ("%-10d %-10u", shmid, ipcp->uid);
			printf (" %-10d %-10d\n",
				shmseg.shm_cpid, shmseg.shm_lpid);
			break;

		default:
		        printf("0x%08x ",ipcp->KEY );
			if (pw)
				printf ("%-10d %-10.10s", shmid, pw->pw_name);
			else
				printf ("%-10d %-10u", shmid, ipcp->uid);
			printf (" %-10o %-10lu %-10ld %-6s %-6s\n",
				ipcp->mode & 0777,
				/*
				 * earlier: int, Austin has size_t
				 */
				(unsigned long) shmseg.shm_segsz,
				/*
				 * glibc-2.1.3 and earlier has unsigned short;
				 * Austin has shmatt_t
				 */
				(long) shmseg.shm_nattch,
				ipcp->mode & SHM_DEST ? _("dest") : " ",
				ipcp->mode & SHM_LOCKED ? _("locked") : " ");
			break;
		}
	}
	return;
}


void do_sem (char format)
{
	int maxid, semid, id;
	struct semid_ds semary;
	struct seminfo seminfo;
	struct ipc_perm *ipcp = &semary.sem_perm;
	struct passwd *pw;
	union semun arg;

	arg.array = (ushort *)  (void *) &seminfo;
	maxid = semctl (0, 0, SEM_INFO, arg);
	if (maxid < 0) {
		printf (_("kernel not configured for semaphores\n"));
		return;
	}

	switch (format) {
	case LIMITS:
		printf (_("------ Semaphore Limits --------\n"));
		arg.array = (ushort *) (void *) &seminfo; /* damn union */
		if ((semctl (0, 0, IPC_INFO, arg)) < 0 )
			return;
		printf (_("max number of arrays = %d\n"), seminfo.semmni);
		printf (_("max semaphores per array = %d\n"), seminfo.semmsl);
		printf (_("max semaphores system wide = %d\n"), seminfo.semmns);
		printf (_("max ops per semop call = %d\n"), seminfo.semopm);
		printf (_("semaphore max value = %d\n"), seminfo.semvmx);
		return;

	case STATUS:
=======
	/*
	 * Print data
	 */
	if (ipc_shm_get_info(-1, &shmds) < 1)
		return;

	for (shmdsp = shmds; shmdsp->next != NULL; shmdsp = shmdsp->next) {
		if (format == CREATOR)  {
			ipc_print_perms(stdout, &shmdsp->shm_perm);
			continue;
		}
		pw = getpwuid(shmdsp->shm_perm.uid);
		switch (format) {
		case TIME:
			if (pw)
				printf ("%-10d %-10.10s", shmdsp->shm_perm.id, pw->pw_name);
			else
				printf ("%-10d %-10u", shmdsp->shm_perm.id, shmdsp->shm_perm.uid);
			/* ctime uses static buffer: use separate calls */
			printf(" %-20.16s", shmdsp->shm_atim
			       ? xctime(&shmdsp->shm_atim) + 4 : _("Not set"));
			printf(" %-20.16s", shmdsp->shm_dtim
			       ? xctime(&shmdsp->shm_dtim) + 4 : _("Not set"));
			printf(" %-20.16s\n", shmdsp->shm_ctim
			       ? xctime(&shmdsp->shm_ctim) + 4 : _("Not set"));
			break;
		case PID:
			if (pw)
				printf ("%-10d %-10.10s", shmdsp->shm_perm.id, pw->pw_name);
			else
				printf ("%-10d %-10u", shmdsp->shm_perm.id, shmdsp->shm_perm.uid);
			printf (" %-10u %-10u\n",
				shmdsp->shm_cprid, shmdsp->shm_lprid);
			break;

		default:
			printf("0x%08x ", shmdsp->shm_perm.key);
			if (pw)
				printf ("%-10d %-10.10s", shmdsp->shm_perm.id, pw->pw_name);
			else
				printf ("%-10d %-10u", shmdsp->shm_perm.id, shmdsp->shm_perm.uid);
			printf (" %-10o ", shmdsp->shm_perm.mode & 0777);

			if (unit == IPC_UNIT_HUMAN)
				ipc_print_size(unit, NULL, shmdsp->shm_segsz, "    ", 6);
			else
				ipc_print_size(unit, NULL, shmdsp->shm_segsz, NULL, -10);

			printf (" %-10ju %-6s %-6s\n",
				shmdsp->shm_nattch,
				shmdsp->shm_perm.mode & SHM_DEST ? _("dest") : " ",
				shmdsp->shm_perm.mode & SHM_LOCKED ? _("locked") : " ");
			break;
		}
	}

	ipc_shm_free_info(shmds);
	return;
}

static void do_sem (char format)
{
	struct passwd *pw;
	struct sem_data *semds, *semdsp;

	switch (format) {
	case LIMITS:
	{
		struct ipc_limits lim;

		printf (_("------ Semaphore Limits --------\n"));
		if (ipc_sem_get_limits(&lim))
			return;
		printf (_("max number of arrays = %d\n"), lim.semmni);
		printf (_("max semaphores per array = %d\n"), lim.semmsl);
		printf (_("max semaphores system wide = %d\n"), lim.semmns);
		printf (_("max ops per semop call = %d\n"), lim.semopm);
		printf (_("semaphore max value = %u\n"), lim.semvmx);
		return;
	}
	case STATUS:
	{
		struct seminfo seminfo;
		union semun arg;
		arg.array = (ushort *)  (void *) &seminfo;
		if (semctl (0, 0, SEM_INFO, arg) < 0) {
			printf (_("kernel not configured for semaphores\n"));
			return;
		}
>>>>>>> master-vanilla
		printf (_("------ Semaphore Status --------\n"));
		printf (_("used arrays = %d\n"), seminfo.semusz);
		printf (_("allocated semaphores = %d\n"), seminfo.semaem);
		return;
<<<<<<< HEAD
=======
	}
>>>>>>> master-vanilla

	case CREATOR:
		printf (_("------ Semaphore Arrays Creators/Owners --------\n"));
		printf ("%-10s %-10s %-10s %-10s %-10s %-10s\n",
			_("semid"),_("perms"),_("cuid"),_("cgid"),_("uid"),_("gid"));
		break;

	case TIME:
		printf (_("------ Semaphore Operation/Change Times --------\n"));
		printf ("%-8s %-10s %-26.24s %-26.24s\n",
			_("semid"),_("owner"),_("last-op"),_("last-changed"));
		break;

	case PID:
		break;

	default:
		printf (_("------ Semaphore Arrays --------\n"));
		printf ("%-10s %-10s %-10s %-10s %-10s\n",
			_("key"),_("semid"),_("owner"),_("perms"),_("nsems"));
		break;
	}

<<<<<<< HEAD
	for (id = 0; id <= maxid; id++) {
		arg.buf = (struct semid_ds *) &semary;
		semid = semctl (id, 0, SEM_STAT, arg);
		if (semid < 0)
			continue;
		if (format == CREATOR)  {
			print_perms (semid, ipcp);
			continue;
		}
		pw = getpwuid(ipcp->uid);
		switch (format) {
		case TIME:
			if (pw)
				printf ("%-8d %-10.10s", semid, pw->pw_name);
			else
				printf ("%-8d %-10u", semid, ipcp->uid);
			printf ("  %-26.24s", semary.sem_otime
				? ctime(&semary.sem_otime) : _("Not set"));
			printf (" %-26.24s\n", semary.sem_ctime
				? ctime(&semary.sem_ctime) : _("Not set"));
=======
	/*
	 * Print data
	 */
	if (ipc_sem_get_info(-1, &semds) < 1)
		return;

	for (semdsp = semds; semdsp->next != NULL; semdsp = semdsp->next) {
		if (format == CREATOR)  {
			ipc_print_perms(stdout, &semdsp->sem_perm);
			continue;
		}
		pw = getpwuid(semdsp->sem_perm.uid);
		switch (format) {
		case TIME:
			if (pw)
				printf ("%-8d %-10.10s", semdsp->sem_perm.id, pw->pw_name);
			else
				printf ("%-8d %-10u", semdsp->sem_perm.id, semdsp->sem_perm.uid);
			printf ("  %-26.24s", semdsp->sem_otime
				? xctime(&semdsp->sem_otime) : _("Not set"));
			printf (" %-26.24s\n", semdsp->sem_ctime
				? xctime( &semdsp->sem_ctime) : _("Not set"));
>>>>>>> master-vanilla
			break;
		case PID:
			break;

		default:
<<<<<<< HEAD
		        printf("0x%08x ", ipcp->KEY);
			if (pw)
				printf ("%-10d %-10.10s", semid, pw->pw_name);
			else
				printf ("%-10d %-10u", semid, ipcp->uid);
			printf (" %-10o %-10ld\n",
				ipcp->mode & 0777,
				/*
				 * glibc-2.1.3 and earlier has unsigned short;
				 * glibc-2.1.91 has variation between
				 * unsigned short and unsigned long
				 * Austin prescribes unsigned short.
				 */
				(long) semary.sem_nsems);
			break;
		}
	}
}


void do_msg (char format)
{
	int maxid, msqid, id;
	struct msqid_ds msgque;
	struct msginfo msginfo;
	struct ipc_perm *ipcp = &msgque.msg_perm;
	struct passwd *pw;

	maxid = msgctl (0, MSG_INFO, (struct msqid_ds *) (void *) &msginfo);
	if (maxid < 0) {
		printf (_("kernel not configured for message queues\n"));
		return;
	}

	switch (format) {
	case LIMITS:
		if ((msgctl (0, IPC_INFO, (struct msqid_ds *) (void *) &msginfo)) < 0 )
			return;
		printf (_("------ Messages Limits --------\n"));
		printf (_("max queues system wide = %d\n"), msginfo.msgmni);
		printf (_("max size of message (bytes) = %d\n"), msginfo.msgmax);
		printf (_("default max size of queue (bytes) = %d\n"), msginfo.msgmnb);
		return;

	case STATUS:
=======
			printf("0x%08x ", semdsp->sem_perm.key);
			if (pw)
				printf ("%-10d %-10.10s", semdsp->sem_perm.id, pw->pw_name);
			else
				printf ("%-10d %-10u", semdsp->sem_perm.id, semdsp->sem_perm.uid);
			printf (" %-10o %-10ju\n",
				semdsp->sem_perm.mode & 0777,
				semdsp->sem_nsems);
			break;
		}
	}

	ipc_sem_free_info(semds);
	return;
}

static void do_msg (char format, int unit)
{
	struct passwd *pw;
	struct msg_data *msgds, *msgdsp;

	switch (format) {
	case LIMITS:
	{
		struct ipc_limits lim;

		if (ipc_msg_get_limits(&lim))
			return;
		printf (_("------ Messages Limits --------\n"));
		printf (_("max queues system wide = %d\n"), lim.msgmni);
		ipc_print_size(unit == IPC_UNIT_DEFAULT ? IPC_UNIT_BYTES : unit,
			       _("max size of message"), lim.msgmax, "\n", 0);
		ipc_print_size(unit == IPC_UNIT_DEFAULT ? IPC_UNIT_BYTES : unit,
			       _("default max size of queue"), lim.msgmnb, "\n", 0);
		return;
	}
	case STATUS:
	{
		struct msginfo msginfo;
		if (msgctl (0, MSG_INFO, (struct msqid_ds *) (void *) &msginfo) < 0) {
			printf (_("kernel not configured for message queues\n"));
			return;
		}
>>>>>>> master-vanilla
		printf (_("------ Messages Status --------\n"));
#ifndef __FreeBSD_kernel__
		printf (_("allocated queues = %d\n"), msginfo.msgpool);
		printf (_("used headers = %d\n"), msginfo.msgmap);
#endif
<<<<<<< HEAD
		printf (_("used space = %d bytes\n"), msginfo.msgtql);
		return;

=======
		ipc_print_size(unit, _("used space"), msginfo.msgtql,
			       unit == IPC_UNIT_DEFAULT ? _(" bytes\n") : "\n", 0);
		return;
	}
>>>>>>> master-vanilla
	case CREATOR:
		printf (_("------ Message Queues Creators/Owners --------\n"));
		printf ("%-10s %-10s %-10s %-10s %-10s %-10s\n",
			_("msqid"),_("perms"),_("cuid"),_("cgid"),_("uid"),_("gid"));
		break;

	case TIME:
		printf (_("------ Message Queues Send/Recv/Change Times --------\n"));
		printf ("%-8s %-10s %-20s %-20s %-20s\n",
			_("msqid"),_("owner"),_("send"),_("recv"),_("change"));
		break;

	case PID:
		printf (_("------ Message Queues PIDs --------\n"));
		printf ("%-10s %-10s %-10s %-10s\n",
			_("msqid"),_("owner"),_("lspid"),_("lrpid"));
		break;

	default:
		printf (_("------ Message Queues --------\n"));
		printf ("%-10s %-10s %-10s %-10s %-12s %-12s\n",
			_("key"), _("msqid"), _("owner"), _("perms"),
<<<<<<< HEAD
			_("used-bytes"), _("messages"));
		break;
	}

	for (id = 0; id <= maxid; id++) {
		msqid = msgctl (id, MSG_STAT, &msgque);
		if (msqid < 0)
			continue;
		if (format == CREATOR)  {
			print_perms (msqid, ipcp);
			continue;
		}
		pw = getpwuid(ipcp->uid);
		switch (format) {
		case TIME:
			if (pw)
				printf ("%-8d %-10.10s", msqid, pw->pw_name);
			else
				printf ("%-8d %-10u", msqid, ipcp->uid);
			printf (" %-20.16s", msgque.msg_stime
				? ctime(&msgque.msg_stime) + 4 : _("Not set"));
			printf (" %-20.16s", msgque.msg_rtime
				? ctime(&msgque.msg_rtime) + 4 : _("Not set"));
			printf (" %-20.16s\n", msgque.msg_ctime
				? ctime(&msgque.msg_ctime) + 4 : _("Not set"));
			break;
		case PID:
			if (pw)
				printf ("%-8d %-10.10s", msqid, pw->pw_name);
			else
				printf ("%-8d %-10u", msqid, ipcp->uid);
			printf ("  %5d     %5d\n",
				msgque.msg_lspid, msgque.msg_lrpid);
			break;

		default:
		        printf( "0x%08x ",ipcp->KEY );
			if (pw)
				printf ("%-10d %-10.10s", msqid, pw->pw_name);
			else
				printf ("%-10d %-10u", msqid, ipcp->uid);
			printf (" %-10o %-12ld %-12ld\n",
				ipcp->mode & 0777,
				/*
				 * glibc-2.1.3 and earlier has unsigned short;
				 * glibc-2.1.91 has variation between
				 * unsigned short, unsigned long
				 * Austin has msgqnum_t
				 */
				(long) msgque.msg_cbytes,
				(long) msgque.msg_qnum);
			break;
		}
	}
	return;
}


void print_shm (int shmid)
{
	struct shmid_ds shmds;
	struct ipc_perm *ipcp = &shmds.shm_perm;

	if (shmctl (shmid, IPC_STAT, &shmds) == -1)
		err(EXIT_FAILURE, _("shmctl failed"));

	printf (_("\nShared memory Segment shmid=%d\n"), shmid);
	printf (_("uid=%u\tgid=%u\tcuid=%u\tcgid=%u\n"),
		ipcp->uid, ipcp->gid, ipcp->cuid, ipcp->cgid);
	printf (_("mode=%#o\taccess_perms=%#o\n"),
		ipcp->mode, ipcp->mode & 0777);
	printf (_("bytes=%lu\tlpid=%d\tcpid=%d\tnattch=%ld\n"),
		(unsigned long) shmds.shm_segsz, shmds.shm_lpid, shmds.shm_cpid,
		(long) shmds.shm_nattch);
	printf (_("att_time=%-26.24s\n"),
		shmds.shm_atime ? ctime (&shmds.shm_atime) : _("Not set"));
	printf (_("det_time=%-26.24s\n"),
		shmds.shm_dtime ? ctime (&shmds.shm_dtime) : _("Not set"));
	printf (_("change_time=%-26.24s\n"), ctime (&shmds.shm_ctime));
	printf ("\n");
	return;
}


void print_msg (int msqid)
{
	struct msqid_ds buf;
	struct ipc_perm *ipcp = &buf.msg_perm;

	if (msgctl (msqid, IPC_STAT, &buf) == -1)
		err(EXIT_FAILURE, _("msgctl failed"));

	printf (_("\nMessage Queue msqid=%d\n"), msqid);
	printf (_("uid=%u\tgid=%u\tcuid=%u\tcgid=%u\tmode=%#o\n"),
		ipcp->uid, ipcp->gid, ipcp->cuid, ipcp->cgid, ipcp->mode);
	printf (_("cbytes=%ld\tqbytes=%ld\tqnum=%ld\tlspid=%d\tlrpid=%d\n"),
		/*
		 * glibc-2.1.3 and earlier has unsigned short;
		 * glibc-2.1.91 has variation between
		 * unsigned short, unsigned long
		 * Austin has msgqnum_t (for msg_qbytes)
		 */
		(long) buf.msg_cbytes, (long) buf.msg_qbytes,
		(long) buf.msg_qnum, buf.msg_lspid, buf.msg_lrpid);
	printf (_("send_time=%-26.24s\n"),
		buf.msg_stime ? ctime (&buf.msg_stime) : _("Not set"));
	printf (_("rcv_time=%-26.24s\n"),
		buf.msg_rtime ? ctime (&buf.msg_rtime) : _("Not set"));
	printf (_("change_time=%-26.24s\n"),
		buf.msg_ctime ? ctime (&buf.msg_ctime) : _("Not set"));
	printf ("\n");
	return;
}

void print_sem (int semid)
{
	struct semid_ds semds;
	struct ipc_perm *ipcp = &semds.sem_perm;
	union semun arg;
	size_t i;

	arg.buf = &semds;
	if (semctl (semid, 0, IPC_STAT, arg) < 0)
		err(EXIT_FAILURE, _("semctl failed"));

	printf (_("\nSemaphore Array semid=%d\n"), semid);
	printf (_("uid=%u\t gid=%u\t cuid=%u\t cgid=%u\n"),
		ipcp->uid, ipcp->gid, ipcp->cuid, ipcp->cgid);
	printf (_("mode=%#o, access_perms=%#o\n"),
		ipcp->mode, ipcp->mode & 0777);
	printf (_("nsems = %ld\n"), (long) semds.sem_nsems);
	printf (_("otime = %-26.24s\n"),
		semds.sem_otime ? ctime (&semds.sem_otime) : _("Not set"));
	printf (_("ctime = %-26.24s\n"), ctime (&semds.sem_ctime));

	printf ("%-10s %-10s %-10s %-10s %-10s\n",
		_("semnum"),_("value"),_("ncount"),_("zcount"),_("pid"));
	arg.val = 0;
	for (i=0; i< semds.sem_nsems; i++) {
		int val, ncnt, zcnt, pid;
		val = semctl (semid, i, GETVAL, arg);
		ncnt = semctl (semid, i, GETNCNT, arg);
		zcnt = semctl (semid, i, GETZCNT, arg);
		pid = semctl (semid, i, GETPID, arg);
		if (val < 0 || ncnt < 0 || zcnt < 0 || pid < 0)
			err(EXIT_FAILURE, _("semctl failed"));

		printf ("%-10zd %-10d %-10d %-10d %-10d\n",
			i, val, ncnt, zcnt, pid);
	}
	printf ("\n");
	return;
=======
			unit == IPC_UNIT_HUMAN ? _("size") : _("used-bytes"),
			_("messages"));
		break;
	}

	/*
	 * Print data
	 */
	if (ipc_msg_get_info(-1, &msgds) < 1)
		return;

	for (msgdsp = msgds; msgdsp->next != NULL; msgdsp = msgdsp->next) {
		if (format == CREATOR) {
			ipc_print_perms(stdout, &msgdsp->msg_perm);
			continue;
		}
		pw = getpwuid(msgdsp->msg_perm.uid);
		switch (format) {
		case TIME:
			if (pw)
				printf ("%-8d %-10.10s", msgdsp->msg_perm.id, pw->pw_name);
			else
				printf ("%-8d %-10u", msgdsp->msg_perm.id, msgdsp->msg_perm.uid);
			printf (" %-20.16s", msgdsp->q_stime
				? xctime(&msgdsp->q_stime) + 4 : _("Not set"));
			printf (" %-20.16s", msgdsp->q_rtime
				? xctime(&msgdsp->q_rtime) + 4 : _("Not set"));
			printf (" %-20.16s\n", msgdsp->q_ctime
				? xctime(&msgdsp->q_ctime) + 4 : _("Not set"));
			break;
		case PID:
			if (pw)
				printf ("%-8d %-10.10s", msgdsp->msg_perm.id, pw->pw_name);
			else
				printf ("%-8d %-10u", msgdsp->msg_perm.id, msgdsp->msg_perm.uid);
			printf ("  %5d     %5d\n",
				msgdsp->q_lspid, msgdsp->q_lrpid);
			break;

		default:
			printf( "0x%08x ",msgdsp->msg_perm.key );
			if (pw)
				printf ("%-10d %-10.10s", msgdsp->msg_perm.id, pw->pw_name);
			else
				printf ("%-10d %-10u", msgdsp->msg_perm.id, msgdsp->msg_perm.uid);
			printf (" %-10o ", msgdsp->msg_perm.mode & 0777);

			if (unit == IPC_UNIT_HUMAN)
				ipc_print_size(unit, NULL, msgdsp->q_cbytes, "      ", 6);
			else
				ipc_print_size(unit, NULL, msgdsp->q_cbytes, NULL, -12);

			printf (" %-12ju\n", msgdsp->q_qnum);
			break;
		}
	}

	ipc_msg_free_info(msgds);
	return;
}

static void print_shm(int shmid, int unit)
{
	struct shm_data *shmdata;

	if (ipc_shm_get_info(shmid, &shmdata) < 1) {
		warnx(_("id %d not found"), shmid);
		return;
	}

	printf(_("\nShared memory Segment shmid=%d\n"), shmid);
	printf(_("uid=%u\tgid=%u\tcuid=%u\tcgid=%u\n"),
	       shmdata->shm_perm.uid, shmdata->shm_perm.uid,
	       shmdata->shm_perm.cuid, shmdata->shm_perm.cgid);
	printf(_("mode=%#o\taccess_perms=%#o\n"), shmdata->shm_perm.mode,
	       shmdata->shm_perm.mode & 0777);
	ipc_print_size(unit, unit == IPC_UNIT_HUMAN ? _("size=") : _("bytes="),
		       shmdata->shm_segsz, "\t", 0);
	printf(_("lpid=%u\tcpid=%u\tnattch=%jd\n"),
	       shmdata->shm_lprid, shmdata->shm_cprid,
	       shmdata->shm_nattch);
	printf(_("att_time=%-26.24s\n"),
	       shmdata->shm_atim ? xctime(&(shmdata->shm_atim)) : _("Not set"));
	printf(_("det_time=%-26.24s\n"),
	       shmdata->shm_dtim ? xctime(&shmdata->shm_dtim) : _("Not set"));
	printf(_("change_time=%-26.24s\n"), xctime(&shmdata->shm_ctim));
	printf("\n");

	ipc_shm_free_info(shmdata);
}

void print_msg(int msgid, int unit)
{
	struct msg_data *msgdata;

	if (ipc_msg_get_info(msgid, &msgdata) < 1) {
		warnx(_("id %d not found"), msgid);
		return;
	}

	printf(_("\nMessage Queue msqid=%d\n"), msgid);
	printf(_("uid=%u\tgid=%u\tcuid=%u\tcgid=%u\tmode=%#o\n"),
	       msgdata->msg_perm.uid, msgdata->msg_perm.uid,
	       msgdata->msg_perm.cuid, msgdata->msg_perm.cgid,
	       msgdata->msg_perm.mode);
	ipc_print_size(unit, unit == IPC_UNIT_HUMAN ? _("csize=") : _("cbytes="),
		       msgdata->q_cbytes, "\t", 0);
	ipc_print_size(unit, unit == IPC_UNIT_HUMAN ? _("qsize=") : _("qbytes="),
		       msgdata->q_qbytes, "\t", 0);
	printf("qnum=%jd\tlspid=%d\tlrpid=%d\n",
	       msgdata->q_qnum,
	       msgdata->q_lspid, msgdata->q_lrpid);
	printf(_("send_time=%-26.24s\n"),
	       msgdata->q_stime ? xctime(&msgdata->q_stime) : _("Not set"));
	printf(_("rcv_time=%-26.24s\n"),
	       msgdata->q_rtime ? xctime(&msgdata->q_rtime) : _("Not set"));
	printf(_("change_time=%-26.24s\n"),
	       msgdata->q_ctime ? xctime(&msgdata->q_ctime) : _("Not set"));
	printf("\n");

	ipc_msg_free_info(msgdata);
}

static void print_sem(int semid)
{
	struct sem_data *semdata;
	size_t i;

	if (ipc_sem_get_info(semid, &semdata) < 1) {
		warnx(_("id %d not found"), semid);
		return;
	}

	printf(_("\nSemaphore Array semid=%d\n"), semid);
	printf(_("uid=%u\t gid=%u\t cuid=%u\t cgid=%u\n"),
	       semdata->sem_perm.uid, semdata->sem_perm.uid,
	       semdata->sem_perm.cuid, semdata->sem_perm.cgid);
	printf(_("mode=%#o, access_perms=%#o\n"),
	       semdata->sem_perm.mode, semdata->sem_perm.mode & 0777);
	printf(_("nsems = %ju\n"), semdata->sem_nsems);
	printf(_("otime = %-26.24s\n"),
	       semdata->sem_otime ? xctime(&semdata->sem_otime) : _("Not set"));
	printf(_("ctime = %-26.24s\n"), xctime(&semdata->sem_ctime));

	printf("%-10s %-10s %-10s %-10s %-10s\n",
	       _("semnum"), _("value"), _("ncount"), _("zcount"), _("pid"));

	for (i = 0; i < semdata->sem_nsems; i++) {
		struct sem_elem *e = &semdata->elements[i];
		printf("%-10zu %-10d %-10d %-10d %-10d\n",
		       i, e->semval, e->ncount, e->zcount, e->pid);
	}
	printf("\n");
	ipc_sem_free_info(semdata);
>>>>>>> master-vanilla
}
