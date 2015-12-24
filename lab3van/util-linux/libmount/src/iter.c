/*
 * Copyright (C) 2009 Karel Zak <kzak@redhat.com>
 *
 * This file may be redistributed under the terms of the
 * GNU Lesser General Public License.
 */

/**
 * SECTION: iter
 * @title: Iterator
 * @short_description: unified iterator
 *
<<<<<<< HEAD
 * The iterator keeps direction and last position for access to the internal
 * library tables/lists.
=======
 * The iterator keeps the direction and the last position
 * for access to the internal library tables/lists.
>>>>>>> master-vanilla
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mountP.h"

/**
 * mnt_new_iter:
 * @direction: MNT_INTER_{FOR,BACK}WARD direction
 *
 * Returns: newly allocated generic libmount iterator.
 */
struct libmnt_iter *mnt_new_iter(int direction)
{
	struct libmnt_iter *itr = calloc(1, sizeof(*itr));
	if (!itr)
		return NULL;
	itr->direction = direction;
	return itr;
}

/**
 * mnt_free_iter:
 * @itr: iterator pointer
 *
<<<<<<< HEAD
 * Deallocates iterator.
=======
 * Deallocates the iterator.
>>>>>>> master-vanilla
 */
void mnt_free_iter(struct libmnt_iter *itr)
{
	free(itr);
}

/**
 * mnt_reset_iter:
 * @itr: iterator pointer
<<<<<<< HEAD
 * @direction: MNT_INTER_{FOR,BACK}WARD or -1 to keep the derection unchanged
 *
 * Resets iterator.
 */
void mnt_reset_iter(struct libmnt_iter *itr, int direction)
{
	assert(itr);

	if (direction == -1)
		direction = itr->direction;

	if (itr) {
		memset(itr, 0, sizeof(*itr));
		itr->direction = direction;
	}
=======
 * @direction: MNT_INTER_{FOR,BACK}WARD or -1 to keep the direction unchanged
 *
 * Resets the iterator.
 */
void mnt_reset_iter(struct libmnt_iter *itr, int direction)
{
	if (direction == -1)
		direction = itr->direction;

	memset(itr, 0, sizeof(*itr));
	itr->direction = direction;
>>>>>>> master-vanilla
}

/**
 * mnt_iter_get_direction:
 * @itr: iterator pointer
 *
<<<<<<< HEAD
 * Returns: MNT_INTER_{FOR,BACK}WARD or negative number in case of error.
 */
int mnt_iter_get_direction(struct libmnt_iter *itr)
{
	assert(itr);
	return itr ? itr->direction : -EINVAL;
=======
 * Returns: MNT_INTER_{FOR,BACK}WARD
 */
int mnt_iter_get_direction(struct libmnt_iter *itr)
{
	return itr->direction;
>>>>>>> master-vanilla
}