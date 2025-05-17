#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>
#include <stdio.h>
#include <string.h>

int main(...) {
    const char *pattern = "[0-9]";

    pcre2_match_data *match_data = pcre2_match_data_create_from_pattern(re, NULL);
    const char *matchy = "123abc2";
    const int result_code = pcre2_match(re, (PCRE2_SPTR)matchy, strlen(matchy), 0, 0, match_data, NULL);
    if (result_code < 0) {
        printf("Pooper\n");
        return - 2;
    }

    PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(match_data);
    for (int i = 0; i < result_code; i++){
        const PCRE2_SIZE substring_start = ovector[2*i];
        const PCRE2_SIZE substring_length = ovector[2*i+1] - ovector[2*i];
        printf("%2d: %d, %d", i, (int)substring_length, substring_start);
    }
}
