#ifndef AOCC_REGEX
#define AOCC_REGEX
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>
#include "strings.h"

typedef pcre2_code pattern;
typedef struct match_data match_data;

pattern *re_compile(str p, int *err_code);
void re_free_pat(pattern *p);
/**
 * @return Index of the first match of pattern p in str s or -1 if no match
 */
long re_find(const pattern *p, str s);

match_data *re_prepare_match(pattern *p, str s);
bool re_next_match(match_data *data);

long re_get_index(const match_data *data);
bool re_get_group(const match_data *data, str *dest, size_t index);

void re_free_match(match_data *data);

#endif
