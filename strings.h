#ifndef AOCC_STRING
#define AOCC_STRING
#include "vector.h"

struct string {
  size_t cap;
  size_t len;
  char *buffer;
};
typedef struct string str;
typedef vector str_vec;

bool str_is_null(str s);
str str_new_empty(size_t capacity);
str str_wrap(char *s, size_t n);
str str_wrap_c(char *s);
void str_free(str s);
size_t str_len(str s);
void str_set(str *s, const char *value, size_t len);

str str_new(size_t capacity, const char *value, size_t len);
str str_copy(size_t capacity, str original);
void str_copy_to(str *dest, str src);
str str_cat_new(str l, str r);
size_t str_cap(str s);
size_t str_rem_cap(str s);
void str_cat_l(str *l, str r);
bool str_eq(str a, str b);
bool str_eq_c(str a, const char *literal, size_t n);
int str_find_at(str haystack, str needle, size_t start);
int str_find(str haystack, str needle);
int str_find_c(str haystack, const char *needle, size_t n);
bool str_contains(str haystack, str needle);
str str_new_substr(str s, size_t start, size_t end);
str_vec *str_split(str src, str delim);
bool str_tok(str *dest, str src, size_t *pos_p, str delim);
bool str_set_cap(str *s, size_t capacity);
const char *strc(str s);
void str_trim(str *s);
long str_to_long(str s);

str vector_get_str(const str_vec *vec, size_t i);
void str_vector_free(str_vec *vec);

#define str_set_l(s, value) str_set(s, value, sizeof(value) - 1)
#define str_new_l(value) str_new(sizeof(value) - 1, value, sizeof(value) - 1)
#define strlit(value) str_wrap(value, sizeof(value) - 1)
#define str_eq_l(s, l) str_eq_c(s, l, sizeof(l) - 1)
#define str_find_l(h, n) str_find_c(h, n, sizeof(n))

#endif