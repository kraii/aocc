#include <signal.h>
#include <stdlib.h>
#include <bits/signum-generic.h>
#include <assert.h>
#include <string.h>
#include "strings.h"

#include <ctype.h>
#include <stdio.h>

#include "vector.h"

static void terminate(const string *s) {
    s->buffer[s->len] = '\0';
}

/**
 * Allocates a new string with the given capacity (>0) and length 0
 */
string *string_new_empty(const size_t capacity) {
    if (capacity == 0) {
        return NULL;
    }

    string *result = malloc(sizeof(string));
    if (result == NULL) {
        raise(SIGTRAP);
        return NULL;
    }
    // Allocate 1 extra char for \0
    char* buffer = malloc(sizeof(char) * (capacity + 1));
    if (buffer == NULL) {
        free(result);
        return NULL;
    }

    result->cap = capacity;
    result->len = 0;
    result->buffer = buffer;
    terminate(result);
    return result;
}

string string_wrap(char *s, const size_t n) {
    const string result  = {n, n, s};
    return result;
}

void string_free(string *s) {
    free(s->buffer);
    free(s);
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
string *string_new(const size_t capacity, const char *value, const size_t len) {
    string *s = string_new_empty(capacity);
    if (s == NULL) {
        return NULL;
    }
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

void string_copy_to(string *dest, const string *src) {
    const size_t len = min(src->len, dest->cap);
    memcpy(dest->buffer, src->buffer, len * sizeof(char));
    dest->len = len;
    terminate(dest);
}

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
 * @return the total capacity of the given string
 */
size_t string_cap(const string *s) {
    return s->cap;
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

/**
 *
 * @return true if the given strings have equal contents false otherwise.
 * Capacity is not considered.
 */
bool string_eq(const string *a, const string *b) {
    if (a->len != b->len) {
        return false;
    }
    return memcmp(a->buffer, b->buffer, a->len * sizeof(char)) == 0;
}

bool string_eq_c(const string *a, char* literal, const size_t n) {
    const string l = {n, n, literal};
    return string_eq(a, &l);
}

/**
 * @return The first index of needle in haystack, or -1 if not found
 */
int string_find_at(const string *haystack, const string *needle, const size_t start) {
    const int n = string_len(needle);
    const int m = string_len(haystack);

    if (n == 0) {
        return 0;
    }

    const int max = m - n;
    for (int i = start; i <= max; i++) {
        while (i <= max && needle->buffer[0] != haystack->buffer[i]) i++;

        if (i > max) {
            return -1;
        }
        if (needle->len == 1) {
            return i;
        }

        int j = i + 1;
        const int end = i + n - 1;

        //TODO: Would memcmp be better?
        for (int k = 1; j < end && needle->buffer[k] == haystack->buffer[j]; k++) {
            j++;
        }
        if (j == end) {
            return i;
        }
    }
    return -1;
}

int string_find(const string *haystack, const string *needle) {
    return string_find_at(haystack, needle, 0);
}


/**
 * @return The first index of needle in haystack, or -1 if not found
 */
int string_find_c(const string *haystack, char *needle, const size_t n) {
    string target;
    target.cap = n;
    target.len = n;
    target.buffer = needle;
    return string_find(haystack, &target);
}

bool string_contains(const string *haystack, const string *needle) {
    return string_find(haystack, needle) != -1;
}

/**
 * @param s a string
 * @param start the start index of the substring, inclusive
 * @param end the index at the end of the string, not inclusive
 * @return A new string containing the part of the string between the given indexes.
 * NULL if start >= end or if start is out of bounds
 */
string *string_new_substring(const string *s, const size_t start, const size_t end) {
    if (start >= s->len || start >= end) {
        return NULL;
    }
    const size_t len = min(end, s->len) - start;
    return string_new(len, s->buffer + start, len);
}

vector *string_split(const string *src, const string *delim) {
    vector *result = vector_new(sizeof(string*));
    int match = string_find(src, delim);
    size_t index = 0;
    while (match != -1 && index < src->len) {
        const string *token = string_new_substring(src, index, match);
        vector_push(result, &token);
        index = match + delim->len;
        match = string_find_at(src, delim, index);
    }
    const string *token = string_new_substring(src, index, src->len+1);
    vector_push(result, &token);

    return result;
}

bool string_tok(string *dest,  const string *src, size_t *posp, const string *delim) {
    const size_t pos = *posp;
    if (pos >= src->len || src->len == 0) {
        return false;
    }
    const int match = string_find_at(src, delim, pos);
    size_t to_copy;
    if (match == -1) {
        to_copy = src->len - pos;
        *posp = src->len;
    } else {
        *posp = match + delim->len;
        to_copy = match - pos;
    }

    if (to_copy > src->len) {
        // overflow so nothing to copy
        return false;
    }
    string_set(dest, &src->buffer[pos], to_copy);
    return match != -1;
}

bool string_set_cap(string *s, const size_t capacity) {
    assert(capacity >= s->len);
    char *buff = realloc(s->buffer, sizeof(char) * (capacity + 1));
    if (buff == NULL) {
        return false;
    }
    s->buffer = buff;
    s->cap = capacity;
    return true;
}

const char *string_c(const string *s) {
    return s->buffer;
}

void string_trim(string *s) {
    size_t i = 0;

    while (i < s->len-1 && isspace(s->buffer[i])) {
        i++;
    }
    if (i > 0) {
        s->len -= i;
        memmove(s->buffer, &s->buffer[i], s->len * sizeof(char));
    }


    while (s->len != 0 && isspace(s->buffer[s->len-1])) {
        s->len--;
    }
    terminate(s);
}

long string_to_l(string *s) {
    char *end;
    const long result = strtol(s->buffer, &end, 10);
    assert(end != s->buffer);
    return result;
}
