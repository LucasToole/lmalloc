/* Lucas Toole 2022 */

#include <stdio.h>

#include "lmalloc.h"

int main(void) {
	printf("Hello Main\n");

	int *coolarray = l_malloc(4 * 20);
	l_free(coolarray);

	return 0;
}
