/*
 *  ipcmk.c - used to create ad-hoc IPC segments
 *
 *  Copyright (C) 2008 Hayden A. James (hayden.james@gmail.com)
 *  Copyright (C) 2008 Karel Zak <kzak@redhat.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
<<<<<<< HEAD
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>

#include "nls.h"
#include "c.h"

static const char *progname;

key_t createKey(void)
{
	srandom( time( NULL ) );
	return random();
}

int createShm(size_t size, int permission)
{
	int result = -1;
	int shmid;
	key_t key = createKey();

	if (-1 != (shmid = shmget(key, size, permission | IPC_CREAT)))
		result = shmid;

	return result;
}

int createMsg(int permission)
{
	int result = -1;
	int msgid;
	key_t key = createKey();

	if (-1 != (msgid = msgget(key, permission | IPC_CREAT)))
		result = msgid;

	return result;
}

int createSem(int nsems, int permission)
{
	int result = -1;
	int semid;
	key_t key = createKey();

	if (-1 != (semid = semget(key, nsems, permission | IPC_CREAT)))
		result = semid;

	return result;
}

void usage(int rc)
{
	FILE *out = rc == EXIT_FAILURE ? stderr : stdout;

	fputs(_("\nUsage:\n"), out);
	fprintf(out,
	     _(" %s [options]\n"), progname);

	fputs(_("\nOptions:\n"), out);
	fputs(_(" -M <size>     create shared memory segment of size <size>\n"
		" -S <nsems>    create semaphore array with <nsems> elements\n"
		" -Q            create message queue\n"
		" -p <mode>     permission for the resource (default is 0644)\n"), out);

	fputs(_("\nFor more information see ipcmk(1).\n"), out);

	exit(rc);
=======
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <errno.h>
#include <getopt.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/time.h>

#include "c.h"
#include "nls.h"
#include "randutils.h"
#include "strutils.h"
#include "closestream.h"

static int create_shm(size_t size, int permission)
{
	key_t key;

	random_get_bytes(&key, sizeof(key));
	return shmget(key, size, permission | IPC_CREAT);
}

static int create_msg(int permission)
{
	key_t key;

	random_get_bytes(&key, sizeof(key));
	return msgget(key, permission | IPC_CREAT);
}

static int create_sem(int nsems, int permission)
{
	key_t key;

	random_get_bytes(&key, sizeof(key));
	return semget(key, nsems, permission | IPC_CREAT);
}

static void __attribute__ ((__noreturn__)) usage(FILE * out)
{
	fputs(USAGE_HEADER, out);
	fprintf(out, _(" %s [options]\n"), program_invocation_short_name);

	fputs(USAGE_SEPARATOR, out);
	fputs(_("Create various IPC resources.\n"), out);

	fputs(USAGE_OPTIONS, out);
	fputs(_(" -M, --shmem <size>       create shared memory segment of size <size>\n"), out);
	fputs(_(" -S, --semaphore <number> create semaphore array with <number> elements\n"), out);
	fputs(_(" -Q, --queue              create message queue\n"), out);
	fputs(_(" -p, --mode <mode>        permission for the resource (default is 0644)\n"), out);

	fputs(USAGE_SEPARATOR, out);
	fputs(USAGE_HELP, out);
	fputs(USAGE_VERSION, out);
	fprintf(out, USAGE_MAN_TAIL("ipcmk(1)"));

	exit(out == stderr ? EXIT_FAILURE : EXIT_SUCCESS);
>>>>>>> master-vanilla
}

int main(int argc, char **argv)
{
	int permission = 0644;
	int opt;
	size_t size = 0;
	int nsems = 0;
<<<<<<< HEAD
	int doShm = 0, doMsg = 0, doSem = 0;

	progname = program_invocation_short_name;
	if (!progname)
		progname = "ipcmk";
=======
	int ask_shm = 0, ask_msg = 0, ask_sem = 0;
	static const struct option longopts[] = {
		{"shmem", required_argument, NULL, 'M'},
		{"semaphore", required_argument, NULL, 'S'},
		{"queue", no_argument, NULL, 'Q'},
		{"mode", required_argument, NULL, 'p'},
		{"version", no_argument, NULL, 'V'},
		{"help", no_argument, NULL, 'h'},
		{NULL, 0, NULL, 0}
	};
>>>>>>> master-vanilla

	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);
<<<<<<< HEAD

	while((opt = getopt(argc, argv, "hM:QS:p:")) != -1) {
		switch(opt) {
		case 'M':
			size = atoi(optarg);
			doShm = 1;
			break;
		case 'Q':
			doMsg = 1;
			break;
		case 'S':
			nsems = atoi(optarg);
			doSem = 1;
=======
	atexit(close_stdout);

	while((opt = getopt_long(argc, argv, "hM:QS:p:Vh", longopts, NULL)) != -1) {
		switch(opt) {
		case 'M':
			size = strtosize_or_err(optarg, _("failed to parse size"));
			ask_shm = 1;
			break;
		case 'Q':
			ask_msg = 1;
			break;
		case 'S':
			nsems = strtos32_or_err(optarg, _("failed to parse elements"));
			ask_sem = 1;
>>>>>>> master-vanilla
			break;
		case 'p':
			permission = strtoul(optarg, NULL, 8);
			break;
		case 'h':
<<<<<<< HEAD
			usage(EXIT_SUCCESS);
			break;
		default:
			doShm = doMsg = doSem = 0;
=======
			usage(stdout);
			break;
		case 'V':
			printf(UTIL_LINUX_VERSION);
			return EXIT_SUCCESS;
		default:
			ask_shm = ask_msg = ask_sem = 0;
>>>>>>> master-vanilla
			break;
		}
	}

<<<<<<< HEAD
	if(!doShm && !doMsg && !doSem)
		usage(EXIT_FAILURE);

	if (doShm) {
		int shmid;
		if (-1 == (shmid = createShm(size, permission)))
=======
	if(!ask_shm && !ask_msg && !ask_sem)
		usage(stderr);

	if (ask_shm) {
		int shmid;
		if (-1 == (shmid = create_shm(size, permission)))
>>>>>>> master-vanilla
			err(EXIT_FAILURE, _("create share memory failed"));
		else
			printf(_("Shared memory id: %d\n"), shmid);
	}

<<<<<<< HEAD
	if (doMsg) {
		int msgid;
		if (-1 == (msgid = createMsg(permission)))
=======
	if (ask_msg) {
		int msgid;
		if (-1 == (msgid = create_msg(permission)))
>>>>>>> master-vanilla
			err(EXIT_FAILURE, _("create message queue failed"));
		else
			printf(_("Message queue id: %d\n"), msgid);
	}

<<<<<<< HEAD
	if (doSem) {
		int semid;
		if (-1 == (semid = createSem(nsems, permission)))
=======
	if (ask_sem) {
		int semid;
		if (-1 == (semid = create_sem(nsems, permission)))
>>>>>>> master-vanilla
			err(EXIT_FAILURE, _("create semaphore failed"));
		else
			printf(_("Semaphore id: %d\n"), semid);
	}

	return EXIT_SUCCESS;
}
