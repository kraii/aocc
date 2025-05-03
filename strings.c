#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <bits/signum-generic.h>

enum String_status {
    Success = 0,
    Bounds_Check_Failure
};

struct string {
    size_t cap;
    size_t len;
    char buffer[];
};

typedef struct string string;

string *new_empty_string(const size_t capacity) {
    // Allocate 1 extra char for \0
    string *result = malloc(offsetof(string, buffer) + (sizeof(char) * (capacity + 1)));
    result->cap = capacity;
    result->len = 0;
    result->buffer[0] = '\0';
    return result;
}

int string_set(string *s, const char *value, const size_t len) {
    if (len > s->cap) {
        raise(SIGTRAP);
        return Bounds_Check_Failure;
    }

    memcpy(s->buffer, value, len * sizeof(char));
    s->buffer[len] = '\0';
    s->len = len;
    return Success;
}

string *new_string(const size_t capacity, const char*value, const size_t len) {
    string *s = new_empty_string(capacity);
    if (string_set(s, value, len) != Success) {
        free(s);
        return NULL;
    }

    return s;
}