#include "janitor.h"

static size_t *
janitor_hash(void *ptr)
{

}

static void *
smalloc(size_t size)
{   
    /* by calling calloc, we don't need memset */
    void * fd = calloc(size);
    if (!fd){
        fprintf(stderr, "unable to allocate memory\n");
        exit(1);
    }
    
    return fd;
}

void 
janitor_start(janitor_t *gc)
{
    gc->nObjects = 0;
    gc->nFrees = 0;
    gc->maxObjects = JANITOR_INITIAL_GC;

}

void 
janitor_stop(janitor_t *gc)
{
    janitor_sweep(gc);
}
