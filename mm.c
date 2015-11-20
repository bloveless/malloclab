/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your identifying information in the following struct.
 ********************************************************/
 team_t team = {
    /* Team name */
  "349",
    /* First member's full name */
  "Brennon Loveless",
    /* First member's UID */
  "u0550904",
    /* Second member's full name (leave blank if none) */
  "Brandon Sara",
    /* Second member's UID (leave blank if none) */
  "u0516100"
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

typedef struct meta_block {
  size_t size;
  struct meta_block* next;
  int free;
  struct meta_block* prev;
} meta_block;

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

meta_block* global_head = NULL;
meta_block* global_tail = NULL;

meta_block* find_free_block(size_t size);
meta_block* get_block_ptr(void *ptr);
meta_block* request_space(size_t size);

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
  global_head = NULL;
  global_tail = NULL;
  return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void* mm_malloc(size_t size)
{
  int newsize = ALIGN(size + sizeof(meta_block));
  meta_block* block;

  if(size <= 0) {
    return NULL;
  }

  if(global_head == NULL) {
    block = request_space(newsize);
    if(!block) {
      return NULL;
    }
  }
  else {
    block = find_free_block(newsize);
    if(!block) {
      block = request_space(newsize);
      if(!block) {
        return NULL;
      }
    }
    else {
      block->free = 0;
    }
  }

  return block + 1;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
  if(!ptr) {
    return;
  }

  meta_block* block = (meta_block*)ptr - 1;
  block->free = 1;

  meta_block* prev_block = block->prev;
  meta_block* next_block = block->next;

  if(next_block && next_block->free) {
    block->size = block->size + next_block->size;
    block->next = next_block->next;
  }

  if(prev_block && prev_block->free) {
    prev_block->size = prev_block->size + block->size;
    prev_block->next = block->next;
  }

}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void* mm_realloc(void *ptr, size_t new_size)
{
  void *oldptr = ptr;
  void *newptr;
  size_t copySize;

  newptr = mm_malloc(new_size);

  // If new size is 0 then just free the space
  if(new_size <= 0) {
    free(oldptr);
    return 0;
  }

  // If the pointer is null then this is just a malloc
  if(ptr == NULL) {
    return malloc(new_size);
  }







  /*
   * THIS IS WHERE I IS, YO
   */





  if (newptr == NULL) {
    return NULL;
  }

  meta_block* old_block = (meta_block*) oldptr - 1;
  copySize = old_block->size;
  // copySize = *(size_t *)((char *)oldptr - sizeof(meta_block));

  if (new_size < copySize) {
    copySize = new_size;
  }

  memcpy(newptr, oldptr, copySize);
  mm_free(oldptr);

  return newptr;
}

meta_block* find_free_block(size_t size)
{
  meta_block *current = global_head;

  while (current && !(current->free && current->size >= size)) {
    current = current->next;
  }

  return current;
}

meta_block* request_space(size_t size)
{
  void* request = mem_sbrk(size);

  // assert((void*)block == request); // Not thread safe.
  if (request == (void*) -1) {
    return NULL; // sbrk failed.
  }

  meta_block* block = (meta_block*) request;
  block->size = size;
  block->next = NULL;
  // save the current global tail to the block as the previous block
  block->prev = global_tail;
  block->free = 0;

  if(!global_head) {
    // first time through create the head
    global_head = block;
  } else {
    // if global tail is not null link back to the current block
    // so we get a doubly linked list
    global_tail->next = block;
  }

  // reset the global tail to the current block
  global_tail = block;

  return block;
}
