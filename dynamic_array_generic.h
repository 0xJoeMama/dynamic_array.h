#ifndef DA_NEW_IMPL
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define COMMAND(name) {#name, name##_cmd}
#define FUDGE 2

#define DynamicArray_t(type) DynamicArray_##type##_t
#define DynamicArray(type) DynamicArray_##type

// TODO: ifdef for implementation code
#define DA_DECLARE(type)                                                       \
  struct DynamicArray(type) {                                                  \
    type *buf;                                                                 \
    size_t cap;                                                                \
    size_t len;                                                                \
  };                                                                           \
                                                                               \
  int da_init_##type(struct DynamicArray(type) * da, size_t initial_cap) {     \
    da->buf = malloc(initial_cap * sizeof(type));                              \
    if (!da->buf)                                                              \
      return 0;                                                                \
                                                                               \
    da->cap = initial_cap;                                                     \
    da->len = 0;                                                               \
                                                                               \
    return 1;                                                                  \
  }                                                                            \
                                                                               \
  type *da_get_raw_##type(struct DynamicArray(type) * da, size_t idx) {        \
    return da->buf + idx;                                                      \
  }                                                                            \
                                                                               \
  int da_grow_##type(struct DynamicArray(type) * da) {                         \
    void *new_buf = realloc(da->buf, da->cap * FUDGE * sizeof(type));          \
    if (!new_buf)                                                              \
      return 0;                                                                \
                                                                               \
    da->buf = (type *)new_buf;                                                 \
    da->cap *= FUDGE;                                                          \
    return 1;                                                                  \
  }                                                                            \
                                                                               \
  int da_push_##type(struct DynamicArray(type) * da, type el) {                \
    if (da->len == da->cap) {                                                  \
      /* attempt to grow if we don't have room */                              \
      if (!da_grow_##type(da))                                                 \
        return 0;                                                              \
    }                                                                          \
    /* da_get_raw is blind so it might return an invalid address               \
      ignore that case since we know we are in the dynamic array(we resized    \
      *before) */                                                              \
    type *target = da_get_raw_##type(da, da->len);                             \
    memcpy(target, &el, sizeof(type));                                         \
    da->len++;                                                                 \
                                                                               \
    return 1;                                                                  \
  }                                                                            \
                                                                               \
  void da_deinit_##type(struct DynamicArray(type) * da) {                      \
    free(da->buf);                                                             \
    memset(da, 0, sizeof(struct DynamicArray(type)));                          \
  }                                                                            \
                                                                               \
  int da_shrink_##type(struct DynamicArray(type) * da) {                       \
    /* we can't shrink if we have more than half our capacity                  \
    we also can't shrink if we have capacity one because of portability        \
    issues(man realloc) */                                                     \
    if ((da->len >= da->cap / FUDGE) || da->cap == 1)                          \
      return 1;                                                                \
                                                                               \
    void *new_buf = realloc(da->buf, da->cap / FUDGE * sizeof(type));          \
    if (!new_buf)                                                              \
      return 0;                                                                \
                                                                               \
    da->buf = (type *)new_buf;                                                 \
    da->cap /= FUDGE;                                                          \
                                                                               \
    return 1;                                                                  \
  }                                                                            \
                                                                               \
  int da_pop_##type(struct DynamicArray(type) * da, type * dst) {              \
    /* make sure we have at least 1 element */                                 \
    if (da->len == 0)                                                          \
      return 0;                                                                \
                                                                               \
    da->len = da->len - 1;                          /* reduce our length */    \
    type *element = da_get_raw_##type(da, da->len); /* get the last element */ \
    memcpy(dst, element, sizeof(type)); /* copy it into the dst pointer */     \
                                                                               \
    /* if shrinking fails it means nothing to us since our job is to just      \
    remove an element */                                                       \
    da_shrink_##type(da);                                                      \
                                                                               \
    return 1;                                                                  \
  }                                                                            \
                                                                               \
  typedef struct DynamicArray(type) DynamicArray_t(type)

#define da_init(da, initial_cap, type) (da_init_##type(da, initial_cap))
#define da_get(da, idx, type)                                                  \
  (idx < (da)->len ? da_get_raw_##type(da, idx) : NULL)
#define da_push(da, el, type) (da_push_##type(da, el))
#define da_deinit(da, type) (da_deinit_##type(da))
#define da_pop(da, dst, type) (da_pop_##type(da, dst))

#define DA_NEW_IMPL
#endif
