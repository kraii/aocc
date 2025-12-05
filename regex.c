#include "regex.h"

#include <stdio.h>
#include <string.h>

pattern *re_compile(const str p, int *err_code) {
  PCRE2_SIZE err_offset;
  const pcre2_code *re = pcre2_compile((PCRE2_SPTR)strc(p), str_len(p), 0, err_code, &err_offset, NULL);
  if (re == NULL) {
    return NULL;
  }
  return (pattern *)re;
}

void re_free_pat(pattern *p) { pcre2_code_free(p); }

long re_find(const pattern *p, const str s) {
  pcre2_match_data *match_data = pcre2_match_data_create_from_pattern(p, NULL);
  const int result_code = pcre2_match(p, (PCRE2_SPTR)strc(s), str_len(s), 0, 0, match_data, NULL);

  if (result_code <= 0) {
    pcre2_match_data_free(match_data);
    return -1;
  }

  const PCRE2_SIZE *output = pcre2_get_ovector_pointer(match_data);
  const PCRE2_SIZE match_start = output[0];
  pcre2_match_data_free(match_data);

  return (long)match_start;
}

struct match_data {
  pcre2_match_data *pcre2;
  pattern *pattern;
  str target;
  size_t index;
  int rc;
};

match_data *re_prepare_match(pattern *p, const str s) {
  pcre2_match_data *pcr2_data = pcre2_match_data_create_from_pattern(p, NULL);

  if (pcr2_data == NULL) {
    return NULL;
  }

  match_data *data = malloc(sizeof(match_data));
  data->pcre2 = pcr2_data;
  data->pattern = p;
  data->target = s;
  data->index = 0;
  data->rc = -1;

  return data;
}

bool re_next_match(match_data *data) {
  data->rc = pcre2_match(data->pattern, (PCRE2_SPTR)strc(data->target), str_len(data->target), data->index, 0,
                         data->pcre2, NULL);
  if (data->rc <= 0) {
    return false;
  }
  const PCRE2_SIZE *output = pcre2_get_ovector_pointer(data->pcre2);
  const PCRE2_SIZE match_end = output[1];
  data->index = match_end;
  return true;
}

long re_get_index(const match_data *data) {
  if (data->rc <= 0) {
    return -1;
  }
  const PCRE2_SIZE *output = pcre2_get_ovector_pointer(data->pcre2);
  const PCRE2_SIZE match_start = output[0];
  return match_start;
}

bool re_get_group(const match_data *data, str *dest, const size_t index) {
  PCRE2_SIZE len_capture;
  int rc = pcre2_substring_length_bynumber(data->pcre2, index, &len_capture);
  if (rc != 0 || len_capture == 0) {
    return false;
  }
  if (str_cap(*dest) < len_capture) {
    str_set_cap(dest, len_capture);
  }

  len_capture++;
  rc = pcre2_substring_copy_bynumber(data->pcre2, index, (PCRE2_UCHAR *)dest->buffer, &len_capture);
  dest->len = len_capture;
  return rc == 0;
}

str re_get_error_message(const int err_code) {
  constexpr int max_error_size = 100;
  char err[max_error_size];

  pcre2_get_error_message(err_code, (PCRE2_UCHAR *) err, max_error_size);
  const size_t len = strlen(err);
  return str_new(len, err, len);
}

void re_free_match(match_data *data) {
  pcre2_match_data_free(data->pcre2);
  free(data);
}
