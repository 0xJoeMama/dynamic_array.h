# dynamic\_array.h
An implementation of a growable heap array in C.
This library uses the header-only library scheme to make it usable even in obsurd contexts.

# Using
Copy paste the [header library file](./dynamic_array.h) in the directory your C file is in.
Then you just need to add `#define IMPL_DA` and include the header file after that macro definition.
An example is shown below but you can also see the [testing file](./dynamic_array_tests.c).

```c
#define IMPL_DA
#include "dynamic_array.h"

int main() {
    DynamicArray_t da;
    if (!da_init(&da, 2, int)) 
        return 1;

    int i = 0;
    da_push(&da, &i);
    i++;
    da_push(&da, &i);
    i++;
    // a resize will happen after the next one
    da_push(&da, &i);

    da_deinit(&da);
    return 0;
}
```

# Warning
The `da_init` and `da_push` functions can fail depending on the amount of free memory on the system. Always check their return values.

