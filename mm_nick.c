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
    "Forever Alone",
    /* First member's full name */
    "Val Nicholas Hallstrom",
    /* First member's UID */
    "u0812277",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's UID (leave blank if none) */
    ""
};

struct block
{
    size_t size;
    struct block *next_block;
    int free;
    int fill;
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(struct block)))

void *head = NULL;
void *tail = NULL;

/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    //head = NULL;
    //tail = NULL;
    return 0;
}

struct block *get_free_block(size_t size);

struct block *get_space(struct block *last_block, size_t size);

void print_heap();

/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    //printf("Allocating size: %d\n", size);
    struct block *block;
    if(!head) //First time
    {
        printf("First time in trace file\n");
        block = get_space(NULL, size);
        if(!block)
        {
            return NULL;
        }
        head = block;
    }
    else
    {
        block = get_free_block(size);
        if (!block) //Couldn't fine free space, make new space
        {
            block = get_space(tail, size);
            if (!block)
            {
                return NULL;
            }
        }
        else
        {
            //printf("Found free space\n");
            block->free = 0;
            //Try to split free block up.
        }
    }
    //print_heap();
    return block + 1;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    if (!ptr)
    {
        return;
    }
    //Merge blocks
    struct block *block_ptr = (struct block*)ptr - 1;
    block_ptr->free = 1;
    //printf("Freeing size: %d\n", block_ptr->size);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    if (!ptr)
    {
        return malloc(size);
    }

    struct block *block_ptr = (struct block*)ptr - 1;
    if (block_ptr->size >= size)
    {
        return ptr;
    }

    void *new_ptr;
    new_ptr = malloc(size);
    if (!new_ptr) {
        return NULL; // TODO: set errno on failure.
    }
    memcpy(new_ptr, ptr, block_ptr->size);
    free(ptr);
    return new_ptr;
}

struct block *get_free_block(size_t size)
{
    struct block *current_block = head;
    while(current_block && !(current_block->free && current_block->size >= size))
    {
        current_block = current_block->next_block;
    }
    return current_block;
}

struct block *get_space(struct block *last_block, size_t size)
{
    struct block *block;
    block = mem_sbrk(ALIGN(size) + 16);
    //printf("Block made at: %d\n", block);
    if(block == (void*)-1)
    {
        return NULL;
    }
    if(last_block)
    {
        last_block->next_block = block;
    }
    tail = block;
    block->size = ALIGN(size);
    block->free = 0;
    block->next_block = NULL;
    return block;
}

void print_heap()
{
    int count = 0;
    struct block *current_block = head;
    while(current_block)
    {
        printf("Block #%d\n", count);
        printf("size: %d\n", current_block->size);
        printf("free: %d\n", current_block->free);
        current_block = current_block->next_block;
        count++;
    }
}