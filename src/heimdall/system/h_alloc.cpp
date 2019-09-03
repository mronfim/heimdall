#include <stdlib.h>
#include "h_alloc.h"
#include "log.h"

void *h_alloc(size_t num, size_t size)
{
    void *mem = calloc(num, size);

    if (mem == NULL) {
        log_fail("h_alloc(%lu, %lu) failed", num, size);
    }

    return mem;
}

void *h_realloc(void *ptr, size_t new_size)
{
    void *mem = realloc(ptr, new_size);

    if (mem == NULL) {
        log_fail("h_realloc(0x%x, %lu) failed", ptr, new_size);
    }

    return mem;
}
