#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "test.h"
#include "log.h"


int test_logging(test_context_t *tctx){

  struct stat sb;
  if(stat("logtest/with/some/subdirs/app.log", &sb)==0){
    //truncating test file
    FILE * f = fopen("logtest/with/some/subdirs/app.log", "w");
    fclose(f);
  }

  /*TODO assert contents after log calls*/
  int rv;
  rv = log_init("logtest/with/some/subdirs", "app.log", DEBUG);
  check(rv == 0, tctx);

  log_ctx_t * ctx = _log_get_context();

  check(strcmp("logtest/with/some/subdirs/app.log", ctx->path)==0, tctx);

  printf("%s\n", ctx->path);

  FILE * fd = ctx->fd;

  check(fd != NULL, tctx);

  fprintf(ctx->fd, "apa gnu gitarr\n");

  log_message(TRACE, __FUNCTION__, __LINE__, "this should not show %d", 23);
  log_message(DEBUG, __FUNCTION__, __LINE__, "this should show! %p", ctx);
  log_message(FATAL, __FUNCTION__, __LINE__, "this should show! %p", ctx);
  log_message(INFO, __FUNCTION__, __LINE__, "this should show! %p", ctx);
  log_message(WARN, __FUNCTION__, __LINE__, "this should show! %p", ctx);

  LOG(WARN, "lennart swahn %d %s", 1, "apa");
  LOG_FATAL("lennart swahn %d %s", 1, "apa");
  LOG_INFO("lennart swahn %d %s", 1, "apa");

  log_destroy();

  return 0;
}

int main(){
  test_context_t context;
  test_context_init(&context);

  test_ctx(test_logging, "test_logging", &context);

  test_context_show_result(&context);
}
