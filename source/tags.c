/**
 * tags.c: parse tags
 */
#include "pi.h"

#define TAG_START (struct tag *)0x100

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
