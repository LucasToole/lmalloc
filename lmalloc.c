/* Lucas Toole 2022 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <sys/mman.h>

#include "lmalloc.h"

#define LMALLOC_MAX_SIZE_CLASSES 10

/*
 * The size classes in bytes are (max):
 * [16, 32, 64, 128, 256, 512, 1024, 2048, 4096, >4096]
 */

struct free_block {
	struct free_block *next_block;
};

struct allocated_block {
	size_t size;
	void *block;
};

static struct free_block *free_list[LMALLOC_MAX_SIZE_CLASSES] = { NULL };

static int malloc_initialized = 0;

static void grow_free(size_t size) {
	size += 4096;
	void *addr = mmap(NULL, size, (PROT_EXEC | PROT_READ | PROT_WRITE), MAP_ANON, -1, 0);
	struct free_block *new = addr;
	if (free_list[LMALLOC_MAX_SIZE_CLASSES - 1]) {
		new->next_block = free_list[LMALLOC_MAX_SIZE_CLASSES - 1];
	}
	else {
		new->next_block = NULL;
	}
	free_list[LMALLOC_MAX_SIZE_CLASSES - 1] = new;
}

void *l_malloc(size_t size) {
	if (size == 0)
		return NULL;

	// Ensure size is divisible by 16
	if (size < 16)
		size = 16;
	else {
		int mod = size % 16;
		size += mod;
	}

	if (!malloc_initialized) {
		grow_free(size);
	}


	int i = ((size / 16) - 1);
	if (i > LMALLOC_MAX_SIZE_CLASSES - 1)
		i = LMALLOC_MAX_SIZE_CLASSES -1;

	struct free_block *tmp_block;

	for (; i < LMALLOC_MAX_SIZE_CLASSES; i++) {
		tmp_block = free_list[i];
		if (tmp_block) {
			if (tmp_block->next_block) {
				free_list[i] = tmp_block->next_block;
			}
			else {
				free_list[i] = NULL;
			}
			tmp_block->next_block = NULL;
			break;
		}
		if (i == (LMALLOC_MAX_SIZE_CLASSES - 1)) {
			grow_free(size);
			i = LMALLOC_MAX_SIZE_CLASSES - 2;
		}
	}

	int list = i;

	struct allocated_block *new_block = (void *)tmp_block;
	new_block->size = size;

	struct new_free_block *free_block = (void*) new_block + new_block->size;
	int diff = pow(2, list + 4) - size;

	if (diff) {
		list = ((diff / 16) - 1);
		free_list[list] = (void *)free_block;
	}
	else {
		free_list[list] = NULL;
	}

	if (errno != ENOMEM) {
		return NULL;
	}

	return new_block->block;
}

void l_free(void *ptr) {
	struct allocated_block *temp_block = ptr - 4;
	int list = ((temp_block->size / 16) - 1);
	struct free_block *new_free_block = (void *) temp_block;
	if (list > LMALLOC_MAX_SIZE_CLASSES - 1)
		list = LMALLOC_MAX_SIZE_CLASSES - 1;

	if (free_list[list]) {
		new_free_block->next_block = free_list[list];
		free_list[list] = new_free_block;
	}
	else {
		new_free_block->next_block = NULL;
		free_list[list] = new_free_block;
	}
}

void *l_calloc(size_t nmemb, size_t size) {
	printf("%ld, %ld\n", nmemb, size);
	return NULL;
}

void *l_realloc(void *ptr, size_t size) {
	printf("%p, %ld\n", ptr, size);
	return NULL;
}
