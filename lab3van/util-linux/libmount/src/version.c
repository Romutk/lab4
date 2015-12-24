/*
<<<<<<< HEAD
 * version.c - Return the version of the blkid library
=======
 * version.c - Return the version of the libmount library
>>>>>>> master-vanilla
 *
 * Copyright (C) 2008 Karel Zak <kzak@redhat.com>
 * [Based on libblkid/version.c by Theodore Ts'o]
 *
 * See COPYING.libmount for the License of this software.
 */

/**
<<<<<<< HEAD
 * SECTION: version
 * @title: Version functions
 * @short_description: functions to get library version.
=======
 * SECTION: version-utils
 * @title: Version functions
 * @short_description: functions to get the library version.
>>>>>>> master-vanilla
 */

#include <ctype.h>

#include "mountP.h"

static const char *lib_version = LIBMOUNT_VERSION;
<<<<<<< HEAD
=======
static const char *lib_features[] = {
#ifdef HAVE_LIBSELINUX
	"selinux",
#endif
#ifdef HAVE_SMACK
	"smack",
#endif
#if !defined(NDEBUG)
	"assert",	/* libc assert.h stuff */
#endif
	"debug",	/* always enabled */
	NULL
};
>>>>>>> master-vanilla

/**
 * mnt_parse_version_string:
 * @ver_string: version string (e.g "2.18.0")
 *
 * Returns: release version code.
 */
int mnt_parse_version_string(const char *ver_string)
{
	const char *cp;
	int version = 0;

<<<<<<< HEAD
=======
	assert(ver_string);

>>>>>>> master-vanilla
	for (cp = ver_string; *cp; cp++) {
		if (*cp == '.')
			continue;
		if (!isdigit(*cp))
			break;
		version = (version * 10) + (*cp - '0');
	}
	return version;
}

/**
 * mnt_get_library_version:
<<<<<<< HEAD
 * @ver_string: return pointer to the static library version string
=======
 * @ver_string: return pointer to the static library version string if not NULL
>>>>>>> master-vanilla
 *
 * Returns: release version number.
 */
int mnt_get_library_version(const char **ver_string)
{
	if (ver_string)
		*ver_string = lib_version;

	return mnt_parse_version_string(lib_version);
}

<<<<<<< HEAD
=======
/**
 * mnt_get_library_features:
 * @features: returns a pointer to the static array of strings, the array is
 *            terminated by NULL.
 *
 * Returns: number of items in the features array not including the last NULL,
 *          or less than zero in case of error
 *
 * Example:
 * <informalexample>
 *   <programlisting>
 *	const char *features;
 *
 *	mnt_get_library_features(&features);
 *	while (features && *features)
 *		printf("%s\n", *features++);
 *   </programlisting>
 * </informalexample>
 *
 */
int mnt_get_library_features(const char ***features)
{
	if (!features)
		return -EINVAL;

	*features = lib_features;
	return ARRAY_SIZE(lib_features) - 1;
}

>>>>>>> master-vanilla
#ifdef TEST_PROGRAM
int test_version(struct libmnt_test *ts, int argc, char *argv[])
{
	const char *ver;
<<<<<<< HEAD
=======
	const char **features;
>>>>>>> master-vanilla

	mnt_get_library_version(&ver);

	printf("Library version: %s\n", ver);
	printf("Library API version: " LIBMOUNT_VERSION "\n");
<<<<<<< HEAD
=======
	printf("Library features:");

	mnt_get_library_features(&features);
	while (features && *features)
		printf(" %s", *features++);
>>>>>>> master-vanilla

	if (mnt_get_library_version(NULL) ==
			mnt_parse_version_string(LIBMOUNT_VERSION))
		return 0;

	return -1;
}

int main(int argc, char *argv[])
{
	struct libmnt_test ts[] = {
		{ "--print", test_version, "print versions" },
		{ NULL }
	};

	return mnt_run_test(ts, argc, argv);
}
#endif
