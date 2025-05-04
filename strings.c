#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <bits/signum-generic.h>

struct string {
    size_t cap;
    size_t len;
    char buffer[/*cap*/];
};

typedef struct string string;

static void terminate(string* s) {
    s->buffer[s->len] = '\0';
}


/**
 * Allocates a new string with the given capacity (>0) and length 0
 */
string *string_new_empty(const size_t capacity) {
    if (capacity == 0) {
        return NULL;
    }

    // Allocate 1 extra char for \0
    string *result = malloc(offsetof(string, buffer) + (sizeof(char) * (capacity + 1)));
    if (result == NULL) {
        raise(SIGTRAP);
        return NULL;
    }

    result->cap = capacity;
    result->len = 0;
    terminate(result);
    return result;
}


static size_t min(const size_t a, const size_t b) {
    return b < a ? b : a;
}

/**
 * @return the length of the given string
 */
size_t string_len(const string *s) {
    return s->len;
}

/**
 * Sets the contents of the given string to the value of buffer
 * If len > capacity will only copy up to capacity
 */
void string_set(string *s, const char *value, const size_t len) {
    memcpy(s->buffer, value, min(len, s->cap) * sizeof(char));
    s->len = min(len, s->cap);
    terminate(s);
}

/**
 * Allocates a new string with the value of the given buffer
 * If len > capacity will only copy up to capacity
 */
string *string_new(const size_t capacity, const char*value, const size_t len) {
    string *s = string_new_empty(capacity);
    string_set(s, value, len);
    return s;
}

/**
 * Allocates a new string that is a copy of the given string
 * If len of the original > capacity will only copy up to capacity
 */
string *string_copy(const size_t capacity, const string *original) {
    return string_new(capacity, original->buffer, min(capacity, original->len));
}

#define string_set_l(s, value) string_set(s, value, sizeof(value)-1)
#define string_new_l(value) string_new(sizeof(value) - 1, value, sizeof(value) -1)

/**
 * @return A newly allocated string with the result of concatenating l and r
 */
string *string_cat_new(const string *l, const string *r) {
    string *result = string_copy(l->len + r->len, l);
    memcpy(result->buffer + l->len, r->buffer, r->len * sizeof(char));
    result->len = l->len + r->len;
    terminate(result);
    return result;
}

/**
 * @return The remaining capacity of the given string
 */
size_t string_rem_cap(const string *s) {
    return s->cap - s->len;
}

/**
 * Write the result of concatenating l and r into l
 * will truncate at the max capacity of l
 */
void string_cat_l(string *l, const string *r) {
    const size_t rem_cap = string_rem_cap(l);
    if (rem_cap <= 0) {
        // No space to concat anything so give up
        return;
    }
    const size_t to_cat = min(rem_cap, r->len);
    memcpy(l->buffer + l->len, r->buffer, to_cat * sizeof(char));
    l->len = l->len + to_cat;

    terminate(l);
}
