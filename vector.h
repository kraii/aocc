#ifndef AOCC_VECTOR
#define AOCC_VECTOR
#include "stddef.h"

typedef struct vector vector;
typedef int (*vec_cmp_function_type) (const void *, const void *);

vector *vector_new(size_t elem_size);
vector *vector_newc(size_t elem_size, size_t initial_capacity);

bool vector_push(vector *vec, const void *e);
bool vector_push_i(vector *vec, int e);

void *vector_get(const vector *vec, size_t index);
int vector_get_i(const vector *vec, size_t index);
void *vector_get_p(const vector *vec, size_t index);

size_t vector_capacity(const vector *v);
size_t vector_len(const vector *v);

void vector_free(vector *vec);
void vector_free_all(vector *vec);
void vector_set(const vector *vec, size_t i, const void *e);
void vector_set_i(const vector *vec, size_t i, int e);
void *vector_pop(vector *vec);
int vector_pop_i(vector *vec);
void vector_qsort(vector *vec, vec_cmp_function_type cmp);
void vector_cpy(vector *dest, const vector *src);
void vector_clear(vector *vec);
#endif
