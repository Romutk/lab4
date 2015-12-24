#!/bin/sh

#
# Helps generate autoconf/automake stuff, when code is checked out from SCM.
#
# Copyright (C) 2006-2010 - Karel Zak <kzak@redhat.com>
#

srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.

THEDIR=`pwd`
cd $srcdir
DIE=0
<<<<<<< HEAD
HAS_GTKDOC=1

test -f mount/mount.c || {
	echo
	echo "You must run this script in the top-level util-linux directory"
=======

# provide simple gettext backward compatibility
autopoint_fun ()
{
	# we have to deal with set -e ...
	ret="0"

	# check against this hardcoded set of alternative gettext versions
	gt_ver=`gettext --version |\
		sed -n -e 's/.* \(0\.17\|0\.18\|0\.18\.[1-2]\)$/\1/p'`

	if [ -n "$gt_ver" ]; then
		echo "warning: forcing autopoint to use old gettext $gt_ver"
		rm -f configure.ac.autogenbak
		sed -i.autogenbak configure.ac \
			-e "s/\(AM_GNU_GETTEXT_VERSION\).*/\1([$gt_ver])/"
	fi

	autopoint "$@" || ret=$?

	if [ -n "$gt_ver" ]; then
		mv configure.ac.autogenbak configure.ac
	fi

	return $ret
}

test -f sys-utils/mount.c || {
	echo
	echo "You must run this script in the top-level util-linux directory."
>>>>>>> master-vanilla
	echo
	DIE=1
}

(autopoint --version) < /dev/null > /dev/null 2>&1 || {
        echo
<<<<<<< HEAD
        echo "You must have autopoint installed to generate util-linux build system."
=======
        echo "You must have autopoint installed to generate the util-linux build system."
>>>>>>> master-vanilla
        echo "The autopoint command is part of the GNU gettext package."
	echo
        DIE=1
}
(autoconf --version) < /dev/null > /dev/null 2>&1 || {
	echo
<<<<<<< HEAD
	echo "You must have autoconf installed to generate util-linux build system."
=======
	echo "You must have autoconf installed to generate the util-linux build system."
>>>>>>> master-vanilla
	echo
	DIE=1
}
(autoheader --version) < /dev/null > /dev/null 2>&1 || {
	echo
<<<<<<< HEAD
	echo "You must have autoheader installed to generate util-linux build system."
=======
	echo "You must have autoheader installed to generate the util-linux build system."
>>>>>>> master-vanilla
	echo "The autoheader command is part of the GNU autoconf package."
	echo
	DIE=1
}
<<<<<<< HEAD
(libtool --version) < /dev/null > /dev/null 2>&1 || {
	echo
	echo "You must have libtool-2 installed to generate util-linux build system."
=======
(libtoolize --version) < /dev/null > /dev/null 2>&1 || {
	echo
	echo "You must have libtool-2 installed to generate the util-linux build system."
>>>>>>> master-vanilla
	echo
	DIE=1
}
(automake --version) < /dev/null > /dev/null 2>&1 || {
	echo
<<<<<<< HEAD
	echo "You must have automake installed to generate util-linux build system."
	echo 
=======
	echo "You must have automake installed to generate the util-linux build system."
	echo
>>>>>>> master-vanilla
	DIE=1
}

ltver=$(libtoolize --version | awk '/^libtoolize/ { print $4 }')
<<<<<<< HEAD
=======
ltver=${ltver:-"none"}
>>>>>>> master-vanilla
test ${ltver##2.} = "$ltver" && {
	echo "You must have libtool version >= 2.x.x, but you have $ltver."
	DIE=1
}

if test "$DIE" -eq 1; then
	exit 1
fi

echo
<<<<<<< HEAD
echo "Generate build-system by:"
=======
echo "Generating build-system with:"
>>>>>>> master-vanilla
echo "   autopoint:  $(autopoint --version | head -1)"
echo "   aclocal:    $(aclocal --version | head -1)"
echo "   autoconf:   $(autoconf --version | head -1)"
echo "   autoheader: $(autoheader --version | head -1)"
echo "   automake:   $(automake --version | head -1)"
echo "   libtoolize: $(libtoolize --version | head -1)"

rm -rf autom4te.cache

set -e
po/update-potfiles
<<<<<<< HEAD
autopoint --force $AP_OPTS
=======
autopoint_fun --force $AP_OPTS
>>>>>>> master-vanilla
if ! grep -q datarootdir po/Makefile.in.in; then
	echo autopoint does not honor dataroot variable, patching.
	sed -i -e 's/^datadir *=\(.*\)/datarootdir = @datarootdir@\
datadir = @datadir@/g' po/Makefile.in.in
fi
libtoolize --force $LT_OPTS
aclocal -I m4 $AL_OPTS
autoconf $AC_OPTS
autoheader $AH_OPTS

automake --add-missing $AM_OPTS

<<<<<<< HEAD
cd $THEDIR
=======
cd "$THEDIR"
>>>>>>> master-vanilla

echo
echo "Now type '$srcdir/configure' and 'make' to compile."
echo


