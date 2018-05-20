#include "janitor.h"

// in order to keep track of our head and tail
janitor_t *head, *tail;

// implement a mutex such that threads do not interfere with memory allocation
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
    /* define our static variables */
    size_t total_size;  // represents total size of linked list
    void *block;        // current block in context
    janitor_t *header;  // current context block

    /* error-check size */
    if (!size)
        return NULL;

    /* critical section start */
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

    /* switch context to next free block
        - if there is no head block for the list, set header as head
        - if a tail block is present, set the next element to point to header, now the new tail
    */
    if (!head)
        head = header;
    if (tail)
        tail->next = header;

    tail = header;

    /* unlock critical section */
    pthread_mutex_unlock(&global_malloc_lock);

    /* returned memory after break */
    return (void *)(header + 1);
}

void *
jcalloc(size_t num, size_t size)
{
    size_t total_size;
    void *block;

    /* check if parameters were provided */
    if (!num || !size)
        return (void *) NULL;

    /* check if size_t bounds adhere to multiplicative inverse properties */
    total_size = num * size;
    if ( size != total_size / num)
        return (void *) NULL;

    /* perform conventional malloc with total_size */
    block = jmalloc(total_size);
    if (!block)
        return (void *) NULL;

    /* zero out our newly heap allocated block */
    memset(block, 0, size);
    return block;
}

void
jfree(void *block)
{
  janitor_t *header, *tmp;
  void * programbreak;

  /* if the block is provided */
  if (!block)
      return (void) NULL;

  /* start critical section */
  pthread_mutex_lock(&global_malloc_lock);

  /* set header as previous bloc
  header = (janitor_t *) block - 1;

  /* start programbreak at byte 0 */
  programbreak = sbrk(0);

  /* start to NULL out block until break point of heap
      - header (previous block) size + target block should meet
  */
  if (( char *) block + header->size == programbreak){

      /* check if block is only allocated block (since it is both head and tail), and NULL */
      if (head == tail){
          head = tail = NULL;
      } else {

          /* copy head into tmp block, NULL each block from tail back to head */
          tmp = head;
          while (tmp) {
              if (tmp->next == tail){
                  tmp->next = NULL;
                  tail = tmp;
              }
              tmp = tmp->next;
          }
      }

      /* move break back to memory address after deallocation */
      sbrk(0 - BLOCK_SIZE - header->size);

      /* unlock critical section*/
      pthread_mutex_unlock(&global_malloc_lock);

      /* returns nothing */
      return (void) NULL;
  }

  /* set flag to unmarked */
  header->flag = 1;

  /* unlock critical section */
  pthread_mutex_unlock(&global_malloc_lock);
}

void *
jrealloc(void *block, size_t size)
{
    janitor_t *header;
    void *ret;

    /* create a new block if parameters not set */
    if (!block)
        return jmalloc(size);

    /* set header to be block's previous bit */
    header = (janitor_t *) block - 1;

    /* check if headers size is greater than specified paramater */
    if (header->size >= size)
        return block;

    /* create a new block allocation */
    ret = jmalloc(size);

    /* add content from previous block to newly allocated block */
    if (ret){
        memcpy(ret, block, header->size);
        jfree(block);
    }

    return ret;
}
