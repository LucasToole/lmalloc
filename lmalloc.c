/* Lucas Toole 2022 */

#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>

#include "lmalloc.h"

#define LMALLOC_LIST_SIZE 257

struct l_free_block {
	struct l_free_block *next_block;
};

struct l_alloc_block {
	size_t size;
};

static struct l_free_block *free_list[LMALLOC_LIST_SIZE] = { NULL };

static void grow_list(size_t size) {
	size += 4112;
	struct l_free_block *new_block = (void *)mmap(NULL, size, (PROT_EXEC | PROT_READ | PROT_WRITE), MAP_ANON | MAP_SHARED, -1, 0);
	new_block->next_block = NULL;
	free_list[LMALLOC_LIST_SIZE - 1] = new_block;
}

void *l_malloc(size_t size) {
	if (size == 0)
		return NULL;

	// Ensure size is always divisible by 16
	if (size < 16)
		size = 16;
	size += (size % 16);

	int bucket = (size / 16) - 1;
	if (bucket > LMALLOC_LIST_SIZE)
		bucket = LMALLOC_LIST_SIZE;
	for (; bucket < LMALLOC_LIST_SIZE; bucket++) {
		if (free_list[bucket]) {
			goto found;
		}
	}
	// Not found
	grow_list(size);
	if (errno == ENOMEM)
		return NULL;
	bucket = LMALLOC_LIST_SIZE - 1;

 found:
	uint8_t *new_alloc_block = (uint8_t *)free_list[bucket];
	memcpy(new_alloc_block + 8, &size, 8);

	int new_size = (bucket + 1) * 16;
	int diff = new_size - size;
	if (diff >= 32) {
		struct l_free_block *new_free_block = (void *)new_alloc_block + 16 + size;
		int new_bucket = (diff / 16) - 1;
		if (free_list[new_bucket]) {
			new_free_block->next_block = free_list[new_bucket];
			free_list[new_bucket] = new_free_block;
		}
		else {
			new_free_block->next_block = NULL;
			free_list[new_bucket] = new_free_block;
		}
	}

	if (errno == ENOMEM)
		return NULL;

	return (void *)(new_alloc_block + 16);
}

void l_free(void *ptr) {
	uint8_t *allocated_block = (ptr - 16);
	size_t size = (size_t)*(allocated_block + 8);
	// Ensure size is always divisible by 16
	if (size < 16)
		size = 16;
	size += (size % 16);

	int bucket = (size / 16) - 1;
	if (bucket > LMALLOC_LIST_SIZE)
		bucket = LMALLOC_LIST_SIZE;

	struct l_free_block *new_free_block = (void *) allocated_block;
	if (free_list[bucket])
		new_free_block->next_block = free_list[bucket];
	else
		new_free_block->next_block = NULL;
	free_list[bucket] = new_free_block;
}

void *l_calloc(size_t nmemb, size_t size) {
	uint8_t *memory = l_malloc(nmemb * size);
	memset(memory, 0, nmemb * size);

	return (void *)memory;
}
