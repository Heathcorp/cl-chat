#include "trans_buffer.h"
#include "utils.h"

#include <stdlib.h>

struct trans_buffer* trans_buffer_init() {
	struct trans_buffer* obj = malloc(sizeof(struct trans_buffer));

	obj->data = malloc(TRANS_BUFFER_SIZE);
	obj->length = 0;
	obj->readLength = 0;
	obj->containsUnread = FALSE;

	return obj;
}

int trans_buffer_free(struct trans_buffer* obj) {
	free(obj->data);
	free(obj);
}
