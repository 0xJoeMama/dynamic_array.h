#include <stdio.h>
#include <stdlib.h>

#define IMPL_DA
#include "dynamic_array.h"

typedef struct {
  int x;
  char **y;
  char name[200];
} Asd_t;

int main(void) {
  DynamicArray_t da;
  if (!da_init(&da, 5, int))
    return 1;

  for (int i = 0; i < 100; i++)
    da_push(&da, &i);

  for (size_t i = 0; i < da.len; i++)
    printf("%d\n", *da_get(&da, i, int));

  printf("\nLength is : %zu, cap is : %zu\n", da.len, da.cap);
  int curr;
  while (da_pop(&da, &curr))
    printf("%d ", curr);

  printf("\nLength is : %zu, cap is : %zu\n", da.len, da.cap);

  da_deinit(&da);

  DynamicArray_t new_arr;

  if (!da_init(&new_arr, 10, Asd_t))
    return 1;

  Asd_t v = {.x = 12,
             .y = malloc(sizeof(char *)),
             .name = "Asdasdasdasdasdasdasdasdassssssssssssssssss"};
  da_push(&new_arr, &v);

  free(v.y);
  for (size_t i = 0; i < new_arr.len; i++)
    printf("%s\n", da_get(&new_arr, i, Asd_t)->name);

  da_deinit(&new_arr);

  return 0;
}
