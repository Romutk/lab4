/*
 * EFI GPT partition parsing code
 *
 * Copyright (C) 2009 Karel Zak <kzak@redhat.com>
 *
 * This file may be redistributed under the terms of the
 * GNU Lesser General Public License.
 *
 * This code is not copy & past from any other implementation.
 *
 * For more information about GPT start your study at:
 * http://en.wikipedia.org/wiki/GUID_Partition_Table
 * http://technet.microsoft.com/en-us/library/cc739412(WS.10).aspx
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
<<<<<<< HEAD

#include "partitions.h"
#include "crc32.h"
#include "dos.h"
=======
#include <limits.h>

#include "partitions.h"
#include "crc32.h"
>>>>>>> master-vanilla

#define GPT_PRIMARY_LBA	1

/* Signature - “EFI PART” */
#define GPT_HEADER_SIGNATURE 0x5452415020494645ULL
<<<<<<< HEAD
=======
#define GPT_HEADER_SIGNATURE_STR "EFI PART"
>>>>>>> master-vanilla

/* basic types */
typedef uint16_t efi_char16_t;

/* UUID */
typedef struct {
	uint32_t time_low;
	uint16_t time_mid;
	uint16_t time_hi_and_version;
	uint8_t clock_seq_hi;
	uint8_t clock_seq_low;
	uint8_t node[6];
} efi_guid_t;


#define GPT_UNUSED_ENTRY_GUID \
	    ((efi_guid_t) { 0x00000000, 0x0000, 0x0000, 0x00, 0x00, \
	                    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }})
struct gpt_header {
	uint64_t	signature;		/* "EFI PART" */
	uint32_t	revision;
<<<<<<< HEAD
	uint32_t	header_size;		/* usualy 92 bytes */
=======
	uint32_t	header_size;		/* usually 92 bytes */
>>>>>>> master-vanilla
	uint32_t	header_crc32;		/* checksum of header with this
						 * field zeroed during calculation */
	uint32_t	reserved1;

	uint64_t	my_lba;			/* location of this header copy */
	uint64_t	alternate_lba;		/* location of the other header copy */
	uint64_t	first_usable_lba;	/* lirst usable LBA for partitions */
	uint64_t	last_usable_lba;	/* last usable LBA for partitions */

	efi_guid_t	disk_guid;		/* disk UUID */

	uint64_t	partition_entries_lba;	/* always 2 in primary header copy */
	uint32_t	num_partition_entries;
	uint32_t	sizeof_partition_entry;
	uint32_t	partition_entry_array_crc32;

	/*
	 * The rest of the block is reserved by UEFI and must be zero. EFI
	 * standard handles this by:
	 *
	 * uint8_t		reserved2[ BLKSSZGET - 92 ];
	 *
	 * This definition is useless in practice. It is necessary to read
	 * whole block from the device rather than sizeof(struct gpt_header)
	 * only.
	 */
} __attribute__ ((packed));

/*** not used
struct gpt_entry_attributes {
	uint64_t	required_to_function:1;
	uint64_t	reserved:47;
        uint64_t	type_guid_specific:16;
} __attribute__ ((packed));
***/

struct gpt_entry {
	efi_guid_t	partition_type_guid;	/* type UUID */
	efi_guid_t	unique_partition_guid;	/* partition UUID */
	uint64_t	starting_lba;
	uint64_t	ending_lba;

	/*struct gpt_entry_attributes	attributes;*/

	uint64_t	attributes;

	efi_char16_t	partition_name[72 / sizeof(efi_char16_t)]; /* UTF-16LE string*/
} __attribute__ ((packed));


/*
 * EFI uses crc32 with ~0 seed and xor's with ~0 at the end.
 */
<<<<<<< HEAD
static inline uint32_t count_crc32(const unsigned char *buf, size_t len)
{
	return (crc32(~0L, buf, len) ^ ~0L);
=======
static inline uint32_t count_crc32(const unsigned char *buf, size_t len,
				   size_t exclude_off, size_t exclude_len)
{
	return (crc32_exclude_offset(~0L, buf, len, exclude_off, exclude_len) ^ ~0L);
>>>>>>> master-vanilla
}

static inline unsigned char *get_lba_buffer(blkid_probe pr,
					uint64_t lba, size_t bytes)
{
	return blkid_probe_get_buffer(pr,
			blkid_probe_get_sectorsize(pr) * lba, bytes);
}

static inline int guidcmp(efi_guid_t left, efi_guid_t right)
{
	return memcmp(&left, &right, sizeof (efi_guid_t));
}

/*
<<<<<<< HEAD
 * UUID is traditionaly 16 byte big-endian array, except Intel EFI
=======
 * UUID is traditionally 16 byte big-endian array, except Intel EFI
>>>>>>> master-vanilla
 * specification where the UUID is a structure of little-endian fields.
 */
static void swap_efi_guid(efi_guid_t *uid)
{
	uid->time_low = swab32(uid->time_low);
	uid->time_mid = swab16(uid->time_mid);
	uid->time_hi_and_version = swab16(uid->time_hi_and_version);
}

static int last_lba(blkid_probe pr, uint64_t *lba)
{
	blkid_loff_t sz = blkid_probe_get_size(pr);
<<<<<<< HEAD
	if (sz < blkid_probe_get_sectorsize(pr))
		return -1;

	*lba = (sz >> 9) - 1;
=======
	unsigned int ssz = blkid_probe_get_sectorsize(pr);

	if (sz < ssz)
		return -1;

	*lba = (sz / ssz) - 1ULL;
>>>>>>> master-vanilla
	return 0;
}

/*
 * Protective (legacy) MBR.
 *
 * This MBR contains standard DOS partition table with a single partition, type
 * of 0xEE.  The partition usually encompassing the entire GPT drive - or 2TiB
 * for large disks.
 *
 * Note that Apple uses GPT/MBR hybrid disks, where the DOS partition table is
 * synchronized with GPT. This synchronization has many restriction of course
 * (due DOS PT limitations).
 *
 * Note that the PMBR detection is optional (enabled by default) and could be
 * disabled by BLKID_PARTS_FOPCE_GPT flag (see also blkid_paertitions_set_flags()).
 */
<<<<<<< HEAD
static int is_pmbr_valid(blkid_probe pr)
=======
static int is_pmbr_valid(blkid_probe pr, int *has)
>>>>>>> master-vanilla
{
	int flags = blkid_partitions_get_flags(pr);
	unsigned char *data;
	struct dos_partition *p;
	int i;

<<<<<<< HEAD
=======
	if (has)
		*has = 0;
>>>>>>> master-vanilla
	if (flags & BLKID_PARTS_FORCE_GPT)
		goto ok;			/* skip PMBR check */

	data = blkid_probe_get_sector(pr, 0);
<<<<<<< HEAD
	if (!data)
		goto failed;

	if (!is_valid_mbr_signature(data))
		goto failed;

	p = (struct dos_partition *) (data + BLKID_MSDOS_PT_OFFSET);

	for (i = 0; i < 4; i++, p++) {
		if (p->sys_type == BLKID_GPT_PARTITION)
=======
	if (!data) {
		if (errno)
			return -errno;
		goto failed;
	}

	if (!mbr_is_valid_magic(data))
		goto failed;

	for (i = 0, p = mbr_get_partition(data, 0); i < 4; i++, p++) {
		if (p->sys_ind == MBR_GPT_PARTITION)
>>>>>>> master-vanilla
			goto ok;
	}
failed:
	return 0;
ok:
<<<<<<< HEAD
=======
	if (has)
		*has = 1;
>>>>>>> master-vanilla
	return 1;
}

/*
 * Reads GPT header to @hdr and returns a pointer to @hdr or NULL in case of
 * error. The function also returns GPT entries in @ents.
 *
 * Note, this function does not allocate any memory. The GPT header has fixed
 * size so we use stack, and @ents returns memory from libblkid buffer (so the
 * next blkid_probe_get_buffer() will overwrite this buffer).
 *
 * This function checks validity of header and entries array. A corrupted
 * header is not returned.
 */
static struct gpt_header *get_gpt_header(
				blkid_probe pr, struct gpt_header *hdr,
				struct gpt_entry **ents, uint64_t lba,
				uint64_t lastlba)
{
	struct gpt_header *h;
<<<<<<< HEAD
	uint32_t crc, orgcrc;
=======
	uint32_t crc;
>>>>>>> master-vanilla
	uint64_t lu, fu;
	size_t esz;
	uint32_t hsz, ssz;

	ssz = blkid_probe_get_sectorsize(pr);

	/* whole sector is allocated for GPT header */
	h = (struct gpt_header *) get_lba_buffer(pr, lba, ssz);
	if (!h)
		return NULL;

	if (le64_to_cpu(h->signature) != GPT_HEADER_SIGNATURE)
		return NULL;

	hsz = le32_to_cpu(h->header_size);

	/* EFI: The HeaderSize must be greater than 92 and must be less
	 *      than or equal to the logical block size.
	 */
	if (hsz > ssz || hsz < sizeof(*h))
		return NULL;

	/* Header has to be verified when header_crc32 is zero */
<<<<<<< HEAD
	orgcrc = le32_to_cpu(h->header_crc32);
	h->header_crc32 = 0;

	crc = count_crc32((unsigned char *) h, hsz);
	if (crc != orgcrc) {
		DBG(DEBUG_LOWPROBE, printf("GPT header corrupted\n"));
		return NULL;
	}
	h->header_crc32 = cpu_to_le32(orgcrc);

	/* Valid header has to be at MyLBA */
	if (le64_to_cpu(h->my_lba) != lba) {
		DBG(DEBUG_LOWPROBE, printf(
			"GPT->MyLBA mismatch with real position\n"));
=======
	crc = count_crc32((unsigned char *) h, hsz,
			offsetof(struct gpt_header, header_crc32),
			sizeof(h->header_crc32));

	if (crc != le32_to_cpu(h->header_crc32)) {
		DBG(LOWPROBE, ul_debug("GPT header corrupted"));
		return NULL;
	}

	/* Valid header has to be at MyLBA */
	if (le64_to_cpu(h->my_lba) != lba) {
		DBG(LOWPROBE, ul_debug(
			"GPT->MyLBA mismatch with real position"));
>>>>>>> master-vanilla
		return NULL;
	}

	fu = le64_to_cpu(h->first_usable_lba);
	lu = le64_to_cpu(h->last_usable_lba);

	/* Check if First and Last usable LBA makes sense */
	if (lu < fu || fu > lastlba || lu > lastlba) {
<<<<<<< HEAD
		DBG(DEBUG_LOWPROBE, printf(
			"GPT->{First,Last}UsableLBA out of range\n"));
=======
		DBG(LOWPROBE, ul_debug(
			"GPT->{First,Last}UsableLBA out of range"));
>>>>>>> master-vanilla
		return NULL;
	}

	/* The header has to be outside usable range */
	if (fu < lba && lba < lu) {
<<<<<<< HEAD
		DBG(DEBUG_LOWPROBE, printf("GPT header is inside usable area\n"));
=======
		DBG(LOWPROBE, ul_debug("GPT header is inside usable area"));
		return NULL;
	}

	if (le32_to_cpu(h->num_partition_entries) == 0 ||
	    le32_to_cpu(h->sizeof_partition_entry) == 0 ||
	    ULONG_MAX / le32_to_cpu(h->num_partition_entries) < le32_to_cpu(h->sizeof_partition_entry)) {
		DBG(LOWPROBE, ul_debug("GPT entries undefined"));
>>>>>>> master-vanilla
		return NULL;
	}

	/* Size of blocks with GPT entries */
	esz = le32_to_cpu(h->num_partition_entries) *
			le32_to_cpu(h->sizeof_partition_entry);
<<<<<<< HEAD
	if (!esz) {
		DBG(DEBUG_LOWPROBE, printf("GPT entries undefined\n"));
		return NULL;
	}
=======
>>>>>>> master-vanilla

	/* The header seems valid, save it
	 * (we don't care about zeros in hdr->reserved2 area) */
	memcpy(hdr, h, sizeof(*h));
	h = hdr;

	/* Read GPT entries */
	*ents = (struct gpt_entry *) get_lba_buffer(pr,
				le64_to_cpu(h->partition_entries_lba), esz);
	if (!*ents) {
<<<<<<< HEAD
		DBG(DEBUG_LOWPROBE, printf("GPT entries unreadable\n"));
=======
		DBG(LOWPROBE, ul_debug("GPT entries unreadable"));
>>>>>>> master-vanilla
		return NULL;
	}

	/* Validate entries */
<<<<<<< HEAD
	crc = count_crc32((unsigned char *) *ents, esz);
	if (crc != le32_to_cpu(h->partition_entry_array_crc32)) {
		DBG(DEBUG_LOWPROBE, printf("GPT entries corrupted\n"));
=======
	crc = count_crc32((unsigned char *) *ents, esz, 0, 0);
	if (crc != le32_to_cpu(h->partition_entry_array_crc32)) {
		DBG(LOWPROBE, ul_debug("GPT entries corrupted"));
>>>>>>> master-vanilla
		return NULL;
	}

	return h;
}

static int probe_gpt_pt(blkid_probe pr,
		const struct blkid_idmag *mag __attribute__((__unused__)))
{
	uint64_t lastlba = 0, lba;
	struct gpt_header hdr, *h;
	struct gpt_entry *e;
	blkid_parttable tab = NULL;
	blkid_partlist ls;
	uint64_t fu, lu;
	uint32_t ssf, i;
<<<<<<< HEAD

=======
	efi_guid_t guid;
	int ret;
>>>>>>> master-vanilla

	if (last_lba(pr, &lastlba))
		goto nothing;

<<<<<<< HEAD
	if (!is_pmbr_valid(pr))
		goto nothing;

	h = get_gpt_header(pr, &hdr, &e, (lba = GPT_PRIMARY_LBA), lastlba);
	if (!h)
		h = get_gpt_header(pr, &hdr, &e, (lba = lastlba), lastlba);

	if (!h)
		goto nothing;

	blkid_probe_use_wiper(pr, lba * blkid_probe_get_size(pr), 8);

	if (blkid_partitions_need_typeonly(pr))
		/* caller does not ask for details about partitions */
		return 0;

	ls = blkid_probe_get_partlist(pr);
	if (!ls)
		goto err;

	tab = blkid_partlist_new_parttable(ls, "gpt", lba << 9);
	if (!tab)
		goto err;

=======
	ret = is_pmbr_valid(pr, NULL);
	if (ret < 0)
		return ret;
	else if (ret == 0)
		goto nothing;

	errno = 0;
	h = get_gpt_header(pr, &hdr, &e, (lba = GPT_PRIMARY_LBA), lastlba);
	if (!h && !errno)
		h = get_gpt_header(pr, &hdr, &e, (lba = lastlba), lastlba);

	if (!h) {
		if (errno)
			return -errno;
		goto nothing;
	}

	blkid_probe_use_wiper(pr, lba * blkid_probe_get_size(pr), 8);

	if (blkid_probe_set_magic(pr, blkid_probe_get_sectorsize(pr) * lba,
			      sizeof(GPT_HEADER_SIGNATURE_STR) - 1,
			      (unsigned char *) GPT_HEADER_SIGNATURE_STR))
		goto err;

	guid = h->disk_guid;
	swap_efi_guid(&guid);

	if (blkid_partitions_need_typeonly(pr)) {
		/* Non-binary interface -- caller does not ask for details
		 * about partitions, just set generic varibles only. */
		blkid_partitions_set_ptuuid(pr, (unsigned char *) &guid);
		return BLKID_PROBE_OK;
	}

	ls = blkid_probe_get_partlist(pr);
	if (!ls)
		goto nothing;

	tab = blkid_partlist_new_parttable(ls, "gpt",
				blkid_probe_get_sectorsize(pr) * lba);
	if (!tab)
		goto err;

	blkid_parttable_set_uuid(tab, (const unsigned char *) &guid);

>>>>>>> master-vanilla
	ssf = blkid_probe_get_sectorsize(pr) / 512;

	fu = le64_to_cpu(h->first_usable_lba);
	lu = le64_to_cpu(h->last_usable_lba);

	for (i = 0; i < le32_to_cpu(h->num_partition_entries); i++, e++) {

		blkid_partition par;
		uint64_t start = le64_to_cpu(e->starting_lba);
		uint64_t size = le64_to_cpu(e->ending_lba) -
					le64_to_cpu(e->starting_lba) + 1ULL;

		/* 00000000-0000-0000-0000-000000000000 entry */
		if (!guidcmp(e->partition_type_guid, GPT_UNUSED_ENTRY_GUID)) {
			blkid_partlist_increment_partno(ls);
			continue;
		}
		/* the partition has to inside usable range */
		if (start < fu || start + size - 1 > lu) {
<<<<<<< HEAD
			DBG(DEBUG_LOWPROBE, printf(
				"GPT entry[%d] overflows usable area - ignore\n",
=======
			DBG(LOWPROBE, ul_debug(
				"GPT entry[%d] overflows usable area - ignore",
>>>>>>> master-vanilla
				i));
			blkid_partlist_increment_partno(ls);
			continue;
		}

		par = blkid_partlist_add_partition(ls, tab,
					start * ssf, size * ssf);
		if (!par)
			goto err;

		blkid_partition_set_utf8name(par,
			(unsigned char *) e->partition_name,
			sizeof(e->partition_name), BLKID_ENC_UTF16LE);

<<<<<<< HEAD
		swap_efi_guid(&e->unique_partition_guid);
		swap_efi_guid(&e->partition_type_guid);

		blkid_partition_set_uuid(par,
			(const unsigned char *) &e->unique_partition_guid);

		blkid_partition_set_type_uuid(par,
			(const unsigned char *) &e->partition_type_guid);

		blkid_partition_set_flags(par, e->attributes);
	}

	return 0;

nothing:
	return 1;
err:
	return -1;
=======
		guid = e->unique_partition_guid;
		swap_efi_guid(&guid);
		blkid_partition_set_uuid(par, (const unsigned char *) &guid);

		guid = e->partition_type_guid;
		swap_efi_guid(&guid);
		blkid_partition_set_type_uuid(par, (const unsigned char *) &guid);

		blkid_partition_set_flags(par, le64_to_cpu(e->attributes));
	}

	return BLKID_PROBE_OK;

nothing:
	return BLKID_PROBE_NONE;

err:
	return -ENOMEM;
>>>>>>> master-vanilla
}


const struct blkid_idinfo gpt_pt_idinfo =
{
	.name		= "gpt",
	.probefunc	= probe_gpt_pt,
	.minsz		= 1024 * 1440 + 1,	/* ignore floppies */

	/*
	 * It would be possible to check for DOS signature (0xAA55), but
	 * unfortunately almost all EFI GPT implemenations allow to optionaly
	 * skip the legacy MBR. We follows this behavior and MBR is optional.
	 * See is_valid_pmbr().
	 *
	 * It means we have to always call probe_gpt_pt().
	 */
	.magics		= BLKID_NONE_MAGIC
};

<<<<<<< HEAD
=======


/* probe for *alone* protective MBR */
static int probe_pmbr_pt(blkid_probe pr,
		const struct blkid_idmag *mag __attribute__((__unused__)))
{
	int has = 0;
	struct gpt_entry *e;
	uint64_t lastlba = 0;
	struct gpt_header hdr;

	if (last_lba(pr, &lastlba))
		goto nothing;

	is_pmbr_valid(pr, &has);
	if (!has)
		goto nothing;

	if (!get_gpt_header(pr, &hdr, &e, GPT_PRIMARY_LBA, lastlba) &&
	    !get_gpt_header(pr, &hdr, &e, lastlba, lastlba))
		return 0;
nothing:
	return 1;
}

const struct blkid_idinfo pmbr_pt_idinfo =
{
	.name		= "PMBR",
	.probefunc	= probe_pmbr_pt,
	.magics		=
	{
		{ .magic = "\x55\xAA", .len = 2, .sboff = 510 },
		{ NULL }
	}
};

>>>>>>> master-vanilla
