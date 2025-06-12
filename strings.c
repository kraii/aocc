#include "strings.h"
#include <assert.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include <ctype.h>
#include <stdio.h>

#include "vector.h"

#define str_null ((str){0, 0, NULL})

static void terminate(const str *s) { s->buffer[s->len] = '\0'; }

bool str_is_null(const str s) { return s.buffer == NULL; }

/**
 * Allocates a new string with the given capacity (>0) and length 0
 */
str str_new_empty(const size_t capacity) {
  if (capacity == 0) {
    return str_null;
  }

  char *buffer = malloc(sizeof(char) * (capacity + 1));
  if (buffer == NULL) {
    return str_null;
  }
  const str result = {capacity, 0, buffer};
  terminate(&result);
  return result;
}

str str_wrap(char *s, const size_t n) {
  const str result = {n, n, s};
  return result;
}

str str_wrap_c(char *s) {
  const int len = strlen(s);
  const str result = {len, len, s};
  return result;
}

void str_free(const str s) { free(s.buffer); }

static size_t min(const size_t a, const size_t b) { return b < a ? b : a; }

/**
 * @return the length of the given string
 */
size_t str_len(const str s) { return s.len; }

/**
 * Sets the contents of the given string to the value of buffer
 * If len > capacity will only copy up to capacity
 */
void str_set(str *s, const char *value, const size_t len) {
  memcpy(s->buffer, value, min(len, s->cap) * sizeof(char));
  s->len = min(len, s->cap);
  terminate(s);
}

/**
 * Allocates a new string with the value of the given buffer
 * If len > capacity will only copy up to capacity
 */
str str_new(const size_t capacity, const char *value, const size_t len) {
  str s = str_new_empty(capacity);
  if (s.buffer == NULL) {
    return str_null;
  }
  str_set(&s, value, len);
  return s;
}

/**
 * Allocates a new string that is a copy of the given string
 * If len of the original > capacity will only copy up to capacity
 */
str str_copy(const size_t capacity, const str original) {
  return str_new(capacity, original.buffer, min(capacity, original.len));
}

void str_copy_to(str *dest, const str src) {
  const size_t len = min(src.len, dest->cap);
  memcpy(dest->buffer, src.buffer, len * sizeof(char));
  dest->len = len;
  terminate(dest);
}

/**
 * @return A newly allocated string with the result of concatenating l and r
 */
str str_cat_new(const str l, const str r) {
  str result = str_copy(l.len + r.len, l);
  memcpy(result.buffer + l.len, r.buffer, r.len * sizeof(char));
  result.len = l.len + r.len;
  terminate(&result);
  return result;
}

/**
 * @return the total capacity of the given string
 */
size_t str_cap(const str s) { return s.cap; }

/**
 * @return The remaining capacity of the given string
 */
size_t str_rem_cap(const str s) { return s.cap - s.len; }

/**
 * Write the result of concatenating l and r into l
 * will truncate at the max capacity of l
 */
void str_cat_l(str *l, const str r) {
  const size_t rem_cap = str_rem_cap(*l);
  if (rem_cap <= 0) {
    // No space to concat anything so give up
    return;
  }
  const size_t to_cat = min(rem_cap, r.len);
  memcpy(l->buffer + l->len, r.buffer, to_cat * sizeof(char));
  l->len = l->len + to_cat;

  terminate(l);
}

/**
 *
 * @return true if the given strings have equal contents false otherwise.
 * Capacity is not considered.
 */
bool str_eq(const str a, const str b) {
  if (a.len != b.len) {
    return false;
  }
  return memcmp(a.buffer, b.buffer, a.len * sizeof(char)) == 0;
}

bool str_eq_c(const str a, const char *literal, const size_t n) {
  const str l = {n, n, (char *)literal};
  return str_eq(a, l);
}

/**
 * @return The first index of needle in haystack, or -1 if not found
 */
int str_find_at(const str haystack, const str needle, const size_t start) {
  const int n = str_len(needle);
  const int m = str_len(haystack);

  if (n == 0) {
    return 0;
  }

  const int max = m - n;
  for (int i = start; i <= max; i++) {
    while (i <= max && needle.buffer[0] != haystack.buffer[i])
      i++;

    if (i > max) {
      return -1;
    }
    if (needle.len == 1) {
      return i;
    }

    int j = i + 1;
    const int end = i + n - 1;

    // TODO: Would memcmp be better?
    for (int k = 1; j < end && needle.buffer[k] == haystack.buffer[j]; k++) {
      j++;
    }
    if (j == end) {
      return i;
    }
  }
  return -1;
}

int str_find(const str haystack, const str needle) { return str_find_at(haystack, needle, 0); }

/**
 * @return The first index of needle in haystack, or -1 if not found
 */
int str_find_c(const str haystack, const char *needle, const size_t n) {
  const str target = {n, n, (char *)needle};
  return str_find(haystack, target);
}

bool str_contains(const str haystack, const str needle) { return str_find(haystack, needle) != -1; }

/**
 * @param s a string
 * @param start the start index of the substring, inclusive
 * @param end the index at the end of the string, not inclusive
 * @return A new string containing the part of the string between the given
 * indexes. NULL if start >= end or if start is out of bounds
 */
str str_new_substr(const str s, const size_t start, const size_t end) {
  if (start >= s.len || start >= end) {
    return str_null;
  }
  const size_t len = min(end, s.len) - start;
  return str_new(len, s.buffer + start, len);
}

str_vec *str_split(const str src, const str delim) {
  str_vec *result = vector_new(sizeof(str));
  int match = str_find(src, delim);
  size_t index = 0;
  while (match != -1 && index < src.len) {
    const str token = str_new_substr(src, index, match);
    vector_push(result, &token);
    index = match + delim.len;
    match = str_find_at(src, delim, index);
  }
  const str token = str_new_substr(src, index, src.len + 1);
  vector_push(result, &token);

  return result;
}

bool str_tok(str *dest, const str src, size_t *pos_p, const str delim) {
  const size_t pos = *pos_p;
  if (pos >= src.len || src.len == 0) {
    return false;
  }
  const int match = str_find_at(src, delim, pos);
  size_t to_copy;
  if (match == -1) {
    to_copy = src.len - pos;
    *pos_p = src.len;
  } else {
    *pos_p = match + delim.len;
    to_copy = match - pos;
  }

  if (to_copy > src.len) {
    // overflow so nothing to copy
    return false;
  }

  str_set(dest, &src.buffer[pos], to_copy);
  return true;
}

bool str_set_cap(str *s, const size_t capacity) {
  assert(capacity >= s->len);
  char *buff = realloc(s->buffer, sizeof(char) * (capacity + 1));
  if (buff == NULL) {
    return false;
  }
  s->buffer = buff;
  s->cap = capacity;
  return true;
}

const char *strc(const str s) { return s.buffer; }

void str_trim(str *s) {
  size_t i = 0;

  while (i < s->len - 1 && isspace(s->buffer[i])) {
    i++;
  }
  if (i > 0) {
    s->len -= i;
    memmove(s->buffer, &s->buffer[i], s->len * sizeof(char));
  }

  while (s->len != 0 && isspace(s->buffer[s->len - 1])) {
    s->len--;
  }
  terminate(s);
}

long str_to_long(const str s) {
  char *end;
  const long result = strtol(s.buffer, &end, 10);
  assert(end != s.buffer);
  return result;
}

str vector_get_str(const str_vec *vec, const size_t i) {
  void *r = vector_get(vec, i);
  assert(r != NULL);
  return *(str *)r;
}

void str_vec_free(str_vec *vec) {
  for (size_t i = 0; i < vector_len(vec); i++) {
    str_free(vector_get_str(vec, i));
  }
  vector_free(vec);
}