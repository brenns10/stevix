/**
 * tags.c: parse tags
 */
#include "pi.h"

#define TAG_START (struct tag *)0x100

/**
 * Locate a particular "tag". These tags are sections of data located in a
 * special segment of memory, which are used to give the OS useful information.
 * One such piece of information is the kernel command line (tag 9).
 * @tag_number: what tag number do you want? (probably tag 9, cmdline)
 * @return: a pointer to the tag (use struct fields for easy access)
 */
struct tag *find_tag(uint16_t tag_number)
{
	struct tag *tag = TAG_START;
	while (tag->num_words) {
		if (tag->tag_number == tag_number) {
			return tag;
		}
		tag = (void*)tag + tag->num_words * 4;
	}
	return 0;
}
