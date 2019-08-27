#ifndef H_ALLOC_H_
#define H_ALLOC_H_

void *h_alloc(size_t num, size_t size);
void *h_realloc(void *ptr, size_t new_size);

#endif  // H_ALLOC_H_
