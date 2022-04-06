# Lucas's Malloc

A very simple Malloc/Free/Calloc using segregated fits.

## Design
Lmalloc uses a very simplistic implementation of segregated fits. It can only allocate
memory in multiples of 16 and is designed mainly for allocations smaller than 4096 bytes.
At this time it does not support coalescing and is therefore unsuitable for most tasks,
but was fun to implement.

Note for anyone reading this: This was a fun project that was completed in a day. Please
do not use it. It is not well designed, it is most likely not bug free, I have my doubts
about how performant it is compared to existing allocators, and it is not standards compliant.
