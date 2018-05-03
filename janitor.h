#ifndef JANITOR_H
#define JANITOR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define BLOCK_SIZE sizeof(janitor_t)

typedef janitor_block {
    size_t size;
    struct janitor_block *next;
    int flag;
} janitor_t;

void *
jmalloc(size_t size);

void *
jcalloc(size_t num, size_t size);

void *
jrealloc(void *block, size_t size);

void
jfree(void *block);

#endif
