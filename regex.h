#ifndef AOCC_REGEX
#define AOCC_REGEX
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>
#include "vector.h"
#include "strings.h"

typedef pcre2_code pattern;

pattern *re_compile(str p, int *err_code);
void re_free(pattern *p);
bool re_matches(const pattern *p, str s);

#endif
