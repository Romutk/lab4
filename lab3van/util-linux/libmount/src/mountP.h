/*
 * mountP.h - private library header file
 *
<<<<<<< HEAD
 * Copyright (C) 2008-2009 Karel Zak <kzak@redhat.com>
=======
 * Copyright (C) 2008-2012 Karel Zak <kzak@redhat.com>
>>>>>>> master-vanilla
 *
 * This file may be redistributed under the terms of the
 * GNU Lesser General Public License.
 */

#ifndef _LIBMOUNT_PRIVATE_H
#define _LIBMOUNT_PRIVATE_H

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
<<<<<<< HEAD
#include <unistd.h>

#include "c.h"
#include "list.h"
#include "libmount.h"

/* features */
#define CONFIG_LIBMOUNT_ASSERT
#define CONFIG_LIBMOUNT_DEBUG

#ifdef CONFIG_LIBMOUNT_ASSERT
#include <assert.h>
#endif

/*
 * Debug
 */
#if defined(TEST_PROGRAM) && !defined(LIBMOUNT_DEBUG)
#define CONFIG_LIBMOUNT_DEBUG
#endif

=======
#include <sys/vfs.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>

#include "c.h"
#include "list.h"
#include "debug.h"
#include "libmount.h"

/*
 * Debug
 */
#define MNT_DEBUG_HELP		(1 << 0)
>>>>>>> master-vanilla
#define MNT_DEBUG_INIT		(1 << 1)
#define MNT_DEBUG_CACHE		(1 << 2)
#define MNT_DEBUG_OPTIONS	(1 << 3)
#define MNT_DEBUG_LOCKS		(1 << 4)
#define MNT_DEBUG_TAB		(1 << 5)
#define MNT_DEBUG_FS		(1 << 6)
<<<<<<< HEAD
#define MNT_DEBUG_OPTS		(1 << 7)
#define MNT_DEBUG_UPDATE	(1 << 8)
#define MNT_DEBUG_UTILS		(1 << 9)
#define MNT_DEBUG_CXT		(1 << 10)
#define MNT_DEBUG_DIFF		(1 << 11)
#define MNT_DEBUG_ALL		0xFFFF

#ifdef CONFIG_LIBMOUNT_DEBUG
# include <stdio.h>
# include <stdarg.h>

# define DBG(m,x)	do { \
				if ((MNT_DEBUG_ ## m) & libmount_debug_mask) {\
					fprintf(stderr, "libmount: %8s: ", # m); \
					x; \
				} \
			} while(0)

# define DBG_FLUSH	do { fflush(stderr); } while(0)

extern int libmount_debug_mask;

static inline void __attribute__ ((__format__ (__printf__, 1, 2)))
mnt_debug(const char *mesg, ...)
{
	va_list ap;
	va_start(ap, mesg);
	vfprintf(stderr, mesg, ap);
	va_end(ap);
	fputc('\n', stderr);
}

static inline void __attribute__ ((__format__ (__printf__, 2, 3)))
mnt_debug_h(void *handler, const char *mesg, ...)
{
	va_list ap;

	fprintf(stderr, "[%p]: ", handler);
	va_start(ap, mesg);
	vfprintf(stderr, mesg, ap);
	va_end(ap);
	fputc('\n', stderr);
}

#else /* !CONFIG_LIBMOUNT_DEBUG */
# define DBG(m,x) do { ; } while(0)
# define DBG_FLUSH do { ; } while(0)
#endif

/* extension for files in the /etc/fstab.d directory */
=======
#define MNT_DEBUG_UPDATE	(1 << 7)
#define MNT_DEBUG_UTILS		(1 << 8)
#define MNT_DEBUG_CXT		(1 << 9)
#define MNT_DEBUG_DIFF		(1 << 10)
#define MNT_DEBUG_MONITOR	(1 << 11)

#define MNT_DEBUG_ALL		0xFFFF

UL_DEBUG_DECLARE_MASK(libmount);
#define DBG(m, x)	__UL_DBG(libmount, MNT_DEBUG_, m, x)
#define ON_DBG(m, x)	__UL_DBG_CALL(libmount, MNT_DEBUG_, m, x)
#define DBG_FLUSH	__UL_DBG_FLUSH(libmount, MNT_DEBUG_)

/* extension for files in the directory */
>>>>>>> master-vanilla
#define MNT_MNTTABDIR_EXT	".fstab"

/* library private paths */
#define MNT_RUNTIME_TOPDIR	"/run"
#define MNT_RUNTIME_TOPDIR_OLD	"/dev"

#define MNT_PATH_UTAB		MNT_RUNTIME_TOPDIR "/mount/utab"
#define MNT_PATH_UTAB_OLD	MNT_RUNTIME_TOPDIR_OLD "/.mount/utab"

#define MNT_UTAB_HEADER	"# libmount utab file\n"

#ifdef TEST_PROGRAM
struct libmnt_test {
	const char	*name;
	int		(*body)(struct libmnt_test *ts, int argc, char *argv[]);
	const char	*usage;
};

/* test.c */
extern int mnt_run_test(struct libmnt_test *tests, int argc, char *argv[]);
#endif

/* utils.c */
<<<<<<< HEAD
extern int endswith(const char *s, const char *sx);
extern int startswith(const char *s, const char *sx);

extern int mnt_chdir_to_parent(const char *target, char **filename);
=======
extern int mnt_valid_tagname(const char *tagname);
extern int append_string(char **a, const char *b);

extern const char *mnt_statfs_get_fstype(struct statfs *vfs);
extern int is_file_empty(const char *name);

extern int mnt_is_readonly(const char *path)
			__attribute__((nonnull));

extern int mnt_parse_offset(const char *str, size_t len, uintmax_t *res);

extern int mnt_chdir_to_parent(const char *target, char **filename);

>>>>>>> master-vanilla
extern char *mnt_get_username(const uid_t uid);
extern int mnt_get_uid(const char *username, uid_t *uid);
extern int mnt_get_gid(const char *groupname, gid_t *gid);
extern int mnt_in_group(gid_t gid);

<<<<<<< HEAD
extern char *mnt_get_mountpoint(const char *path);
extern char *mnt_get_fs_root(const char *path, const char *mountpoint);
extern int mnt_open_uniq_filename(const char *filename, char **name);
=======
extern int mnt_open_uniq_filename(const char *filename, char **name);

>>>>>>> master-vanilla
extern int mnt_has_regular_utab(const char **utab, int *writable);
extern const char *mnt_get_utab_path(void);

extern int mnt_get_filesystems(char ***filesystems, const char *pattern);
extern void mnt_free_filesystems(char **filesystems);

<<<<<<< HEAD
/* tab.c */
extern struct libmnt_fs *mnt_table_get_fs_root(struct libmnt_table *tb,
                                        struct libmnt_fs *fs,
                                        unsigned long mountflags,
                                        char **fsroot);
=======
extern char *mnt_get_kernel_cmdline_option(const char *name);
extern int mnt_stat_mountpoint(const char *target, struct stat *st);

/* tab.c */
extern int is_mountinfo(struct libmnt_table *tb);
extern int mnt_table_set_parser_fltrcb(	struct libmnt_table *tb,
					int (*cb)(struct libmnt_fs *, void *),
					void *data);

extern int __mnt_table_parse_mtab(struct libmnt_table *tb,
					const char *filename,
					struct libmnt_table *u_tb);

extern struct libmnt_fs *mnt_table_get_fs_root(struct libmnt_table *tb,
					struct libmnt_fs *fs,
					unsigned long mountflags,
					char **fsroot);
>>>>>>> master-vanilla

/*
 * Generic iterator
 */
struct libmnt_iter {
        struct list_head        *p;		/* current position */
        struct list_head        *head;		/* start position */
	int			direction;	/* MNT_ITER_{FOR,BACK}WARD */
};

#define IS_ITER_FORWARD(_i)	((_i)->direction == MNT_ITER_FORWARD)
#define IS_ITER_BACKWARD(_i)	((_i)->direction == MNT_ITER_BACKWARD)

#define MNT_ITER_INIT(itr, list) \
	do { \
		(itr)->p = IS_ITER_FORWARD(itr) ? \
				(list)->next : (list)->prev; \
		(itr)->head = (list); \
	} while(0)

#define MNT_ITER_ITERATE(itr, res, restype, member) \
	do { \
		res = list_entry((itr)->p, restype, member); \
		(itr)->p = IS_ITER_FORWARD(itr) ? \
				(itr)->p->next : (itr)->p->prev; \
	} while(0)


/*
<<<<<<< HEAD
 * This struct represents one entry in mtab/fstab/mountinfo file.
=======
 * This struct represents one entry in a mtab/fstab/mountinfo file.
>>>>>>> master-vanilla
 * (note that fstab[1] means the first column from fstab, and so on...)
 */
struct libmnt_fs {
	struct list_head ents;

<<<<<<< HEAD
=======
	int		refcount;	/* reference counter */
>>>>>>> master-vanilla
	int		id;		/* mountinfo[1]: ID */
	int		parent;		/* mountinfo[2]: parent */
	dev_t		devno;		/* mountinfo[3]: st_dev */

	char		*bindsrc;	/* utab, full path from fstab[1] for bind mounts */

<<<<<<< HEAD
	char		*source;	/* fstab[1], mountinfo[10]:
=======
	char		*source;	/* fstab[1], mountinfo[10], swaps[1]:
>>>>>>> master-vanilla
                                         * source dev, file, dir or TAG */
	char		*tagname;	/* fstab[1]: tag name - "LABEL", "UUID", ..*/
	char		*tagval;	/*           tag value */

	char		*root;		/* mountinfo[4]: root of the mount within the FS */
	char		*target;	/* mountinfo[5], fstab[2]: mountpoint */
	char		*fstype;	/* mountinfo[9], fstab[3]: filesystem type */

	char		*optstr;	/* fstab[4], merged options */
	char		*vfs_optstr;	/* mountinfo[6]: fs-independent (VFS) options */
<<<<<<< HEAD
=======
	char		*opt_fields;	/* mountinfo[7]: optional fields */
>>>>>>> master-vanilla
	char		*fs_optstr;	/* mountinfo[11]: fs-dependent options */
	char		*user_optstr;	/* userspace mount options */
	char		*attrs;		/* mount attributes */

	int		freq;		/* fstab[5]: dump frequency in days */
	int		passno;		/* fstab[6]: pass number on parallel fsck */

<<<<<<< HEAD
	int		flags;		/* MNT_FS_* flags */
=======
	/* /proc/swaps */
	char		*swaptype;	/* swaps[2]: device type (partition, file, ...) */
	off_t		size;		/* swaps[3]: swaparea size */
	off_t		usedsize;	/* swaps[4]: used size */
	int		priority;	/* swaps[5]: swap priority */

	int		flags;		/* MNT_FS_* flags */
	pid_t		tid;		/* /proc/<tid>/mountinfo otherwise zero */

	char		*comment;	/* fstab comment */
>>>>>>> master-vanilla

	void		*userdata;	/* library independent data */
};

/*
 * fs flags
 */
#define MNT_FS_PSEUDO	(1 << 1) /* pseudo filesystem */
#define MNT_FS_NET	(1 << 2) /* network filesystem */
#define MNT_FS_SWAP	(1 << 3) /* swap device */
#define MNT_FS_KERNEL	(1 << 4) /* data from /proc/{mounts,self/mountinfo} */
#define MNT_FS_MERGED	(1 << 5) /* already merged data from /run/mount/utab */

<<<<<<< HEAD
extern int __mnt_fs_get_flags(struct libmnt_fs *fs);
extern int __mnt_fs_set_flags(struct libmnt_fs *fs, int flags);

=======
#define mnt_fs_is_regular(_f)	(!(mnt_fs_is_pseudofs(_f) \
				   || mnt_fs_is_netfs(_f) \
				   || mnt_fs_is_swaparea(_f)))
>>>>>>> master-vanilla

/*
 * mtab/fstab/mountinfo file
 */
struct libmnt_table {
	int		fmt;		/* MNT_FMT_* file format */
<<<<<<< HEAD
	int		nents;		/* number of valid entries */
=======
	int		nents;		/* number of entries */
	int		refcount;	/* reference counter */
	int		comms;		/* enable/disable comment parsing */
	char		*comm_intro;	/* First comment in file */
	char		*comm_tail;	/* Last comment in file */
>>>>>>> master-vanilla

	struct libmnt_cache *cache;		/* canonicalized paths/tags cache */

        int		(*errcb)(struct libmnt_table *tb,
				 const char *filename, int line);

<<<<<<< HEAD
	struct list_head	ents;	/* list of entries (libmnt_fs) */
=======
	int		(*fltrcb)(struct libmnt_fs *fs, void *data);
	void		*fltrcb_data;


	struct list_head	ents;	/* list of entries (libmnt_fs) */
	void		*userdata;
>>>>>>> master-vanilla
};

extern struct libmnt_table *__mnt_new_table_from_file(const char *filename, int fmt);

/*
 * Tab file format
 */
enum {
	MNT_FMT_GUESS,
	MNT_FMT_FSTAB,			/* /etc/{fs,m}tab */
	MNT_FMT_MTAB = MNT_FMT_FSTAB,	/* alias */
	MNT_FMT_MOUNTINFO,		/* /proc/#/mountinfo */
<<<<<<< HEAD
	MNT_FMT_UTAB			/* /dev/.mount/utab */
};

=======
	MNT_FMT_UTAB,			/* /run/mount/utab */
	MNT_FMT_SWAPS			/* /proc/swaps */
};

/*
 * Additional mounts
 */
struct libmnt_addmount {
	unsigned long mountflags;

	struct list_head	mounts;
};
>>>>>>> master-vanilla

/*
 * Mount context -- high-level API
 */
struct libmnt_context
{
	int	action;		/* MNT_ACT_{MOUNT,UMOUNT} */
	int	restricted;	/* root or not? */

	char	*fstype_pattern;	/* for mnt_match_fstype() */
	char	*optstr_pattern;	/* for mnt_match_options() */

	struct libmnt_fs *fs;		/* filesystem description (type, mountpoint, device, ...) */

	struct libmnt_table *fstab;	/* fstab (or mtab for some remounts) entries */
	struct libmnt_table *mtab;	/* mtab entries */
<<<<<<< HEAD
=======
	struct libmnt_table *utab;	/* rarely used by umount only */
>>>>>>> master-vanilla

	int	(*table_errcb)(struct libmnt_table *tb,	/* callback for libmnt_table structs */
			 const char *filename, int line);

<<<<<<< HEAD
=======
	int	(*table_fltrcb)(struct libmnt_fs *fs, void *data);	/* callback for libmnt_table structs */
	void	*table_fltrcb_data;

	char	*(*pwd_get_cb)(struct libmnt_context *);		/* get encryption password */
	void	(*pwd_release_cb)(struct libmnt_context *, char *);	/* release password */

>>>>>>> master-vanilla
	int	optsmode;	/* fstab optstr mode MNT_OPTSMODE_{AUTO,FORCE,IGNORE} */
	int	loopdev_fd;	/* open loopdev */

	unsigned long	mountflags;	/* final mount(2) flags */
	const void	*mountdata;	/* final mount(2) data, string or binary data */

	unsigned long	user_mountflags;	/* MNT_MS_* (loop=, user=, ...) */

<<<<<<< HEAD
=======
	struct list_head	addmounts;	/* additional mounts */

>>>>>>> master-vanilla
	struct libmnt_cache	*cache;	/* paths cache */
	struct libmnt_lock	*lock;	/* mtab lock */
	struct libmnt_update	*update;/* mtab/utab update */

	const char	*mtab_path; /* path to mtab */
	int		mtab_writable; /* is mtab writable */

	const char	*utab_path; /* path to utab */
	int		utab_writable; /* is utab writable */

	int	flags;		/* private context flags */
<<<<<<< HEAD
	int	ambi;		/* libblkid returns ambivalent result */
=======
>>>>>>> master-vanilla

	char	*helper;	/* name of the used /sbin/[u]mount.<type> helper */
	int	helper_status;	/* helper wait(2) status */
	int	helper_exec_status; /* 1: not called yet, 0: success, <0: -errno */

	char	*orig_user;	/* original (non-fixed) user= option */

<<<<<<< HEAD
=======
	pid_t	*children;	/* "mount -a --fork" PIDs */
	int	nchildren;	/* number of children */
	pid_t	pid;		/* 0=parent; PID=child */


>>>>>>> master-vanilla
	int	syscall_status;	/* 1: not called yet, 0: success, <0: -errno */
};

/* flags */
#define MNT_FL_NOMTAB		(1 << 1)
#define MNT_FL_FAKE		(1 << 2)
#define MNT_FL_SLOPPY		(1 << 3)
#define MNT_FL_VERBOSE		(1 << 4)
#define MNT_FL_NOHELPERS	(1 << 5)
#define MNT_FL_LOOPDEL		(1 << 6)
#define MNT_FL_LAZY		(1 << 7)
#define MNT_FL_FORCE		(1 << 8)
#define MNT_FL_NOCANONICALIZE	(1 << 9)
#define MNT_FL_RDONLY_UMOUNT	(1 << 11)	/* remount,ro after EBUSY umount(2) */
<<<<<<< HEAD

#define MNT_FL_EXTERN_FS	(1 << 15)	/* cxt->fs is not private */
#define MNT_FL_EXTERN_FSTAB	(1 << 16)	/* cxt->fstab is not private */
#define MNT_FL_EXTERN_CACHE	(1 << 17)	/* cxt->cache is not private */
=======
#define MNT_FL_FORK		(1 << 12)
#define MNT_FL_NOSWAPMATCH	(1 << 13)
>>>>>>> master-vanilla

#define MNT_FL_MOUNTDATA	(1 << 20)
#define MNT_FL_TAB_APPLIED	(1 << 21)	/* mtab/fstab merged to cxt->fs */
#define MNT_FL_MOUNTFLAGS_MERGED (1 << 22)	/* MS_* flags was read from optstr */
#define MNT_FL_SAVED_USER	(1 << 23)
#define MNT_FL_PREPARED		(1 << 24)
#define MNT_FL_HELPER		(1 << 25)	/* [u]mount.<type> */
<<<<<<< HEAD
#define MNT_FL_LOOPDEV_READY	(1 << 26)	/* /dev/loop<N> initialized by library */
=======
#define MNT_FL_LOOPDEV_READY	(1 << 26)	/* /dev/loop<N> initialized by the library */
#define MNT_FL_MOUNTOPTS_FIXED  (1 << 27)
#define MNT_FL_TABPATHS_CHECKED	(1 << 28)
>>>>>>> master-vanilla

/* default flags */
#define MNT_FL_DEFAULT		0

/* lock.c */
extern int mnt_lock_use_simplelock(struct libmnt_lock *ml, int enable);

/* optmap.c */
extern const struct libmnt_optmap *mnt_optmap_get_entry(
			     struct libmnt_optmap const **maps,
<<<<<<< HEAD
                             int nmaps, const char *name,
=======
                             int nmaps,
			     const char *name,
>>>>>>> master-vanilla
                             size_t namelen,
			     const struct libmnt_optmap **mapent);

/* optstr.c */
extern int mnt_optstr_remove_option_at(char **optstr, char *begin, char *end);
extern int mnt_optstr_fix_gid(char **optstr, char *value, size_t valsz, char **next);
extern int mnt_optstr_fix_uid(char **optstr, char *value, size_t valsz, char **next);
extern int mnt_optstr_fix_secontext(char **optstr, char *value, size_t valsz, char **next);
extern int mnt_optstr_fix_user(char **optstr);

/* fs.c */
<<<<<<< HEAD
extern struct libmnt_fs *mnt_copy_mtab_fs(const struct libmnt_fs *fs);
extern int __mnt_fs_set_source_ptr(struct libmnt_fs *fs, char *source);
extern int __mnt_fs_set_fstype_ptr(struct libmnt_fs *fs, char *fstype);

/* context.c */
extern int mnt_context_prepare_srcpath(struct libmnt_context *cxt);
extern int mnt_context_prepare_target(struct libmnt_context *cxt);
=======
extern struct libmnt_fs *mnt_copy_mtab_fs(const struct libmnt_fs *fs)
			__attribute__((nonnull));
extern int __mnt_fs_set_source_ptr(struct libmnt_fs *fs, char *source)
			__attribute__((nonnull(1)));
extern int __mnt_fs_set_fstype_ptr(struct libmnt_fs *fs, char *fstype)
			__attribute__((nonnull(1)));

/* context.c */
extern int mnt_context_mtab_writable(struct libmnt_context *cxt);
extern int mnt_context_utab_writable(struct libmnt_context *cxt);
extern const char *mnt_context_get_writable_tabpath(struct libmnt_context *cxt);

extern int mnt_context_get_mtab_for_target(struct libmnt_context *cxt,
				    struct libmnt_table **mtab, const char *tgt);

extern int mnt_context_prepare_srcpath(struct libmnt_context *cxt);
extern int mnt_context_prepare_target(struct libmnt_context *cxt);
extern int mnt_context_guess_srcpath_fstype(struct libmnt_context *cxt, char **type);
>>>>>>> master-vanilla
extern int mnt_context_guess_fstype(struct libmnt_context *cxt);
extern int mnt_context_prepare_helper(struct libmnt_context *cxt,
				      const char *name, const char *type);
extern int mnt_context_prepare_update(struct libmnt_context *cxt);
<<<<<<< HEAD
extern struct libmnt_fs *mnt_context_get_fs(struct libmnt_context *cxt);
=======
>>>>>>> master-vanilla
extern int mnt_context_merge_mflags(struct libmnt_context *cxt);
extern int mnt_context_update_tabs(struct libmnt_context *cxt);

extern int mnt_context_umount_setopt(struct libmnt_context *cxt, int c, char *arg);
extern int mnt_context_mount_setopt(struct libmnt_context *cxt, int c, char *arg);

<<<<<<< HEAD
extern int mnt_context_is_loopdev(struct libmnt_context *cxt);
=======
extern int mnt_context_is_loopdev(struct libmnt_context *cxt)
			__attribute__((nonnull));

extern int mnt_context_propagation_only(struct libmnt_context *cxt)
			__attribute__((nonnull));

extern struct libmnt_addmount *mnt_new_addmount(void);
extern void mnt_free_addmount(struct libmnt_addmount *ad);

>>>>>>> master-vanilla
extern int mnt_context_setup_loopdev(struct libmnt_context *cxt);
extern int mnt_context_delete_loopdev(struct libmnt_context *cxt);
extern int mnt_context_clear_loopdev(struct libmnt_context *cxt);

<<<<<<< HEAD
/* tab_update.c */
extern struct libmnt_fs *mnt_update_get_fs(struct libmnt_update *upd);
extern int mnt_update_set_filename(struct libmnt_update *upd,
				   const char *filename, int userspace_only);
=======
extern int mnt_fork_context(struct libmnt_context *cxt);

extern int mnt_context_set_tabfilter(struct libmnt_context *cxt,
				     int (*fltr)(struct libmnt_fs *, void *),
				     void *data);

/* tab_update.c */
extern int mnt_update_set_filename(struct libmnt_update *upd,
				   const char *filename, int userspace_only);
extern int mnt_update_already_done(struct libmnt_update *upd,
				   struct libmnt_lock *lc);
>>>>>>> master-vanilla

#endif /* _LIBMOUNT_PRIVATE_H */
