#ifndef AOCC_STRING
#define AOCC_STRING
#include "vector.h"
struct string {
    size_t cap;
    size_t len;
    char *buffer;
};
typedef struct string string;

string *string_new_empty(size_t capacity);
string string_wrap(char *s, size_t n);
void string_free(string *s);
size_t string_len(const string *s);
void string_set(string *s, const char *value, size_t len);
string *string_new(size_t capacity, const char *value, size_t len);
string *string_copy(size_t capacity, const string *original);
string *string_cat_new(const string *l, const string *r);
size_t string_cap(const string *s);
size_t string_rem_cap(const string *s);
void string_cat_l(string *l, const string *r);
bool string_eq(const string *a, const string *b);
bool string_eq_c(const string *a, char* literal, size_t n);
int string_find_at(const string *haystack, const string *needle, size_t start);
int string_find(const string *haystack, const string *needle);
int string_find_c(const string *haystack, char *needle, size_t n);
bool string_contains(const string *haystack, const string *needle);
string *string_new_substring(const string *s, size_t start, size_t end);
vector *string_split(const string *src, const string *delim);
bool string_set_cap(string *s, size_t capacity);
const char *string_c(const string *s);
void string_trim(string *s);

#define string_set_l(s, value) string_set(s, value, sizeof(value)-1)
#define string_new_l(value) string_new(sizeof(value) - 1, value, sizeof(value) -1)
#define string_l(value) string_wrap(value, sizeof(value) - 1)
#define string_eq_l(s, l) string_eq_c(s, l, sizeof(l) - 1)
#define string_find_l(h, n) string_find_c(h, n, sizeof(n))


#endif