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

typedef struct block_meta {
  size_t size;
  struct block_meta *next;
  int free;
  // int magic; // For debugging only. TODO: remove this in non-debug mode.
} block_meta;

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

block_meta* global_base = NULL;

block_meta* find_free_block(block_meta **last, size_t size);
block_meta* get_block_ptr(void *ptr);
block_meta* request_space(block_meta* last, size_t size);

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
  return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void* mm_malloc(size_t size)
{
  block_meta* block;
  int newsize = ALIGN(size + SIZE_T_SIZE);
  void *p = mem_sbrk(newsize);

  if(size <= 0) {
    return NULL;
  }

  if(global_base == NULL) {
    block = request_space(NULL, newsize);
    printf("\n\nNEW BASE BLOCK\n\n`");
    if(!block) {
      return NULL;
    }

    global_base = block;
  }
  else {
    block_meta* last = global_base;
    block = find_free_block(&last, newsize);
    if(!block) {
      block = request_space(last, newsize);
      if(!block) {
        return NULL;
      }
    }
    else {
      block->free = 0;
    }
  }

  return p;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
  // printf("FREE: %d\n", ptr);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void* mm_realloc(void *ptr, size_t size)
{
  void *oldptr = ptr;
  void *newptr;
  size_t copySize;

  newptr = mm_malloc(size);

  if (newptr == NULL) {
    return NULL;
  }

  copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);

  if (size < copySize) {
    copySize = size;
  }

  memcpy(newptr, oldptr, copySize);
  mm_free(oldptr);

  return newptr;
}

block_meta* find_free_block(block_meta **last, size_t size)
{
  block_meta *current = global_base;

  while (current && !(current->free && current->size >= size)) {
    *last = current;
    current = current->next;
  }

  return current;
}

block_meta* get_block_ptr(void *ptr)
{
  return (block_meta*)ptr - 1;
}

block_meta* request_space(block_meta* last, size_t size)
{
  block_meta* block;
  block = mem_sbrk(0);
  void* request = mem_sbrk(size);
  // assert((void*)block == request); // Not thread safe.
  if (request == (void*) -1) {
    return NULL; // sbrk failed.
  }

  if (last) { // NULL on first request.
    last->next = block;
  }

  block->size = size;
  block->next = NULL;
  block->free = 0;
  // block->magic = 0x12345678;
  return block;
}
