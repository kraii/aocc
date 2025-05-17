#include "regex.h"

pattern * re_compile(const str p, int *err_code) {
    PCRE2_SIZE err_offset;
    const pcre2_code *re = pcre2_compile((PCRE2_SPTR)strc(p), str_len(p), 0, err_code, &err_offset, NULL);
    if (re == NULL) {
        return NULL;
    }
    return (pattern *)re;
}

void re_free(pattern *p) {
    pcre2_code_free(p);
}

bool re_matches(const pattern *p, const str s) {
    pcre2_match_data *match_data = pcre2_match_data_create_from_pattern(p, NULL);
    const int result_code = pcre2_match(p, (PCRE2_SPTR)strc(s), str_len(s), 0, 0, match_data, NULL);
    pcre2_match_data_free(match_data);
    return result_code > 0;
}

