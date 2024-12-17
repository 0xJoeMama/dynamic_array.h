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

int da_init_raw(DynamicArray_t *da, size_t initial_cap, size_t elem_sz);
void *da_get_raw(DynamicArray_t *da, size_t idx);
int da_grow(DynamicArray_t *da);
int da_push(DynamicArray_t *da, void *el);
int da_pop(DynamicArray_t *da, void *dst);
int da_shrink(DynamicArray_t *da);

#ifdef IMPL_DA
#define da_init(da, cap, type) (da_init_raw(da, cap, sizeof(type)))
#define da_get(da, idx, type)                                                  \
  ((type *)(idx <= (da)->len ? da_get_raw(da, idx) : NULL))
void da_deinit(DynamicArray_t *da);

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

  // da_get is blind so it might return an invalid address
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
