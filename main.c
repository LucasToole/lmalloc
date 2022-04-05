/* Lucas Toole 2022 */

#include <stdio.h>

#include "lmalloc.h"

int main(void) {
	printf("Hello Main\n");
	int *nullptr = l_malloc(0);
	printf("Hello %p\n", nullptr);
	return 0;
}
