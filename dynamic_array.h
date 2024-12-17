#ifndef DA_H
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  void *buf;
  size_t cap;
  size_t len;
  size_t elem_sz;
} DynamicArray_t;

int da_init_raw(DynamicArray_t *da, size_t initial_cap, size_t elem_sz);
void *da_get_raw(DynamicArray_t *da, size_t idx);
int da_resize(DynamicArray_t *da);
int da_push_raw(DynamicArray_t *da, void *el);

#define da_init(da, cap, type) (da_init_raw(da, cap, sizeof(type)))
#define da_get(da, idx, type)                                                  \
  ((type *)(idx <= (da)->len ? da_get_raw(da, idx) : NULL))
#define da_push(da, el) (da_push_raw(da, &el))
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

int da_resize(DynamicArray_t *da) {
  void *old = da->buf;
  da->buf = realloc(old, da->cap * 2 * da->elem_sz);
  if (!da->buf) {
    free(old);
    return 0;
  }

  da->cap *= 2;
  return 1;
}

int da_push_raw(DynamicArray_t *da, void *el) {
  if (da->len == da->cap) {
    // attempt to double it's size
    if (!da_resize(da))
      return 0;
  }

  // da_get is blind so it just returns an invalid address
  void *target = da_get_raw(da, da->len);
  memcpy(target, el, da->elem_sz);

  da->len++;

  return 1;
}

void da_deinit(DynamicArray_t *da) {
  free(da->buf);
  memset(da, 0, sizeof(DynamicArray_t));
}

#endif

#define DA_H
#endif
