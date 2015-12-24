/*
 * mkfs.minix.c - make a linux (minix) file-system.
 *
 * (C) 1991 Linus Torvalds. This file may be redistributed as per
 * the Linux copyright.
 */

/*
 * DD.MM.YY
 *
 * 24.11.91  -	Time began. Used the fsck sources to get started.
 *
 * 25.11.91  -	Corrected some bugs. Added support for ".badblocks"
 *		The algorithm for ".badblocks" is a bit weird, but
 *		it should work. Oh, well.
 *
 * 25.01.92  -	Added the -l option for getting the list of bad blocks
 *		out of a named file. (Dave Rivers, rivers@ponds.uucp)
 *
 * 28.02.92  -	Added %-information when using -c.
 *
 * 28.02.93  -	Added support for other namelengths than the original
 *		14 characters so that I can test the new kernel routines..
 *
 * 09.10.93  -	Make exit status conform to that required by fsutil
 *		(Rik Faith, faith@cs.unc.edu)
 *
 * 31.10.93  -	Added inode request feature, for backup floppies: use
 *		32 inodes, for a news partition use more.
 *		(Scott Heavner, sdh@po.cwru.edu)
 *
 * 03.01.94  -	Added support for file system valid flag.
 *		(Dr. Wettstein, greg%wind.uucp@plains.nodak.edu)
 *
 * 30.10.94  -  Added support for v2 filesystem
 *		(Andreas Schwab, schwab@issan.informatik.uni-dortmund.de)
 * 
 * 09.11.94  -	Added test to prevent overwrite of mounted fs adapted
 *		from Theodore Ts'o's (tytso@athena.mit.edu) mke2fs
 *		program.  (Daniel Quinlan, quinlan@yggdrasil.com)
 *
 * 03.20.95  -	Clear first 512 bytes of filesystem to make certain that
 *		the filesystem is not misidentified as a MS-DOS FAT filesystem.
 *		(Daniel Quinlan, quinlan@yggdrasil.com)
 *
 * 02.07.96  -  Added small patch from Russell King to make the program a
 *		good deal more portable (janl@math.uio.no)
 *
 * 06.29.11  -  Overall cleanups for util-linux and v3 support
 *              Davidlohr Bueso <dave@gnu.org>
 *
<<<<<<< HEAD
 * Usage:  mkfs [-c | -l filename ] [-12v3] [-nXX] [-iXX] device [size-in-blocks]
 *
 *	-c for readablility checking (SLOW!)
 *      -l for getting a list of bad blocks from a file.
 *	-n for namelength (currently the kernel only uses 14 or 30)
 *	-i for number of inodes
 *      -1 for v1 filesystem
 *	-2,-v for v2 filesystem
 *      -3 for v3 filesystem
 *
 * The device may be a block device or a image of one, but this isn't
 * enforced (but it's not much fun on a character device :-). 
=======
 * 06.20.15  -  Do not infinite loop or crash on large devices
 *              Joshua Hudson <joshudson@gmail.com>
 *
>>>>>>> master-vanilla
 */

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/stat.h>
#include <mntent.h>
#include <getopt.h>
#include <err.h>

#include "blkdev.h"
#include "minix_programs.h"
#include "nls.h"
#include "pathnames.h"
#include "bitops.h"
#include "exitcodes.h"
#include "strutils.h"
<<<<<<< HEAD
#include "writeall.h"
=======
#include "all-io.h"
#include "closestream.h"
#include "ismounted.h"
#include "xalloc.h"
>>>>>>> master-vanilla

#define MINIX_ROOT_INO 1
#define MINIX_BAD_INO 2

#define TEST_BUFFER_BLOCKS 16
#define MAX_GOOD_BLOCKS 512

#define MINIX_MAX_INODES 65535

<<<<<<< HEAD
/*
 * Global variables used in minix_programs.h inline fuctions
 */
int fs_version = 1;
=======
#define DEFAULT_FS_VERSION 1

/*
 * Global variables used in minix_programs.h inline fuctions
 */
int fs_version = DEFAULT_FS_VERSION;
>>>>>>> master-vanilla
char *super_block_buffer;

static char *inode_buffer = NULL;

#define Inode (((struct minix_inode *) inode_buffer) - 1)
#define Inode2 (((struct minix2_inode *) inode_buffer) - 1)

<<<<<<< HEAD
static char *program_name = "mkfs";
static char *device_name;
static int DEV = -1;
static unsigned long long BLOCKS;
static int check;
static int badblocks;

/*
 * default (changed to 30, per Linus's
 * suggestion, Sun Nov 21 08:05:07 1993)
 * This should be changed in the future to 60,
 * since v3 needs to be the default nowadays (2011)
 */
static size_t namelen = 30;
static size_t dirsize = 32;
static int magic = MINIX_SUPER_MAGIC2;
static int version2;

static char root_block[MINIX_BLOCK_SIZE];

static char boot_block_buffer[512];

static unsigned short good_blocks_table[MAX_GOOD_BLOCKS];
static int used_good_blocks;
static unsigned long req_nr_inodes;

=======
struct fs_control {
	char *device_name;		/* device on a Minix file system is created */
	int device_fd;			/* open file descriptor of the device */
	unsigned long long fs_blocks;	/* device block count for the file system */
	int fs_used_blocks;		/* used blocks on a device */
	int fs_bad_blocks;		/* number of bad blocks found from device */
	uint16_t fs_namelen;		/* maximum length of filenames */
	size_t fs_dirsize;		/* maximum size of directory */
	unsigned long fs_inodes;	/* number of inodes */
	int fs_magic;			/* file system magic number */
	unsigned int
	 check_blocks:1;		/* check for bad blocks */
};

static char root_block[MINIX_BLOCK_SIZE];
static char boot_block_buffer[512];
static unsigned short good_blocks_table[MAX_GOOD_BLOCKS];
>>>>>>> master-vanilla

static char *inode_map;
static char *zone_map;

#define zone_in_use(x) (isset(zone_map,(x)-get_first_zone()+1) != 0)

#define mark_inode(x) (setbit(inode_map,(x)))
#define unmark_inode(x) (clrbit(inode_map,(x)))

#define mark_zone(x) (setbit(zone_map,(x)-get_first_zone()+1))
#define unmark_zone(x) (clrbit(zone_map,(x)-get_first_zone()+1))

<<<<<<< HEAD

static void __attribute__((__noreturn__))
usage(void) {
	errx(MKFS_USAGE, _("Usage: %s [-c | -l filename] [-nXX] [-iXX] /dev/name [blocks]"),
	     program_name);
}

/*
 * Check to make certain that our new filesystem won't be created on
 * an already mounted partition.  Code adapted from mke2fs, Copyright
 * (C) 1994 Theodore Ts'o.  Also licensed under GPL.
 */
static void check_mount(void) {
	FILE * f;
	struct mntent * mnt;

	if ((f = setmntent (_PATH_MOUNTED, "r")) == NULL)
		return;
	while ((mnt = getmntent (f)) != NULL)
		if (strcmp (device_name, mnt->mnt_fsname) == 0)
			break;
	endmntent (f);
	if (!mnt)
		return;

	errx(MKFS_ERROR, _("%s is mounted; will not make a filesystem here!"),
			device_name);
}
=======
static void __attribute__((__noreturn__)) usage(FILE *out)
{
	fputs(USAGE_HEADER, out);
	fprintf(out, _(" %s [options] /dev/name [blocks]\n"), program_invocation_short_name);
	fputs(USAGE_OPTIONS, out);
	fputs(_(" -1                      use Minix version 1\n"), out);
	fputs(_(" -2, -v                  use Minix version 2\n"), out);
	fputs(_(" -3                      use Minix version 3\n"), out);
	fputs(_(" -n, --namelength <num>  maximum length of filenames\n"), out);
	fputs(_(" -i, --inodes <num>      number of inodes for the filesystem\n"), out);
	fputs(_(" -c, --check             check the device for bad blocks\n"), out);
	fputs(_(" -l, --badblocks <file>  list of bad blocks from file\n"), out);
	fputs(USAGE_SEPARATOR, out);
	fputs(USAGE_HELP, out);
	fputs(USAGE_VERSION, out);
	fprintf(out, USAGE_MAN_TAIL("mkfs.minix(8)"));
	exit(out == stderr ? MKFS_EX_USAGE : MKFS_EX_OK);
}

#ifdef TEST_SCRIPT
static inline time_t mkfs_minix_time(time_t *t)
{
	const char *str = getenv("MKFS_MINIX_TEST_SECOND_SINCE_EPOCH");
	time_t sec;

	if (str && sscanf(str, "%ld", &sec) == 1)
		return sec;
	return time(t);
}
#else				/* !TEST_SCRIPT */
# define mkfs_minix_time(x) time(x)
#endif
>>>>>>> master-vanilla

static void super_set_state(void)
{
	switch (fs_version) {
	case 1:
	case 2:
		Super.s_state |= MINIX_VALID_FS;
		Super.s_state &= ~MINIX_ERROR_FS;
		break;
<<<<<<< HEAD
	}
}

static void write_tables(void) {
	unsigned long imaps = get_nimaps();
	unsigned long zmaps = get_nzmaps();
	unsigned long buffsz = get_inode_buffer_size();
=======
	default: /* v3 */
		break;
	}
}

static void write_tables(const struct fs_control *ctl) {
	unsigned long imaps = get_nimaps();
	unsigned long zmaps = get_nzmaps();
	size_t buffsz = get_inode_buffer_size();
>>>>>>> master-vanilla

	/* Mark the super block valid. */
	super_set_state();

<<<<<<< HEAD
	if (lseek(DEV, 0, SEEK_SET))
		err(MKFS_ERROR, _("%s: seek to boot block failed "
				   " in write_tables"), device_name);
	if (write_all(DEV, boot_block_buffer, 512))
		err(MKFS_ERROR, _("%s: unable to clear boot sector"), device_name);
	if (MINIX_BLOCK_SIZE != lseek(DEV, MINIX_BLOCK_SIZE, SEEK_SET))
		err(MKFS_ERROR, _("%s: seek failed in write_tables"), device_name);

	if (write_all(DEV, super_block_buffer, MINIX_BLOCK_SIZE))
		err(MKFS_ERROR, _("%s: unable to write super-block"), device_name);

	if (write_all(DEV, inode_map, imaps * MINIX_BLOCK_SIZE))
		err(MKFS_ERROR, _("%s: unable to write inode map"), device_name);

	if (write_all(DEV, zone_map, zmaps * MINIX_BLOCK_SIZE))
		err(MKFS_ERROR, _("%s: unable to write zone map"), device_name);

	if (write_all(DEV, inode_buffer, buffsz))
		err(MKFS_ERROR, _("%s: unable to write inodes"), device_name);
}

static void write_block(int blk, char * buffer) {
	if (blk*MINIX_BLOCK_SIZE != lseek(DEV, blk*MINIX_BLOCK_SIZE, SEEK_SET))
		errx(MKFS_ERROR, _("%s: seek failed in write_block"), device_name);

	if (write_all(DEV, buffer, MINIX_BLOCK_SIZE))
		errx(MKFS_ERROR, _("%s: write failed in write_block"), device_name);
}

static int get_free_block(void) {
=======
	if (lseek(ctl->device_fd, 0, SEEK_SET))
		err(MKFS_EX_ERROR, _("%s: seek to boot block failed "
				   " in write_tables"), ctl->device_name);
	if (write_all(ctl->device_fd, boot_block_buffer, 512))
		err(MKFS_EX_ERROR, _("%s: unable to clear boot sector"), ctl->device_name);
	if (MINIX_BLOCK_SIZE != lseek(ctl->device_fd, MINIX_BLOCK_SIZE, SEEK_SET))
		err(MKFS_EX_ERROR, _("%s: seek failed in write_tables"), ctl->device_name);

	if (write_all(ctl->device_fd, super_block_buffer, MINIX_BLOCK_SIZE))
		err(MKFS_EX_ERROR, _("%s: unable to write super-block"), ctl->device_name);

	if (write_all(ctl->device_fd, inode_map, imaps * MINIX_BLOCK_SIZE))
		err(MKFS_EX_ERROR, _("%s: unable to write inode map"), ctl->device_name);

	if (write_all(ctl->device_fd, zone_map, zmaps * MINIX_BLOCK_SIZE))
		err(MKFS_EX_ERROR, _("%s: unable to write zone map"), ctl->device_name);

	if (write_all(ctl->device_fd, inode_buffer, buffsz))
		err(MKFS_EX_ERROR, _("%s: unable to write inodes"), ctl->device_name);
}

static void write_block(const struct fs_control *ctl, int blk, char * buffer) {
	if (blk * MINIX_BLOCK_SIZE != lseek(ctl->device_fd, blk * MINIX_BLOCK_SIZE, SEEK_SET))
		errx(MKFS_EX_ERROR, _("%s: seek failed in write_block"), ctl->device_name);

	if (write_all(ctl->device_fd, buffer, MINIX_BLOCK_SIZE))
		errx(MKFS_EX_ERROR, _("%s: write failed in write_block"), ctl->device_name);
}

static int get_free_block(struct fs_control *ctl) {
>>>>>>> master-vanilla
	unsigned int blk;
	unsigned int zones = get_nzones();
	unsigned int first_zone = get_first_zone();

<<<<<<< HEAD
	if (used_good_blocks+1 >= MAX_GOOD_BLOCKS)
		errx(MKFS_ERROR, _("%s: too many bad blocks"), device_name);
	if (used_good_blocks)
		blk = good_blocks_table[used_good_blocks-1]+1;
=======
	if (ctl->fs_used_blocks + 1 >= MAX_GOOD_BLOCKS)
		errx(MKFS_EX_ERROR, _("%s: too many bad blocks"), ctl->device_name);
	if (ctl->fs_used_blocks)
		blk = good_blocks_table[ctl->fs_used_blocks - 1] + 1;
>>>>>>> master-vanilla
	else
		blk = first_zone;
	while (blk < zones && zone_in_use(blk))
		blk++;
	if (blk >= zones)
<<<<<<< HEAD
		errx(MKFS_ERROR, _("%s: not enough good blocks"), device_name);
	good_blocks_table[used_good_blocks] = blk;
	used_good_blocks++;
	return blk;
}

static void mark_good_blocks(void) {
	int blk;

	for (blk=0 ; blk < used_good_blocks ; blk++)
=======
		errx(MKFS_EX_ERROR, _("%s: not enough good blocks"), ctl->device_name);
	good_blocks_table[ctl->fs_used_blocks] = blk;
	ctl->fs_used_blocks++;
	return blk;
}

static void mark_good_blocks(const struct fs_control *ctl) {
	int blk;

	for (blk=0 ; blk < ctl->fs_used_blocks ; blk++)
>>>>>>> master-vanilla
		mark_zone(good_blocks_table[blk]);
}

static inline int next(unsigned long zone) {
	unsigned long zones = get_nzones();
	unsigned long first_zone = get_first_zone();

	if (!zone)
		zone = first_zone-1;
	while (++zone < zones)
		if (zone_in_use(zone))
			return zone;
	return 0;
}

<<<<<<< HEAD
static void make_bad_inode_v1(void)
=======
static void make_bad_inode_v1(struct fs_control *ctl)
>>>>>>> master-vanilla
{
	struct minix_inode * inode = &Inode[MINIX_BAD_INO];
	int i,j,zone;
	int ind=0,dind=0;
	unsigned short ind_block[MINIX_BLOCK_SIZE>>1];
	unsigned short dind_block[MINIX_BLOCK_SIZE>>1];

#define NEXT_BAD (zone = next(zone))

<<<<<<< HEAD
	if (!badblocks)
		return;
	mark_inode(MINIX_BAD_INO);
	inode->i_nlinks = 1;
	inode->i_time = time(NULL);
	inode->i_mode = S_IFREG + 0000;
	inode->i_size = badblocks*MINIX_BLOCK_SIZE;
=======
	if (!ctl->fs_bad_blocks)
		return;
	mark_inode(MINIX_BAD_INO);
	inode->i_nlinks = 1;
	inode->i_time = mkfs_minix_time(NULL);
	inode->i_mode = S_IFREG + 0000;
	inode->i_size = ctl->fs_bad_blocks * MINIX_BLOCK_SIZE;
>>>>>>> master-vanilla
	zone = next(0);
	for (i=0 ; i<7 ; i++) {
		inode->i_zone[i] = zone;
		if (!NEXT_BAD)
			goto end_bad;
	}
<<<<<<< HEAD
	inode->i_zone[7] = ind = get_free_block();
=======
	inode->i_zone[7] = ind = get_free_block(ctl);
>>>>>>> master-vanilla
	memset(ind_block,0,MINIX_BLOCK_SIZE);
	for (i=0 ; i<512 ; i++) {
		ind_block[i] = zone;
		if (!NEXT_BAD)
			goto end_bad;
	}
<<<<<<< HEAD
	inode->i_zone[8] = dind = get_free_block();
	memset(dind_block,0,MINIX_BLOCK_SIZE);
	for (i=0 ; i<512 ; i++) {
		write_block(ind,(char *) ind_block);
		dind_block[i] = ind = get_free_block();
=======
	inode->i_zone[8] = dind = get_free_block(ctl);
	memset(dind_block,0,MINIX_BLOCK_SIZE);
	for (i=0 ; i<512 ; i++) {
		write_block(ctl, ind,(char *) ind_block);
		dind_block[i] = ind = get_free_block(ctl);
>>>>>>> master-vanilla
		memset(ind_block,0,MINIX_BLOCK_SIZE);
		for (j=0 ; j<512 ; j++) {
			ind_block[j] = zone;
			if (!NEXT_BAD)
				goto end_bad;
		}
	}
<<<<<<< HEAD
	errx(MKFS_ERROR, _("%s: too many bad blocks"), device_name);
end_bad:
	if (ind)
		write_block(ind, (char *) ind_block);
	if (dind)
		write_block(dind, (char *) dind_block);
}

static void make_bad_inode_v2_v3 (void)
=======
	errx(MKFS_EX_ERROR, _("%s: too many bad blocks"), ctl->device_name);
end_bad:
	if (ind)
		write_block(ctl, ind, (char *) ind_block);
	if (dind)
		write_block(ctl, dind, (char *) dind_block);
}

static void make_bad_inode_v2_v3 (struct fs_control *ctl)
>>>>>>> master-vanilla
{
	struct minix2_inode *inode = &Inode2[MINIX_BAD_INO];
	int i, j, zone;
	int ind = 0, dind = 0;
	unsigned long ind_block[MINIX_BLOCK_SIZE >> 2];
	unsigned long dind_block[MINIX_BLOCK_SIZE >> 2];

<<<<<<< HEAD
	if (!badblocks)
		return;
	mark_inode (MINIX_BAD_INO);
	inode->i_nlinks = 1;
	inode->i_atime = inode->i_mtime = inode->i_ctime = time (NULL);
	inode->i_mode = S_IFREG + 0000;
	inode->i_size = badblocks * MINIX_BLOCK_SIZE;
=======
	if (!ctl->fs_bad_blocks)
		return;
	mark_inode (MINIX_BAD_INO);
	inode->i_nlinks = 1;
	inode->i_atime = inode->i_mtime = inode->i_ctime = mkfs_minix_time(NULL);
	inode->i_mode = S_IFREG + 0000;
	inode->i_size = ctl->fs_bad_blocks * MINIX_BLOCK_SIZE;
>>>>>>> master-vanilla
	zone = next (0);
	for (i = 0; i < 7; i++) {
		inode->i_zone[i] = zone;
		if (!NEXT_BAD)
			goto end_bad;
	}
<<<<<<< HEAD
	inode->i_zone[7] = ind = get_free_block ();
=======
	inode->i_zone[7] = ind = get_free_block (ctl);
>>>>>>> master-vanilla
	memset (ind_block, 0, MINIX_BLOCK_SIZE);
	for (i = 0; i < 256; i++) {
		ind_block[i] = zone;
		if (!NEXT_BAD)
			goto end_bad;
	}
<<<<<<< HEAD
	inode->i_zone[8] = dind = get_free_block ();
	memset (dind_block, 0, MINIX_BLOCK_SIZE);
	for (i = 0; i < 256; i++) {
		write_block (ind, (char *) ind_block);
		dind_block[i] = ind = get_free_block ();
=======
	inode->i_zone[8] = dind = get_free_block (ctl);
	memset (dind_block, 0, MINIX_BLOCK_SIZE);
	for (i = 0; i < 256; i++) {
		write_block (ctl, ind, (char *) ind_block);
		dind_block[i] = ind = get_free_block (ctl);
>>>>>>> master-vanilla
		memset (ind_block, 0, MINIX_BLOCK_SIZE);
		for (j = 0; j < 256; j++) {
			ind_block[j] = zone;
			if (!NEXT_BAD)
				goto end_bad;
		}
	}
	/* Could make triple indirect block here */
<<<<<<< HEAD
	errx(MKFS_ERROR, _("%s: too many bad blocks"), device_name);
 end_bad:
	if (ind)
		write_block (ind, (char *) ind_block);
	if (dind)
		write_block (dind, (char *) dind_block);
}

static void make_bad_inode(void)
{
	if (fs_version < 2)
		return make_bad_inode_v1();
	return make_bad_inode_v2_v3();
}

static void make_root_inode_v1(void) {
	struct minix_inode * inode = &Inode[MINIX_ROOT_INO];

	mark_inode(MINIX_ROOT_INO);
	inode->i_zone[0] = get_free_block();
	inode->i_nlinks = 2;
	inode->i_time = time(NULL);
	if (badblocks)
		inode->i_size = 3*dirsize;
	else {
		root_block[2*dirsize] = '\0';
		root_block[2*dirsize+1] = '\0';
		inode->i_size = 2*dirsize;
=======
	errx(MKFS_EX_ERROR, _("%s: too many bad blocks"), ctl->device_name);
 end_bad:
	if (ind)
		write_block (ctl, ind, (char *) ind_block);
	if (dind)
		write_block (ctl, dind, (char *) dind_block);
}

static void make_bad_inode(struct fs_control *ctl)
{
	if (fs_version < 2) {
		make_bad_inode_v1(ctl);
		return;
	}
	make_bad_inode_v2_v3(ctl);
}

static void make_root_inode_v1(struct fs_control *ctl) {
	struct minix_inode * inode = &Inode[MINIX_ROOT_INO];

	mark_inode(MINIX_ROOT_INO);
	inode->i_zone[0] = get_free_block(ctl);
	inode->i_nlinks = 2;
	inode->i_time = mkfs_minix_time(NULL);
	if (ctl->fs_bad_blocks)
		inode->i_size = 3 * ctl->fs_dirsize;
	else {
		root_block[2 * ctl->fs_dirsize] = '\0';
		root_block[2 * ctl->fs_dirsize + 1] = '\0';
		inode->i_size = 2 * ctl->fs_dirsize;
>>>>>>> master-vanilla
	}
	inode->i_mode = S_IFDIR + 0755;
	inode->i_uid = getuid();
	if (inode->i_uid)
		inode->i_gid = getgid();
<<<<<<< HEAD
	write_block(inode->i_zone[0],root_block);
}

static void make_root_inode_v2_v3 (void) {
	struct minix2_inode *inode = &Inode2[MINIX_ROOT_INO];

	mark_inode (MINIX_ROOT_INO);
	inode->i_zone[0] = get_free_block ();
	inode->i_nlinks = 2;
	inode->i_atime = inode->i_mtime = inode->i_ctime = time (NULL);

	if (badblocks)
		inode->i_size = 3 * dirsize;
	else {
		root_block[2 * dirsize] = '\0';
		if (fs_version == 2)
			inode->i_size = 2 * dirsize;
=======
	write_block(ctl, inode->i_zone[0],root_block);
}

static void make_root_inode_v2_v3 (struct fs_control *ctl) {
	struct minix2_inode *inode = &Inode2[MINIX_ROOT_INO];

	mark_inode (MINIX_ROOT_INO);
	inode->i_zone[0] = get_free_block (ctl);
	inode->i_nlinks = 2;
	inode->i_atime = inode->i_mtime = inode->i_ctime = mkfs_minix_time(NULL);

	if (ctl->fs_bad_blocks)
		inode->i_size = 3 * ctl->fs_dirsize;
	else {
		root_block[2 * ctl->fs_dirsize] = '\0';
		inode->i_size = 2 * ctl->fs_dirsize;
>>>>>>> master-vanilla
	}

	inode->i_mode = S_IFDIR + 0755;
	inode->i_uid = getuid();
	if (inode->i_uid)
		inode->i_gid = getgid();
<<<<<<< HEAD
	write_block (inode->i_zone[0], root_block);
}

static void make_root_inode(void)
{
	if (fs_version < 2)
		return make_root_inode_v1();
	return make_root_inode_v2_v3();
}

static void super_set_nzones(void)
{
	switch (fs_version) {
	case 3:
	case 2:
		Super.s_zones = BLOCKS;
		break;
	default: /* v1 */
		Super.s_nzones = BLOCKS;
=======
	write_block (ctl, inode->i_zone[0], root_block);
}

static void make_root_inode(struct fs_control *ctl)
{
	char *tmp = root_block;

	if (fs_version == 3) {
		*(uint32_t *) tmp = 1;
		strcpy(tmp + 4, ".");
		tmp += ctl->fs_dirsize;
		*(uint32_t *) tmp = 1;
		strcpy(tmp + 4, "..");
		tmp += ctl->fs_dirsize;
		*(uint32_t *) tmp = 2;
		strcpy(tmp + 4, ".badblocks");
	} else {
		*(uint16_t *) tmp = 1;
		strcpy(tmp + 2, ".");
		tmp += ctl->fs_dirsize;
		*(uint16_t *) tmp = 1;
		strcpy(tmp + 2, "..");
		tmp += ctl->fs_dirsize;
		*(uint16_t *) tmp = 2;
		strcpy(tmp + 2, ".badblocks");
	}
	if (fs_version < 2) {
		make_root_inode_v1(ctl);
		return;
	}
	make_root_inode_v2_v3(ctl);
}

static void super_set_nzones(const struct fs_control *ctl)
{
	switch (fs_version) {
	case 3:
		Super3.s_zones = ctl->fs_blocks;
		break;
	case 2:
		Super.s_zones = ctl->fs_blocks;
		break;
	default: /* v1 */
		Super.s_nzones = ctl->fs_blocks;
>>>>>>> master-vanilla
		break;
	}
}

static void super_init_maxsize(void)
{
	switch (fs_version) {
	case 3:
		Super3.s_max_size = 2147483647L;
		break;
	case 2:
		Super.s_max_size =  0x7fffffff;
		break;
	default: /* v1 */
		Super.s_max_size = (7+512+512*512)*1024;
		break;
	}
}

<<<<<<< HEAD
static void super_set_map_blocks(unsigned long inodes)
=======
static void super_set_map_blocks(const struct fs_control *ctl, unsigned long inodes)
>>>>>>> master-vanilla
{
	switch (fs_version) {
	case 3:
		Super3.s_imap_blocks = UPPER(inodes + 1, BITS_PER_BLOCK);
<<<<<<< HEAD
		Super3.s_zmap_blocks = UPPER(BLOCKS - (1+get_nimaps()+inode_blocks()),
					     BITS_PER_BLOCK+1);
=======
		Super3.s_zmap_blocks = UPPER(ctl->fs_blocks - (1 + get_nimaps() + inode_blocks()),
					     BITS_PER_BLOCK + 1);
>>>>>>> master-vanilla
		Super3.s_firstdatazone = first_zone_data();
		break;
	default:
		Super.s_imap_blocks = UPPER(inodes + 1, BITS_PER_BLOCK);
<<<<<<< HEAD
		Super.s_zmap_blocks = UPPER(BLOCKS - (1+get_nimaps()+inode_blocks()),
					     BITS_PER_BLOCK+1);
=======
		Super.s_zmap_blocks = UPPER(ctl->fs_blocks - (1 + get_nimaps() + inode_blocks()),
					     BITS_PER_BLOCK + 1);
>>>>>>> master-vanilla
		Super.s_firstdatazone = first_zone_data();
		break;
	}
}

<<<<<<< HEAD
static void super_set_magic(void)
{
	switch (fs_version) {
	case 3:
		Super3.s_magic = magic;
		break;
	default:
		Super.s_magic = magic;
=======
static void super_set_magic(const struct fs_control *ctl)
{
	switch (fs_version) {
	case 3:
		Super3.s_magic = ctl->fs_magic;
		break;
	default:
		Super.s_magic = ctl->fs_magic;
>>>>>>> master-vanilla
		break;
	}
}

<<<<<<< HEAD
static void setup_tables(void) {
	unsigned long inodes, zmaps, imaps, zones, i;

	super_block_buffer = calloc(1, MINIX_BLOCK_SIZE);
	if (!super_block_buffer)
		err(MKFS_ERROR, _("%s: unable to alloc buffer for superblock"),
				device_name);

	memset(boot_block_buffer,0,512);
	super_set_magic();

	if (fs_version == 3) {
		Super3.s_log_zone_size = 0;
		Super3.s_blocksize = BLOCKS;
=======
static void setup_tables(const struct fs_control *ctl) {
	unsigned long inodes, zmaps, imaps, zones, i;

	super_block_buffer = xcalloc(1, MINIX_BLOCK_SIZE);

	memset(boot_block_buffer,0,512);
	super_set_magic(ctl);

	if (fs_version == 3) {
		Super3.s_log_zone_size = 0;
		Super3.s_blocksize = MINIX_BLOCK_SIZE;
>>>>>>> master-vanilla
	}
	else {
		Super.s_log_zone_size = 0;
	}

	super_init_maxsize();
<<<<<<< HEAD
	super_set_nzones();
	zones = get_nzones();

	/* some magic nrs: 1 inode / 3 blocks */
	if ( req_nr_inodes == 0 ) 
		inodes = BLOCKS/3;
	else
		inodes = req_nr_inodes;
=======
	super_set_nzones(ctl);
	zones = get_nzones();

	/* some magic nrs: 1 inode / 3 blocks for smaller filesystems,
	 * for one inode / 16 blocks for large ones. mkfs will eventually
	 * crab about too far when getting close to the maximum size. */
	if (ctl->fs_inodes == 0)
		if (2048 * 1024 < ctl->fs_blocks)	/* 2GB */
			inodes = ctl->fs_blocks / 16;
		else if (512 * 1024 < ctl->fs_blocks)	/* 0.5GB */
			inodes = ctl->fs_blocks / 8;
		else
			inodes = ctl->fs_blocks / 3;
	else
		inodes = ctl->fs_inodes;
>>>>>>> master-vanilla
	/* Round up inode count to fill block size */
	if (fs_version == 2 || fs_version == 3)
		inodes = ((inodes + MINIX2_INODES_PER_BLOCK - 1) &
			  ~(MINIX2_INODES_PER_BLOCK - 1));
	else
		inodes = ((inodes + MINIX_INODES_PER_BLOCK - 1) &
			  ~(MINIX_INODES_PER_BLOCK - 1));
<<<<<<< HEAD
	if (inodes > MINIX_MAX_INODES)
		inodes = MINIX_MAX_INODES;
	if (fs_version == 3)
		Super3.s_ninodes = inodes;
	else
		Super.s_ninodes = inodes;

	super_set_map_blocks(inodes);
	imaps = get_nimaps();
	zmaps = get_nzmaps();

	inode_map = malloc(imaps * MINIX_BLOCK_SIZE);
	zone_map = malloc(zmaps * MINIX_BLOCK_SIZE);
	if (!inode_map || !zone_map)
		err(MKFS_ERROR, _("%s: unable to allocate buffers for maps"),
				device_name);
	memset(inode_map,0xff,imaps * MINIX_BLOCK_SIZE);
	memset(zone_map,0xff,zmaps * MINIX_BLOCK_SIZE);
=======

	if (fs_version == 3)
		Super3.s_ninodes = inodes;
	else {
		Super.s_ninodes = inodes;
		if (inodes > MINIX_MAX_INODES)
			inodes = MINIX_MAX_INODES;
	}
	super_set_map_blocks(ctl, inodes);
	if (MINIX_MAX_INODES < first_zone_data())
		errx(MKFS_EX_ERROR,
		     _("First data block at %jd, which is too far (max %d).\n"
		       "Try specifying fewer inodes by passing --inodes <num>"),
		     first_zone_data(),
		     MINIX_MAX_INODES);
	imaps = get_nimaps();
	zmaps = get_nzmaps();

	inode_map = xmalloc(imaps * MINIX_BLOCK_SIZE);
	zone_map = xmalloc(zmaps * MINIX_BLOCK_SIZE);
	memset(inode_map,0xff,imaps * MINIX_BLOCK_SIZE);
	memset(zone_map,0xff,zmaps * MINIX_BLOCK_SIZE);

>>>>>>> master-vanilla
	for (i = get_first_zone() ; i<zones ; i++)
		unmark_zone(i);
	for (i = MINIX_ROOT_INO ; i<=inodes; i++)
		unmark_inode(i);
<<<<<<< HEAD
	inode_buffer = malloc(get_inode_buffer_size());
	if (!inode_buffer)
		err(MKFS_ERROR, _("%s: unable to allocate buffer for inodes"),
				device_name);
	memset(inode_buffer,0, get_inode_buffer_size());
	printf(_("%lu inodes\n"), inodes);
	printf(_("%lu blocks\n"), zones);
	printf(_("Firstdatazone=%ld (%ld)\n"), get_first_zone(), first_zone_data());
	printf(_("Zonesize=%d\n"),MINIX_BLOCK_SIZE<<get_zone_size());
	printf(_("Maxsize=%ld\n\n"),get_max_size());
=======

	inode_buffer = xmalloc(get_inode_buffer_size());
	memset(inode_buffer,0, get_inode_buffer_size());

	printf(P_("%lu inode\n", "%lu inodes\n", inodes), inodes);
	printf(P_("%lu block\n", "%lu blocks\n", zones), zones);
	printf(_("Firstdatazone=%jd (%jd)\n"), get_first_zone(), first_zone_data());
	printf(_("Zonesize=%zu\n"), (size_t) MINIX_BLOCK_SIZE << get_zone_size());
	printf(_("Maxsize=%zu\n\n"),get_max_size());
>>>>>>> master-vanilla
}

/*
 * Perform a test of a block; return the number of
 * blocks readable/writeable.
 */
<<<<<<< HEAD
static long do_check(char * buffer, int try, unsigned int current_block) {
	long got;
	
	/* Seek to the correct loc. */
	if (lseek(DEV, current_block * MINIX_BLOCK_SIZE, SEEK_SET) !=
		       current_block * MINIX_BLOCK_SIZE )
		err(MKFS_ERROR, _("%s: seek failed during testing of blocks"),
				device_name);

	/* Try the read */
	got = read(DEV, buffer, try * MINIX_BLOCK_SIZE);
=======
static size_t do_check(const struct fs_control *ctl, char * buffer, int try, unsigned int current_block) {
	ssize_t got;
	
	/* Seek to the correct loc. */
	if (lseek(ctl->device_fd, current_block * MINIX_BLOCK_SIZE, SEEK_SET) !=
		       current_block * MINIX_BLOCK_SIZE )
		err(MKFS_EX_ERROR, _("%s: seek failed during testing of blocks"),
				ctl->device_name);

	/* Try the read */
	got = read(ctl->device_fd, buffer, try * MINIX_BLOCK_SIZE);
>>>>>>> master-vanilla
	if (got < 0) got = 0;	
	if (got & (MINIX_BLOCK_SIZE - 1 )) {
		printf(_("Weird values in do_check: probably bugs\n"));
	}
	got /= MINIX_BLOCK_SIZE;
	return got;
}

static unsigned int currently_testing = 0;

static void alarm_intr(int alnum __attribute__ ((__unused__))) {
	unsigned long zones = get_nzones();

	if (currently_testing >= zones)
		return;
	signal(SIGALRM,alarm_intr);
	alarm(5);
	if (!currently_testing)
		return;
	printf("%d ...", currently_testing);
	fflush(stdout);
}

<<<<<<< HEAD
static void check_blocks(void) {
	int try,got;
=======
static void check_blocks(struct fs_control *ctl) {
	size_t try, got;
>>>>>>> master-vanilla
	static char buffer[MINIX_BLOCK_SIZE * TEST_BUFFER_BLOCKS];
	unsigned long zones = get_nzones();
	unsigned long first_zone = get_first_zone();

	currently_testing=0;
	signal(SIGALRM,alarm_intr);
	alarm(5);
	while (currently_testing < zones) {
<<<<<<< HEAD
		if (lseek(DEV,currently_testing*MINIX_BLOCK_SIZE,SEEK_SET) !=
		    currently_testing*MINIX_BLOCK_SIZE)
			errx(MKFS_ERROR, _("%s: seek failed in check_blocks"),
					device_name);
		try = TEST_BUFFER_BLOCKS;
		if (currently_testing + try > zones)
			try = zones-currently_testing;
		got = do_check(buffer, try, currently_testing);
=======
		if (lseek(ctl->device_fd, currently_testing * MINIX_BLOCK_SIZE,SEEK_SET) !=
		    currently_testing*MINIX_BLOCK_SIZE)
			errx(MKFS_EX_ERROR, _("%s: seek failed in check_blocks"),
					ctl->device_name);
		try = TEST_BUFFER_BLOCKS;
		if (currently_testing + try > zones)
			try = zones-currently_testing;
		got = do_check(ctl, buffer, try, currently_testing);
>>>>>>> master-vanilla
		currently_testing += got;
		if (got == try)
			continue;
		if (currently_testing < first_zone)
<<<<<<< HEAD
			errx(MKFS_ERROR, _("%s: bad blocks before data-area: "
					"cannot make fs"), device_name);
		mark_zone(currently_testing);
		badblocks++;
		currently_testing++;
	}
	if (badblocks > 1)
		printf(_("%d bad blocks\n"), badblocks);
	else if (badblocks == 1)
		printf(_("one bad block\n"));
}

static void get_list_blocks(char *filename) {
=======
			errx(MKFS_EX_ERROR, _("%s: bad blocks before data-area: "
					"cannot make fs"), ctl->device_name);
		mark_zone(currently_testing);
		ctl->fs_bad_blocks++;
		currently_testing++;
	}
	if (ctl->fs_bad_blocks > 0)
		printf(P_("%d bad block\n", "%d bad blocks\n", ctl->fs_bad_blocks), ctl->fs_bad_blocks);
}

static void get_list_blocks(struct fs_control *ctl, char *filename) {
>>>>>>> master-vanilla
	FILE *listfile;
	unsigned long blockno;

	listfile = fopen(filename,"r");
	if (listfile == NULL)
<<<<<<< HEAD
		err(MKFS_ERROR, _("%s: can't open file of bad blocks"),
				device_name);

	while (!feof(listfile)) {
		if (fscanf(listfile,"%ld\n", &blockno) != 1) {
			printf(_("badblock number input error on line %d\n"), badblocks + 1);
			errx(MKFS_ERROR, _("%s: cannot read badblocks file"),
					device_name);
		}
		mark_zone(blockno);
		badblocks++;
	}
	fclose(listfile);

	if(badblocks > 1)
		printf(_("%d bad blocks\n"), badblocks);
	else if (badblocks == 1)
		printf(_("one bad block\n"));
}

int main(int argc, char ** argv) {
	int i;
	char * tmp;
	struct stat statbuf;
	char * listfile = NULL;
	char * p;

	if (argc && *argv)
		program_name = *argv;
	if ((p = strrchr(program_name, '/')) != NULL)
		program_name = p+1;
=======
		err(MKFS_EX_ERROR, _("%s: can't open file of bad blocks"),
				ctl->device_name);

	while (!feof(listfile)) {
		if (fscanf(listfile,"%lu\n", &blockno) != 1) {
			printf(_("badblock number input error on line %d\n"), ctl->fs_bad_blocks + 1);
			errx(MKFS_EX_ERROR, _("%s: cannot read badblocks file"),
					ctl->device_name);
		}
		mark_zone(blockno);
		ctl->fs_bad_blocks++;
	}
	fclose(listfile);

	if (ctl->fs_bad_blocks > 0)
		printf(P_("%d bad block\n", "%d bad blocks\n", ctl->fs_bad_blocks), ctl->fs_bad_blocks);
}

static int find_super_magic(const struct fs_control *ctl)
{
	switch (fs_version) {
	case 1:
		if (ctl->fs_namelen == 14)
			return MINIX_SUPER_MAGIC;
		else
			return MINIX_SUPER_MAGIC2;
		break;
	case 2:
		if (ctl->fs_namelen == 14)
			return MINIX2_SUPER_MAGIC;
		else
			return MINIX2_SUPER_MAGIC2;
		break;
	case 3:
		return MINIX3_SUPER_MAGIC;
	default:
		abort();
	}
}

static void determine_device_blocks(struct fs_control *ctl, const struct stat *statbuf)
{
	unsigned long long dev_blocks;

	if (S_ISBLK(statbuf->st_mode)) {
		int sectorsize;

		if (blkdev_get_sector_size(ctl->device_fd, &sectorsize) == -1)
			sectorsize = DEFAULT_SECTOR_SIZE;	/* kernel < 2.3.3 */
		if (MINIX_BLOCK_SIZE < sectorsize)
			errx(MKFS_EX_ERROR, _("block size smaller than physical "
					      "sector size of %s"), ctl->device_name);
		if (blkdev_get_size(ctl->device_fd, &dev_blocks) == -1)
			errx(MKFS_EX_ERROR, _("cannot determine size of %s"), ctl->device_name);
		dev_blocks /= MINIX_BLOCK_SIZE;
	} else if (!S_ISBLK(statbuf->st_mode))
		dev_blocks = statbuf->st_size / MINIX_BLOCK_SIZE;
	if (!ctl->fs_blocks)
		ctl->fs_blocks = dev_blocks;
	else if (dev_blocks < ctl->fs_blocks)
		errx(MKFS_EX_ERROR,
		     _("%s: requested blocks (%llu) exceeds available (%llu) blocks\n"),
		     ctl->device_name, ctl->fs_blocks, dev_blocks);
	if (ctl->fs_blocks < 10)
		errx(MKFS_EX_ERROR, _("%s: number of blocks too small"), ctl->device_name);
	if (fs_version == 1 && ctl->fs_blocks > MINIX_MAX_INODES)
		ctl->fs_blocks = MINIX_MAX_INODES;
	if (ctl->fs_blocks > (4 + ((MINIX_MAX_INODES - 4) * BITS_PER_BLOCK)))
		ctl->fs_blocks = 4 + ((MINIX_MAX_INODES - 4) * BITS_PER_BLOCK);	/* Utter maximum: Clip. */
}

static void check_user_instructions(struct fs_control *ctl)
{
	switch (fs_version) {
	case 1:
	case 2:
		if (ctl->fs_namelen == 14 || ctl->fs_namelen == 30)
			ctl->fs_dirsize = ctl->fs_namelen + 2;
		else
			errx(MKFS_EX_ERROR, _("unsupported name length: %d"), ctl->fs_namelen);
		break;
	case 3:
		if (ctl->fs_namelen == 60)
			ctl->fs_dirsize = ctl->fs_namelen + 4;
		else
			errx(MKFS_EX_ERROR, _("unsupported name length: %d"), ctl->fs_namelen);
		break;
	default:
		errx(MKFS_EX_ERROR, _("unsupported minix file system version: %d"), fs_version);
	}
	ctl->fs_magic = find_super_magic(ctl);
}

int main(int argc, char ** argv)
{
	struct fs_control ctl = {
		.fs_namelen = 30,	/* keep in sync with DEFAULT_FS_VERSION */
		0
	};
	int i;
	struct stat statbuf;
	char * listfile = NULL;
	static const struct option longopts[] = {
		{"namelength", required_argument, NULL, 'n'},
		{"inodes", required_argument, NULL, 'i'},
		{"check", no_argument, NULL, 'c'},
		{"badblocks", required_argument, NULL, 'l'},
		{"version", no_argument, NULL, 'V'},
		{"help", no_argument, NULL, 'h'},
		{NULL, 0, NULL, 0}
	};
>>>>>>> master-vanilla

	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);
<<<<<<< HEAD

	if (argc == 2 &&
	    (!strcmp(argv[1], "-V") || !strcmp(argv[1], "--version"))) {
		printf(_("%s (%s)\n"), program_name, PACKAGE_STRING);
		exit(0);
	}

	if (INODE_SIZE * MINIX_INODES_PER_BLOCK != MINIX_BLOCK_SIZE)
		errx(MKFS_ERROR, _("%s: bad inode size"), device_name);
	if (INODE2_SIZE * MINIX2_INODES_PER_BLOCK != MINIX_BLOCK_SIZE)
		errx(MKFS_ERROR, _("%s: bad inode size"), device_name);

	opterr = 0;
	while ((i = getopt(argc, argv, "ci:l:n:v123")) != -1)
		switch (i) {
		case 'c':
			check=1; break;
		case 'i':
			req_nr_inodes = (unsigned long) atol(optarg);
			break;
		case 'l':
			listfile = optarg; break;
		case 'n':
			i = strtoul(optarg,&tmp,0);
			if (*tmp)
				usage();
			if (i == 14)
				magic = MINIX_SUPER_MAGIC;
			else if (i == 30)
				magic = MINIX_SUPER_MAGIC2;
			else
				usage();
			namelen = i;
			dirsize = i+2;
			break;
		case '1':
			fs_version = 1;
			break;
		case '2':
		case 'v': /* kept for backwards compatiblitly */
			fs_version = 2;
			version2 = 1;
			break;
		case '3':
			fs_version = 3;
			break;
		default:
			usage();
		}
	argc -= optind;
	argv += optind;
	if (argc > 0 && !device_name) {
		device_name = argv[0];
		argc--;
		argv++;
	}
	if (argc > 0) {
		BLOCKS = strtol(argv[0],&tmp,0);
		if (*tmp) {
			printf(_("strtol error: number of blocks not specified"));
			usage();
		}
	}

	if (!device_name) {
		usage();
	}
	check_mount();		/* is it already mounted? */
	tmp = root_block;
	*(short *)tmp = 1;
	strcpy(tmp+2,".");
	tmp += dirsize;
	*(short *)tmp = 1;
	strcpy(tmp+2,"..");
	tmp += dirsize;
	*(short *)tmp = 2;
	strcpy(tmp+2,".badblocks");
	if (stat(device_name, &statbuf) < 0)
		err(MKFS_ERROR, _("%s: stat failed"), device_name);
	if (S_ISBLK(statbuf.st_mode))
		DEV = open(device_name,O_RDWR | O_EXCL);
	else
		DEV = open(device_name,O_RDWR);

	if (DEV<0)
		err(MKFS_ERROR, _("%s: open failed"), device_name);

	if (S_ISBLK(statbuf.st_mode)) {
		int sectorsize;

		if (blkdev_get_sector_size(DEV, &sectorsize) == -1)
			sectorsize = DEFAULT_SECTOR_SIZE;		/* kernel < 2.3.3 */

		if (blkdev_is_misaligned(DEV))
			warnx(_("%s: device is misaligned"), device_name);

		if (MINIX_BLOCK_SIZE < sectorsize)
			errx(MKFS_ERROR, _("block size smaller than physical "
					"sector size of %s"), device_name);
		if (!BLOCKS) {
			if (blkdev_get_size(DEV, &BLOCKS) == -1)
				errx(MKFS_ERROR, _("cannot determine size of %s"),
					device_name);
			BLOCKS /= MINIX_BLOCK_SIZE;
		}
	} else if (!S_ISBLK(statbuf.st_mode)) {
		if (!BLOCKS)
			BLOCKS = statbuf.st_size / MINIX_BLOCK_SIZE;
		check=0;
	} else if (statbuf.st_rdev == 0x0300 || statbuf.st_rdev == 0x0340)
		errx(MKFS_ERROR, _("will not try to make filesystem on '%s'"), device_name);
	if (BLOCKS < 10)
		errx(MKFS_ERROR, _("%s: number of blocks too small"), device_name);

	if (fs_version == 3)
		magic = MINIX3_SUPER_MAGIC;

	if (fs_version == 2) {
		if (namelen == 14)
			magic = MINIX2_SUPER_MAGIC;
		else
			magic = MINIX2_SUPER_MAGIC2;
	} else
		if (BLOCKS > MINIX_MAX_INODES)
			BLOCKS = MINIX_MAX_INODES;
	setup_tables();
	if (check)
		check_blocks();
	else if (listfile)
		get_list_blocks(listfile);

	make_root_inode();
	make_bad_inode();

	mark_good_blocks();
	write_tables();
	close(DEV);

	return 0;
=======
	atexit(close_stdout);

	while ((i = getopt_long(argc, argv, "1v23n:i:cl:Vh", longopts, NULL)) != -1)
		switch (i) {
		case '1':
			fs_version = 1;
			break;
		case 'v': /* kept for backwards compatiblitly */
			warnx(_("-v is ambiguous, use '-2' instead"));
			/* fallthrough */
		case '2':
			fs_version = 2;
			break;
		case '3':
			fs_version = 3;
			ctl.fs_namelen = 60;
			break;
		case 'n':
			ctl.fs_namelen = strtou16_or_err(optarg,
					_("failed to parse maximum length of filenames"));
			break;
		case 'i':
			ctl.fs_inodes = strtoul_or_err(optarg,
					_("failed to parse number of inodes"));
			break;
		case 'c':
			ctl.check_blocks = 1;
			break;
		case 'l':
			listfile = optarg;
			break;
		case 'V':
			printf(UTIL_LINUX_VERSION);
			return MKFS_EX_OK;
		case 'h':
			usage(stdout);
		default:
			usage(stderr);
		}
	argc -= optind;
	argv += optind;
	if (argc > 0) {
		ctl.device_name = argv[0];
		argc--;
		argv++;
	}
	if (argc > 0)
		ctl.fs_blocks = strtoul_or_err(argv[0], _("failed to parse number of blocks"));

	if (!ctl.device_name) {
		usage(stderr);
	}
	check_user_instructions(&ctl);
	if (is_mounted(ctl.device_name))
		errx(MKFS_EX_ERROR, _("%s is mounted; will not make a filesystem here!"),
			ctl.device_name);
	if (stat(ctl.device_name, &statbuf) < 0)
		err(MKFS_EX_ERROR, _("stat of %s failed"), ctl.device_name);
	ctl.device_fd = open_blkdev_or_file(&statbuf, ctl.device_name, O_RDWR);
	if (ctl.device_fd < 0)
		err(MKFS_EX_ERROR, _("cannot open %s"), ctl.device_name);
	determine_device_blocks(&ctl, &statbuf);
	setup_tables(&ctl);
	if (ctl.check_blocks)
		check_blocks(&ctl);
	else if (listfile)
		get_list_blocks(&ctl, listfile);

	make_root_inode(&ctl);
	make_bad_inode(&ctl);

	mark_good_blocks(&ctl);
	write_tables(&ctl);
	if (close_fd(ctl.device_fd) != 0)
		err(MKFS_EX_ERROR, _("write failed"));

	return MKFS_EX_OK;
>>>>>>> master-vanilla
}
