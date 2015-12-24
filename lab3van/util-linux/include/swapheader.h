#ifndef _SWAPHEADER_H
#define _SWAPHEADER_H

<<<<<<< HEAD
struct swap_header_v1 {
        char         bootbits[1024];    /* Space for disklabel etc. */
	unsigned int version;
	unsigned int last_page;
	unsigned int nr_badpages;
	unsigned int padding[125];
	unsigned int badpages[1];
};


#define SWAP_UUID_LENGTH 16
#define SWAP_LABEL_LENGTH 16

struct swap_header_v1_2 {
	char	      bootbits[1024];    /* Space for disklabel etc. */
	unsigned int  version;
	unsigned int  last_page;
	unsigned int  nr_badpages;
	unsigned char uuid[SWAP_UUID_LENGTH];
	char	      volume_name[SWAP_LABEL_LENGTH];
	unsigned int  padding[117];
	unsigned int  badpages[1];
=======
#define SWAP_VERSION 1
#define SWAP_UUID_LENGTH 16
#define SWAP_LABEL_LENGTH 16
#define SWAP_SIGNATURE "SWAPSPACE2"
#define SWAP_SIGNATURE_SZ (sizeof(SWAP_SIGNATURE) - 1)

#include <stdint.h>

struct swap_header_v1_2 {
	char	      bootbits[1024];    /* Space for disklabel etc. */
	uint32_t      version;
	uint32_t      last_page;
	uint32_t      nr_badpages;
	unsigned char uuid[SWAP_UUID_LENGTH];
	char	      volume_name[SWAP_LABEL_LENGTH];
	uint32_t      padding[117];
	uint32_t      badpages[1];
>>>>>>> master-vanilla
};

#endif /* _SWAPHEADER_H */