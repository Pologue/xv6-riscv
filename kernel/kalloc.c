// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

#define RESERVED 16 * 1024 * 1024 // 16MB

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run
{
  struct run *next;
};

struct
{
  struct spinlock lock;
  struct run *freelist;
} kmem;

struct header
{
  struct header *next;
  uint64 size;
  int is_free;
};

typedef struct header Header;

void *base = (void *)PHYSTOP - RESERVED + 1;
static Header *free_list;

void kinit()
{
  initlock(&kmem.lock, "kmem");
  freerange(end, (void *)PHYSTOP - RESERVED);
}

void freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char *)PGROUNDUP((uint64)pa_start);
  for (; p + PGSIZE <= (char *)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by pa,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void kfree(void *pa)
{
  struct run *r;

  if (((uint64)pa % PGSIZE) != 0 || (char *)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run *)pa;

  acquire(&kmem.lock);
  r->next = kmem.freelist;
  kmem.freelist = r;
  release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  if (r)
    kmem.freelist = r->next;
  release(&kmem.lock);

  if (r)
    memset((char *)r, 5, PGSIZE); // fill with junk
  return (void *)r;
}

// allocator in kernel/kalloc.c
void *
malloc(uint64 nbytes)
{
  printf("malloc: try to allocate %d bytes\n", nbytes);
  // TODO
  Header *curr, *prev;
  uint64 nunits;

  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
  if ((prev = free_list) == 0) // free_list not initialized
  {
    // initialize free_list
    free_list = prev = (Header *)base;
    free_list->size = (RESERVED - sizeof(Header)) / sizeof(Header);
    free_list->next = free_list;
  }

  // find a free block
  for(curr = prev->next; ; prev = curr, curr = curr->next){
    if(curr->size >= nunits){
      if(curr->size == nunits)
        prev->next = curr->next;
      else {
        curr->size -= nunits;
        curr += curr->size;
        curr->size = nunits;
      }
      free_list = prev;
      return (void*)(curr + 1);
    }
    if(curr == free_list)
      return 0;
  }
}

// free memory in kernel/kalloc.c
void free(void *addr)
{
  printf("free: address %d\n", addr);
  // TODO
  Header *bp, *p;

  bp = (Header*)addr - 1;
  for(p = free_list; !(bp > p && bp < p->next); p = p->next)
    if(p >= p->next && (bp > p || bp < p->next))
      break;
  if(bp + bp->size == p->next){
    bp->size += p->next->size;
    bp->next = p->next->next;
  } else
    bp->next = p->next;
  if(p + p->size == bp){
    p->size += bp->size;
    p->next = bp->next;
  } else
    p->next = bp;
  free_list = p;
}