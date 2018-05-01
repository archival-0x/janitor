#include "janitor.h"

janitor_t *head, *tail;
pthread_mutex_t global_malloc_lock;

static janitor_t *
get_free_block(size_t size)
{   
    /* set head block as current block */
    janitor_t *current = head;
    while (current) {
        /* check if block is marked free and if ample space is provided for allocation */
        if (current->flag && current->size >= size)
            return current;
        /* check next block */
        current = current->next;
    }
    return NULL;
}

void *
jmalloc(size_t size)
{
    size_t total_size;
    void *block;
    janitor_t *header;

    /* error-check size */
    if (!size)
        return NULL;

    pthread_mutex_lock(&global_malloc_lock);

    /* first-fit: check if there already is a block size that meets our allocation size and immediately fill it and return */
    header = get_free_block(size);
    if (header){
        header->flag = 0;
        pthread_mutex_unlock(&global_malloc_lock);
        return (void *) (header + 1);
    }
    
    /* if not found, continue by extending the size of the heap with sbrk, extending the break to meet our allocation */
    total_size = BLOCK_SIZE + size;
    block = sbrk(total_size);
    if (block == (void *) -1 ) {
        pthread_mutex_unlock(&global_malloc_lock);
        return NULL;
    }
    
    /* set struct entries with allocation specification and mark as not free */
    header = block;
    header->size = size;
    header->flag = 0;
    header->next = NULL;

    if (!head)
        head = header;
    if (tail)
        tail->next = header;

    tail = header;
    pthread_mutex_unlock(&global_malloc_lock);
    return (void *)(header - 1);
}

void *
jcalloc(void *block, size_t size)
{

}

void *
jrealloc(void *block, size_t size)
{

}

void
free(void *block)
{
    janitor_t *header, *tmp;
    void * programbreak;

    if (!block)
        return NULL;
    
    pthread_mutex_lock(&global_malloc_lock);
    header = (janitor_t *) block - 1;
    
    programbreak = sbrk(0);
    // TODO
}


