#ifndef DA_H
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define FUDGE 2

typedef struct {
  void *buf;
  size_t cap;
  size_t len;
  size_t elem_sz;
} DynamicArray_t;

// dont use unless you really know what you are doing
int da_init_raw(DynamicArray_t *da, size_t initial_cap, size_t elem_sz);
void *da_get_raw(DynamicArray_t *da, size_t idx);
int da_grow(DynamicArray_t *da);
int da_shrink(DynamicArray_t *da);

// use these, as a normal person ought to
//
// initialize a dynamic array with an initial capacity of 'cap' elements with
// type 'type'
#define da_init(da, cap, type) (da_init_raw(da, cap, sizeof(type)))
// append an element at the end of the dynamic array
int da_push(DynamicArray_t *da, void *el);
// get a reference to the element of the array at index 'idx'
// unlike da_get_raw this macro is safe to call with any index and will just
// return NULL if that index is out of range(aka will lead to segfaults on oob
// access)
#define da_get(da, idx, type)                                                  \
  ((type *)(idx <= (da)->len ? da_get_raw(da, idx) : NULL))
// remove an element from the end of the dynamic array and store it at the
// location 'dst' points to
int da_pop(DynamicArray_t *da, void *dst);
// clean a dynamic array up
void da_deinit(DynamicArray_t *da);

#ifdef IMPL_DA
int da_init_raw(DynamicArray_t *da, size_t initial_cap, size_t elem_sz) {
  da->buf = malloc(initial_cap * elem_sz);
  if (!da->buf)
    return 0;

  da->cap = initial_cap;
  da->len = 0;
  da->elem_sz = elem_sz;

  return 1;
}

void *da_get_raw(DynamicArray_t *da, size_t idx) {
  return (void *)((char *)da->buf + idx * da->elem_sz);
}

int da_grow(DynamicArray_t *da) {
  void *new_buf = realloc(da->buf, da->cap * FUDGE * da->elem_sz);
  if (!new_buf)
    return 0;

  da->buf = new_buf;
  da->cap *= FUDGE;
  return 1;
}

int da_push(DynamicArray_t *da, void *el) {
  if (da->len == da->cap) {
    // attempt to grow if we don't have room
    if (!da_grow(da))
      return 0;
  }

  // da_get_raw is blind so it might return an invalid address
  // ignore that case since we know we are in the dynamic array(we resized
  // before)
  void *target = da_get_raw(da, da->len);
  memcpy(target, el, da->elem_sz);

  da->len++;

  return 1;
}

void da_deinit(DynamicArray_t *da) {
  free(da->buf);
  memset(da, 0, sizeof(DynamicArray_t));
}

int da_shrink(DynamicArray_t *da) {
  // we can't shrink if we have more than half our capacity
  // we also can't shrink if we have capacity one because of portability
  // issues(man realloc)
  if ((da->len >= da->cap / FUDGE) || da->cap == 1)
    return 1;

  void *new_buf = realloc(da->buf, da->cap / FUDGE * da->elem_sz);
  if (!new_buf)
    return 0;

  da->buf = new_buf;
  da->cap /= FUDGE;

  return 1;
}

int da_pop(DynamicArray_t *da, void *dst) {
  // make sure we have at least 1 element
  if (da->len == 0)
    return 0;

  da->len = da->len - 1;                   // reduce our length
  void *element = da_get_raw(da, da->len); // get the last element
  memcpy(dst, element, da->elem_sz);       // copy it into the dst pointer

  // if shrinking fails it means nothing to us since our job is to just remove
  // an element
  da_shrink(da);

  return 1;
}

#endif

#define DA_H
#endif
