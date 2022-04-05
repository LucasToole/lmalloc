/* Lucas Toole 2022 */

#ifndef LMALLOC_H
#define LMALLOC_H


void *l_malloc(size_t);
void l_free(void *);
void *l_calloc(size_t, size_t);
void *l_realloc(void *, size_t);


#endif /* LMALLOC_H */
