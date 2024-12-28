#include "dynamic_array.h"
#include <stdio.h>
#include <string.h>

DA_IMPL(int);
DA_IMPL(char);

typedef struct as {
  int x;
} as_t;

typedef struct {
  int x;
  char **y;
  char name[200];
} Asd_t;

// array
DA_IMPL(Asd_t);
// array of arrays
DA_IMPL(DynamicArray_t(Asd_t));
// array of arrays of arrays
DA_IMPL(DynamicArray_t(DynamicArray_t(Asd_t)));

void asd_t_deinit(Asd_t asd) {
  free(asd.y);
}

int main(void) {
  DynamicArray_t(int) da;
  if (!da_init(int)(&da, 5))
    return 1;

  for (int i = 0; i < 100; i++)
    da_push(int)(&da, i);

  for (size_t i = 0; i < da.len; i++)
    printf("%d\n", *da_get(int)(&da, i));

  printf("\nLength is : %zu, cap is : %zu\n", da.len, da.cap);
  int curr;
  while (da_pop(int)(&da, &curr))
    printf("%d ", curr);

  printf("\nLength is : %zu, cap is : %zu\n", da.len, da.cap);

  da_deinit(int)(&da, NULL);

  DynamicArray_t(Asd_t) new_arr;

  if (!da_init(Asd_t)(&new_arr, 10))
    return 1;

  Asd_t v = {.x = 12,
             .y = malloc(sizeof(char *)),
             .name = "Asdasdasdasdasdasdasdasdassssssssssssssssss"};
  da_push(Asd_t)(&new_arr, v);

  for (size_t i = 0; i < new_arr.len; i++)
    printf("%s\n", da_get(Asd_t)(&new_arr, i)->name);

  da_deinit(Asd_t)(&new_arr, asd_t_deinit);

  // Hey! Vsauce. Michael here.
  // *This* is a string. Or is it?
  DynamicArray_t(char) is_this_a_string;
  if (!da_init(char)(&is_this_a_string, 10)) return 1;

  char asd[] = "asdasdasdasdasd";
  for (size_t i = 0; i < sizeof(asd) / sizeof(char); i++)
    da_push(char)(&is_this_a_string, asd[i]);

  da_deinit(char)(&is_this_a_string, NULL);

  return 0;
}
