#include "dynamic_array_generic.h"
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

DA_IMPL(Asd_t);

int main(void) {
  DynamicArray_t(int) da;
  if (!da_init(&da, 5, int))
    return 1;

  for (int i = 0; i < 100; i++)
    da_push(&da, i, int);

  for (size_t i = 0; i < da.len; i++)
    printf("%d\n", *da_get(&da, i, int));

  printf("\nLength is : %zu, cap is : %zu\n", da.len, da.cap);
  int curr;
  while (da_pop(&da, &curr, int))
    printf("%d ", curr);

  printf("\nLength is : %zu, cap is : %zu\n", da.len, da.cap);

  da_deinit(&da, int);

  DynamicArray_t(Asd_t) new_arr;

  if (!da_init(&new_arr, 10, Asd_t))
    return 1;

  Asd_t v = {.x = 12,
             .y = malloc(sizeof(char *)),
             .name = "Asdasdasdasdasdasdasdasdassssssssssssssssss"};
  da_push(&new_arr, v, Asd_t);

  free(v.y);
  for (size_t i = 0; i < new_arr.len; i++)
    printf("%s\n", da_get(&new_arr, i, Asd_t)->name);

  da_deinit(&new_arr, Asd_t);

  // Hey! Vsauce. Michael here.
  // *This* is a string. Or is it?
  DynamicArray_t(char) is_this_a_string;
  da_init(&is_this_a_string, 10, char);

  char asd[] = "asdasdasdasdasd";
  for (size_t i = 0; i < sizeof(asd) / sizeof(char); i++)
    da_push(&is_this_a_string, asd[i], char);

  da_deinit(&is_this_a_string, char);

  return 0;
}
