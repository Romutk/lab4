/* setterm.c, set terminal attributes.
 *
 * Copyright (C) 1990 Gordon Irlam (gordoni@cs.ua.oz.au).  Conditions of use,
 * modification, and redistribution are contained in the file COPYRIGHT that
 * forms part of this distribution.
 * 
 * Adaption to Linux by Peter MacDonald.
 *
 * Enhancements by Mika Liljeberg (liljeber@cs.Helsinki.FI)
 *
 * Beep modifications by Christophe Jolif (cjolif@storm.gatelink.fr.net)
 *
 * Sanity increases by Cafeine Addict [sic].
 *
 * Powersave features by todd j. derr <tjd@wordsmith.org>
 *
 * Converted to terminfo by Kars de Jong (jongk@cs.utwente.nl)
 *
<<<<<<< HEAD
 * 1999-02-22 Arkadiusz Mi∂kiewicz <misiek@pld.ORG.PL>
 * - added Native Language Support
 *
 *
 * Syntax:
 *
 * setterm
 *   [ -term terminal_name ]
 *   [ -reset ]
 *   [ -initialize ]
 *   [ -cursor [on|off] ]
 *   [ -repeat [on|off] ]
 *   [ -appcursorkeys [on|off] ]
 *   [ -linewrap [on|off] ]
 *   [ -snow [on|off] ]
 *   [ -softscroll [on|off] ]
 *   [ -defaults ]
 *   [ -foreground black|red|green|yellow|blue|magenta|cyan|white|default ]
 *   [ -background black|red|green|yellow|blue|magenta|cyan|white|default ]
 *   [ -ulcolor black|grey|red|green|yellow|blue|magenta|cyan|white ]
 *   [ -ulcolor bright red|green|yellow|blue|magenta|cyan|white ]
 *   [ -hbcolor black|grey|red|green|yellow|blue|magenta|cyan|white ]
 *   [ -hbcolor bright red|green|yellow|blue|magenta|cyan|white ]
 *   [ -inversescreen [on|off] ]
 *   [ -bold [on|off] ]
 *   [ -half-bright [on|off] ]
 *   [ -blink [on|off] ]
 *   [ -reverse [on|off] ]
 *   [ -underline [on|off] ]
 *   [ -store ]
 *   [ -clear [ all|rest ] ]
 *   [ -tabs [tab1 tab2 tab3 ... ] ]     (tabn = 1-160)
 *   [ -clrtabs [ tab1 tab2 tab3 ... ]   (tabn = 1-160)
 *   [ -regtabs [1-160] ]
 *   [ -blank [0-60|force|poke|] ]
 *   [ -dump   [1-NR_CONS ] ]
 *   [ -append [1-NR_CONS ] ]
 *   [ -file dumpfilename ]
 *   [ -standout [attr] ]
 *   [ -msg [on|off] ]
 *   [ -msglevel [0-8] ]
 *   [ -powersave [on|vsync|hsync|powerdown|off] ]
 *   [ -powerdown [0-60] ]
 *   [ -blength [0-2000] ]
 *   [ -bfreq freq ]
 *   [ -version ]
 *   [ -help ]
 *
 *
 * Semantics:
 *
 * Setterm writes to standard output a character string that will
 * invoke the specified terminal capabilities.  Where possibile
=======
 * 1999-02-22 Arkadiusz Mi≈õkiewicz <misiek@pld.ORG.PL>
 * - added Native Language Support
 *
 * Semantics:
 *
 * Setterm writes to standard output a character string that will
 * invoke the specified terminal capabilities.  Where possible
>>>>>>> master-vanilla
 * terminfo is consulted to find the string to use.  Some options
 * however do not correspond to a terminfo capability.  In this case if
 * the terminal type is "con*", or "linux*" the string that invokes
 * the specified capabilities on the PC Linux virtual console driver
 * is output.  Options that are not implemented by the terminal are
 * ignored.
 *
 * The following options are non-obvious.
 *
 *   -term can be used to override the TERM environment variable.
 *
 *   -reset displays the terminal reset string, which typically resets the
 *      terminal to its power on state.
 *
 *   -initialize displays the terminal initialization string, which typically
 *      sets the terminal's rendering options, and other attributes to the
 *      default values.
 *
 *   -default sets the terminal's rendering options to the default values.
 *
 *   -store stores the terminal's current rendering options as the default
 *      values.  */

<<<<<<< HEAD
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <fcntl.h>
#ifndef NCURSES_CONST
#define NCURSES_CONST const	/* define before including term.h */
#endif
#include <term.h>

#ifdef HAVE_NCURSES_H
#include <ncurses.h>
#elif defined(HAVE_NCURSES_NCURSES_H)
#include <ncurses/ncurses.h>
#endif

#include <sys/param.h>		/* for MAXPATHLEN */
#include <sys/ioctl.h>
#include <sys/time.h>
#ifdef HAVE_LINUX_TIOCL_H
#include <linux/tiocl.h>
#endif

#include "c.h"
#include "xalloc.h"
#include "nls.h"

#if __GNU_LIBRARY__ < 5
#ifndef __alpha__
# include <linux/unistd.h>
#define __NR_klogctl __NR_syslog
_syscall3(int, klogctl, int, type, char*, buf, int, len);
#else /* __alpha__ */
#define klogctl syslog
#endif
#endif
extern int klogctl(int type, char *buf, int len);
=======
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/klog.h>
#include <sys/param.h>		/* for MAXPATHLEN */
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>

#ifndef NCURSES_CONST
# define NCURSES_CONST const	/* define before including term.h */
#endif
#ifdef HAVE_NCURSES_H
# include <ncurses.h>
#elif defined(HAVE_NCURSES_NCURSES_H)
# include <ncurses/ncurses.h>
#endif
/* must include after ncurses.h */
#include <term.h>

#ifdef HAVE_LINUX_TIOCL_H
# include <linux/tiocl.h>
#endif

#include "c.h"
#include "closestream.h"
#include "nls.h"
#include "optutils.h"
#include "strutils.h"
#include "xalloc.h"
>>>>>>> master-vanilla

/* Constants. */

/* Non-standard return values. */
#define EXIT_DUMPFILE	-1

<<<<<<< HEAD
/* Keyboard types. */
#define PC	 0
#define OLIVETTI 1
#define DUTCH    2
#define EXTENDED 3

/* Colors. */
#define BLACK   0
#define RED     1
#define GREEN   2
#define YELLOW  3
#define BLUE    4
#define MAGENTA 5
#define CYAN    6
#define WHITE   7
#define GREY	8
#define DEFAULT 9

/* Blank commands */
#define BLANKSCREEN	-1
#define UNBLANKSCREEN	-2
#define BLANKEDSCREEN	-3

/* <linux/tiocl.h> fallback */
#ifndef TIOCL_BLANKSCREEN
# define TIOCL_UNBLANKSCREEN	4       /* unblank screen */
# define TIOCL_SETVESABLANK	10      /* set vesa blanking mode */
# define TIOCL_BLANKSCREEN	14	/* keep screen blank even if a key is pressed */
# define TIOCL_BLANKEDSCREEN	15	/* return which vt was blanked */
#endif

/* Control sequences. */
#define ESC "\033"
#define DCS "\033P"
#define ST  "\033\\"

/* Static variables. */

/* Option flags.  Set if the option is to be invoked. */
int opt_term, opt_reset, opt_initialize, opt_cursor;
int opt_linewrap, opt_snow, opt_softscroll, opt_default, opt_foreground;
int opt_background, opt_bold, opt_blink, opt_reverse, opt_underline;
int opt_store, opt_clear, opt_blank, opt_snap, opt_snapfile, opt_standout;
int opt_append, opt_ulcolor, opt_hbcolor, opt_halfbright, opt_repeat;
int opt_tabs, opt_clrtabs, opt_regtabs, opt_appcursorkeys, opt_inversescreen;
int opt_msg, opt_msglevel, opt_powersave, opt_powerdown;
int opt_blength, opt_bfreq;

/* Option controls.  The variable names have been contracted to ensure
 * uniqueness.
 */
char *opt_te_terminal_name;	/* Terminal name. */
int opt_cu_on, opt_li_on, opt_sn_on, opt_so_on, opt_bo_on, opt_hb_on, opt_bl_on;
int opt_re_on, opt_un_on, opt_rep_on, opt_appck_on, opt_invsc_on;
int opt_msg_on;			/* Boolean switches. */
int opt_ke_type;		/* Keyboard type. */
int opt_fo_color, opt_ba_color;	/* Colors. */
int opt_ul_color, opt_hb_color;
int opt_cl_all;			/* Clear all or rest. */
int opt_bl_min;			/* Blank screen. */
int opt_blength_l;
int opt_bfreq_f;
int opt_sn_num;			/* Snap screen. */
int opt_st_attr;
int opt_rt_len;			/* regular tab length */
int opt_tb_array[161];		/* Array for tab list */
int opt_msglevel_num;
int opt_ps_mode, opt_pd_min;	/* powersave mode/powerdown time */

char opt_sn_name[PATH_MAX + 1] = "screen.dump";

static void screendump(int vcnum, FILE *F);

/* Command line parsing routines.
 *
 * Note that it is an error for a given option to be invoked more than once.
 */

static void
parse_term(int argc, char **argv, int *option, char **opt_term, int *bad_arg) {
	/* argc: Number of arguments for this option. */
	/* argv: Arguments for this option. */
	/* option: Term flag to set. */
	/* opt_term: Terminal name to set. */
	/* bad_arg: Set to true if an error is detected. */

/* Parse a -term specification. */

	if (argc != 1 || *option)
		*bad_arg = TRUE;
	*option = TRUE;
	if (argc == 1)
		*opt_term = argv[0];
}

static void
parse_none(int argc, char **argv __attribute__ ((__unused__)), int *option, int *bad_arg) {
	/* argc: Number of arguments for this option. */
	/* argv: Arguments for this option. */
	/* option: Term flag to set. */
	/* bad_arg: Set to true if an error is detected. */

/* Parse a parameterless specification. */

	if (argc != 0 || *option)
		*bad_arg = TRUE;
	*option = TRUE;
}

static void
parse_switch(int argc, char **argv, int *option, int *opt_on, int *bad_arg) {
	/* argc: Number of arguments for this option. */
	/* argv: Arguments for this option. */
	/* option: Option flag to set. */
	/* opt_on: Boolean option switch to set or reset. */
	/* bad_arg: Set to true if an error is detected. */

/* Parse a boolean (on/off) specification. */

	if (argc > 1 || *option)
		*bad_arg = TRUE;
	*option = TRUE;
	if (argc == 1) {
		if (strcmp(argv[0], "on") == 0)
			*opt_on = TRUE;
		else if (strcmp(argv[0], "off") == 0)
			*opt_on = FALSE;
		else
			*bad_arg = TRUE;
	} else {
		*opt_on = TRUE;
	}
}

static void
par_color(int argc, char **argv, int *option, int *opt_color, int *bad_arg) {
	/* argc: Number of arguments for this option. */
	/* argv: Arguments for this option. */
	/* option: Color flag to set. */
	/* opt_color: Color to set. */
	/* bad_arg: Set to true if an error is detected. */

/* Parse a -foreground or -background specification. */

	if (argc != 1 || *option)
		*bad_arg = TRUE;
	*option = TRUE;
	if (argc == 1) {
		if (strcmp(argv[0], "black") == 0)
			*opt_color = BLACK;
		else if (strcmp(argv[0], "red") == 0)
			*opt_color = RED;
		else if (strcmp(argv[0], "green") == 0)
			*opt_color = GREEN;
		else if (strcmp(argv[0], "yellow") == 0)
			*opt_color = YELLOW;
		else if (strcmp(argv[0], "blue") == 0)
			*opt_color = BLUE;
		else if (strcmp(argv[0], "magenta") == 0)
			*opt_color = MAGENTA;
		else if (strcmp(argv[0], "cyan") == 0)
			*opt_color = CYAN;
		else if (strcmp(argv[0], "white") == 0)
			*opt_color = WHITE;
		else if (strcmp(argv[0], "default") == 0)
			*opt_color = DEFAULT;
		else if (isdigit(argv[0][0]))
			*opt_color = atoi(argv[0]);
		else
			*bad_arg = TRUE;

		if(*opt_color < 0 || *opt_color > 9 || *opt_color == 8)
			*bad_arg = TRUE;
	}
}

static void
par_color2(int argc, char **argv, int *option, int *opt_color, int *bad_arg) {
	/* argc: Number of arguments for this option. */
	/* argv: Arguments for this option. */
	/* option: Color flag to set. */
	/* opt_color: Color to set. */
	/* bad_arg: Set to true if an error is detected. */

/* Parse a -ulcolor or -hbcolor specification. */

	if (!argc || argc > 2 || *option)
		*bad_arg = TRUE;
	*option = TRUE;
	*opt_color = 0;
	if (argc == 2) {
		if (strcmp(argv[0], "bright") == 0)
			*opt_color = 8;
		else {
			*bad_arg = TRUE;
			return;
		}
	}
	if (argc) {
		if (strcmp(argv[argc-1], "black") == 0) {
			if(*opt_color)
				*bad_arg = TRUE;
			else
				*opt_color = BLACK;
		} else if (strcmp(argv[argc-1], "grey") == 0) {
			if(*opt_color)
				*bad_arg = TRUE;
			else
				*opt_color = GREY;
		} else if (strcmp(argv[argc-1], "red") == 0)
			*opt_color |= RED;
		else if (strcmp(argv[argc-1], "green") == 0)
			*opt_color |= GREEN;
		else if (strcmp(argv[argc-1], "yellow") == 0)
			*opt_color |= YELLOW;
		else if (strcmp(argv[argc-1], "blue") == 0)
			*opt_color |= BLUE;
		else if (strcmp(argv[argc-1], "magenta") == 0)
			*opt_color |= MAGENTA;
		else if (strcmp(argv[argc-1], "cyan") == 0)
			*opt_color |= CYAN;
		else if (strcmp(argv[argc-1], "white") == 0)
			*opt_color |= WHITE;
		else if (isdigit(argv[argc-1][0]))
			*opt_color = atoi(argv[argc-1]);
		else
			*bad_arg = TRUE;
		if(*opt_color < 0 || *opt_color > 15)
			*bad_arg = TRUE;
	}
}

static void
parse_clear(int argc, char **argv, int *option, int *opt_all, int *bad_arg) {
	/* argc: Number of arguments for this option. */
	/* argv: Arguments for this option. */
	/* option: Clear flag to set. */
	/* opt_all: Clear all switch to set or reset. */
	/* bad_arg: Set to true if an error is detected. */

/* Parse a -clear specification. */

	if (argc > 1 || *option)
		*bad_arg = TRUE;
	*option = TRUE;
	if (argc == 1) {
		if (strcmp(argv[0], "all") == 0)
			*opt_all = TRUE;
		else if (strcmp(argv[0], "rest") == 0)
			*opt_all = FALSE;
		else
			*bad_arg = TRUE;
	} else {
		*opt_all = TRUE;
	}
}

static void
parse_blank(int argc, char **argv, int *option, int *opt_all, int *bad_arg) {
	/* argc: Number of arguments for this option. */
	/* argv: Arguments for this option. */
	/* option: Clear flag to set. */
	/* opt_all: Clear all switch to set or reset. */
	/* bad_arg: Set to true if an error is detected. */

/* Parse a -blank specification. */

	if (argc > 1 || *option)
		*bad_arg = TRUE;
	*option = TRUE;
	if (argc == 1) {
		if (!strcmp(argv[0], "force"))
			*opt_all = BLANKSCREEN;
		else if (!strcmp(argv[0], "poke"))
			*opt_all = UNBLANKSCREEN;
		else {
			*opt_all = atoi(argv[0]);
			if ((*opt_all > 60) || (*opt_all < 0))
				*bad_arg = TRUE;
		}
	} else {
		*opt_all = BLANKEDSCREEN;
	}
}

static void
parse_powersave(int argc, char **argv, int *option, int *opt_mode, int *bad_arg) {
	/* argc: Number of arguments for this option. */
	/* argv: Arguments for this option. */
	/* option: powersave flag to set. */
	/* opt_mode: Powersaving mode, defined in vesa_blank.c */
	/* bad_arg: Set to true if an error is detected. */

/* Parse a -powersave mode specification. */

	if (argc > 1 || *option)
		*bad_arg = TRUE;
	*option = TRUE;
	if (argc == 1) {
		if (strcmp(argv[0], "on") == 0)
			*opt_mode = 1;
		else if (strcmp(argv[0], "vsync") == 0)
			*opt_mode = 1;
		else if (strcmp(argv[0], "hsync") == 0)
			*opt_mode = 2;
		else if (strcmp(argv[0], "powerdown") == 0)
			*opt_mode = 3;
		else if (strcmp(argv[0], "off") == 0)
			*opt_mode = 0;
		else
			*bad_arg = TRUE;
	} else {
		*opt_mode = 0;
	}
}

#if 0
static void
parse_standout(int argc, char *argv, int *option, int *opt_all, int *bad_arg) {
	/* argc: Number of arguments for this option. */
	/* argv: Arguments for this option. */
	/* option: Clear flag to set. */
	/* opt_all: Clear all switch to set or reset. */
	/* bad_arg: Set to true if an error is detected. */

/* Parse a -standout specification. */

	if (argc > 1 || *option)
		*bad_arg = TRUE;
	*option = TRUE;
	if (argc == 1)
		*opt_all = atoi(argv[0]);
	else
		*opt_all = -1;
}
#endif

static void
parse_msglevel(int argc, char **argv, int *option, int *opt_all, int *bad_arg) {
	/* argc: Number of arguments for this option. */
	/* argv: Arguments for this option. */
	/* option: Clear flag to set. */
	/* opt_all: Clear all switch to set or reset. */
	/* bad_arg: Set to true if an error is detected. */

	if (argc > 1 || *option)
		*bad_arg = TRUE;
	*option = TRUE;
	if (argc == 1) {
		*opt_all = atoi(argv[0]);
		if (*opt_all < 0 || *opt_all > 8)
			*bad_arg = TRUE;
	} else {
		*opt_all = -1;
	}
}

static void
parse_snap(int argc, char **argv, int *option, int *opt_all, int *bad_arg) {
	/* argc: Number of arguments for this option. */
	/* argv: Arguments for this option. */
	/* option: Clear flag to set. */
	/* opt_all: Clear all switch to set or reset. */
	/* bad_arg: Set to true if an error is detected. */

/* Parse a -dump or -append specification. */

	if (argc > 1 || *option)
		*bad_arg = TRUE;
	*option = TRUE;
	if (argc == 1) {
		*opt_all = atoi(argv[0]);
		if ((*opt_all <= 0))
			*bad_arg = TRUE;
	} else {
		*opt_all = 0;
	}
}

static void
parse_snapfile(int argc, char **argv, int *option, int *opt_all, int *bad_arg) {
	/* argc: Number of arguments for this option. */
	/* argv: Arguments for this option. */
	/* option: Clear flag to set. */
	/* opt_all: Clear all switch to set or reset. */
	/* bad_arg: Set to true if an error is detected. */

/* Parse a -file specification. */

	if (argc != 1 || *option)
		*bad_arg = TRUE;
	*option = TRUE;
	memset(opt_all, 0, PATH_MAX + 1);
	if (argc == 1)
		strncpy((char *)opt_all, argv[0], PATH_MAX);
}

static void
parse_tabs(int argc, char **argv, int *option, int *tab_array, int *bad_arg) {
	/* argc: Number of arguments for this option. */
	/* argv: Arguments for this option. */
	/* option: Clear flag to set. */
	/* tab_array: Array of tabs */
	/* bad_arg: Set to true if an error is detected. */

	if (*option || argc > 160)
		*bad_arg = TRUE;
	*option = TRUE;
	tab_array[argc] = -1;
	while(argc--) {
		tab_array[argc] = atoi(argv[argc]);
		if(tab_array[argc] < 1 || tab_array[argc] > 160) {
			*bad_arg = TRUE;
			return;
		}
	}
}

static void
parse_clrtabs(int argc, char **argv, int *option, int *tab_array, int *bad_arg) {
	/* argc: Number of arguments for this option. */
	/* argv: Arguments for this option. */
	/* option: Clear flag to set. */
	/* tab_array: Array of tabs */
	/* bad_arg: Set to true if an error is detected. */

	if (*option || argc > 160)
		*bad_arg = TRUE;
	*option = TRUE;
	if(argc == 0) {
		tab_array[0] = -1;
		return;
	}
	tab_array[argc] = -1;
	while(argc--) {
		tab_array[argc] = atoi(argv[argc]);
		if(tab_array[argc] < 1 || tab_array[argc] > 160) {
			*bad_arg = TRUE;
			return;
		}
	}
}

static void
parse_regtabs(int argc, char **argv, int *option, int *opt_len, int *bad_arg) {
	/* argc: Number of arguments for this option. */
	/* argv: Arguments for this option. */
	/* option: Clear flag to set. */
	/* opt_len: Regular tab length. */
	/* bad_arg: Set to true if an error is detected. */

	if (*option || argc > 1)
		*bad_arg = TRUE;
	*option = TRUE;
	if(argc == 0) {
		*opt_len = 8;
		return;
	}
	*opt_len = atoi(argv[0]);
	if(*opt_len < 1 || *opt_len > 160) {
		*bad_arg = TRUE;
		return;
	}
}


static void
parse_blength(int argc, char **argv, int *option, int *opt_all, int *bad_arg) {
	/* argc: Number of arguments for this option. */
	/* argv: Arguments for this option. */
	/* option: Clear flag to set. */
	/* opt_all */
	/* bad_arg: Set to true if an error is detected. */

/* Parse  -blength specification. */

	if (argc > 1 || *option)
		*bad_arg = TRUE;
	*option = TRUE;
	if (argc == 1) {
		*opt_all = atoi(argv[0]);
		if (*opt_all > 2000)
			*bad_arg = TRUE;
	} else {
		*opt_all = 0;
	}
}

static void
parse_bfreq(int argc, char **argv, int *option, int *opt_all, int *bad_arg) {
	/* argc: Number of arguments for this option. */
	/* argv: Arguments for this option. */
	/* option: Clear flag to set. */
	/* opt_all */
	/* bad_arg: Set to true if an error is detected. */

/* Parse  -bfreq specification. */

	if (argc > 1 || *option)
		*bad_arg = TRUE;
	*option = TRUE;
	if (argc == 1) {
		*opt_all = atoi(argv[0]);
	} else {
		*opt_all = 0;
	}
}


static void
show_tabs(void) {
	int i, co = tigetnum("cols");

	if(co > 0) {
		printf("\r         ");
		for(i = 10; i < co-2; i+=10)
			printf("%-10d", i);
		putchar('\n');
		for(i = 1; i <= co; i++)
			putchar(i%10+'0');
		putchar('\n');
		for(i = 1; i < co; i++)
=======
/* Colors. */
enum {
	BLACK = 0,
	RED,
	GREEN,
	YELLOW,
	BLUE,
	MAGENTA,
	CYAN,
	WHITE,
	GREY,
	DEFAULT
};

/* Blank commands */
enum {
	BLANKSCREEN	= -1,
	UNBLANKSCREEN	= -2,
	BLANKEDSCREEN	= -3
};

/* <linux/tiocl.h> fallback */
#ifndef TIOCL_BLANKSCREEN
enum {
	TIOCL_UNBLANKSCREEN	=  4,	/* unblank screen */
	TIOCL_SETVESABLANK	= 10,	/* set vesa blanking mode */
	TIOCL_BLANKSCREEN	= 14,	/* keep screen blank even if a key is pressed */
	TIOCL_BLANKEDSCREEN	= 15	/* return which vt was blanked */
};
#endif

/* Powersave modes */
enum {
	VESA_BLANK_MODE_OFF = 0,
	VESA_BLANK_MODE_SUSPENDV,
	VESA_BLANK_MODE_SUSPENDH,
	VESA_BLANK_MODE_POWERDOWN
};

/* klogctl() actions */
enum {
	SYSLOG_ACTION_CONSOLE_OFF	= 6,
	SYSLOG_ACTION_CONSOLE_ON	= 7,
	SYSLOG_ACTION_CONSOLE_LEVEL	= 8
};

/* Console log levels */
enum {
	CONSOLE_LEVEL_MIN = 0,
	CONSOLE_LEVEL_MAX = 8
};

/* Various numbers  */
#define DEFAULT_TAB_LEN	8
#define	BLANK_MAX	60
#define	TABS_MAX	160
#define BLENGTH_MAX	2000

/* Command controls. */
struct setterm_control {
	char *opt_te_terminal_name;	/* terminal name */
	int opt_bl_min;		/* blank screen */
	int opt_blength_l;	/* bell duration in milliseconds */
	int opt_bfreq_f;	/* bell frequency in Hz */
	int opt_sn_num;		/* console number to be snapshoted */
	char *opt_sn_name;	/* path to write snap */
	char *in_device;	/* device to snapshot */
	int opt_msglevel_num;	/* printk() loging level */
	int opt_ps_mode;	/* powersave mode */
	int opt_pd_min;		/* powerdown time */
	int opt_rt_len;		/* regular tab length */
	int opt_tb_array[TABS_MAX + 1];	/* array for tab list */
	/* colors */
	int opt_fo_color:4, opt_ba_color:4, opt_ul_color:4, opt_hb_color:4;
	/* boolean options */
	unsigned int opt_cu_on:1, opt_li_on:1, opt_bo_on:1, opt_hb_on:1,
	    opt_bl_on:1, opt_re_on:1, opt_un_on:1, opt_rep_on:1,
	    opt_appck_on:1, opt_invsc_on:1, opt_msg_on:1, opt_cl_all:1,
	    vcterm:1;
	/* Option flags.  Set when an option is invoked. */
	uint64_t opt_term:1, opt_reset:1, opt_initialize:1, opt_cursor:1,
	    opt_linewrap:1, opt_default:1, opt_foreground:1,
	    opt_background:1, opt_bold:1, opt_blink:1, opt_reverse:1,
	    opt_underline:1, opt_store:1, opt_clear:1, opt_blank:1,
	    opt_snap:1, opt_snapfile:1, opt_append:1, opt_ulcolor:1,
	    opt_hbcolor:1, opt_halfbright:1, opt_repeat:1, opt_tabs:1,
	    opt_clrtabs:1, opt_regtabs:1, opt_appcursorkeys:1,
	    opt_inversescreen:1, opt_msg:1, opt_msglevel:1, opt_powersave:1,
	    opt_powerdown:1, opt_blength:1, opt_bfreq:1;
};

static int parse_febg_color(const char *arg)
{
	int color;

	if (strcmp(arg, "black") == 0)
		return BLACK;
	else if (strcmp(arg, "red") == 0)
		return RED;
	else if (strcmp(arg, "green") == 0)
		return GREEN;
	else if (strcmp(arg, "yellow") == 0)
		return YELLOW;
	else if (strcmp(arg, "blue") == 0)
		return BLUE;
	else if (strcmp(arg, "magenta") == 0)
		return MAGENTA;
	else if (strcmp(arg, "cyan") == 0)
		return CYAN;
	else if (strcmp(arg, "white") == 0)
		return WHITE;
	else if (strcmp(arg, "default") == 0)
		return DEFAULT;
	else
		color = strtos32_or_err(arg, _("argument error"));
	if (color < BLACK || DEFAULT < color || color == GREY)
		errx(EXIT_FAILURE, _("argument error: %s"), arg);
	return color;
}

static int parse_ulhb_color(char **argv, int *optind)
{
	char *color_name;
	int bright = 0;
	int color = -1;

	if (argv[*optind] && strcmp(argv[*optind - 1], "bright") == 0) {
		bright = 1;
		color_name = argv[*optind];
		(*optind)++;
	} else
		color_name = argv[*optind - 1];

	if (strcmp(color_name, "black") == 0)
		color = BLACK;
	else if (strcmp(color_name, "grey") == 0)
		color = GREY;
	else if (strcmp(color_name, "red") == 0)
		color = RED;
	else if (strcmp(color_name, "green") == 0)
		color = GREEN;
	else if (strcmp(color_name, "yellow") == 0)
		color = YELLOW;
	else if (strcmp(color_name, "blue") == 0)
		color = BLUE;
	else if (strcmp(color_name, "magenta") == 0)
		color = MAGENTA;
	else if (strcmp(color_name, "cyan") == 0)
		color = CYAN;
	else if (strcmp(color_name, "white") == 0)
		color = WHITE;
	else {
		color = strtos32_or_err(color_name, _("argument error"));
		if (color < BLACK || DEFAULT < color)
			errx(EXIT_FAILURE, _("argument error: %s"), color_name);
	}
	if (bright && (color == BLACK || color == GREY))
		errx(EXIT_FAILURE, _("argument error: bright %s is not supported"), color_name);

	return color;
}

static char *find_optional_arg(char **argv, char *optarg, int *optind)
{
	char *arg;
	if (optarg)
		return optarg;
	else {
		arg = argv[*optind];
		if (!arg || arg[0] == '-')
			return NULL;
	}
	(*optind)++;
	return arg;
}

static int parse_blank(char **argv, char *optarg, int *optind)
{
	char *arg;

	arg = find_optional_arg(argv, optarg, optind);
	if (!arg)
		return BLANKEDSCREEN;
	if (!strcmp(arg, "force"))
		return BLANKSCREEN;
	else if (!strcmp(arg, "poke"))
		return UNBLANKSCREEN;
	else {
		int ret;

		ret = strtos32_or_err(arg, _("argument error"));
		if (ret < 0 || BLANK_MAX < ret)
			errx(EXIT_FAILURE, _("argument error: %s"), arg);
		return ret;
	}
}

static int parse_powersave(const char *arg)
{
	if (strcmp(arg, "on") == 0)
		return VESA_BLANK_MODE_SUSPENDV;
	else if (strcmp(arg, "vsync") == 0)
		return VESA_BLANK_MODE_SUSPENDV;
	else if (strcmp(arg, "hsync") == 0)
		return VESA_BLANK_MODE_SUSPENDH;
	else if (strcmp(arg, "powerdown") == 0)
		return VESA_BLANK_MODE_POWERDOWN;
	else if (strcmp(arg, "off") == 0)
		return VESA_BLANK_MODE_OFF;
	errx(EXIT_FAILURE, _("argument error: %s"), arg);
}

static int parse_msglevel(const char *arg)
{
	int ret;

	ret = strtos32_or_err(arg, _("argument error"));
	if (ret < CONSOLE_LEVEL_MIN || CONSOLE_LEVEL_MAX < ret)
		errx(EXIT_FAILURE, _("argument error: %s"), arg);
	return ret;
}

static int parse_snap(char **argv, char *optarg, int *optind)
{
	int ret;
	char *arg;

	arg = find_optional_arg(argv, optarg, optind);
	if (!arg)
		return 0;
	ret = strtos32_or_err(arg, _("argument error"));
	if (ret < 1)
		errx(EXIT_FAILURE, _("argument error: %s"), arg);
	return ret;
}

static void parse_tabs(char **argv, char *optarg, int *optind, int *tab_array)
{
	int i = 0;

	if (optarg) {
		tab_array[i] = strtos32_or_err(optarg, _("argument error"));
		i++;
	}
	while (argv[*optind]) {
		if (TABS_MAX < i)
			errx(EXIT_FAILURE, _("too many tabs"));
		if (argv[*optind][0] == '-')
			break;
		tab_array[i] = strtos32_or_err(argv[*optind], _("argument error"));
		(*optind)++;
		i++;
	}
	tab_array[i] = -1;
}

static int parse_regtabs(char **argv, char *optarg, int *optind)
{
	int ret;
	char *arg;

	arg = find_optional_arg(argv, optarg, optind);
	if (!arg)
		return DEFAULT_TAB_LEN;
	ret = strtos32_or_err(arg, _("argument error"));
	if (ret < 1 || TABS_MAX < ret)
		errx(EXIT_FAILURE, _("argument error: %s"), arg);
	return ret;
}

static int parse_blength(char **argv, char *optarg, int *optind)
{
	int ret = -1;
	char *arg;

	arg = find_optional_arg(argv, optarg, optind);
	if (!arg)
		return 0;
	ret = strtos32_or_err(arg, _("argument error"));
	if (ret < 0 || BLENGTH_MAX < ret)
		errx(EXIT_FAILURE, _("argument error: %s"), arg);
	return ret;
}

static int parse_bfreq(char **argv, char *optarg, int *optind)
{
	char *arg;

	arg = find_optional_arg(argv, optarg, optind);
	if (!arg)
		return 0;
	return strtos32_or_err(arg, _("argument error"));
}

static void __attribute__((__noreturn__)) usage(FILE *out)
{
	fputs(USAGE_HEADER, out);
	fprintf(out,
	      _(" %s [options]\n"), program_invocation_short_name);

	fputs(USAGE_SEPARATOR, out);
	fputs(_("Set the attributes of a terminal.\n"), out);

	fputs(USAGE_OPTIONS, out);
	fputs(_(" --term          <terminal_name>   override TERM environment variable\n"), out);
	fputs(_(" --reset                           reset terminal to power-on state\n"), out);
	fputs(_(" --initialize                      display init string, and use default settings\n"), out);
	fputs(_(" --default                         use default terminal settings\n"), out);
	fputs(_(" --store                           save current terminal settings as default\n"), out);
	fputs(_(" --cursor        [on|off]          display cursor\n"), out);
	fputs(_(" --repeat        [on|off]          keyboard repeat\n"), out);
	fputs(_(" --appcursorkeys [on|off]          cursor key application mode\n"), out);
	fputs(_(" --linewrap      [on|off]          continue on a new line when a line is full\n"), out);
	fputs(_(" --inversescreen [on|off]          swap colors for the whole screen\n"), out);
	fputs(_(" --foreground    default|<color>   set foreground color\n"), out);
	fputs(_(" --background    default|<color>   set background color\n"), out);
	fputs(_(" --ulcolor       [bright] <color>  set underlined text color\n"), out);
	fputs(_(" --hbcolor       [bright] <color>  set bold text color\n"), out);
	fputs(_("                 <color>: black blue cyan green grey magenta red white yellow\n"), out);
	fputs(_(" --bold          [on|off]          bold\n"), out);
	fputs(_(" --half-bright   [on|off]          dim\n"), out);
	fputs(_(" --blink         [on|off]          blink\n"), out);
	fputs(_(" --underline     [on|off]          underline\n"), out);
	fputs(_(" --reverse       [on|off]          swap foreground and background colors\n"), out);
	fputs(_(" --clear         [all|rest]        clear screen and set cursor position\n"), out);
	fputs(_(" --tabs          [<number>...]     set these tab stop positions, or show them\n"), out);
	fputs(_(" --clrtabs       [<number>...]     clear these tab stop positions, or all\n"), out);
	fputs(_(" --regtabs       [1-160]           set a regular tab stop interval\n"), out);
	fputs(_(" --blank         [0-60|force|poke] set time of inactivity before screen blanks\n"), out);
	fputs(_(" --dump          [<number>]        write vcsa<number> console dump to file\n"), out);
	fputs(_(" --append        [<number>]        append vcsa<number> console dump to file\n"), out);
	fputs(_(" --file          <filename>        name of the dump file\n"), out);
	fputs(_(" --msg           [on|off]          send kernel messages to console\n"), out);
	fputs(_(" --msglevel      0-8               kernel console log level\n"), out);
	fputs(_(" --powersave     [on|vsync|hsync|powerdown|off]\n"), out);
	fputs(_("                                   set vesa powersaving features\n"), out);
	fputs(_(" --powerdown     [0-60]            set vesa powerdown interval in minutes\n"), out);
	fputs(_(" --blength       [0-2000]          duration of the bell in milliseconds\n"), out);
	fputs(_(" --bfreq         <number>          bell frequency in Hertz\n"), out);
	fputs(_(" --version                         show version information and exit\n"), out);
	fputs(_(" --help                            display this help and exit\n"), out);
	fprintf(out, USAGE_MAN_TAIL("setterm(1)"));
	exit(out == stderr ? EXIT_FAILURE : EXIT_SUCCESS);
}

static int __attribute__((__pure__)) set_opt_flag(int opt)
{
	if (opt)
		errx(EXIT_FAILURE, _("duplicate use of an option"));
	return 1;
}

static void parse_option(struct setterm_control *ctl, int argc, char **argv)
{
	int c;
	enum {
		OPT_TERM = CHAR_MAX + 1,
		OPT_RESET,
		OPT_INITIALIZE,
		OPT_CURSOR,
		OPT_REPEAT,
		OPT_APPCURSORKEYS,
		OPT_LINEWRAP,
		OPT_DEFAULT,
		OPT_FOREGROUND,
		OPT_BACKGROUND,
		OPT_ULCOLOR,
		OPT_HBCOLOR,
		OPT_INVERSESCREEN,
		OPT_BOLD,
		OPT_HALF_BRIGHT,
		OPT_BLINK,
		OPT_REVERSE,
		OPT_UNDERLINE,
		OPT_STORE,
		OPT_CLEAR,
		OPT_TABS,
		OPT_CLRTABS,
		OPT_REGTABS,
		OPT_BLANK,
		OPT_DUMP,
		OPT_APPEND,
		OPT_FILE,
		OPT_MSG,
		OPT_MSGLEVEL,
		OPT_POWERSAVE,
		OPT_POWERDOWN,
		OPT_BLENGTH,
		OPT_BFREQ,
		OPT_VERSION,
		OPT_HELP
	};
	static const struct option longopts[] = {
		{"term", required_argument, NULL, OPT_TERM},
		{"reset", no_argument, NULL, OPT_RESET},
		{"initialize", no_argument, NULL, OPT_INITIALIZE},
		{"cursor", required_argument, NULL, OPT_CURSOR},
		{"repeat", required_argument, NULL, OPT_REPEAT},
		{"appcursorkeys", required_argument, NULL, OPT_APPCURSORKEYS},
		{"linewrap", required_argument, NULL, OPT_LINEWRAP},
		{"default", no_argument, NULL, OPT_DEFAULT},
		{"foreground", required_argument, NULL, OPT_FOREGROUND},
		{"background", required_argument, NULL, OPT_BACKGROUND},
		{"ulcolor", required_argument, NULL, OPT_ULCOLOR},
		{"hbcolor", required_argument, NULL, OPT_HBCOLOR},
		{"inversescreen", required_argument, NULL, OPT_INVERSESCREEN},
		{"bold", required_argument, NULL, OPT_BOLD},
		{"half-bright", required_argument, NULL, OPT_HALF_BRIGHT},
		{"blink", required_argument, NULL, OPT_BLINK},
		{"reverse", required_argument, NULL, OPT_REVERSE},
		{"underline", required_argument, NULL, OPT_UNDERLINE},
		{"store", no_argument, NULL, OPT_STORE},
		{"clear", required_argument, NULL, OPT_CLEAR},
		{"tabs", optional_argument, NULL, OPT_TABS},
		{"clrtabs", optional_argument, NULL, OPT_CLRTABS},
		{"regtabs", optional_argument, NULL, OPT_REGTABS},
		{"blank", optional_argument, NULL, OPT_BLANK},
		{"dump", optional_argument, NULL, OPT_DUMP},
		{"append", required_argument, NULL, OPT_APPEND},
		{"file", required_argument, NULL, OPT_FILE},
		{"msg", required_argument, NULL, OPT_MSG},
		{"msglevel", required_argument, NULL, OPT_MSGLEVEL},
		{"powersave", required_argument, NULL, OPT_POWERSAVE},
		{"powerdown", optional_argument, NULL, OPT_POWERDOWN},
		{"blength", optional_argument, NULL, OPT_BLENGTH},
		{"bfreq", optional_argument, NULL, OPT_BFREQ},
		{"version", no_argument, NULL, OPT_VERSION},
		{"help", no_argument, NULL, OPT_HELP},
		{NULL, 0, NULL, 0}
	};
	static const ul_excl_t excl[] = {
		{ OPT_DEFAULT, OPT_STORE },
		{ OPT_TABS, OPT_CLRTABS, OPT_REGTABS },
		{ OPT_MSG, OPT_MSGLEVEL },
		{ 0 }
	};
	int excl_st[ARRAY_SIZE(excl)] = UL_EXCL_STATUS_INIT;

	while ((c = getopt_long_only(argc, argv, "", longopts, NULL)) != -1) {
		err_exclusive_options(c, longopts, excl, excl_st);
		switch (c) {
		case OPT_TERM:
			ctl->opt_term = set_opt_flag(ctl->opt_term);
			ctl->opt_te_terminal_name = optarg;
			break;
		case OPT_RESET:
			ctl->opt_reset = set_opt_flag(ctl->opt_reset);
			break;
		case OPT_INITIALIZE:
			ctl->opt_initialize = set_opt_flag(ctl->opt_initialize);
			break;
		case OPT_CURSOR:
			ctl->opt_cursor = set_opt_flag(ctl->opt_cursor);
			ctl->opt_cu_on = parse_switch(optarg, _("argument error"),
						"on", "off", NULL);
			break;
		case OPT_REPEAT:
			ctl->opt_repeat = set_opt_flag(ctl->opt_repeat);
			ctl->opt_rep_on = parse_switch(optarg, _("argument error"),
						"on", "off", NULL);
			break;
		case OPT_APPCURSORKEYS:
			ctl->opt_appcursorkeys = set_opt_flag(ctl->opt_appcursorkeys);
			ctl->opt_appck_on = parse_switch(optarg, _("argument error"),
						"on", "off", NULL);
			break;
		case OPT_LINEWRAP:
			ctl->opt_linewrap = set_opt_flag(ctl->opt_linewrap);
			ctl->opt_li_on = parse_switch(optarg, _("argument error"),
						"on", "off", NULL);
			break;
		case OPT_DEFAULT:
			ctl->opt_default = set_opt_flag(ctl->opt_default);
			break;
		case OPT_FOREGROUND:
			ctl->opt_foreground = set_opt_flag(ctl->opt_foreground);
			ctl->opt_fo_color = parse_febg_color(optarg);
			break;
		case OPT_BACKGROUND:
			ctl->opt_background = set_opt_flag(ctl->opt_background);
			ctl->opt_ba_color = parse_febg_color(optarg);
			break;
		case OPT_ULCOLOR:
			ctl->opt_ulcolor = set_opt_flag(ctl->opt_ulcolor);
			ctl->opt_ul_color = parse_ulhb_color(argv, &optind);
			break;
		case OPT_HBCOLOR:
			ctl->opt_hbcolor = set_opt_flag(ctl->opt_hbcolor);
			ctl->opt_hb_color = parse_ulhb_color(argv, &optind);
			break;
		case OPT_INVERSESCREEN:
			ctl->opt_inversescreen = set_opt_flag(ctl->opt_inversescreen);
			ctl->opt_invsc_on = parse_switch(optarg, _("argument error"),
						"on", "off", NULL);
			break;
		case OPT_BOLD:
			ctl->opt_bold = set_opt_flag(ctl->opt_bold);
			ctl->opt_bo_on = parse_switch(optarg, _("argument error"),
						"on", "off", NULL);
			break;
		case OPT_HALF_BRIGHT:
			ctl->opt_halfbright = set_opt_flag(ctl->opt_halfbright);
			ctl->opt_hb_on = parse_switch(optarg, _("argument error"),
						"on", "off", NULL);
			break;
		case OPT_BLINK:
			ctl->opt_blink = set_opt_flag(ctl->opt_blink);
			ctl->opt_bl_on = parse_switch(optarg, _("argument error"),
						"on", "off", NULL);
			break;
		case OPT_REVERSE:
			ctl->opt_reverse = set_opt_flag(ctl->opt_reverse);
			ctl->opt_re_on = parse_switch(optarg, _("argument error"),
						"on", "off", NULL);
			break;
		case OPT_UNDERLINE:
			ctl->opt_underline = set_opt_flag(ctl->opt_underline);
			ctl->opt_un_on = parse_switch(optarg, _("argument error"),
						"on", "off", NULL);
			break;
		case OPT_STORE:
			ctl->opt_store = set_opt_flag(ctl->opt_store);
			break;
		case OPT_CLEAR:
			ctl->opt_clear = set_opt_flag(ctl->opt_clear);
			ctl->opt_cl_all = parse_switch(optarg, _("argument error"),
						"all", "reset", NULL);
			break;
		case OPT_TABS:
			ctl->opt_tabs = set_opt_flag(ctl->opt_tabs);
			parse_tabs(argv, optarg, &optind, ctl->opt_tb_array);
			break;
		case OPT_CLRTABS:
			ctl->opt_clrtabs = set_opt_flag(ctl->opt_clrtabs);
			parse_tabs(argv, optarg, &optind, ctl->opt_tb_array);
			break;
		case OPT_REGTABS:
			ctl->opt_regtabs = set_opt_flag(ctl->opt_regtabs);
			ctl->opt_rt_len = parse_regtabs(argv, optarg, &optind);
			break;
		case OPT_BLANK:
			ctl->opt_blank = set_opt_flag(ctl->opt_blank);
			ctl->opt_bl_min = parse_blank(argv, optarg, &optind);
			break;
		case OPT_DUMP:
			ctl->opt_snap = set_opt_flag(ctl->opt_snap);
			ctl->opt_sn_num = parse_snap(argv, optarg, &optind);
			break;
		case OPT_APPEND:
			ctl->opt_append = set_opt_flag(ctl->opt_append);
			ctl->opt_sn_num = parse_snap(argv, optarg, &optind);
			break;
		case OPT_FILE:
			ctl->opt_snapfile = set_opt_flag(ctl->opt_snapfile);
			ctl->opt_sn_name = optarg;
			break;
		case OPT_MSG:
			ctl->opt_msg = set_opt_flag(ctl->opt_msg);
			ctl->opt_msg_on = parse_switch(optarg, _("argument error"),
						"on", "off", NULL);
			break;
		case OPT_MSGLEVEL:
			ctl->opt_msglevel = set_opt_flag(ctl->opt_msglevel);
			ctl->opt_msglevel_num = parse_msglevel(optarg);
			if (ctl->opt_msglevel_num == 0) {
				ctl->opt_msg = set_opt_flag(ctl->opt_msg);
				ctl->opt_msg_on |= 1;
			}
			break;
		case OPT_POWERSAVE:
			ctl->opt_powersave = set_opt_flag(ctl->opt_powersave);
			ctl->opt_ps_mode = parse_powersave(optarg);
			break;
		case OPT_POWERDOWN:
			ctl->opt_powerdown = set_opt_flag(ctl->opt_powerdown);
			ctl->opt_pd_min = parse_blank(argv, optarg, &optind);
			break;
		case OPT_BLENGTH:
			ctl->opt_blength = set_opt_flag(ctl->opt_blength);
			ctl->opt_blength_l = parse_blength(argv, optarg, &optind);
			break;
		case OPT_BFREQ:
			ctl->opt_bfreq = set_opt_flag(ctl->opt_bfreq);
			ctl->opt_bfreq_f = parse_bfreq(argv, optarg, &optind);
			break;
		case OPT_VERSION:
			printf(UTIL_LINUX_VERSION);
			exit(EXIT_SUCCESS);
		case OPT_HELP:
			usage(stdout);
		default:
			usage(stderr);
		}
	}
}

/* Return the specified terminfo string, or an empty string if no such
 * terminfo capability exists.  */
static char *ti_entry(const char *name)
{
	char *buf_ptr;

	if ((buf_ptr = tigetstr((char *)name)) == (char *)-1)
		buf_ptr = NULL;
	return buf_ptr;
}

static void show_tabs(void)
{
	int i, co = tigetnum("cols");

	if (co > 0) {
		printf("\r         ");
		for (i = 10; i < co - 2; i += 10)
			printf("%-10d", i);
		putchar('\n');
		for (i = 1; i <= co; i++)
			putchar(i % 10 + '0');
		putchar('\n');
		for (i = 1; i < co; i++)
>>>>>>> master-vanilla
			printf("\tT\b");
		putchar('\n');
	}
}

<<<<<<< HEAD
static void __attribute__ ((__noreturn__))
usage(FILE *out) {
/* Print error message about arguments, and the command's syntax. */

	if (out == stderr)
		warnx(_("Argument error."));

	fputs(_("\nUsage:\n"), out);
	fprintf(out,
	      _(" %s [options]\n"), program_invocation_short_name);

	fputs(_("\nOptions:\n"), out);
	fputs(_(" -term <terminal_name>\n"
		" -reset\n"
		" -initialize\n"
		" -cursor <on|off>\n"
		" -repeat <on|off>\n"
		" -appcursorkeys <on|off>\n"
		" -linewrap <on|off>\n"
		" -default\n"
		" -foreground <black|blue|green|cyan|red|magenta|yellow|white|default>\n"
		" -background <black|blue|green|cyan|red|magenta|yellow|white|default>\n"
		" -ulcolor <black|grey|blue|green|cyan|red|magenta|yellow|white>\n"
		" -ulcolor <bright blue|green|cyan|red|magenta|yellow|white>\n"
		" -hbcolor <black|grey|blue|green|cyan|red|magenta|yellow|white>\n"
		" -hbcolor <bright blue|green|cyan|red|magenta|yellow|white>\n"
		" -inversescreen <on|off>\n"
		" -bold <on|off>\n"
		" -half-bright <on|off>\n"
		" -blink <on|off>\n"
		" -reverse <on|off>\n"
		" -underline <on|off>\n"
		" -store >\n"
		" -clear <all|rest>\n"
		" -tabs < tab1 tab2 tab3 ... >      (tabn = 1-160)\n"
		" -clrtabs < tab1 tab2 tab3 ... >   (tabn = 1-160)\n"
		" -regtabs <1-160>\n"
		" -blank <0-60|force|poke>\n"
		" -dump   <1-NR_CONSOLES>\n"
		" -append <1-NR_CONSOLES>\n"
		" -file dumpfilename\n"
		" -msg <on|off>\n"
		" -msglevel <0-8>\n"
		" -powersave <on|vsync|hsync|powerdown|off>\n"
		" -powerdown <0-60>\n"
		" -blength <0-2000>\n"
		" -bfreq freqnumber\n"
		" -version\n"
		" -help\n"), out);

	fprintf(out, _("\nFor more information see lsblk(1).\n"));

	exit(out == stderr ? EXIT_FAILURE : EXIT_SUCCESS);
}

#define STRCMP(str1,str2) strncmp(str1,str2,strlen(str1))

static void
parse_option(char *option, int argc, char **argv, int *bad_arg) {
	/* option: Option with leading '-' removed. */
	/* argc: Number of arguments for this option. */
	/* argv: Arguments for this option. */
	/* bad_arg: Set to true if an error is detected. */

/* Parse a single specification. */

	if (STRCMP(option, "term") == 0)
		parse_term(argc, argv, &opt_term, &opt_te_terminal_name, bad_arg);
	else if (STRCMP(option, "reset") == 0)
		parse_none(argc, argv, &opt_reset, bad_arg);
	else if (STRCMP(option, "initialize") == 0)
		parse_none(argc, argv, &opt_initialize, bad_arg);
	else if (STRCMP(option, "cursor") == 0)
		parse_switch(argc, argv, &opt_cursor, &opt_cu_on, bad_arg);
	else if (STRCMP(option, "repeat") == 0)
		parse_switch(argc, argv, &opt_repeat, &opt_rep_on, bad_arg);
	else if (STRCMP(option, "appcursorkeys") == 0)
		parse_switch(argc, argv, &opt_appcursorkeys, &opt_appck_on, bad_arg);
	else if (STRCMP(option, "linewrap") == 0)
		parse_switch(argc, argv, &opt_linewrap, &opt_li_on, bad_arg);
#if 0
	else if (STRCMP(option, "snow") == 0)
		parse_switch(argc, argv, &opt_snow, &opt_sn_on, bad_arg);
	else if (STRCMP(option, "softscroll") == 0)
		parse_switch(argc, argv, &opt_softscroll, &opt_so_on, bad_arg);
#endif
	else if (STRCMP(option, "default") == 0)
		parse_none(argc, argv, &opt_default, bad_arg);
	else if (STRCMP(option, "foreground") == 0)
		par_color(argc, argv, &opt_foreground, &opt_fo_color, bad_arg);
	else if (STRCMP(option, "background") == 0)
		par_color(argc, argv, &opt_background, &opt_ba_color, bad_arg);
	else if (STRCMP(option, "ulcolor") == 0)
		par_color2(argc, argv, &opt_ulcolor, &opt_ul_color, bad_arg);
	else if (STRCMP(option, "hbcolor") == 0)
		par_color2(argc, argv, &opt_hbcolor, &opt_hb_color, bad_arg);
	else if (STRCMP(option, "inversescreen") == 0)
		parse_switch(argc, argv, &opt_inversescreen, &opt_invsc_on, bad_arg);
	else if (STRCMP(option, "bold") == 0)
		parse_switch(argc, argv, &opt_bold, &opt_bo_on, bad_arg);
	else if (STRCMP(option, "half-bright") == 0)
		parse_switch(argc, argv, &opt_halfbright, &opt_hb_on, bad_arg);
	else if (STRCMP(option, "blink") == 0)
		parse_switch(argc, argv, &opt_blink, &opt_bl_on, bad_arg);
	else if (STRCMP(option, "reverse") == 0)
		parse_switch(argc, argv, &opt_reverse, &opt_re_on, bad_arg);
	else if (STRCMP(option, "underline") == 0)
		parse_switch(argc, argv, &opt_underline, &opt_un_on, bad_arg);
	else if (STRCMP(option, "store") == 0)
		parse_none(argc, argv, &opt_store, bad_arg);
	else if (STRCMP(option, "clear") == 0)
		parse_clear(argc, argv, &opt_clear, &opt_cl_all, bad_arg);
	else if (STRCMP(option, "tabs") == 0)
		parse_tabs(argc, argv, &opt_tabs, opt_tb_array, bad_arg);
	else if (STRCMP(option, "clrtabs") == 0)
		parse_clrtabs(argc, argv, &opt_clrtabs, opt_tb_array, bad_arg);
	else if (STRCMP(option, "regtabs") == 0)
		parse_regtabs(argc, argv, &opt_regtabs, &opt_rt_len, bad_arg);
	else if (STRCMP(option, "blank") == 0)
		parse_blank(argc, argv, &opt_blank, &opt_bl_min, bad_arg);
	else if (STRCMP(option, "dump") == 0)
		parse_snap(argc, argv, &opt_snap, &opt_sn_num, bad_arg);
	else if (STRCMP(option, "append") == 0)
		parse_snap(argc, argv, &opt_append, &opt_sn_num, bad_arg);
	else if (STRCMP(option, "file") == 0)
		parse_snapfile(argc, argv, &opt_snapfile, (int *)opt_sn_name, bad_arg);
	else if (STRCMP(option, "msg") == 0)
		parse_switch(argc, argv, &opt_msg, &opt_msg_on, bad_arg);
	else if (STRCMP(option, "msglevel") == 0)
		parse_msglevel(argc, argv, &opt_msglevel, &opt_msglevel_num, bad_arg);
	else if (STRCMP(option, "powersave") == 0)
		parse_powersave(argc, argv, &opt_powersave, &opt_ps_mode, bad_arg);
	else if (STRCMP(option, "powerdown") == 0)
		parse_blank(argc, argv, &opt_powerdown, &opt_pd_min, bad_arg);
	else if (STRCMP(option, "blength") == 0)
		parse_blength(argc, argv, &opt_blength, &opt_blength_l, bad_arg);
	else if (STRCMP(option, "bfreq") == 0)
		parse_bfreq(argc, argv, &opt_bfreq, &opt_bfreq_f, bad_arg);
#if 0
	else if (STRCMP(option, "standout") == 0)
		parse_standout(argc, argv, &opt_standout, &opt_st_attr, bad_arg);
#endif
	else if (STRCMP(option, "version") == 0) {
		printf(_("%s from %s\n"), program_invocation_short_name,
					  PACKAGE_STRING);
		exit(EXIT_SUCCESS);
	} else if (STRCMP(option, "help") == 0)
		usage(stdout);
	else
		*bad_arg = TRUE;
}

/* End of command line parsing routines. */

static char *ti_entry(const char *name) {
	/* name: Terminfo capability string to lookup. */

/* Return the specified terminfo string, or an empty string if no such terminfo
 * capability exists.
 */

	char *buf_ptr;

	if ((buf_ptr = tigetstr((char *)name)) == (char *)-1)
		buf_ptr = NULL;
	return buf_ptr;
}

static void
perform_sequence(int vcterm) {
	/* vcterm: Set if terminal is a virtual console. */

	int result;
/* Perform the selected options. */

	/* -reset. */
	if (opt_reset) {
		putp(ti_entry("rs1"));
	}

	/* -initialize. */
	if (opt_initialize) {
		putp(ti_entry("is2"));
	}

	/* -cursor [on|off]. */
	if (opt_cursor) {
		if (opt_cu_on)
=======
static int open_snapshot_device(struct setterm_control *ctl)
{
	int fd;

	if (ctl->opt_sn_num)
		xasprintf(&ctl->in_device, "/dev/vcsa%d", ctl->opt_sn_num);
	else
		xasprintf(&ctl->in_device, "/dev/vcsa");
	fd = open(ctl->in_device, O_RDONLY);
	if (fd < 0)
		err(EXIT_DUMPFILE, _("cannot read %s"), ctl->in_device);
	return fd;
}

static void set_blanking(struct setterm_control *ctl)
{
	char ioctlarg;
	int ret;

	if (0 <= ctl->opt_bl_min) {
		printf("\033[9;%d]", ctl->opt_bl_min);
		return;
	}
	switch (ctl->opt_bl_min) {
	case BLANKSCREEN:
		ioctlarg = TIOCL_BLANKSCREEN;
		if (ioctl(STDIN_FILENO, TIOCLINUX, &ioctlarg))
			warn(_("cannot force blank"));
		break;
	case UNBLANKSCREEN:
		ioctlarg = TIOCL_UNBLANKSCREEN;
		if (ioctl(STDIN_FILENO, TIOCLINUX, &ioctlarg))
			warn(_("cannot force unblank"));
		break;
	case BLANKEDSCREEN:
		ioctlarg = TIOCL_BLANKEDSCREEN;
		ret = ioctl(STDIN_FILENO, TIOCLINUX, &ioctlarg);
		if (ret < 0)
			warn(_("cannot get blank status"));
		else
			printf("%d\n", ret);
		break;
	default:		/* should be impossible to reach */
		abort();
	}
	return;
}

static void screendump(struct setterm_control *ctl)
{
	unsigned char header[4];
	unsigned int rows, cols;
	int fd;
	FILE *out;
	size_t i, j;
	ssize_t rc;
	char *inbuf, *outbuf, *p, *q;

	/* open source and destination files */
	fd = open_snapshot_device(ctl);
	if (!ctl->opt_sn_name)
		ctl->opt_sn_name = "screen.dump";
	out = fopen(ctl->opt_sn_name, ctl->opt_snap ? "w" : "a");
	if (!out)
		err(EXIT_DUMPFILE, _("can not open dump file %s for output"), ctl->opt_sn_name);
	/* determine snapshot size */
	if (read(fd, header, 4) != 4)
		err(EXIT_DUMPFILE, _("cannot read %s"), ctl->in_device);
	rows = header[0];
	cols = header[1];
	if (rows * cols == 0)
		err(EXIT_DUMPFILE, _("cannot read %s"), ctl->in_device);
	/* allocate buffers */
	inbuf = xmalloc(rows * cols * 2);
	outbuf = xmalloc(rows * (cols + 1));
	/* read input */
	rc = read(fd, inbuf, rows * cols * 2);
	if (rc < 0 || (size_t)rc != rows * cols * 2)
		err(EXIT_DUMPFILE, _("cannot read %s"), ctl->in_device);
	p = inbuf;
	q = outbuf;
	/* copy inbuf to outbuf */
	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
			*q++ = *p;
			p += 2;
		}
		while (j-- > 0 && q[-1] == ' ')
			q--;
		*q++ = '\n';
	}
	fwrite(outbuf, 1, q - outbuf, out);
	/* clean up allocations */
	close(fd);
	free(inbuf);
	free(outbuf);
	free(ctl->in_device);
	if (close_stream(out) != 0)
		errx(EXIT_FAILURE, _("write error"));
	return;
}

/* Some options are applicable when terminal is virtual console. */
static int vc_only(struct setterm_control *ctl, const char *err)
{
	if (!ctl->vcterm) {
		if (err)
			warnx(_("terminal %s does not support %s"),
			      ctl->opt_te_terminal_name, err);
	}
	return ctl->vcterm;
}

static void perform_sequence(struct setterm_control *ctl)
{
	int result;

	/* -reset. */
	if (ctl->opt_reset)
		putp(ti_entry("rs1"));

	/* -initialize. */
	if (ctl->opt_initialize)
		putp(ti_entry("is2"));

	/* -cursor [on|off]. */
	if (ctl->opt_cursor) {
		if (ctl->opt_cu_on)
>>>>>>> master-vanilla
			putp(ti_entry("cnorm"));
		else
			putp(ti_entry("civis"));
	}

<<<<<<< HEAD
	/* -linewrap [on|off]. Vc only (vt102) */
	if (opt_linewrap && vcterm) {
		if (opt_li_on)
			printf("\033[?7h");
		else
			printf("\033[?7l");
	}

	/* -repeat [on|off]. Vc only (vt102) */
	if (opt_repeat && vcterm) {
		if (opt_rep_on)
			printf("\033[?8h");
		else
			printf("\033[?8l");
	}

	/* -appcursorkeys [on|off]. Vc only (vt102) */
	if (opt_appcursorkeys && vcterm) {
		if (opt_appck_on)
			printf("\033[?1h");
		else
			printf("\033[?1l");
	}

#if 0
	/* -snow [on|off].  Vc only. */
	if (opt_snow && vcterm) {
		if (opt_sn_on)
			printf("%s%s%s", DCS, "snow.on", ST);
		else
			printf("%s%s%s", DCS, "snow.off", ST);
	}

	/* -softscroll [on|off].  Vc only. */
	if (opt_softscroll && vcterm) {
		if (opt_so_on)
			printf("%s%s%s", DCS, "softscroll.on", ST);
		else
			printf("%s%s%s", DCS, "softscroll.off", ST);
	}
#endif

	/* -default.  Vc sets default rendition, otherwise clears all
	 * attributes.
	 */
	if (opt_default) {
		if (vcterm)
=======
	/* -linewrap [on|off]. */
	if (ctl->opt_linewrap)
		fputs(ctl->opt_li_on ? "\033[?7h" : "\033[?7l", stdout);

	/* -repeat [on|off]. */
	if (ctl->opt_repeat && vc_only(ctl, "--repeat"))
		fputs(ctl->opt_rep_on ? "\033[?8h" : "\033[?8l", stdout);

	/* -appcursorkeys [on|off]. */
	if (ctl->opt_appcursorkeys && vc_only(ctl, "--appcursorkeys"))
		fputs(ctl->opt_appck_on ? "\033[?1h" : "\033[?1l", stdout);

	/* -default.  Vc sets default rendition, otherwise clears all
	 * attributes. */
	if (ctl->opt_default) {
		if (vc_only(ctl, NULL))
>>>>>>> master-vanilla
			printf("\033[0m");
		else
			putp(ti_entry("sgr0"));
	}

<<<<<<< HEAD
	/* -foreground black|red|green|yellow|blue|magenta|cyan|white|default.
	 * Vc only (ANSI).
	 */
	if (opt_foreground && vcterm) {
		printf("%s%s%c%s", ESC, "[3", '0' + opt_fo_color, "m");
	}

	/* -background black|red|green|yellow|blue|magenta|cyan|white|default.
	 * Vc only (ANSI).
	 */
	if (opt_background && vcterm) {
		printf("%s%s%c%s", ESC, "[4", '0' + opt_ba_color, "m");
	}

	/* -ulcolor black|red|green|yellow|blue|magenta|cyan|white|default.
	 * Vc only.
	 */
	if (opt_ulcolor && vcterm) {
		printf("\033[1;%d]", opt_ul_color);
	}

	/* -hbcolor black|red|green|yellow|blue|magenta|cyan|white|default.
	 * Vc only.
	 */
	if (opt_hbcolor && vcterm) {
		printf("\033[2;%d]", opt_hb_color);
	}

	/* -inversescreen [on|off].  Vc only (vt102).
	 */
	if (opt_inversescreen) {
		if (vcterm) {
			if (opt_invsc_on)
				printf("\033[?5h");
			else
				printf("\033[?5l");
		}
	}

	/* -bold [on|off].  Vc behaves as expected, otherwise off turns off
	 * all attributes.
	 */
	if (opt_bold) {
		if (opt_bo_on)
			putp(ti_entry("bold"));
		else {
			if (vcterm)
				printf("%s%s", ESC, "[22m");
=======
	/* -foreground black|red|green|yellow|blue|magenta|cyan|white|default. */
	if (ctl->opt_foreground)
		printf("\033[3%c%s", '0' + ctl->opt_fo_color, "m");

	/* -background black|red|green|yellow|blue|magenta|cyan|white|default. */
	if (ctl->opt_background)
		printf("\033[4%c%s", '0' + ctl->opt_ba_color, "m");

	/* -ulcolor black|red|green|yellow|blue|magenta|cyan|white|default. */
	if (ctl->opt_ulcolor && vc_only(ctl, "--ulcolor"))
		printf("\033[1;%d]", ctl->opt_ul_color);

	/* -hbcolor black|red|green|yellow|blue|magenta|cyan|white|default. */
	if (ctl->opt_hbcolor)
		printf("\033[2;%d]", ctl->opt_hb_color);

	/* -inversescreen [on|off]. */
	if (ctl->opt_inversescreen)
		fputs(ctl->opt_invsc_on ? "\033[?5h" : "\033[?5l", stdout);

	/* -bold [on|off].  Vc behaves as expected, otherwise off turns off
	 * all attributes. */
	if (ctl->opt_bold) {
		if (ctl->opt_bo_on)
			putp(ti_entry("bold"));
		else {
			if (vc_only(ctl, NULL))
				fputs("\033[22m", stdout);
>>>>>>> master-vanilla
			else
				putp(ti_entry("sgr0"));
		}
	}

<<<<<<< HEAD
	/* -half-bright [on|off].  Vc behaves as expected, otherwise off turns off
	 * all attributes.
	 */
	if (opt_halfbright) {
		if (opt_hb_on)
			putp(ti_entry("dim"));
		else {
			if (vcterm)
				printf("%s%s", ESC, "[22m");
=======
	/* -half-bright [on|off].  Vc behaves as expected, otherwise off
	 * turns off all attributes.  */
	if (ctl->opt_halfbright) {
		if (ctl->opt_hb_on)
			putp(ti_entry("dim"));
		else {
			if (vc_only(ctl, NULL))
				fputs("\033[22m", stdout);
>>>>>>> master-vanilla
			else
				putp(ti_entry("sgr0"));
		}
	}

	/* -blink [on|off].  Vc behaves as expected, otherwise off turns off
<<<<<<< HEAD
	 * all attributes.
	 */
	if (opt_blink) {
		if (opt_bl_on)
			putp(ti_entry("blink"));
		else {
			if (vcterm)
				printf("%s%s", ESC, "[25m");
=======
	 * all attributes. */
	if (ctl->opt_blink) {
		if (ctl->opt_bl_on)
			putp(ti_entry("blink"));
		else {
			if (vc_only(ctl, NULL))
				fputs("\033[25m", stdout);
>>>>>>> master-vanilla
			else
				putp(ti_entry("sgr0"));
		}
	}

	/* -reverse [on|off].  Vc behaves as expected, otherwise off turns
<<<<<<< HEAD
	 * off all attributes.
	 */
	if (opt_reverse) {
		if (opt_re_on)
			putp(ti_entry("rev"));
		else {
			if (vcterm)
				printf("%s%s", ESC, "[27m");
=======
	 * off all attributes. */
	if (ctl->opt_reverse) {
		if (ctl->opt_re_on)
			putp(ti_entry("rev"));
		else {
			if (vc_only(ctl, NULL))
				fputs("\033[27m", stdout);
>>>>>>> master-vanilla
			else
				putp(ti_entry("sgr0"));
		}
	}

	/* -underline [on|off]. */
<<<<<<< HEAD
	if (opt_underline) {
		if (opt_un_on)
			putp(ti_entry("smul"));
		else
			putp(ti_entry("rmul"));
	}

	/* -store.  Vc only. */
	if (opt_store && vcterm) {
		printf("\033[8]");
	}

	/* -clear [all|rest]. */
	if (opt_clear) {
		if (opt_cl_all)
			putp(ti_entry("clear"));
		else
			putp(ti_entry("ed"));
	}

	/* -tabs Vc only. */
	if (opt_tabs && vcterm) {
		int i;

		if (opt_tb_array[0] == -1)
			show_tabs();
		else {
			for(i=0; opt_tb_array[i] > 0; i++)
				printf("\033[%dG\033H", opt_tb_array[i]);
=======
	if (ctl->opt_underline)
		putp(ti_entry(ctl->opt_un_on ? "smul" : "rmul"));

	/* -store. */
	if (ctl->opt_store && vc_only(ctl, "--store"))
		fputs("\033[8]", stdout);

	/* -clear [all|rest]. */
	if (ctl->opt_clear)
		putp(ti_entry(ctl->opt_cl_all ? "clear" : "ed"));

	/* -tabs. */
	if (ctl->opt_tabs) {
		if (ctl->opt_tb_array[0] == -1)
			show_tabs();
		else {
			int i;

			for (i = 0; ctl->opt_tb_array[i] > 0; i++)
				printf("\033[%dG\033H", ctl->opt_tb_array[i]);
>>>>>>> master-vanilla
			putchar('\r');
		}
	}

<<<<<<< HEAD
	/* -clrtabs Vc only. */
	if (opt_clrtabs && vcterm) {
		int i;

		if (opt_tb_array[0] == -1)
			printf("\033[3g");
		else
			for(i=0; opt_tb_array[i] > 0; i++)
				printf("\033[%dG\033[g", opt_tb_array[i]);
		putchar('\r');
	}

	/* -regtabs Vc only. */
	if (opt_regtabs && vcterm) {
		int i;

		printf("\033[3g\r");
		for(i=opt_rt_len+1; i<=160; i+=opt_rt_len)
			printf("\033[%dC\033H",opt_rt_len);
=======
	/* -clrtabs. */
	if (ctl->opt_clrtabs && vc_only(ctl, "--clrtabs")) {
		int i;

		if (ctl->opt_tb_array[0] == -1)
			fputs("\033[3g", stdout);
		else
			for (i = 0; ctl->opt_tb_array[i] > 0; i++)
				printf("\033[%dG\033[g", ctl->opt_tb_array[i]);
		putchar('\r');
	}

	/* -regtabs. */
	if (ctl->opt_regtabs && vc_only(ctl, "--regtabs")) {
		int i;

		fputs("\033[3g\r", stdout);
		for (i = ctl->opt_rt_len + 1; i <= TABS_MAX; i += ctl->opt_rt_len)
			printf("\033[%dC\033H", ctl->opt_rt_len);
>>>>>>> master-vanilla
		putchar('\r');
	}

	/* -blank [0-60]. */
<<<<<<< HEAD
	if (opt_blank && vcterm) {
		if (opt_bl_min >= 0)
			printf("\033[9;%d]", opt_bl_min);
		else if (opt_bl_min == BLANKSCREEN) {
			char ioctlarg = TIOCL_BLANKSCREEN;
			if (ioctl(0,TIOCLINUX,&ioctlarg))
				warn(_("cannot force blank"));
		} else if (opt_bl_min == UNBLANKSCREEN) {
			char ioctlarg = TIOCL_UNBLANKSCREEN;
			if (ioctl(0,TIOCLINUX,&ioctlarg))
				warn(_("cannot force unblank"));
		} else if (opt_bl_min == BLANKEDSCREEN) {
			char ioctlarg = TIOCL_BLANKEDSCREEN;
			int ret;
			ret = ioctl(0,TIOCLINUX,&ioctlarg);
			if (ret < 0)
				warn(_("cannot get blank status"));
			else
				printf("%d\n",ret);
		}
	}

	/* -powersave [on|vsync|hsync|powerdown|off] (console) */
	if (opt_powersave) {
		char ioctlarg[2];
		ioctlarg[0] = TIOCL_SETVESABLANK;
		ioctlarg[1] = opt_ps_mode;
		if (ioctl(0,TIOCLINUX,ioctlarg))
=======
	if (ctl->opt_blank && vc_only(ctl, "--blank"))
		set_blanking(ctl);

	/* -powersave [on|vsync|hsync|powerdown|off] (console) */
	if (ctl->opt_powersave) {
		char ioctlarg[2];
		ioctlarg[0] = TIOCL_SETVESABLANK;
		ioctlarg[1] = ctl->opt_ps_mode;
		if (ioctl(STDIN_FILENO, TIOCLINUX, ioctlarg))
>>>>>>> master-vanilla
			warn(_("cannot (un)set powersave mode"));
	}

	/* -powerdown [0-60]. */
<<<<<<< HEAD
	if (opt_powerdown) {
		printf("\033[14;%d]", opt_pd_min);
	}

#if 0
	/* -standout [num]. */
	if (opt_standout)
		/* nothing */;
#endif

	/* -snap [1-NR_CONS]. */
	if (opt_snap || opt_append) {
		FILE *F;

		F = fopen(opt_sn_name, opt_snap ? "w" : "a");
		if (!F)
			err(EXIT_DUMPFILE, _("can not open dump file %s for output"),
				opt_sn_name); 
		screendump(opt_sn_num, F);
		fclose(F);
	}

	/* -msg [on|off]. */
	if (opt_msg && vcterm) {
		if (opt_msg_on)
			/* 7 -- Enable printk's to console */
			result = klogctl(7, NULL, 0);
		else
			/*  6 -- Disable printk's to console */
			result = klogctl(6, NULL, 0);
=======
	if (ctl->opt_powerdown)
		printf("\033[14;%d]", ctl->opt_pd_min);

	/* -snap [1-NR_CONS]. */
	if (ctl->opt_snap || ctl->opt_append)
		screendump(ctl);

	/* -msg [on|off].  Controls printk's to console. */
	if (ctl->opt_msg && vc_only(ctl, "--msg")) {
		if (ctl->opt_msg_on)
			result = klogctl(SYSLOG_ACTION_CONSOLE_ON, NULL, 0);
		else
			result = klogctl(SYSLOG_ACTION_CONSOLE_OFF, NULL, 0);
>>>>>>> master-vanilla

		if (result != 0)
			warn(_("klogctl error"));
	}

<<<<<<< HEAD
	/* -msglevel [0-8] */
	if (opt_msglevel && vcterm) {
		/* 8 -- Set level of messages printed to console */
		result = klogctl(8, NULL, opt_msglevel_num);
=======
	/* -msglevel [0-8].  Console printk message level. */
	if (ctl->opt_msglevel_num && vc_only(ctl, "--msglevel")) {
		result =
		    klogctl(SYSLOG_ACTION_CONSOLE_LEVEL, NULL,
			    ctl->opt_msglevel_num);
>>>>>>> master-vanilla
		if (result != 0)
			warn(_("klogctl error"));
	}

	/* -blength [0-2000] */
<<<<<<< HEAD
	if (opt_blength && vcterm) {
		printf("\033[11;%d]", opt_blength_l);
	}

	/* -bfreq freqnumber */
	if (opt_bfreq && vcterm) {
		printf("\033[10;%d]", opt_bfreq_f);
	}

}

static void
screendump(int vcnum, FILE * F)
{
	char infile[MAXPATHLEN];
	unsigned char header[4];
	unsigned int rows, cols;
	int fd;
	size_t i, j;
	char *inbuf, *outbuf, *p, *q;

	sprintf(infile, "/dev/vcsa%d", vcnum);
	fd = open(infile, O_RDONLY);
	if (fd < 0 && vcnum == 0) {
		/* vcsa0 is often called vcsa */
		sprintf(infile, "/dev/vcsa");
		fd = open(infile, O_RDONLY);
	}
	if (fd < 0) {
		/* try devfs name - for zero vcnum just /dev/vcc/a */
		/* some gcc's warn for %.u - add 0 */
		sprintf(infile, "/dev/vcc/a%.0u", vcnum);
		fd = open(infile, O_RDONLY);
	}
	if (fd < 0) {
		sprintf(infile, "/dev/vcsa%d", vcnum);
		goto read_error;
	}
	if (read(fd, header, 4) != 4)
		goto read_error;
	rows = header[0];
	cols = header[1];
	if (rows * cols == 0)
		goto read_error;

	inbuf = xmalloc(rows * cols * 2);
	outbuf = xmalloc(rows * (cols + 1));

	if (read(fd, inbuf, rows * cols * 2) != rows * cols * 2)
		goto read_error;
	p = inbuf;
	q = outbuf;
	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
			*q++ = *p;
			p += 2;
		}
		while (j-- > 0 && q[-1] == ' ')
			q--;
		*q++ = '\n';
	}
	if (fwrite(outbuf, 1, q - outbuf, F) != (size_t) (q - outbuf)) {
		warnx(_("Error writing screendump"));
		goto error;
	}
	close(fd);
	return;

      read_error:
	warnx(_("Couldn't read %s"), infile);
      error:
	if (fd >= 0)
		close(fd);
	exit(EXIT_FAILURE);
}

int
main(int argc, char **argv) {
	int bad_arg = FALSE;		/* Set if error in arguments. */
	int arg, modifier;
	char *term;			/* Terminal type. */
	int vcterm;			/* Set if terminal is a virtual console. */
	int errret;

	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);

	if (argc < 2)
		bad_arg = TRUE;

	/* Parse arguments. */

	for (arg = 1; arg < argc;) {
		if (*argv[arg] == '-') {

			/* Parse a single option. */

			for (modifier = arg + 1; modifier < argc; modifier++) {
				if (*argv[modifier] == '-') break;
			}
			parse_option(argv[arg] + 1, modifier - arg - 1,
				     &argv[arg + 1], &bad_arg);
			arg = modifier;
		} else {
			bad_arg = TRUE;
			arg++;
		}
	}

	/* Display syntax message if error in arguments. */

	if (bad_arg)
		usage(stderr);

	/* Find out terminal name. */

	if (opt_term) {
		term = opt_te_terminal_name;
	} else {
		term = getenv("TERM");
		if (term == NULL)
=======
	if (ctl->opt_blength && vc_only(ctl, "--blength")) {
		printf("\033[11;%d]", ctl->opt_blength_l);
	}

	/* -bfreq freqnumber */
	if (ctl->opt_bfreq && vc_only(ctl, "--bfreq")) {
		printf("\033[10;%d]", ctl->opt_bfreq_f);
	}
}

static void init_terminal(struct setterm_control *ctl)
{
	int term_errno;

	if (!ctl->opt_te_terminal_name) {
		ctl->opt_te_terminal_name = getenv("TERM");
		if (ctl->opt_te_terminal_name == NULL)
>>>>>>> master-vanilla
			errx(EXIT_FAILURE, _("$TERM is not defined."));
	}

	/* Find terminfo entry. */
<<<<<<< HEAD

	if (setupterm(term, 1, &errret))
		switch(errret) {
		case -1:
			errx(EXIT_FAILURE, _("terminfo database cannot be found"));
		case 0:
			errx(EXIT_FAILURE, _("%s: unknown terminal type"), term);
=======
	if (setupterm(ctl->opt_te_terminal_name, STDOUT_FILENO, &term_errno))
		switch (term_errno) {
		case -1:
			errx(EXIT_FAILURE, _("terminfo database cannot be found"));
		case 0:
			errx(EXIT_FAILURE, _("%s: unknown terminal type"), ctl->opt_te_terminal_name);
>>>>>>> master-vanilla
		case 1:
			errx(EXIT_FAILURE, _("terminal is hardcopy"));
		}

	/* See if the terminal is a virtual console terminal. */
<<<<<<< HEAD

	vcterm = (!strncmp(term, "con", 3) || !strncmp(term, "linux", 5));

	/* Perform the selected options. */

	perform_sequence(vcterm);
=======
	ctl->vcterm = (!strncmp(ctl->opt_te_terminal_name, "con", 3) ||
		       !strncmp(ctl->opt_te_terminal_name, "linux", 5));
}


int main(int argc, char **argv)
{
	struct setterm_control ctl = { 0 };

	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);
	atexit(close_stdout);

	if (argc < 2)
		usage(stderr);

	parse_option(&ctl, argc, argv);
	init_terminal(&ctl);
	perform_sequence(&ctl);
>>>>>>> master-vanilla

	return EXIT_SUCCESS;
}
