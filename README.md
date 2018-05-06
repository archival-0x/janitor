# janitor

Minimal first-fit memory allocator implementation

## Introduction

__janitor__ is a minimal malloc (and variants) implementation that relies on a singly linked-list in order to store blocks of allocated memory. This works similarly in a way a mark-sweek garbage collector would work, except for the fact that C does not utilize automatic memory deallocation.

## Functions

`jmalloc` - using a first-fit approach, check if our list already contains free blocks that haven't been marked. if not, set the program break of the heap and set the head to the new block.

`jcalloc` - works like malloc, but instead implements `memset` to zero out the newly allocated block

`jrealloc` - allocates new block with `jmalloc` if block does not exist. Otherwise, allocate an empty block of specified size, and copy specified block into newly created block with new size.

`jfree` - checks if specified block and size of all blocks meet at program break. NULL out the block until that point. Set new program break and mark block as deallocated.

## LICENSE

[MIT License](http://codemuch.tech/license.txt)
