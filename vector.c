#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static constexpr int DEFAULT_CAPACITY = 16;
static constexpr int GROWTH_FACTOR = 2;

struct vector {
    size_t len;
    size_t cap;
    size_t elem_size;
    void *data;
};

typedef struct vector vector;

vector *vector_newc(const size_t elem_size, const size_t initial_capacity) {
    vector *v = malloc(sizeof(vector));
    if (v == NULL) {
        return NULL;
    }

    v->len = 0;
    v->cap = initial_capacity;
    v->elem_size = elem_size;
    void *data = calloc(initial_capacity, sizeof(elem_size));
    if (data == NULL) {
        free(v);
        return NULL;
    }
    v->data = data;

    return v;
}

vector *vector_new(const size_t elem_size) {
    return vector_newc(elem_size, DEFAULT_CAPACITY);
}

static bool grow_if_req(vector *vec, const size_t i) {
    if (i > vec->cap) {
        size_t new_cap = vec->cap * GROWTH_FACTOR;
        while (new_cap < i) {
            new_cap *= GROWTH_FACTOR;
        }
        void *data = reallocarray(vec->data, new_cap, vec->elem_size);
        if (data == NULL) {
            return false;
        }
        vec->data = data;
        memset(vec->data + vec->len * vec->elem_size, 0, (new_cap - vec->len) * vec->elem_size);
        vec->cap = new_cap;
    }
    return true;
}

bool vector_push(vector *vec, const void *e) {
    if (!grow_if_req(vec, vec->len + 1)) {
        return false;
    }
    memcpy(vec->data + (vec->len * vec->elem_size), e, vec->elem_size);
    vec->len++;
    return true;
}

bool vector_push_i(vector *vec, const int e) {
    return vector_push(vec, &e);
}

void *vector_get(const vector *vec, const size_t index) {
    assert(index < vec->len);
    return vec->data + index * vec->elem_size;
}

int vector_get_i(const vector *vec, const size_t index) {
    void *r = vector_get(vec, index);
    assert(r != NULL);
    return *(int *) r;
}

size_t vector_capacity(const vector *v) {
    return v->cap;
}

size_t vector_len(const vector *v) {
    return v->len;
}

void vector_free(vector *vec) {
    free(vec->data);
    free(vec);
}

void vector_set(const vector *vec, const size_t i, const void *e) {
    assert(i < vec->len);
    memcpy(vec->data + (i * vec->elem_size), e, vec->elem_size);
}

void vector_set_i(const vector *vec, const size_t i, const int e) {
    vector_set(vec, i, &e);
}
