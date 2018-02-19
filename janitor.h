#ifndef JANITOR_H
#define JANITOR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define JANITOR_INITIAL_GC 1

typedef u32 uint32_t;

typedef struct {
    u32 nObjects;
    u32 maxObjects;

} janitor_t;

typedef struct object{
    u32 marked;
    size_t size;
    void * ptr;

    /* Next object, as this is a linked-list */
    struct object * next;

} JanitorObject;

/* optimized memory allocation */
// static void * smalloc(size_t size);

/* hashing function for pointer */

/* Execute garbage collection */
void janitor_start(janitor_t *gc);
void janitor_stop(janitor_t *gc);

/* Memory allocation */
void * janitor_alloc(janitor_t *gc, size_t size);
void janitor_free(janitor_t *gc);

/* Internal object manipulation */
// static void * janitor_add();
// static void janitor_remove();

/* Mark and sweep */
// static void * janitor_mark(janitor_t *gc);
// static void janitor_sweep(janitor_t *gc);


