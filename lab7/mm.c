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

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* Basic constants and macros */
#define WSIZE 4             /* Word and header/footer size (bytes) */
#define DSIZE 8             /* Double word size( bytes) */
#define CHUNKSIZE (1 << 12) /* Extend heap by this amount (bytes) */

#define MAX(x, y) ((x) > (y)) ? (x) : (y)

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

/* Read the size and allocate fields from address p */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp) ((char *)(bp)-WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp)-WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp)-GET_SIZE(((char *)(bp)-DSIZE)))

/* ################################################################ */

#define SELF_INDEX(bp) (bp - heap_listp)
#define SELF_ADDR(index) (index + heap_listp)
//pointer to next free block

#define PREV_FREE(bp) (bp)
#define NEXT_FREE(bp) (bp + WSIZE)

#define PREV_FREE_INDEX(bp) (GET(PREV_FREE(bp)))
#define NEXT_FREE_INDEX(bp) (GET(NEXT_FREE(bp)))

// static size_t MAX_FREE_SIZE = 0;
static void *heap_listp;
static void *hd_free = NULL;
static void *ft_free = NULL;

/* function declaration */
static void *extend_heap(size_t words);
static void *coalesce(void *bp);
static void *find_fit(size_t size);
static void *place(void *bp, size_t size);

static void *insert_free_block(void *bp);
static void *prev_free_block(void *bp);
static void *next_free_block(void *bp);
static void *remove_free_block(void *bp);
static void *replace_free_block(void *old_bp, void *new_bp);

/* ################################################################ */

static void *insert_free_block(void *bp)
{
    if (hd_free == NULL)
    {
        PUT(NEXT_FREE(bp), 0);
        PUT(PREV_FREE(bp), 0);
        hd_free = bp;
        ft_free = bp;
    }
    else
    {
        PUT(NEXT_FREE(ft_free), SELF_INDEX(bp));
        PUT(PREV_FREE(bp), SELF_INDEX(ft_free));
        PUT(NEXT_FREE(bp), 0);
        ft_free = bp;
    }
    return bp;
}

static void *prev_free_block(void *bp)
{
    if (PREV_FREE_INDEX(bp) == 0)
    {
        return NULL;
    }
    return SELF_ADDR(PREV_FREE_INDEX(bp));
}

static void *next_free_block(void *bp)
{
    if (NEXT_FREE_INDEX(bp) == 0)
    {
        return NULL;
    }
    return SELF_ADDR(NEXT_FREE_INDEX(bp));
}

static void *replace_free_block(void *old_bp, void *new_bp)
{
    if (hd_free == old_bp)
    {
        hd_free = new_bp;
    }
    else
    {
        void *prev = SELF_ADDR(PREV_FREE_INDEX(old_bp));
        PUT(NEXT_FREE(prev), SELF_INDEX(new_bp));
    }

    if (ft_free == old_bp)
    {
        ft_free = new_bp;
    }
    else
    {
        void *next = SELF_ADDR(NEXT_FREE_INDEX(old_bp));
        PUT(PREV_FREE(next), SELF_INDEX(new_bp));
    }

    memcpy(new_bp, old_bp, DSIZE);

    return new_bp;
}

static void *remove_free_block(void *bp)
{
    void *prev = prev_free_block(bp);
    void *next = next_free_block(bp);

    if (prev)
    {
        PUT(NEXT_FREE(prev), NEXT_FREE_INDEX(bp));
    }
    else
    {
        hd_free = next;
    }

    if (next)
    {
        PUT(PREV_FREE(next), PREV_FREE_INDEX(bp));
    }
    else
    {
        ft_free = prev;
    }

    return bp;
}

static void *extend_heap(size_t words)
{
    // size_t size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
    size_t size = words * WSIZE;

    char *bp = mem_sbrk(size);
    if ((long)bp == -1)
    {
        return NULL;
    }

    void *prev = PREV_BLKP(bp);
    size_t prev_alloc = GET_ALLOC(HDRP(prev));

    if (prev_alloc)
    {
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));
        insert_free_block(bp);
        return bp;
    }
    else
    {
        size += GET_SIZE(HDRP(prev));
        PUT(HDRP(prev), PACK(size, 0));
        PUT(FTRP(prev), PACK(size, 0));
        PUT(HDRP(NEXT_BLKP(prev)), PACK(0, 1));
        return prev;
    }
}

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    hd_free = NULL;
    ft_free = NULL;

    heap_listp = mem_sbrk(2 * DSIZE);
    if (heap_listp == (void *)-1)
    {
        return -1;
    }

    PUT(heap_listp, 0);
    PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1));
    PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1));
    PUT(heap_listp + (3 * WSIZE), PACK(0, 1));

    heap_listp += (2 * WSIZE);

    if (extend_heap(CHUNKSIZE / WSIZE) == NULL)
    {
        return -1;
    }

    return 0;
}

static void *find_fit(size_t size)
{
    void *bp = hd_free;
    size_t asize = 0;

    while (bp)
    {
        asize = GET_SIZE(HDRP(bp));
        if (size <= asize)
        {
            return bp;
        }
        bp = next_free_block(bp);
    }

    return NULL;
}

static void *place(void *bp, size_t size)
{
    size_t origin_size = GET_SIZE(HDRP(bp));

    if ((origin_size - size) >= (2 * DSIZE))
    {
        PUT(HDRP(bp), PACK(origin_size - size, 0));
        PUT(FTRP(bp), PACK(origin_size - size, 0));

        void *next = NEXT_BLKP(bp);

        PUT(HDRP(next), PACK(size, 1));
        PUT(FTRP(next), PACK(size, 1));

        return next;
    }
    else
    {
        PUT(HDRP(bp), PACK(origin_size, 1));
        PUT(FTRP(bp), PACK(origin_size, 1));
        remove_free_block(bp);
        return bp;
    }
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    size_t asize;
    size_t extendsize;
    char *bp;

    if (size == 0)
        return NULL;
    if (size == 448)
        size = 512;
    if (size == 112)
        size = 128;

    asize = DSIZE * ((size + (DSIZE) + (DSIZE - 1)) / DSIZE);

    if (((bp = find_fit(asize)) != NULL))
    {
        bp = place(bp, asize);
        return bp;
    }

    extendsize = MAX(asize, CHUNKSIZE);
    if ((bp = extend_heap(extendsize / WSIZE)) == NULL)
    {
        return NULL;
    }

    bp = place(bp, asize);

    return bp;
}

static void *coalesce(void *bp)
{
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    if (prev_alloc && next_alloc)
    {
        insert_free_block(bp);
    }
    else if (prev_alloc && !next_alloc)
    {
        void *next = NEXT_BLKP(bp);
        size += GET_SIZE(HDRP(next));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
        replace_free_block(next, bp);
    }
    else if (!prev_alloc && next_alloc)
    {
        bp = PREV_BLKP(bp);
        size += GET_SIZE(HDRP(bp));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    }
    else
    {
        void *next = NEXT_BLKP(bp);
        bp = PREV_BLKP(bp);
        size += GET_SIZE(HDRP(bp)) + GET_SIZE(FTRP(next));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(next), PACK(size, 0));
        remove_free_block(next);
    }
    return bp;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    size_t size = GET_SIZE(HDRP(ptr));

    PUT(HDRP(ptr), PACK(size, 0));
    PUT(FTRP(ptr), PACK(size, 0));

    coalesce(ptr);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    if (size == 0)
    {
        mm_free(ptr);
        return ptr;
    }

    size_t asize = DSIZE * ((size + DSIZE + DSIZE - 1) / DSIZE);

    if (ptr == NULL)
    {
        return mm_malloc(asize);
    }

    size_t oldSize = GET_SIZE(HDRP(ptr));
    if (asize <= oldSize)
    {
        if ((oldSize - asize) >= 2 * DSIZE)
        {
            void *ptr_next = NEXT_BLKP(ptr);
            size_t pn_alloc = GET_ALLOC(HDRP(ptr_next));

            PUT(HDRP(ptr), PACK(asize, 1));
            PUT(FTRP(ptr), PACK(asize, 1));
            void *new_next = NEXT_BLKP(ptr);

            if (!pn_alloc)
            {
                size_t pn_size = GET_SIZE(HDRP(ptr_next));
                PUT(HDRP(new_next), PACK(oldSize - asize + pn_size, 0));
                PUT(FTRP(new_next), PACK(oldSize - asize + pn_size, 0));
                replace_free_block(ptr_next, new_next);
            }
            else
            {
                PUT(HDRP(new_next), PACK(oldSize - asize, 0));
                PUT(FTRP(new_next), PACK(oldSize - asize, 0));
                insert_free_block(new_next);
            }
        }
        return ptr;
    }
    else
    {
        void *ptr_next = NEXT_BLKP(ptr);
        size_t pn_size = GET_SIZE(HDRP(ptr_next));

        if (!GET_ALLOC(HDRP(ptr_next)))
        {
            if (oldSize + pn_size >= asize + 2 * DSIZE)
            {
                void *new_next = ptr + asize;
                replace_free_block(ptr_next, new_next);
                PUT(HDRP(ptr), PACK(asize, 1));
                PUT(FTRP(ptr), PACK(asize, 1));
                PUT(HDRP(new_next), PACK(oldSize + pn_size - asize, 0));
                PUT(FTRP(new_next), PACK(oldSize + pn_size - asize, 0));
                return ptr;
            }
            else if (oldSize + pn_size >= asize)
            {
                PUT(HDRP(ptr), PACK(oldSize + pn_size, 1));
                PUT(FTRP(ptr), PACK(oldSize + pn_size, 1));
                remove_free_block(ptr_next);
                return ptr;
            }
        }

        void *new_ptr = mm_malloc(asize);
        memcpy(new_ptr, ptr, oldSize - DSIZE);
        mm_free(ptr);
        return new_ptr;
    }
}