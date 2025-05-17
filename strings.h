#ifndef AOCC_STRING
#define AOCC_STRING
#include "vector.h"
struct string {
    size_t cap;
    size_t len;
    char *buffer;
};
typedef struct string str;

bool str_is_null(str s);
str str_new_empty(size_t capacity);
str string_wrap(char *s, size_t n);
void string_free(str s);
size_t string_len(str s);
void string_set(str *s, const char *value, size_t len);

str string_new(size_t capacity, const char *value, size_t len);
str string_copy(size_t capacity, str original);
void string_copy_to(str *dest, str src);
str string_cat_new(str l, str r);
size_t string_cap(str s);
size_t string_rem_cap(str s);
void string_cat_l(str *l, str r);
bool string_eq(str a, str b);
bool string_eq_c(str a, const char* literal, size_t n);
int string_find_at(str haystack, str needle, size_t start);
int string_find(str haystack, str needle);
int string_find_c(str haystack, const char *needle, size_t n);
bool string_contains(str haystack, str needle);
str string_new_substring(str s, size_t start, size_t end);
vector *string_split(str src, str delim);
bool string_tok(str *dest, str src, size_t *pos_p, str delim);
bool string_set_cap(str *s, size_t capacity);
const char *string_c(str s);
void string_trim(str *s);
long string_to_l(str s);

str vector_get_str(const vector *vec, size_t i);


#define string_set_l(s, value) string_set(s, value, sizeof(value)-1)
#define string_new_l(value) string_new(sizeof(value) - 1, value, sizeof(value) -1)
#define string_l(value) string_wrap(value, sizeof(value) - 1)
#define string_eq_l(s, l) string_eq_c(s, l, sizeof(l) - 1)
#define string_find_l(h, n) string_find_c(h, n, sizeof(n))


#endif