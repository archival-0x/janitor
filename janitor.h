#ifndef JANITOR_H
#define JANITOR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum {
    INT,
    PAIR
} Type;

typedef struct {
        

} janitor_t;

// Execution of the garbage collector
static void janitor_start(janitor_t *gc);
static void janitor_stop(janitor_t *gc);

// Object manipulation
void * janitor_alloc(janitor_t *gc, size_t size);
void janitor_free(janitor_t *gc);

