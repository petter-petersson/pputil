#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include "test.h"
#include "strndup_unescaped.h"


int test_convert_nothing(test_context_t * tctx){

  char * in = "Detta is a test";

  char * res = strndup_unescaped(in, strlen(in));

  check(strcmp(res, "Detta is a test") == 0, tctx);

  free(res); 
  res = NULL;

  return 0;
}

int test_convert_unicode_encoded_strings(test_context_t * tctx){

  char * in = "Detta \\u00e4r ett test \\u0026 inget annat";

  char * res = strndup_unescaped(in, strlen(in));

  check(strcmp(res, "Detta är ett test & inget annat") == 0, tctx);

  free(res); 
  res = NULL;
  return 0;
}

int test_convert_newlines(test_context_t * tctx){
  char * in = "Detta\\nis a test";

  char * res = strndup_unescaped(in, strlen(in));

  check(strcmp(res, "Detta\nis a test") == 0, tctx);

  free(res); 
  res = NULL;

  char * in2 = "Detta\\nis a test\\n";

  res = strndup_unescaped(in2, strlen(in2));

  check(strcmp(res, "Detta\nis a test\n") == 0, tctx);

  free(res); 
  res = NULL;
  return 0;
}

int test_remove_backslashes(test_context_t * tctx){
  char * in = "Detta \\\" is a test";

  char * res = strndup_unescaped(in, strlen(in));

  check(strcmp(res, "Detta \" is a test") == 0, tctx);

  free(res); 
  res = NULL;

  char * in2 = "Detta is a test\\\"";
  res = strndup_unescaped(in2, strlen(in2));
  check(strcmp(res, "Detta is a test\"") == 0, tctx);

  free(res); 

  return 0;
}

int main() {
  test_context_t context;
  test_context_init(&context);

  test_ctx(test_remove_backslashes, "test_remove_backslashes", &context);
  test_ctx(test_convert_newlines, "test_convert_newlines", &context);
  test_ctx(test_convert_nothing, "test_convert_nothing", &context);
  test_ctx(test_convert_unicode_encoded_strings, "test_convert_unicode_encoded_strings", &context);

  test_context_show_result(&context);
}





