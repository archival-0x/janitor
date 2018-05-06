#ifndef JANITOR_H
#define JANITOR_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

struct janitor_block {
    size_t size;
    struct janitor_block *next;
    int flag;
};

typedef struct janitor_block janitor_t;

#define BLOCK_SIZE sizeof(janitor_t)

void *
jmalloc(size_t size);

void *
jcalloc(size_t num, size_t size);

void *
jrealloc(void *block, size_t size);

void
jfree(void *block);

#endif
