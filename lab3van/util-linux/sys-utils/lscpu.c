/*
 * lscpu - CPU architecture information helper
 *
 * Copyright (C) 2008 Cai Qian <qcai@redhat.com>
 * Copyright (C) 2008 Karel Zak <kzak@redhat.com>
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
 */

=======
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <assert.h>
>>>>>>> master-vanilla
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>

<<<<<<< HEAD
=======
#if (defined(__x86_64__) || defined(__i386__))
# if !defined( __SANITIZE_ADDRESS__)
#  define INCLUDE_VMWARE_BDOOR
# else
#  warning VMWARE detection disabled by __SANITIZE_ADDRESS__
# endif
#endif

#ifdef INCLUDE_VMWARE_BDOOR
# include <stdint.h>
# include <signal.h>
# include <strings.h>
# include <setjmp.h>
# ifdef HAVE_SYS_IO_H
#  include <sys/io.h>
# endif
#endif

#if defined(HAVE_LIBRTAS)
#include <librtas.h>
#endif

#include <libsmartcols.h>

>>>>>>> master-vanilla
#include "cpuset.h"
#include "nls.h"
#include "xalloc.h"
#include "c.h"
#include "strutils.h"
#include "bitops.h"
<<<<<<< HEAD

=======
#include "path.h"
#include "closestream.h"
#include "optutils.h"
#include "lscpu.h"
>>>>>>> master-vanilla

#define CACHE_MAX 100

/* /sys paths */
#define _PATH_SYS_SYSTEM	"/sys/devices/system"
#define _PATH_SYS_CPU		_PATH_SYS_SYSTEM "/cpu"
<<<<<<< HEAD
=======
#define _PATH_SYS_NODE		_PATH_SYS_SYSTEM "/node"
>>>>>>> master-vanilla
#define _PATH_PROC_XEN		"/proc/xen"
#define _PATH_PROC_XENCAP	_PATH_PROC_XEN "/capabilities"
#define _PATH_PROC_CPUINFO	"/proc/cpuinfo"
#define _PATH_PROC_PCIDEVS	"/proc/bus/pci/devices"
#define _PATH_PROC_SYSINFO	"/proc/sysinfo"
<<<<<<< HEAD
=======
#define _PATH_PROC_STATUS	"/proc/self/status"
#define _PATH_PROC_VZ	"/proc/vz"
#define _PATH_PROC_BC	"/proc/bc"
#define _PATH_PROC_DEVICETREE	"/proc/device-tree"
#define _PATH_DEV_MEM 		"/dev/mem"
>>>>>>> master-vanilla

/* virtualization types */
enum {
	VIRT_NONE	= 0,
	VIRT_PARA,
<<<<<<< HEAD
	VIRT_FULL
=======
	VIRT_FULL,
	VIRT_CONT
>>>>>>> master-vanilla
};
const char *virt_types[] = {
	[VIRT_NONE]	= N_("none"),
	[VIRT_PARA]	= N_("para"),
<<<<<<< HEAD
	[VIRT_FULL]	= N_("full")
};

/* hypervisor vendors */
enum {
	HYPER_NONE	= 0,
	HYPER_XEN,
	HYPER_KVM,
	HYPER_MSHV,
	HYPER_VMWARE
};
=======
	[VIRT_FULL]	= N_("full"),
	[VIRT_CONT]	= N_("container"),
};

>>>>>>> master-vanilla
const char *hv_vendors[] = {
	[HYPER_NONE]	= NULL,
	[HYPER_XEN]	= "Xen",
	[HYPER_KVM]	= "KVM",
	[HYPER_MSHV]	= "Microsoft",
<<<<<<< HEAD
	[HYPER_VMWARE]  = "VMware"
=======
	[HYPER_VMWARE]  = "VMware",
	[HYPER_IBM]	= "IBM",
	[HYPER_VSERVER]	= "Linux-VServer",
	[HYPER_UML]	= "User-mode Linux",
	[HYPER_INNOTEK]	= "Innotek GmbH",
	[HYPER_HITACHI]	= "Hitachi",
	[HYPER_PARALLELS] = "Parallels",
	[HYPER_VBOX]	= "Oracle",
	[HYPER_OS400]	= "OS/400",
	[HYPER_PHYP]	= "pHyp",
	[HYPER_SPAR]	= "Unisys s-Par"
};

const int hv_vendor_pci[] = {
	[HYPER_NONE]	= 0x0000,
	[HYPER_XEN]	= 0x5853,
	[HYPER_KVM]	= 0x0000,
	[HYPER_MSHV]	= 0x1414,
	[HYPER_VMWARE]	= 0x15ad,
	[HYPER_VBOX]	= 0x80ee,
};

const int hv_graphics_pci[] = {
	[HYPER_NONE]	= 0x0000,
	[HYPER_XEN]	= 0x0001,
	[HYPER_KVM]	= 0x0000,
	[HYPER_MSHV]	= 0x5353,
	[HYPER_VMWARE]	= 0x0710,
	[HYPER_VBOX]	= 0xbeef,
>>>>>>> master-vanilla
};

/* CPU modes */
enum {
	MODE_32BIT	= (1 << 1),
	MODE_64BIT	= (1 << 2)
};

/* cache(s) description */
struct cpu_cache {
	char		*name;
	char		*size;

	int		nsharedmaps;
	cpu_set_t	**sharedmaps;
};

<<<<<<< HEAD
=======
/* dispatching modes */
enum {
	DISP_HORIZONTAL = 0,
	DISP_VERTICAL	= 1
};

const char *disp_modes[] = {
	[DISP_HORIZONTAL]	= N_("horizontal"),
	[DISP_VERTICAL]		= N_("vertical")
};

/* cpu polarization */
enum {
	POLAR_UNKNOWN	= 0,
	POLAR_VLOW,
	POLAR_VMEDIUM,
	POLAR_VHIGH,
	POLAR_HORIZONTAL
};

struct polarization_modes {
	char *parsable;
	char *readable;
};

struct polarization_modes polar_modes[] = {
	[POLAR_UNKNOWN]	   = {"U",  "-"},
	[POLAR_VLOW]	   = {"VL", "vert-low"},
	[POLAR_VMEDIUM]	   = {"VM", "vert-medium"},
	[POLAR_VHIGH]	   = {"VH", "vert-high"},
	[POLAR_HORIZONTAL] = {"H",  "horizontal"},
};

>>>>>>> master-vanilla
/* global description */
struct lscpu_desc {
	char	*arch;
	char	*vendor;
	char	*family;
	char	*model;
<<<<<<< HEAD
	char	*virtflag;	/* virtualization flag (vmx, svm) */
	int	hyper;		/* hypervisor vendor ID */
	int	virtype;	/* VIRT_PARA|FULL|NONE ? */
	char	*mhz;
	char	*stepping;
	char    *bogomips;
	char	*flags;
	int	mode;		/* rm, lm or/and tm */

	int		ncpus;		/* number of CPUs */
	cpu_set_t	*online;	/* mask with online CPUs */

	int		nnodes;		/* number of NUMA modes */
=======
	char	*modelname;
	char	*virtflag;	/* virtualization flag (vmx, svm) */
	char	*hypervisor;	/* hypervisor software */
	int	hyper;		/* hypervisor vendor ID */
	int	virtype;	/* VIRT_PARA|FULL|NONE ? */
	char	*mhz;
	char	**maxmhz;	/* maximum mega hertz */
	char	**minmhz;	/* minimum mega hertz */
	char	*stepping;
	char    *bogomips;
	char	*flags;
	int	dispatching;	/* none, horizontal or vertical */
	int	mode;		/* rm, lm or/and tm */

	int		ncpuspos;	/* maximal possible CPUs */
	int		ncpus;		/* number of present CPUs */
	cpu_set_t	*present;	/* mask with present CPUs */
	cpu_set_t	*online;	/* mask with online CPUs */

	int		nthreads;	/* number of online threads */

	int		ncaches;
	struct cpu_cache *caches;

	/*
	 * All maps are sequentially indexed (0..ncpuspos), the array index
	 * does not have match with cpuX number as presented by kernel. You
	 * have to use real_cpu_num() to get the real cpuX number.
	 *
	 * For example, the possible system CPUs are: 1,3,5, it means that
	 * ncpuspos=3, so all arrays are in range 0..3.
	 */
	int		*idx2cpunum;	/* mapping index to CPU num */

	int		nnodes;		/* number of NUMA modes */
	int		*idx2nodenum;	/* Support for discontinuous nodes */
>>>>>>> master-vanilla
	cpu_set_t	**nodemaps;	/* array with NUMA nodes */

	/* books -- based on book_siblings (internal kernel map of cpuX's
	 * hardware threads within the same book */
	int		nbooks;		/* number of all online books */
	cpu_set_t	**bookmaps;	/* unique book_siblings */

	/* sockets -- based on core_siblings (internal kernel map of cpuX's
	 * hardware threads within the same physical_package_id (socket)) */
	int		nsockets;	/* number of all online sockets */
	cpu_set_t	**socketmaps;	/* unique core_siblings */

	/* cores -- based on thread_siblings (internel kernel map of cpuX's
	 * hardware threads within the same core as cpuX) */
	int		ncores;		/* number of all online cores */
	cpu_set_t	**coremaps;	/* unique thread_siblings */

<<<<<<< HEAD
	int		nthreads;	/* number of online threads */

	int		ncaches;
	struct cpu_cache *caches;
};

static size_t sysrootlen;
static char pathbuf[PATH_MAX];
static int maxcpus;		/* size in bits of kernel cpu mask */

#define is_cpu_online(_d, _cpu) \
		((_d) && (_d)->online ? \
			CPU_ISSET_S((_cpu), CPU_ALLOC_SIZE(maxcpus), (_d)->online) : 0)

static FILE *path_fopen(const char *mode, int exit_on_err, const char *path, ...)
		__attribute__ ((__format__ (__printf__, 3, 4)));
static void path_getstr(char *result, size_t len, const char *path, ...)
		__attribute__ ((__format__ (__printf__, 3, 4)));
static int path_getnum(const char *path, ...)
		__attribute__ ((__format__ (__printf__, 1, 2)));
static int path_exist(const char *path, ...)
		__attribute__ ((__format__ (__printf__, 1, 2)));
static cpu_set_t *path_cpuset(const char *path, ...)
		__attribute__ ((__format__ (__printf__, 1, 2)));

/*
 * Parsable output
=======
	int		*polarization;	/* cpu polarization */
	int		*addresses;	/* physical cpu addresses */
	int		*configured;	/* cpu configured */
	int		physsockets;	/* Physical sockets (modules) */
	int		physchips;	/* Physical chips */
	int		physcoresperchip;	/* Physical cores per chip */
};

enum {
	OUTPUT_SUMMARY	= 0,	/* default */
	OUTPUT_PARSABLE,	/* -p */
	OUTPUT_READABLE,	/* -e */
};

enum {
	SYSTEM_LIVE = 0,	/* analyzing a live system */
	SYSTEM_SNAPSHOT,	/* analyzing a snapshot of a different system */
};

struct lscpu_modifier {
	int		mode;		/* OUTPUT_* */
	int		system;		/* SYSTEM_* */
	unsigned int	hex:1,		/* print CPU masks rather than CPU lists */
			compat:1,	/* use backwardly compatible format */
			online:1,	/* print online CPUs */
			offline:1;	/* print offline CPUs */
};

static int maxcpus;		/* size in bits of kernel cpu mask */

#define is_cpu_online(_d, _cpu) \
	((_d) && (_d)->online ? \
		CPU_ISSET_S((_cpu), CPU_ALLOC_SIZE(maxcpus), (_d)->online) : 0)
#define is_cpu_present(_d, _cpu) \
	((_d) && (_d)->present ? \
		CPU_ISSET_S((_cpu), CPU_ALLOC_SIZE(maxcpus), (_d)->present) : 0)

#define real_cpu_num(_d, _i)	((_d)->idx2cpunum[(_i)])

/*
 * IDs
>>>>>>> master-vanilla
 */
enum {
	COL_CPU,
	COL_CORE,
	COL_SOCKET,
	COL_NODE,
	COL_BOOK,
<<<<<<< HEAD
	COL_CACHE
};

static const char *colnames[] =
{
	[COL_CPU] = "CPU",
	[COL_CORE] = "Core",
	[COL_SOCKET] = "Socket",
	[COL_NODE] = "Node",
	[COL_BOOK] = "Book",
	[COL_CACHE] = "Cache"
};


static int column_name_to_id(const char *name, size_t namesz)
{
	size_t i;

	for (i = 0; i < ARRAY_SIZE(colnames); i++) {
		const char *cn = colnames[i];
=======
	COL_CACHE,
	COL_POLARIZATION,
	COL_ADDRESS,
	COL_CONFIGURED,
	COL_ONLINE,
	COL_MAXMHZ,
	COL_MINMHZ,
};

/* column description
 */
struct lscpu_coldesc {
	const char *name;
	const char *help;

	unsigned int  is_abbr:1;	/* name is abbreviation */
};

static struct lscpu_coldesc coldescs[] =
{
	[COL_CPU]          = { "CPU", N_("logical CPU number"), 1 },
	[COL_CORE]         = { "CORE", N_("logical core number") },
	[COL_SOCKET]       = { "SOCKET", N_("logical socket number") },
	[COL_NODE]         = { "NODE", N_("logical NUMA node number") },
	[COL_BOOK]         = { "BOOK", N_("logical book number") },
	[COL_CACHE]        = { "CACHE", N_("shows how caches are shared between CPUs") },
	[COL_POLARIZATION] = { "POLARIZATION", N_("CPU dispatching mode on virtual hardware") },
	[COL_ADDRESS]      = { "ADDRESS", N_("physical address of a CPU") },
	[COL_CONFIGURED]   = { "CONFIGURED", N_("shows if the hypervisor has allocated the CPU") },
	[COL_ONLINE]       = { "ONLINE", N_("shows if Linux currently makes use of the CPU") },
	[COL_MAXMHZ]	   = { "MAXMHZ", N_("shows the maximum MHz of the CPU") },
	[COL_MINMHZ]	   = { "MINMHZ", N_("shows the minimum MHz of the CPU") }
};

static int
column_name_to_id(const char *name, size_t namesz)
{
	size_t i;

	for (i = 0; i < ARRAY_SIZE(coldescs); i++) {
		const char *cn = coldescs[i].name;
>>>>>>> master-vanilla

		if (!strncasecmp(name, cn, namesz) && !*(cn + namesz))
			return i;
	}
	warnx(_("unknown column: %s"), name);
	return -1;
}

<<<<<<< HEAD
static const char *
path_vcreate(const char *path, va_list ap)
{
	if (sysrootlen)
		vsnprintf(pathbuf + sysrootlen,
			  sizeof(pathbuf) - sysrootlen, path, ap);
	else
		vsnprintf(pathbuf, sizeof(pathbuf), path, ap);
	return pathbuf;
}

static FILE *
path_vfopen(const char *mode, int exit_on_error, const char *path, va_list ap)
{
	FILE *f;
	const char *p = path_vcreate(path, ap);

	f = fopen(p, mode);
	if (!f && exit_on_error)
		err(EXIT_FAILURE, _("error: cannot open %s"), p);
	return f;
}

static FILE *
path_fopen(const char *mode, int exit_on_error, const char *path, ...)
{
	FILE *fd;
	va_list ap;

	va_start(ap, path);
	fd = path_vfopen(mode, exit_on_error, path, ap);
	va_end(ap);

	return fd;
}

static void
path_getstr(char *result, size_t len, const char *path, ...)
{
	FILE *fd;
	va_list ap;

	va_start(ap, path);
	fd = path_vfopen("r", 1, path, ap);
	va_end(ap);

	if (!fgets(result, len, fd))
		err(EXIT_FAILURE, _("failed to read: %s"), pathbuf);
	fclose(fd);

	len = strlen(result);
	if (result[len - 1] == '\n')
		result[len - 1] = '\0';
}

static int
path_getnum(const char *path, ...)
{
	FILE *fd;
	va_list ap;
	int result;

	va_start(ap, path);
	fd = path_vfopen("r", 1, path, ap);
	va_end(ap);

	if (fscanf(fd, "%d", &result) != 1) {
		if (ferror(fd))
			err(EXIT_FAILURE, _("failed to read: %s"), pathbuf);
		else
			errx(EXIT_FAILURE, _("parse error: %s"), pathbuf);
	}
	fclose(fd);
	return result;
}

static int
path_exist(const char *path, ...)
{
	va_list ap;
	const char *p;

	va_start(ap, path);
	p = path_vcreate(path, ap);
	va_end(ap);

	return access(p, F_OK) == 0;
}

static cpu_set_t *
path_cpuparse(int islist, const char *path, va_list ap)
{
	FILE *fd;
	cpu_set_t *set;
	size_t setsize, len = maxcpus * 7;
	char buf[len];

	fd = path_vfopen("r", 1, path, ap);

	if (!fgets(buf, len, fd))
		err(EXIT_FAILURE, _("failed to read: %s"), pathbuf);
	fclose(fd);

	len = strlen(buf);
	if (buf[len - 1] == '\n')
		buf[len - 1] = '\0';

	set = cpuset_alloc(maxcpus, &setsize, NULL);
	if (!set)
		err(EXIT_FAILURE, _("failed to callocate cpu set"));

	if (islist) {
		if (cpulist_parse(buf, set, setsize))
			errx(EXIT_FAILURE, _("failed to parse CPU list %s"), buf);
	} else {
		if (cpumask_parse(buf, set, setsize))
			errx(EXIT_FAILURE, _("failed to parse CPU mask %s"), buf);
	}
	return set;
}

static cpu_set_t *
path_cpuset(const char *path, ...)
{
	va_list ap;
	cpu_set_t *set;

	va_start(ap, path);
	set = path_cpuparse(0, path, ap);
	va_end(ap);

	return set;
}

static cpu_set_t *
path_cpulist(const char *path, ...)
{
	va_list ap;
	cpu_set_t *set;

	va_start(ap, path);
	set = path_cpuparse(1, path, ap);
	va_end(ap);

	return set;
}

=======
>>>>>>> master-vanilla
/* Lookup a pattern and get the value from cpuinfo.
 * Format is:
 *
 *	"<pattern>   : <key>"
 */
<<<<<<< HEAD
int lookup(char *line, char *pattern, char **value)
=======
static int
lookup(char *line, char *pattern, char **value)
>>>>>>> master-vanilla
{
	char *p, *v;
	int len = strlen(pattern);

	if (!*line)
		return 0;

	/* pattern */
	if (strncmp(line, pattern, len))
		return 0;

	/* white spaces */
	for (p = line + len; isspace(*p); p++);

	/* separator */
	if (*p != ':')
		return 0;

	/* white spaces */
	for (++p; isspace(*p); p++);

	/* value */
	if (!*p)
		return 0;
	v = p;

	/* end of value */
	len = strlen(line) - 1;
	for (p = line + len; isspace(*(p-1)); p--);
	*p = '\0';

	*value = xstrdup(v);
	return 1;
}

/* Don't init the mode for platforms where we are not able to
 * detect that CPU supports 64-bit mode.
 */
static int
<<<<<<< HEAD
init_mode(void)
{
	int m = 0;

	if (sysrootlen)
=======
init_mode(struct lscpu_modifier *mod)
{
	int m = 0;

	if (mod->system == SYSTEM_SNAPSHOT)
>>>>>>> master-vanilla
		/* reading info from any /{sys,proc} dump, don't mix it with
		 * information about our real CPU */
		return 0;

#if defined(__alpha__) || defined(__ia64__)
	m |= MODE_64BIT;	/* 64bit platforms only */
#endif
	/* platforms with 64bit flag in /proc/cpuinfo, define
	 * 32bit default here */
#if defined(__i386__) || defined(__x86_64__) || \
    defined(__s390x__) || defined(__s390__) || defined(__sparc_v9__)
	m |= MODE_32BIT;
#endif
	return m;
}

<<<<<<< HEAD
static void
read_basicinfo(struct lscpu_desc *desc)
=======
#if defined(HAVE_LIBRTAS)
#define PROCESSOR_MODULE_INFO	43
static int strbe16toh(const char *buf, int offset)
{
	return (buf[offset] << 8) + buf[offset+1];
}

static void read_physical_info_powerpc(struct lscpu_desc *desc)
{
	char buf[BUFSIZ];
	int rc, len, ntypes;

	desc->physsockets = desc->physchips = desc->physcoresperchip = 0;

	rc = rtas_get_sysparm(PROCESSOR_MODULE_INFO, sizeof(buf), buf);
	if (rc < 0)
		return;

	len = strbe16toh(buf, 0);
	if (len < 8)
		return;

	ntypes = strbe16toh(buf, 2);

	assert(ntypes <= 1);
	if (!ntypes)
		return;

	desc->physsockets = strbe16toh(buf, 4);
	desc->physchips = strbe16toh(buf, 6);
	desc->physcoresperchip = strbe16toh(buf, 8);
}
#else
static void read_physical_info_powerpc(
		struct lscpu_desc *desc __attribute__((__unused__)))
{
}
#endif

static void
read_basicinfo(struct lscpu_desc *desc, struct lscpu_modifier *mod)
>>>>>>> master-vanilla
{
	FILE *fp = path_fopen("r", 1, _PATH_PROC_CPUINFO);
	char buf[BUFSIZ];
	struct utsname utsbuf;
<<<<<<< HEAD
=======
	size_t setsize;
>>>>>>> master-vanilla

	/* architecture */
	if (uname(&utsbuf) == -1)
		err(EXIT_FAILURE, _("error: uname failed"));
	desc->arch = xstrdup(utsbuf.machine);

<<<<<<< HEAD
	/* count CPU(s) */
	while(path_exist(_PATH_SYS_SYSTEM "/cpu/cpu%d", desc->ncpus))
		desc->ncpus++;

=======
>>>>>>> master-vanilla
	/* details */
	while (fgets(buf, sizeof(buf), fp) != NULL) {
		if (lookup(buf, "vendor", &desc->vendor)) ;
		else if (lookup(buf, "vendor_id", &desc->vendor)) ;
		else if (lookup(buf, "family", &desc->family)) ;
		else if (lookup(buf, "cpu family", &desc->family)) ;
		else if (lookup(buf, "model", &desc->model)) ;
<<<<<<< HEAD
=======
		else if (lookup(buf, "model name", &desc->modelname)) ;
>>>>>>> master-vanilla
		else if (lookup(buf, "stepping", &desc->stepping)) ;
		else if (lookup(buf, "cpu MHz", &desc->mhz)) ;
		else if (lookup(buf, "flags", &desc->flags)) ;		/* x86 */
		else if (lookup(buf, "features", &desc->flags)) ;	/* s390 */
		else if (lookup(buf, "type", &desc->flags)) ;		/* sparc64 */
		else if (lookup(buf, "bogomips", &desc->bogomips)) ;
		else if (lookup(buf, "bogomips per cpu", &desc->bogomips)) ; /* s390 */
		else
			continue;
	}

<<<<<<< HEAD
	desc->mode = init_mode();
=======
	desc->mode = init_mode(mod);
>>>>>>> master-vanilla

	if (desc->flags) {
		snprintf(buf, sizeof(buf), " %s ", desc->flags);
		if (strstr(buf, " svm "))
<<<<<<< HEAD
			desc->virtflag = strdup("svm");
		else if (strstr(buf, " vmx "))
			desc->virtflag = strdup("vmx");
=======
			desc->virtflag = xstrdup("svm");
		else if (strstr(buf, " vmx "))
			desc->virtflag = xstrdup("vmx");
>>>>>>> master-vanilla
		if (strstr(buf, " lm "))
			desc->mode |= MODE_32BIT | MODE_64BIT;		/* x86_64 */
		if (strstr(buf, " zarch "))
			desc->mode |= MODE_32BIT | MODE_64BIT;		/* s390x */
		if (strstr(buf, " sun4v ") || strstr(buf, " sun4u "))
			desc->mode |= MODE_32BIT | MODE_64BIT;		/* sparc64 */
	}

<<<<<<< HEAD
	fclose(fp);

	if (path_exist(_PATH_SYS_SYSTEM "/cpu/kernel_max"))
		/* note that kernel_max is maximum index [NR_CPUS-1] */
		maxcpus = path_getnum(_PATH_SYS_SYSTEM "/cpu/kernel_max") + 1;

	else if (!sysrootlen)
		/* the root is '/' so we are working with data from the current kernel */
		maxcpus = get_max_number_of_cpus();
	else
		/* we are reading some /sys snapshot instead of the real /sys,
		 * let's use any crazy number... */
		maxcpus = desc->ncpus > 2048 ? desc->ncpus : 2048;

	/* get mask for online CPUs */
	if (path_exist(_PATH_SYS_SYSTEM "/cpu/online")) {
		size_t setsize = CPU_ALLOC_SIZE(maxcpus);
		desc->online = path_cpulist(_PATH_SYS_SYSTEM "/cpu/online");
		desc->nthreads = CPU_COUNT_S(setsize, desc->online);
	}
}

static int
has_pci_device(int vendor, int device)
{
	FILE *f;
	int num, fn, ven, dev;
=======
	if (desc->arch && mod->system != SYSTEM_SNAPSHOT) {
		if (strcmp(desc->arch, "ppc64") == 0)
			desc->mode |= MODE_32BIT | MODE_64BIT;
		else if (strcmp(desc->arch, "ppc") == 0)
			desc->mode |= MODE_32BIT;
	}

	fclose(fp);

	if (path_exist(_PATH_SYS_CPU "/kernel_max"))
		/* note that kernel_max is maximum index [NR_CPUS-1] */
		maxcpus = path_read_s32(_PATH_SYS_CPU "/kernel_max") + 1;

	else if (mod->system == SYSTEM_LIVE)
		/* the root is '/' so we are working with data from the current kernel */
		maxcpus = get_max_number_of_cpus();

	if (maxcpus <= 0)
		/* error or we are reading some /sys snapshot instead of the
		 * real /sys, let's use any crazy number... */
		maxcpus = 2048;

	setsize = CPU_ALLOC_SIZE(maxcpus);

	if (path_exist(_PATH_SYS_CPU "/possible")) {
		cpu_set_t *tmp = path_read_cpulist(maxcpus, _PATH_SYS_CPU "/possible");
		int num, idx;

		desc->ncpuspos = CPU_COUNT_S(setsize, tmp);
		desc->idx2cpunum = xcalloc(desc->ncpuspos, sizeof(int));

		for (num = 0, idx = 0; num < maxcpus; num++) {
			if (CPU_ISSET(num, tmp))
				desc->idx2cpunum[idx++] = num;
		}
		cpuset_free(tmp);
	} else
		err(EXIT_FAILURE, _("failed to determine number of CPUs: %s"),
				_PATH_SYS_CPU "/possible");


	/* get mask for present CPUs */
	if (path_exist(_PATH_SYS_CPU "/present")) {
		desc->present = path_read_cpulist(maxcpus, _PATH_SYS_CPU "/present");
		desc->ncpus = CPU_COUNT_S(setsize, desc->present);
	}

	/* get mask for online CPUs */
	if (path_exist(_PATH_SYS_CPU "/online")) {
		desc->online = path_read_cpulist(maxcpus, _PATH_SYS_CPU "/online");
		desc->nthreads = CPU_COUNT_S(setsize, desc->online);
	}

	/* get dispatching mode */
	if (path_exist(_PATH_SYS_CPU "/dispatching"))
		desc->dispatching = path_read_s32(_PATH_SYS_CPU "/dispatching");
	else
		desc->dispatching = -1;

	if (mod->system == SYSTEM_LIVE)
		read_physical_info_powerpc(desc);
}

static int
has_pci_device(unsigned int vendor, unsigned int device)
{
	FILE *f;
	unsigned int num, fn, ven, dev;
>>>>>>> master-vanilla
	int res = 1;

	f = path_fopen("r", 0, _PATH_PROC_PCIDEVS);
	if (!f)
		return 0;

	 /* for more details about bus/pci/devices format see
	  * drivers/pci/proc.c in linux kernel
	  */
	while(fscanf(f, "%02x%02x\t%04x%04x\t%*[^\n]",
			&num, &fn, &ven, &dev) == 4) {

		if (ven == vendor && dev == device)
			goto found;
	}

	res = 0;
found:
	fclose(f);
	return res;
}

#if defined(__x86_64__) || defined(__i386__)

/*
 * This CPUID leaf returns the information about the hypervisor.
 * EAX : maximum input value for CPUID supported by the hypervisor.
 * EBX, ECX, EDX : Hypervisor vendor ID signature. E.g. VMwareVMware.
 */
#define HYPERVISOR_INFO_LEAF   0x40000000

static inline void
cpuid(unsigned int op, unsigned int *eax, unsigned int *ebx,
			 unsigned int *ecx, unsigned int *edx)
{
	__asm__(
#if defined(__PIC__) && defined(__i386__)
		/* x86 PIC cannot clobber ebx -- gcc bitches */
<<<<<<< HEAD
		"pushl %%ebx;"
		"cpuid;"
		"movl %%ebx, %%esi;"
		"popl %%ebx;"
=======
		"xchg %%ebx, %%esi;"
		"cpuid;"
		"xchg %%esi, %%ebx;"
>>>>>>> master-vanilla
		: "=S" (*ebx),
#else
		"cpuid;"
		: "=b" (*ebx),
#endif
		  "=a" (*eax),
		  "=c" (*ecx),
		  "=d" (*edx)
		: "1" (op), "c"(0));
}

static void
read_hypervisor_cpuid(struct lscpu_desc *desc)
{
	unsigned int eax = 0, ebx = 0, ecx = 0, edx = 0;
	char hyper_vendor_id[13];

	memset(hyper_vendor_id, 0, sizeof(hyper_vendor_id));

	cpuid(HYPERVISOR_INFO_LEAF, &eax, &ebx, &ecx, &edx);
	memcpy(hyper_vendor_id + 0, &ebx, 4);
	memcpy(hyper_vendor_id + 4, &ecx, 4);
	memcpy(hyper_vendor_id + 8, &edx, 4);
	hyper_vendor_id[12] = '\0';

	if (!hyper_vendor_id[0])
		return;

	if (!strncmp("XenVMMXenVMM", hyper_vendor_id, 12))
		desc->hyper = HYPER_XEN;
	else if (!strncmp("KVMKVMKVM", hyper_vendor_id, 9))
		desc->hyper = HYPER_KVM;
	else if (!strncmp("Microsoft Hv", hyper_vendor_id, 12))
		desc->hyper = HYPER_MSHV;
	else if (!strncmp("VMwareVMware", hyper_vendor_id, 12))
		desc->hyper = HYPER_VMWARE;
<<<<<<< HEAD
}

#else	/* ! __x86_64__ */
static void
read_hypervisor_cpuid(struct lscpu_desc *desc)
{
}
#endif

static void
read_hypervisor(struct lscpu_desc *desc)
{
	read_hypervisor_cpuid(desc);

	if (desc->hyper)
		/* hvm */
		desc->virtype = VIRT_FULL;

	else if (path_exist(_PATH_PROC_XEN)) {
		/* Xen para-virt or dom0 */
		FILE *fd = path_fopen("r", 0, _PATH_PROC_XENCAP);
		int dom0 = 0;
=======
	else if (!strncmp("UnisysSpar64", hyper_vendor_id, 12))
		desc->hyper = HYPER_SPAR;
}

#else /* ! (__x86_64__ || __i386__) */
static void
read_hypervisor_cpuid(struct lscpu_desc *desc __attribute__((__unused__)))
{
}
#endif

static int is_compatible(const char *path, const char *str)
{
	FILE *fd = path_fopen("r", 0, "%s", path);

	if (fd) {
		char buf[256];
		size_t i, len;

		memset(buf, 0, sizeof(buf));
		len = fread(buf, 1, sizeof(buf) - 1, fd);
		fclose(fd);

		for (i = 0; i < len;) {
			if (!strcmp(&buf[i], str))
				return 1;
			i += strlen(&buf[i]);
			i++;
		}
	}

	return 0;
}

static int
read_hypervisor_powerpc(struct lscpu_desc *desc)
{
	assert(!desc->hyper);

	 /* IBM iSeries: legacy, para-virtualized on top of OS/400 */
	if (path_exist("/proc/iSeries")) {
		desc->hyper = HYPER_OS400;
		desc->virtype = VIRT_PARA;

	/* PowerNV (POWER Non-Virtualized, bare-metal) */
	} else if (is_compatible(_PATH_PROC_DEVICETREE "/compatible", "ibm,powernv")) {
		desc->hyper = HYPER_NONE;
		desc->virtype = VIRT_NONE;

	/* PowerVM (IBM's proprietary hypervisor, aka pHyp) */
	} else if (path_exist(_PATH_PROC_DEVICETREE "/ibm,partition-name")
		   && path_exist(_PATH_PROC_DEVICETREE "/hmc-managed?")
		   && !path_exist(_PATH_PROC_DEVICETREE "/chosen/qemu,graphic-width")) {
		FILE *fd;
		desc->hyper = HYPER_PHYP;
		desc->virtype = VIRT_PARA;
		fd = path_fopen("r", 0, _PATH_PROC_DEVICETREE "/ibm,partition-name");
		if (fd) {
			char buf[256];
			if (fscanf(fd, "%255s", buf) == 1 && !strcmp(buf, "full"))
				desc->virtype = VIRT_NONE;
			fclose(fd);
		}

	/* Qemu */
	} else if (is_compatible(_PATH_PROC_DEVICETREE "/compatible", "qemu,pseries")) {
		desc->hyper = HYPER_KVM;
		desc->virtype = VIRT_PARA;
	}
	return desc->hyper;
}

#ifdef INCLUDE_VMWARE_BDOOR

#define VMWARE_BDOOR_MAGIC          0x564D5868
#define VMWARE_BDOOR_PORT           0x5658
#define VMWARE_BDOOR_CMD_GETVERSION 10

static UL_ASAN_BLACKLIST
void vmware_bdoor(uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx)
{
	__asm__(
#if defined(__PIC__) && defined(__i386__)
		/* x86 PIC cannot clobber ebx -- gcc bitches */
		"xchg %%ebx, %%esi;"
		"inl (%%dx), %%eax;"
		"xchg %%esi, %%ebx;"
		: "=S" (*ebx),
#else
		"inl (%%dx), %%eax;"
		: "=b" (*ebx),
#endif
		  "=a" (*eax),
		  "=c" (*ecx),
		  "=d" (*edx)
		: "0" (VMWARE_BDOOR_MAGIC),
		  "1" (VMWARE_BDOOR_CMD_GETVERSION),
		  "2" (VMWARE_BDOOR_PORT),
		  "3" (0)
		: "memory");
}

static jmp_buf segv_handler_env;

static void
segv_handler(__attribute__((__unused__)) int sig,
             __attribute__((__unused__)) siginfo_t *info,
             __attribute__((__unused__)) void *ignored)
{
	siglongjmp(segv_handler_env, 1);
}

static int
is_vmware_platform(void)
{
	uint32_t eax, ebx, ecx, edx;
	struct sigaction act, oact;

	/*
	 * The assembly routine for vmware detection works
	 * fine under vmware, even if ran as regular user. But
	 * on real HW or under other hypervisors, it segfaults (which is
	 * expected). So we temporarily install SIGSEGV handler to catch
	 * the signal. All this magic is needed because lscpu
	 * isn't supposed to require root privileges.
	 */
	if (sigsetjmp(segv_handler_env, 1))
		return 0;

	memset(&act, 0, sizeof(act));
	act.sa_sigaction = segv_handler;
	act.sa_flags = SA_SIGINFO;

	if (sigaction(SIGSEGV, &act, &oact))
		err(EXIT_FAILURE, _("error: can not set signal handler"));

	vmware_bdoor(&eax, &ebx, &ecx, &edx);

	if (sigaction(SIGSEGV, &oact, NULL))
		err(EXIT_FAILURE, _("error: can not restore signal handler"));

	return eax != (uint32_t)-1 && ebx == VMWARE_BDOOR_MAGIC;
}

#else /* ! INCLUDE_VMWARE_BDOOR */

static int
is_vmware_platform(void)
{
	return 0;
}

#endif /* INCLUDE_VMWARE_BDOOR */

static void
read_hypervisor(struct lscpu_desc *desc, struct lscpu_modifier *mod)
{
	FILE *fd;

	if (mod->system != SYSTEM_SNAPSHOT) {
		read_hypervisor_cpuid(desc);
		if (!desc->hyper)
			desc->hyper = read_hypervisor_dmi();
		if (!desc->hyper && is_vmware_platform())
			desc->hyper = HYPER_VMWARE;
	}

	if (desc->hyper)
		desc->virtype = VIRT_FULL;

	else if (read_hypervisor_powerpc(desc) > 0) {}

	/* Xen para-virt or dom0 */
	else if (path_exist(_PATH_PROC_XEN)) {
		int dom0 = 0;
		fd = path_fopen("r", 0, _PATH_PROC_XENCAP);
>>>>>>> master-vanilla

		if (fd) {
			char buf[256];

<<<<<<< HEAD
			if (fscanf(fd, "%s", buf) == 1 &&
=======
			if (fscanf(fd, "%255s", buf) == 1 &&
>>>>>>> master-vanilla
			    !strcmp(buf, "control_d"))
				dom0 = 1;
			fclose(fd);
		}
		desc->virtype = dom0 ? VIRT_NONE : VIRT_PARA;
		desc->hyper = HYPER_XEN;

<<<<<<< HEAD
	} else if (has_pci_device(0x5853, 0x0001)) {
		/* Xen full-virt on non-x86_64 */
		desc->hyper = HYPER_XEN;
		desc->virtype = VIRT_FULL;
	}
}

/* add @set to the @ary, unnecesary set is deallocated. */
=======
	/* Xen full-virt on non-x86_64 */
	} else if (has_pci_device( hv_vendor_pci[HYPER_XEN], hv_graphics_pci[HYPER_XEN])) {
		desc->hyper = HYPER_XEN;
		desc->virtype = VIRT_FULL;
	} else if (has_pci_device( hv_vendor_pci[HYPER_VMWARE], hv_graphics_pci[HYPER_VMWARE])) {
		desc->hyper = HYPER_VMWARE;
		desc->virtype = VIRT_FULL;
	} else if (has_pci_device( hv_vendor_pci[HYPER_VBOX], hv_graphics_pci[HYPER_VBOX])) {
		desc->hyper = HYPER_VBOX;
		desc->virtype = VIRT_FULL;

	/* IBM PR/SM */
	} else if (path_exist(_PATH_PROC_SYSINFO)) {
		FILE *sysinfo_fd = path_fopen("r", 0, _PATH_PROC_SYSINFO);
		char buf[BUFSIZ];

		if (!sysinfo_fd)
			return;
		desc->hyper = HYPER_IBM;
		desc->hypervisor = "PR/SM";
		desc->virtype = VIRT_FULL;
		while (fgets(buf, sizeof(buf), sysinfo_fd) != NULL) {
			char *str;

			if (!strstr(buf, "Control Program:"))
				continue;
			if (!strstr(buf, "KVM"))
				desc->hyper = HYPER_IBM;
			else
				desc->hyper = HYPER_KVM;
			str = strchr(buf, ':');
			if (!str)
				continue;
			xasprintf(&str, "%s", str + 1);

			/* remove leading, trailing and repeating whitespace */
			while (*str == ' ')
				str++;
			desc->hypervisor = str;
			str += strlen(str) - 1;
			while ((*str == '\n') || (*str == ' '))
				*(str--) = '\0';
			while ((str = strstr(desc->hypervisor, "  ")))
				memmove(str, str + 1, strlen(str));
		}
		fclose(sysinfo_fd);
	}

	/* OpenVZ/Virtuozzo - /proc/vz dir should exist
	 *		      /proc/bc should not */
	else if (path_exist(_PATH_PROC_VZ) && !path_exist(_PATH_PROC_BC)) {
		desc->hyper = HYPER_PARALLELS;
		desc->virtype = VIRT_CONT;

	/* IBM */
	} else if (desc->vendor &&
		 (strcmp(desc->vendor, "PowerVM Lx86") == 0 ||
		  strcmp(desc->vendor, "IBM/S390") == 0)) {
		desc->hyper = HYPER_IBM;
		desc->virtype = VIRT_FULL;

	/* User-mode-linux */
	} else if (desc->modelname && strstr(desc->modelname, "UML")) {
		desc->hyper = HYPER_UML;
		desc->virtype = VIRT_PARA;

	/* Linux-VServer */
	} else if (path_exist(_PATH_PROC_STATUS)) {
		char buf[BUFSIZ];
		char *val = NULL;

		fd = path_fopen("r", 1, _PATH_PROC_STATUS);
		while (fgets(buf, sizeof(buf), fd) != NULL) {
			if (lookup(buf, "VxID", &val))
				break;
		}
		fclose(fd);

		if (val) {
			while (isdigit(*val))
				++val;
			if (!*val) {
				desc->hyper = HYPER_VSERVER;
				desc->virtype = VIRT_CONT;
			}
		}
	}
}

/* add @set to the @ary, unnecessary set is deallocated. */
>>>>>>> master-vanilla
static int add_cpuset_to_array(cpu_set_t **ary, int *items, cpu_set_t *set)
{
	int i;
	size_t setsize = CPU_ALLOC_SIZE(maxcpus);

	if (!ary)
		return -1;

	for (i = 0; i < *items; i++) {
		if (CPU_EQUAL_S(setsize, set, ary[i]))
			break;
	}
	if (i == *items) {
		ary[*items] = set;
		++*items;
		return 0;
	}
	CPU_FREE(set);
	return 1;
}

static void
<<<<<<< HEAD
read_topology(struct lscpu_desc *desc, int num)
{
	cpu_set_t *thread_siblings, *core_siblings, *book_siblings;
=======
read_topology(struct lscpu_desc *desc, int idx)
{
	cpu_set_t *thread_siblings, *core_siblings, *book_siblings;
	int num = real_cpu_num(desc, idx);
>>>>>>> master-vanilla

	if (!path_exist(_PATH_SYS_CPU "/cpu%d/topology/thread_siblings", num))
		return;

<<<<<<< HEAD
	thread_siblings = path_cpuset(_PATH_SYS_CPU
					"/cpu%d/topology/thread_siblings", num);
	core_siblings = path_cpuset(_PATH_SYS_CPU
					"/cpu%d/topology/core_siblings", num);
	book_siblings = NULL;
	if (path_exist(_PATH_SYS_CPU "/cpu%d/topology/book_siblings", num)) {
		book_siblings = path_cpuset(_PATH_SYS_CPU
					    "/cpu%d/topology/book_siblings", num);
	}
=======
	thread_siblings = path_read_cpuset(maxcpus, _PATH_SYS_CPU
					"/cpu%d/topology/thread_siblings", num);
	core_siblings = path_read_cpuset(maxcpus, _PATH_SYS_CPU
					"/cpu%d/topology/core_siblings", num);
	book_siblings = NULL;
	if (path_exist(_PATH_SYS_CPU "/cpu%d/topology/book_siblings", num))
		book_siblings = path_read_cpuset(maxcpus, _PATH_SYS_CPU
					    "/cpu%d/topology/book_siblings", num);
>>>>>>> master-vanilla

	if (!desc->coremaps) {
		int nbooks, nsockets, ncores, nthreads;
		size_t setsize = CPU_ALLOC_SIZE(maxcpus);

		/* threads within one core */
		nthreads = CPU_COUNT_S(setsize, thread_siblings);
<<<<<<< HEAD
		/* cores within one socket */
		ncores = CPU_COUNT_S(setsize, core_siblings) / nthreads;
		/* number of sockets within one book.
		 * Because of odd / non-present cpu maps and to keep
		 * calculation easy we make sure that nsockets and
		 * nbooks is at least 1.
		 */
		nsockets = desc->ncpus / nthreads / ncores ?: 1;
		/* number of books */
		nbooks = desc->ncpus / nthreads / ncores / nsockets ?: 1;
=======
		if (!nthreads)
			nthreads = 1;

		/* cores within one socket */
		ncores = CPU_COUNT_S(setsize, core_siblings) / nthreads;
		if (!ncores)
			ncores = 1;

		/* number of sockets within one book.  Because of odd /
		 * non-present cpu maps and to keep calculation easy we make
		 * sure that nsockets and nbooks is at least 1.
		 */
		nsockets = desc->ncpus / nthreads / ncores;
		if (!nsockets)
			nsockets = 1;

		/* number of books */
		nbooks = desc->ncpus / nthreads / ncores / nsockets;
		if (!nbooks)
			nbooks = 1;
>>>>>>> master-vanilla

		/* all threads, see also read_basicinfo()
		 * -- fallback for kernels without
		 *    /sys/devices/system/cpu/online.
		 */
		if (!desc->nthreads)
			desc->nthreads = nbooks * nsockets * ncores * nthreads;
<<<<<<< HEAD
		/* For each map we make sure that it can have up to ncpus
=======

		/* For each map we make sure that it can have up to ncpuspos
>>>>>>> master-vanilla
		 * entries. This is because we cannot reliably calculate the
		 * number of cores, sockets and books on all architectures.
		 * E.g. completely virtualized architectures like s390 may
		 * have multiple sockets of different sizes.
		 */
<<<<<<< HEAD
		desc->coremaps = xcalloc(desc->ncpus, sizeof(cpu_set_t *));
		desc->socketmaps = xcalloc(desc->ncpus, sizeof(cpu_set_t *));
		if (book_siblings)
			desc->bookmaps = xcalloc(desc->ncpus, sizeof(cpu_set_t *));
=======
		desc->coremaps = xcalloc(desc->ncpuspos, sizeof(cpu_set_t *));
		desc->socketmaps = xcalloc(desc->ncpuspos, sizeof(cpu_set_t *));
		if (book_siblings)
			desc->bookmaps = xcalloc(desc->ncpuspos, sizeof(cpu_set_t *));
>>>>>>> master-vanilla
	}

	add_cpuset_to_array(desc->socketmaps, &desc->nsockets, core_siblings);
	add_cpuset_to_array(desc->coremaps, &desc->ncores, thread_siblings);
	if (book_siblings)
		add_cpuset_to_array(desc->bookmaps, &desc->nbooks, book_siblings);
}

<<<<<<< HEAD
=======
static void
read_polarization(struct lscpu_desc *desc, int idx)
{
	char mode[64];
	int num = real_cpu_num(desc, idx);

	if (desc->dispatching < 0)
		return;
	if (!path_exist(_PATH_SYS_CPU "/cpu%d/polarization", num))
		return;
	if (!desc->polarization)
		desc->polarization = xcalloc(desc->ncpuspos, sizeof(int));
	path_read_str(mode, sizeof(mode), _PATH_SYS_CPU "/cpu%d/polarization", num);
	if (strncmp(mode, "vertical:low", sizeof(mode)) == 0)
		desc->polarization[idx] = POLAR_VLOW;
	else if (strncmp(mode, "vertical:medium", sizeof(mode)) == 0)
		desc->polarization[idx] = POLAR_VMEDIUM;
	else if (strncmp(mode, "vertical:high", sizeof(mode)) == 0)
		desc->polarization[idx] = POLAR_VHIGH;
	else if (strncmp(mode, "horizontal", sizeof(mode)) == 0)
		desc->polarization[idx] = POLAR_HORIZONTAL;
	else
		desc->polarization[idx] = POLAR_UNKNOWN;
}

static void
read_address(struct lscpu_desc *desc, int idx)
{
	int num = real_cpu_num(desc, idx);

	if (!path_exist(_PATH_SYS_CPU "/cpu%d/address", num))
		return;
	if (!desc->addresses)
		desc->addresses = xcalloc(desc->ncpuspos, sizeof(int));
	desc->addresses[idx] = path_read_s32(_PATH_SYS_CPU "/cpu%d/address", num);
}

static void
read_configured(struct lscpu_desc *desc, int idx)
{
	int num = real_cpu_num(desc, idx);

	if (!path_exist(_PATH_SYS_CPU "/cpu%d/configure", num))
		return;
	if (!desc->configured)
		desc->configured = xcalloc(desc->ncpuspos, sizeof(int));
	desc->configured[idx] = path_read_s32(_PATH_SYS_CPU "/cpu%d/configure", num);
}

static void
read_max_mhz(struct lscpu_desc *desc, int idx)
{
	int num = real_cpu_num(desc, idx);

	if (!path_exist(_PATH_SYS_CPU "/cpu%d/cpufreq/cpuinfo_max_freq", num))
		return;
	if (!desc->maxmhz)
		desc->maxmhz = xcalloc(desc->ncpuspos, sizeof(char *));
	xasprintf(&(desc->maxmhz[idx]), "%.4f",
		  (float)path_read_s32(_PATH_SYS_CPU
				       "/cpu%d/cpufreq/cpuinfo_max_freq", num) / 1000);
}

static void
read_min_mhz(struct lscpu_desc *desc, int idx)
{
	int num = real_cpu_num(desc, idx);

	if (!path_exist(_PATH_SYS_CPU "/cpu%d/cpufreq/cpuinfo_min_freq", num))
		return;
	if (!desc->minmhz)
		desc->minmhz = xcalloc(desc->ncpuspos, sizeof(char *));
	xasprintf(&(desc->minmhz[idx]), "%.4f",
		  (float)path_read_s32(_PATH_SYS_CPU
				       "/cpu%d/cpufreq/cpuinfo_min_freq", num) / 1000);
}

>>>>>>> master-vanilla
static int
cachecmp(const void *a, const void *b)
{
	struct cpu_cache *c1 = (struct cpu_cache *) a;
	struct cpu_cache *c2 = (struct cpu_cache *) b;

	return strcmp(c2->name, c1->name);
}

static void
<<<<<<< HEAD
read_cache(struct lscpu_desc *desc, int num)
{
	char buf[256];
	int i;

	if (!desc->ncaches) {
		while(path_exist(_PATH_SYS_SYSTEM "/cpu/cpu%d/cache/index%d",
=======
read_cache(struct lscpu_desc *desc, int idx)
{
	char buf[256];
	int i;
	int num = real_cpu_num(desc, idx);

	if (!desc->ncaches) {
		while(path_exist(_PATH_SYS_CPU "/cpu%d/cache/index%d",
>>>>>>> master-vanilla
					num, desc->ncaches))
			desc->ncaches++;

		if (!desc->ncaches)
			return;

		desc->caches = xcalloc(desc->ncaches, sizeof(*desc->caches));
	}
	for (i = 0; i < desc->ncaches; i++) {
		struct cpu_cache *ca = &desc->caches[i];
		cpu_set_t *map;

<<<<<<< HEAD
		if (!path_exist(_PATH_SYS_SYSTEM "/cpu/cpu%d/cache/index%d",
=======
		if (!path_exist(_PATH_SYS_CPU "/cpu%d/cache/index%d",
>>>>>>> master-vanilla
				num, i))
			continue;
		if (!ca->name) {
			int type, level;

			/* cache type */
<<<<<<< HEAD
			path_getstr(buf, sizeof(buf),
=======
			path_read_str(buf, sizeof(buf),
>>>>>>> master-vanilla
					_PATH_SYS_CPU "/cpu%d/cache/index%d/type",
					num, i);
			if (!strcmp(buf, "Data"))
				type = 'd';
			else if (!strcmp(buf, "Instruction"))
				type = 'i';
			else
				type = 0;

			/* cache level */
<<<<<<< HEAD
			level = path_getnum(_PATH_SYS_CPU "/cpu%d/cache/index%d/level",
=======
			level = path_read_s32(_PATH_SYS_CPU "/cpu%d/cache/index%d/level",
>>>>>>> master-vanilla
					num, i);
			if (type)
				snprintf(buf, sizeof(buf), "L%d%c", level, type);
			else
				snprintf(buf, sizeof(buf), "L%d", level);

			ca->name = xstrdup(buf);

			/* cache size */
<<<<<<< HEAD
			path_getstr(buf, sizeof(buf),
					_PATH_SYS_CPU "/cpu%d/cache/index%d/size",
					num, i);
			ca->size = xstrdup(buf);
		}

		/* information about how CPUs share different caches */
		map = path_cpuset(_PATH_SYS_CPU "/cpu%d/cache/index%d/shared_cpu_map",
				num, i);

		if (!ca->sharedmaps)
			ca->sharedmaps = xcalloc(desc->ncpus, sizeof(cpu_set_t *));
=======
			if (path_exist(_PATH_SYS_CPU "/cpu%d/cache/index%d/size",num, i)) {
				path_read_str(buf, sizeof(buf),
					_PATH_SYS_CPU "/cpu%d/cache/index%d/size", num, i);
				ca->size = xstrdup(buf);
			} else {
				ca->size = xstrdup("unknown size");
			}
		}

		/* information about how CPUs share different caches */
		map = path_read_cpuset(maxcpus,
				  _PATH_SYS_CPU "/cpu%d/cache/index%d/shared_cpu_map",
				  num, i);

		if (!ca->sharedmaps)
			ca->sharedmaps = xcalloc(desc->ncpuspos, sizeof(cpu_set_t *));
>>>>>>> master-vanilla
		add_cpuset_to_array(ca->sharedmaps, &ca->nsharedmaps, map);
	}
}

<<<<<<< HEAD
static void
read_nodes(struct lscpu_desc *desc)
{
	int i;

	/* number of NUMA node */
	while (path_exist(_PATH_SYS_SYSTEM "/node/node%d", desc->nnodes))
		desc->nnodes++;

	if (!desc->nnodes)
		return;

	desc->nodemaps = xcalloc(desc->nnodes, sizeof(cpu_set_t *));

	/* information about how nodes share different CPUs */
	for (i = 0; i < desc->nnodes; i++)
		desc->nodemaps[i] = path_cpuset(
					_PATH_SYS_SYSTEM "/node/node%d/cpumap",
					i);
}

static void
print_parsable_cell(struct lscpu_desc *desc, int i, int col, int compatible)
{
	int j;
	size_t setsize = CPU_ALLOC_SIZE(maxcpus);

	switch (col) {
	case COL_CPU:
		printf("%d", i);
		break;
	case COL_CORE:
		for (j = 0; j < desc->ncores; j++) {
			if (CPU_ISSET_S(i, setsize, desc->coremaps[j])) {
				printf("%d", j);
				break;
			}
		}
		break;
	case COL_SOCKET:
		for (j = 0; j < desc->nsockets; j++) {
			if (CPU_ISSET_S(i, setsize, desc->socketmaps[j])) {
				printf("%d", j);
				break;
			}
		}
		break;
	case COL_NODE:
		for (j = 0; j < desc->nnodes; j++) {
			if (CPU_ISSET_S(i, setsize, desc->nodemaps[j])) {
				printf("%d", j);
				break;
			}
		}
		break;
	case COL_BOOK:
		for (j = 0; j < desc->nbooks; j++) {
			if (CPU_ISSET_S(i, setsize, desc->bookmaps[j])) {
				printf("%d", j);
				break;
			}
		}
		break;
	case COL_CACHE:
		for (j = desc->ncaches - 1; j >= 0; j--) {
			struct cpu_cache *ca = &desc->caches[j];
			int x;

			for (x = 0; x < ca->nsharedmaps; x++) {
				if (CPU_ISSET_S(i, setsize, ca->sharedmaps[x])) {
					printf("%d", x);
					break;
				}
			}
			if (j != 0)
				putchar(compatible ? ',' : ':');
		}
		break;
	}
}

/*
 * We support two formats:
=======
static inline int is_node_dirent(struct dirent *d)
{
	return
		d &&
#ifdef _DIRENT_HAVE_D_TYPE
		(d->d_type == DT_DIR || d->d_type == DT_UNKNOWN) &&
#endif
		strncmp(d->d_name, "node", 4) == 0 &&
		isdigit_string(d->d_name + 4);
}

static int
nodecmp(const void *ap, const void *bp)
{
	int *a = (int *) ap, *b = (int *) bp;
	return *a - *b;
}

static void
read_nodes(struct lscpu_desc *desc)
{
	int i = 0;
	DIR *dir;
	struct dirent *d;
	char *path;

	/* number of NUMA node */
	path = path_strdup(_PATH_SYS_NODE);
	dir = opendir(path);
	free(path);

	while (dir && (d = readdir(dir))) {
		if (is_node_dirent(d))
			desc->nnodes++;
	}

	if (!desc->nnodes) {
		if (dir)
			closedir(dir);
		return;
	}

	desc->nodemaps = xcalloc(desc->nnodes, sizeof(cpu_set_t *));
	desc->idx2nodenum = xmalloc(desc->nnodes * sizeof(int));

	if (dir) {
		rewinddir(dir);
		while ((d = readdir(dir)) && i < desc->nnodes) {
			if (is_node_dirent(d))
				desc->idx2nodenum[i++] = strtol_or_err(((d->d_name) + 4),
							_("Failed to extract the node number"));
		}
		closedir(dir);
		qsort(desc->idx2nodenum, desc->nnodes, sizeof(int), nodecmp);
	}

	/* information about how nodes share different CPUs */
	for (i = 0; i < desc->nnodes; i++)
		desc->nodemaps[i] = path_read_cpuset(maxcpus,
					_PATH_SYS_NODE "/node%d/cpumap",
					desc->idx2nodenum[i]);
}

static char *
get_cell_data(struct lscpu_desc *desc, int idx, int col,
	      struct lscpu_modifier *mod,
	      char *buf, size_t bufsz)
{
	size_t setsize = CPU_ALLOC_SIZE(maxcpus);
	size_t i;
	int cpu = real_cpu_num(desc, idx);

	*buf = '\0';

	switch (col) {
	case COL_CPU:
		snprintf(buf, bufsz, "%d", cpu);
		break;
	case COL_CORE:
		if (cpuset_ary_isset(cpu, desc->coremaps,
				     desc->ncores, setsize, &i) == 0)
			snprintf(buf, bufsz, "%zu", i);
		break;
	case COL_SOCKET:
		if (cpuset_ary_isset(cpu, desc->socketmaps,
				     desc->nsockets, setsize, &i) == 0)
			snprintf(buf, bufsz, "%zu", i);
		break;
	case COL_NODE:
		if (cpuset_ary_isset(cpu, desc->nodemaps,
				     desc->nnodes, setsize, &i) == 0)
			snprintf(buf, bufsz, "%d", desc->idx2nodenum[i]);
		break;
	case COL_BOOK:
		if (cpuset_ary_isset(cpu, desc->bookmaps,
				     desc->nbooks, setsize, &i) == 0)
			snprintf(buf, bufsz, "%zu", i);
		break;
	case COL_CACHE:
	{
		char *p = buf;
		size_t sz = bufsz;
		int j;

		for (j = desc->ncaches - 1; j >= 0; j--) {
			struct cpu_cache *ca = &desc->caches[j];

			if (cpuset_ary_isset(cpu, ca->sharedmaps,
					     ca->nsharedmaps, setsize, &i) == 0) {
				int x = snprintf(p, sz, "%zu", i);
				if (x <= 0 || (size_t) x + 2 >= sz)
					return NULL;
				p += x;
				sz -= x;
			}
			if (j != 0) {
				*p++ = mod->compat ? ',' : ':';
				*p = '\0';
				sz++;
			}
		}
		break;
	}
	case COL_POLARIZATION:
		if (desc->polarization) {
			int x = desc->polarization[idx];

			snprintf(buf, bufsz, "%s",
				 mod->mode == OUTPUT_PARSABLE ?
						polar_modes[x].parsable :
						polar_modes[x].readable);
		}
		break;
	case COL_ADDRESS:
		if (desc->addresses)
			snprintf(buf, bufsz, "%d", desc->addresses[idx]);
		break;
	case COL_CONFIGURED:
		if (!desc->configured)
			break;
		if (mod->mode == OUTPUT_PARSABLE)
			snprintf(buf, bufsz, "%s",
				 desc->configured[idx] ? _("Y") : _("N"));
		else
			snprintf(buf, bufsz, "%s",
				 desc->configured[idx] ? _("yes") : _("no"));
		break;
	case COL_ONLINE:
		if (!desc->online)
			break;
		if (mod->mode == OUTPUT_PARSABLE)
			snprintf(buf, bufsz, "%s",
				 is_cpu_online(desc, cpu) ? _("Y") : _("N"));
		else
			snprintf(buf, bufsz, "%s",
				 is_cpu_online(desc, cpu) ? _("yes") : _("no"));
		break;
	case COL_MAXMHZ:
		if (desc->maxmhz)
			xstrncpy(buf, desc->maxmhz[idx], bufsz);
		break;
	case COL_MINMHZ:
		if (desc->minmhz)
			xstrncpy(buf, desc->minmhz[idx], bufsz);
		break;
	}
	return buf;
}

static char *
get_cell_header(struct lscpu_desc *desc, int col,
		struct lscpu_modifier *mod,
	        char *buf, size_t bufsz)
{
	*buf = '\0';

	if (col == COL_CACHE) {
		char *p = buf;
		size_t sz = bufsz;
		int i;

		for (i = desc->ncaches - 1; i >= 0; i--) {
			int x = snprintf(p, sz, "%s", desc->caches[i].name);
			if (x <= 0 || (size_t) x + 2 > sz)
				return NULL;
			sz -= x;
			p += x;
			if (i > 0) {
				*p++ = mod->compat ? ',' : ':';
				*p = '\0';
				sz++;
			}
		}
		if (desc->ncaches)
			return buf;
	}
	snprintf(buf, bufsz, "%s", coldescs[col].name);
	return buf;
}

/*
 * [-p] backend, we support two parsable formats:
>>>>>>> master-vanilla
 *
 * 1) "compatible" -- this format is compatible with the original lscpu(1)
 * output and it contains fixed set of the columns. The CACHE columns are at
 * the end of the line and the CACHE is not printed if the number of the caches
 * is zero. The CACHE columns are separated by two commas, for example:
 *
 *    $ lscpu --parse
 *    # CPU,Core,Socket,Node,,L1d,L1i,L2
 *    0,0,0,0,,0,0,0
 *    1,1,0,0,,1,1,0
 *
 * 2) "user defined output" -- this format prints always all columns without
 * special prefix for CACHE column. If there are not CACHEs then the column is
 * empty and the header "Cache" is printed rather than a real name of the cache.
 * The CACHE columns are separated by ':'.
 *
 *	$ lscpu --parse=CPU,CORE,SOCKET,NODE,CACHE
 *	# CPU,Core,Socket,Node,L1d:L1i:L2
 *	0,0,0,0,0:0:0
 *	1,1,0,0,1:1:0
 */
static void
<<<<<<< HEAD
print_parsable(struct lscpu_desc *desc, int cols[], int ncols, int compatible)
{
	int i, c;

=======
print_parsable(struct lscpu_desc *desc, int cols[], int ncols,
	       struct lscpu_modifier *mod)
{
	char buf[BUFSIZ], *data;
	int i;

	/*
	 * Header
	 */
>>>>>>> master-vanilla
	printf(_(
	"# The following is the parsable format, which can be fed to other\n"
	"# programs. Each different item in every column has an unique ID\n"
	"# starting from zero.\n"));

	fputs("# ", stdout);
	for (i = 0; i < ncols; i++) {
<<<<<<< HEAD
		if (cols[i] == COL_CACHE) {
			if (compatible && !desc->ncaches)
				continue;
			if (i > 0)
				putchar(',');
			if (compatible && i != 0)
				putchar(',');
			for (c = desc->ncaches - 1; c >= 0; c--) {
				printf("%s", desc->caches[c].name);
				if (c > 0)
					putchar(compatible ? ',' : ':');
			}
			if (!desc->ncaches)
				fputs(colnames[cols[i]], stdout);
		} else {
			if (i > 0)
				putchar(',');
			fputs(colnames[cols[i]], stdout);
		}
	}
	putchar('\n');

	for (i = 0; i < desc->ncpus; i++) {
		if (desc->online && !is_cpu_online(desc, i))
			continue;
		for (c = 0; c < ncols; c++) {
			if (compatible && cols[c] == COL_CACHE) {
=======
		int col = cols[i];

		if (col == COL_CACHE) {
			if (mod->compat && !desc->ncaches)
				continue;
			if (mod->compat && i != 0)
				putchar(',');
		}
		if (i > 0)
			putchar(',');

		data = get_cell_header(desc, col, mod, buf, sizeof(buf));

		if (data && * data && col != COL_CACHE &&
		    !coldescs[col].is_abbr) {
			/*
			 * For normal column names use mixed case (e.g. "Socket")
			 */
			char *p = data + 1;

			while (p && *p != '\0') {
				*p = tolower((unsigned int) *p);
				p++;
			}
		}
		fputs(data && *data ? data : "", stdout);
	}
	putchar('\n');

	/*
	 * Data
	 */
	for (i = 0; i < desc->ncpuspos; i++) {
		int c;
		int cpu = real_cpu_num(desc, i);

		if (!mod->offline && desc->online && !is_cpu_online(desc, cpu))
			continue;
		if (!mod->online && desc->online && is_cpu_online(desc, cpu))
			continue;
		if (desc->present && !is_cpu_present(desc, cpu))
			continue;
		for (c = 0; c < ncols; c++) {
			if (mod->compat && cols[c] == COL_CACHE) {
>>>>>>> master-vanilla
				if (!desc->ncaches)
					continue;
				if (c > 0)
					putchar(',');
			}
			if (c > 0)
				putchar(',');
<<<<<<< HEAD
			print_parsable_cell(desc, i, cols[c], compatible);
=======

			data = get_cell_data(desc, i, cols[c], mod,
					     buf, sizeof(buf));
			fputs(data && *data ? data : "", stdout);
>>>>>>> master-vanilla
		}
		putchar('\n');
	}
}

<<<<<<< HEAD
=======
/*
 * [-e] backend
 */
static void
print_readable(struct lscpu_desc *desc, int cols[], int ncols,
	       struct lscpu_modifier *mod)
{
	int i;
	char buf[BUFSIZ];
	const char *data;
	struct libscols_table *table;

	scols_init_debug(0);

	table = scols_new_table();
	if (!table)
		 err(EXIT_FAILURE, _("failed to initialize output table"));

	for (i = 0; i < ncols; i++) {
		data = get_cell_header(desc, cols[i], mod, buf, sizeof(buf));
		if (!scols_table_new_column(table, xstrdup(data), 0, 0))
			err(EXIT_FAILURE, _("failed to initialize output column"));
	}

	for (i = 0; i < desc->ncpuspos; i++) {
		int c;
		struct libscols_line *line;
		int cpu = real_cpu_num(desc, i);

		if (!mod->offline && desc->online && !is_cpu_online(desc, cpu))
			continue;
		if (!mod->online && desc->online && is_cpu_online(desc, cpu))
			continue;
		if (desc->present && !is_cpu_present(desc, cpu))
			continue;

		line = scols_table_new_line(table, NULL);
		if (!line)
			err(EXIT_FAILURE, _("failed to initialize output line"));

		for (c = 0; c < ncols; c++) {
			data = get_cell_data(desc, i, cols[c], mod,
					     buf, sizeof(buf));
			if (!data || !*data)
				data = "-";
			scols_line_set_data(line, c, data);
		}
	}

	scols_print_table(table);
	scols_unref_table(table);
}
>>>>>>> master-vanilla

/* output formats "<key>  <value>"*/
#define print_s(_key, _val)	printf("%-23s%s\n", _key, _val)
#define print_n(_key, _val)	printf("%-23s%d\n", _key, _val)

static void
print_cpuset(const char *key, cpu_set_t *set, int hex)
{
	size_t setsize = CPU_ALLOC_SIZE(maxcpus);
	size_t setbuflen = 7 * maxcpus;
	char setbuf[setbuflen], *p;

	if (hex) {
		p = cpumask_create(setbuf, setbuflen, set, setsize);
		printf("%-23s0x%s\n", key, p);
	} else {
		p = cpulist_create(setbuf, setbuflen, set, setsize);
		print_s(key, p);
	}

}

<<<<<<< HEAD
static void
print_readable(struct lscpu_desc *desc, int hex)
=======
/*
 * default output
 */
static void
print_summary(struct lscpu_desc *desc, struct lscpu_modifier *mod)
>>>>>>> master-vanilla
{
	char buf[512];
	int i;
	size_t setsize = CPU_ALLOC_SIZE(maxcpus);

	print_s(_("Architecture:"), desc->arch);

	if (desc->mode) {
<<<<<<< HEAD
		char buf[64], *p = buf;
=======
		char mbuf[64], *p = mbuf;
>>>>>>> master-vanilla

		if (desc->mode & MODE_32BIT) {
			strcpy(p, "32-bit, ");
			p += 8;
		}
		if (desc->mode & MODE_64BIT) {
			strcpy(p, "64-bit, ");
			p += 8;
		}
		*(p - 2) = '\0';
<<<<<<< HEAD
		print_s(_("CPU op-mode(s):"), buf);
=======
		print_s(_("CPU op-mode(s):"), mbuf);
>>>>>>> master-vanilla
	}
#if !defined(WORDS_BIGENDIAN)
	print_s(_("Byte Order:"), "Little Endian");
#else
	print_s(_("Byte Order:"), "Big Endian");
#endif
	print_n(_("CPU(s):"), desc->ncpus);

	if (desc->online)
<<<<<<< HEAD
		print_cpuset(hex ? _("On-line CPU(s) mask:") :
				   _("On-line CPU(s) list:"),
				desc->online, hex);
=======
		print_cpuset(mod->hex ? _("On-line CPU(s) mask:") :
					_("On-line CPU(s) list:"),
				desc->online, mod->hex);
>>>>>>> master-vanilla

	if (desc->online && CPU_COUNT_S(setsize, desc->online) != desc->ncpus) {
		cpu_set_t *set;

		/* Linux kernel provides cpuset of off-line CPUs that contains
		 * all configured CPUs (see /sys/devices/system/cpu/offline),
		 * but want to print real (present in system) off-line CPUs only.
		 */
		set = cpuset_alloc(maxcpus, NULL, NULL);
		if (!set)
			err(EXIT_FAILURE, _("failed to callocate cpu set"));
		CPU_ZERO_S(setsize, set);
<<<<<<< HEAD
		for (i = 0; i < desc->ncpus; i++) {
			if (!is_cpu_online(desc, i))
				CPU_SET_S(i, setsize, set);
		}
		print_cpuset(hex ? _("Off-line CPU(s) mask:") :
				   _("Off-line CPU(s) list:"),
			     set, hex);
=======
		for (i = 0; i < desc->ncpuspos; i++) {
			int cpu = real_cpu_num(desc, i);
			if (!is_cpu_online(desc, cpu) && is_cpu_present(desc, cpu))
				CPU_SET_S(cpu, setsize, set);
		}
		print_cpuset(mod->hex ? _("Off-line CPU(s) mask:") :
					_("Off-line CPU(s) list:"),
			     set, mod->hex);
>>>>>>> master-vanilla
		cpuset_free(set);
	}

	if (desc->nsockets) {
		int cores_per_socket, sockets_per_book, books;

		cores_per_socket = sockets_per_book = books = 0;
		/* s390 detects its cpu topology via /proc/sysinfo, if present.
		 * Using simply the cpu topology masks in sysfs will not give
		 * usable results since everything is virtualized. E.g.
		 * virtual core 0 may have only 1 cpu, but virtual core 2 may
		 * five cpus.
		 * If the cpu topology is not exported (e.g. 2nd level guest)
		 * fall back to old calculation scheme.
		 */
		if (path_exist(_PATH_PROC_SYSINFO)) {
			FILE *fd = path_fopen("r", 0, _PATH_PROC_SYSINFO);
<<<<<<< HEAD
			char buf[BUFSIZ];
			int t0, t1, t2;

			while (fgets(buf, sizeof(buf), fd) != NULL) {
				if (sscanf(buf, "CPU Topology SW:%d%d%d%d%d%d",
=======
			char pbuf[BUFSIZ];
			int t0, t1, t2;

			while (fd && fgets(pbuf, sizeof(pbuf), fd) != NULL) {
				if (sscanf(pbuf, "CPU Topology SW:%d%d%d%d%d%d",
>>>>>>> master-vanilla
					   &t0, &t1, &t2, &books, &sockets_per_book,
					   &cores_per_socket) == 6)
					break;
			}
<<<<<<< HEAD
=======
			if (fd)
				fclose(fd);
>>>>>>> master-vanilla
		}
		print_n(_("Thread(s) per core:"), desc->nthreads / desc->ncores);
		print_n(_("Core(s) per socket:"),
			cores_per_socket ?: desc->ncores / desc->nsockets);
		if (desc->nbooks) {
			print_n(_("Socket(s) per book:"),
				sockets_per_book ?: desc->nsockets / desc->nbooks);
			print_n(_("Book(s):"), books ?: desc->nbooks);
		} else {
			print_n(_("Socket(s):"), sockets_per_book ?: desc->nsockets);
		}
	}
	if (desc->nnodes)
		print_n(_("NUMA node(s):"), desc->nnodes);
	if (desc->vendor)
		print_s(_("Vendor ID:"), desc->vendor);
	if (desc->family)
		print_s(_("CPU family:"), desc->family);
	if (desc->model)
		print_s(_("Model:"), desc->model);
<<<<<<< HEAD
=======
	if (desc->modelname)
		print_s(_("Model name:"), desc->modelname);
>>>>>>> master-vanilla
	if (desc->stepping)
		print_s(_("Stepping:"), desc->stepping);
	if (desc->mhz)
		print_s(_("CPU MHz:"), desc->mhz);
<<<<<<< HEAD
=======
	if (desc->maxmhz)
		print_s(_("CPU max MHz:"), desc->maxmhz[0]);
	if (desc->minmhz)
		print_s(_("CPU min MHz:"), desc->minmhz[0]);
>>>>>>> master-vanilla
	if (desc->bogomips)
		print_s(_("BogoMIPS:"), desc->bogomips);
	if (desc->virtflag) {
		if (!strcmp(desc->virtflag, "svm"))
			print_s(_("Virtualization:"), "AMD-V");
		else if (!strcmp(desc->virtflag, "vmx"))
			print_s(_("Virtualization:"), "VT-x");
	}
<<<<<<< HEAD
	if (desc->hyper) {
		print_s(_("Hypervisor vendor:"), hv_vendors[desc->hyper]);
		print_s(_("Virtualization type:"), virt_types[desc->virtype]);
	}
	if (desc->ncaches) {
		char buf[512];
		int i;

		for (i = desc->ncaches - 1; i >= 0; i--) {
			snprintf(buf, sizeof(buf),
					_("%s cache:"), desc->caches[i].name);
			print_s(buf, desc->caches[i].size);
=======
	if (desc->hypervisor)
		print_s(_("Hypervisor:"), desc->hypervisor);
	if (desc->hyper) {
		print_s(_("Hypervisor vendor:"), hv_vendors[desc->hyper]);
		print_s(_("Virtualization type:"), _(virt_types[desc->virtype]));
	}
	if (desc->dispatching >= 0)
		print_s(_("Dispatching mode:"), _(disp_modes[desc->dispatching]));
	if (desc->ncaches) {
		char cbuf[512];

		for (i = desc->ncaches - 1; i >= 0; i--) {
			snprintf(cbuf, sizeof(cbuf),
					_("%s cache:"), desc->caches[i].name);
			print_s(cbuf, desc->caches[i].size);
>>>>>>> master-vanilla
		}
	}

	for (i = 0; i < desc->nnodes; i++) {
<<<<<<< HEAD
		snprintf(buf, sizeof(buf), _("NUMA node%d CPU(s):"), i);
		print_cpuset(buf, desc->nodemaps[i], hex);
=======
		snprintf(buf, sizeof(buf), _("NUMA node%d CPU(s):"), desc->idx2nodenum[i]);
		print_cpuset(buf, desc->nodemaps[i], mod->hex);
	}

	if (desc->flags)
		print_s(_("Flags:"), desc->flags);

	if (desc->physsockets) {
		print_n(_("Physical sockets:"), desc->physsockets);
		print_n(_("Physical chips:"), desc->physchips);
		print_n(_("Physical cores/chip:"), desc->physcoresperchip);
>>>>>>> master-vanilla
	}
}

static void __attribute__((__noreturn__)) usage(FILE *out)
{
<<<<<<< HEAD
	fputs(_("\nUsage:\n"), out);
	fprintf(out,
	      _(" %s [options]\n"), program_invocation_short_name);

	fputs(_("\nOptions:\n"), out);
	fputs(_(" -h, --help          print this help\n"
		" -p, --parse <list>  print out a parsable instead of a readable format\n"
		" -s, --sysroot <dir> use directory DIR as system root\n"
		" -x, --hex           print hexadecimal masks rather than lists of CPUs\n\n"), out);
=======
	size_t i;

	fputs(USAGE_HEADER, out);
	fprintf(out, _(" %s [options]\n"), program_invocation_short_name);

	fputs(USAGE_SEPARATOR, out);
	fputs(_("Display information about the CPU architecture.\n"), out);

	fputs(USAGE_OPTIONS, out);
	fputs(_(" -a, --all               print both online and offline CPUs (default for -e)\n"), out);
	fputs(_(" -b, --online            print online CPUs only (default for -p)\n"), out);
	fputs(_(" -c, --offline           print offline CPUs only\n"), out);
	fputs(_(" -e, --extended[=<list>] print out an extended readable format\n"), out);
	fputs(_(" -p, --parse[=<list>]    print out a parsable format\n"), out);
	fputs(_(" -s, --sysroot <dir>     use specified directory as system root\n"), out);
	fputs(_(" -x, --hex               print hexadecimal masks rather than lists of CPUs\n"), out);
	fputs(USAGE_SEPARATOR, out);
	fputs(USAGE_HELP, out);
	fputs(USAGE_VERSION, out);

	fprintf(out, _("\nAvailable columns:\n"));

	for (i = 0; i < ARRAY_SIZE(coldescs); i++)
		fprintf(out, " %13s  %s\n", coldescs[i].name, _(coldescs[i].help));

	fprintf(out, USAGE_MAN_TAIL("lscpu(1)"));
>>>>>>> master-vanilla

	exit(out == stderr ? EXIT_FAILURE : EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
<<<<<<< HEAD
	struct lscpu_desc _desc, *desc = &_desc;
	int parsable = 0, c, i, hex = 0;
	int columns[ARRAY_SIZE(colnames)], ncolumns = 0;
	int compatible = 0;

	static const struct option longopts[] = {
		{ "help",	no_argument,       0, 'h' },
		{ "parse",	optional_argument, 0, 'p' },
		{ "sysroot",	required_argument, 0, 's' },
		{ "hex",	no_argument,	   0, 'x' },
		{ NULL,		0, 0, 0 }
	};

	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);

	while ((c = getopt_long(argc, argv, "hp::s:x", longopts, NULL)) != -1) {
		switch (c) {
		case 'h':
			usage(stdout);
		case 'p':
			parsable = 1;
=======
	struct lscpu_modifier _mod = { .mode = OUTPUT_SUMMARY }, *mod = &_mod;
	struct lscpu_desc _desc = { .flags = 0 }, *desc = &_desc;
	int c, i;
	int columns[ARRAY_SIZE(coldescs)], ncolumns = 0;
	int cpu_modifier_specified = 0;

	static const struct option longopts[] = {
		{ "all",        no_argument,       0, 'a' },
		{ "online",     no_argument,       0, 'b' },
		{ "offline",    no_argument,       0, 'c' },
		{ "help",	no_argument,       0, 'h' },
		{ "extended",	optional_argument, 0, 'e' },
		{ "parse",	optional_argument, 0, 'p' },
		{ "sysroot",	required_argument, 0, 's' },
		{ "hex",	no_argument,	   0, 'x' },
		{ "version",	no_argument,	   0, 'V' },
		{ NULL,		0, 0, 0 }
	};

	static const ul_excl_t excl[] = {	/* rows and cols in ASCII order */
		{ 'a','b','c' },
		{ 'e','p' },
		{ 0 }
	};
	int excl_st[ARRAY_SIZE(excl)] = UL_EXCL_STATUS_INIT;

	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);
	atexit(close_stdout);

	while ((c = getopt_long(argc, argv, "abce::hp::s:xV", longopts, NULL)) != -1) {

		err_exclusive_options(c, longopts, excl, excl_st);

		switch (c) {
		case 'a':
			mod->online = mod->offline = 1;
			cpu_modifier_specified = 1;
			break;
		case 'b':
			mod->online = 1;
			cpu_modifier_specified = 1;
			break;
		case 'c':
			mod->offline = 1;
			cpu_modifier_specified = 1;
			break;
		case 'h':
			usage(stdout);
		case 'p':
		case 'e':
>>>>>>> master-vanilla
			if (optarg) {
				if (*optarg == '=')
					optarg++;
				ncolumns = string_to_idarray(optarg,
						columns, ARRAY_SIZE(columns),
						column_name_to_id);
				if (ncolumns < 0)
					return EXIT_FAILURE;
<<<<<<< HEAD
			} else {
				columns[ncolumns++] = COL_CPU;
				columns[ncolumns++] = COL_CORE;
				columns[ncolumns++] = COL_SOCKET;
				columns[ncolumns++] = COL_NODE;
				columns[ncolumns++] = COL_CACHE;
				compatible = 1;
			}
			break;
		case 's':
			sysrootlen = strlen(optarg);
			strncpy(pathbuf, optarg, sizeof(pathbuf));
			pathbuf[sizeof(pathbuf) - 1] = '\0';
			break;
		case 'x':
			hex = 1;
			break;
=======
			}
			mod->mode = c == 'p' ? OUTPUT_PARSABLE : OUTPUT_READABLE;
			break;
		case 's':
			path_set_prefix(optarg);
			mod->system = SYSTEM_SNAPSHOT;
			break;
		case 'x':
			mod->hex = 1;
			break;
		case 'V':
			printf(UTIL_LINUX_VERSION);
			return EXIT_SUCCESS;
>>>>>>> master-vanilla
		default:
			usage(stderr);
		}
	}

<<<<<<< HEAD
	memset(desc, 0, sizeof(*desc));

	read_basicinfo(desc);

	for (i = 0; i < desc->ncpus; i++) {
		if (desc->online && !is_cpu_online(desc, i))
			continue;
		read_topology(desc, i);
		read_cache(desc, i);
	}

	qsort(desc->caches, desc->ncaches, sizeof(struct cpu_cache), cachecmp);

	read_nodes(desc);

	read_hypervisor(desc);

	/* Show time! */
	if (parsable)
		print_parsable(desc, columns, ncolumns, compatible);
	else
		print_readable(desc, hex);
=======
	if (cpu_modifier_specified && mod->mode == OUTPUT_SUMMARY) {
		fprintf(stderr,
			_("%s: options --all, --online and --offline may only "
			  "be used with options --extended or --parse.\n"),
			program_invocation_short_name);
		return EXIT_FAILURE;
	}

	if (argc != optind)
		usage(stderr);

	/* set default cpu display mode if none was specified */
	if (!mod->online && !mod->offline) {
		mod->online = 1;
		mod->offline = mod->mode == OUTPUT_READABLE ? 1 : 0;
	}

	read_basicinfo(desc, mod);

	for (i = 0; i < desc->ncpuspos; i++) {
		read_topology(desc, i);
		read_cache(desc, i);
		read_polarization(desc, i);
		read_address(desc, i);
		read_configured(desc, i);
		read_max_mhz(desc, i);
		read_min_mhz(desc, i);
	}

	if (desc->caches)
		qsort(desc->caches, desc->ncaches,
				sizeof(struct cpu_cache), cachecmp);

	read_nodes(desc);
	read_hypervisor(desc, mod);

	switch(mod->mode) {
	case OUTPUT_SUMMARY:
		print_summary(desc, mod);
		break;
	case OUTPUT_PARSABLE:
		if (!ncolumns) {
			columns[ncolumns++] = COL_CPU;
			columns[ncolumns++] = COL_CORE;
			columns[ncolumns++] = COL_SOCKET;
			columns[ncolumns++] = COL_NODE;
			columns[ncolumns++] = COL_CACHE;
			mod->compat = 1;
		}
		print_parsable(desc, columns, ncolumns, mod);
		break;
	case OUTPUT_READABLE:
		if (!ncolumns) {
			/* No list was given. Just print whatever is there. */
			columns[ncolumns++] = COL_CPU;
			if (desc->nodemaps)
				columns[ncolumns++] = COL_NODE;
			if (desc->bookmaps)
				columns[ncolumns++] = COL_BOOK;
			if (desc->socketmaps)
				columns[ncolumns++] = COL_SOCKET;
			if (desc->coremaps)
				columns[ncolumns++] = COL_CORE;
			if (desc->caches)
				columns[ncolumns++] = COL_CACHE;
			if (desc->online)
				columns[ncolumns++] = COL_ONLINE;
			if (desc->configured)
				columns[ncolumns++] = COL_CONFIGURED;
			if (desc->polarization)
				columns[ncolumns++] = COL_POLARIZATION;
			if (desc->addresses)
				columns[ncolumns++] = COL_ADDRESS;
			if (desc->maxmhz)
				columns[ncolumns++] = COL_MAXMHZ;
			if (desc->minmhz)
				columns[ncolumns++] = COL_MINMHZ;
		}
		print_readable(desc, columns, ncolumns, mod);
		break;
	}
>>>>>>> master-vanilla

	return EXIT_SUCCESS;
}
