/* Lucas Toole 2022 */

#include <stdio.h>
#include <stdlib.h>

#include "lmalloc.h"

void *l_malloc(size_t size) {
	printf("%ld\n", size);
	return NULL;
}

void l_free(void *ptr) {
	printf("%p\n", ptr);
}

void *l_calloc(size_t nmemb, size_t size) {
	printf("%ld, %ld\n", nmemb, size);
	return NULL;
}

void *l_realloc(void *ptr, size_t size) {
	printf("%p, %ld\n", ptr, size);
	return NULL;
}
