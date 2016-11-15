#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <time.h>

#include "mkdir_p.h"
#include "log.h"

log_ctx_t __log_ctx;

void log_message(log_level_t level, const char * func, const int line_no, const char * fmt, ...){

  const char * llstr[]= {
    "NONE",
    "TRACE",
    "DEBUG",
    "INFO",
    "WARN",
    "FATAL"
  };

  const char * lcol[]= {
    "",
    "\x1B[31m",
    "\x1B[32m",
    "\x1B[33m",
    "\x1B[34m",
    "\x1B[35m",
    "\x1B[36m",
    "\x1B[37m",
    "\x1B[0m"
  };

  const char * color_start;
  const char * color_end;

  time_t t;
  char tb[26];
  struct tm* tm_info;
  char fs[MAX_FMT_SIZE];

  va_list arg;

  log_ctx_t * ctx = _log_get_context();
  if(ctx->fd == NULL){
    fprintf(stderr, "%s:%d no file descriptor available\n", __FUNCTION__, __LINE__);
    return;
  }
  if(ctx->level == NONE){
    return;
  }
  if(level > FATAL || level < NONE){
    fprintf(stderr, "illegal log level: %u\n", level);
    return;
  }

  if(level < ctx->level){
    //printf("level belog threshold\n");
    return;
  }


  va_start(arg, fmt);
  vsnprintf(fs, MAX_FMT_SIZE-1, fmt, arg);
  va_end(arg);

  time(&t);
  tm_info = localtime(&t);
  strftime(tb, 26, "%Y-%m-%d %H:%M:%S", tm_info);

  switch(level){
    case INFO:
      color_start = lcol[YEL];
      color_end = lcol[RESET];
      break;
    case WARN:
      color_start = lcol[MAG];
      color_end = lcol[RESET];
      break;
    case FATAL:
      color_start = lcol[RED];
      color_end = lcol[RESET];
      break;
    case NONE:
    case TRACE:
    case DEBUG:
    default:
      color_start = lcol[NOC];
      color_end = lcol[NOC];
      break;
  }

  pthread_mutex_lock(&(ctx->log_mutex));
  fprintf(ctx->fd, "%s[%s] %-5s <%s:%d> %s%s\n", color_start, tb, llstr[level], func, line_no, fs, color_end);
  fflush(ctx->fd); 
  pthread_mutex_unlock(&(ctx->log_mutex));
}

int log_init(const char * dir, const char * filename, log_level_t level){
  
  log_ctx_t * ctx = _log_get_context();

  ctx->fd = NULL;
  ctx->path = NULL;
  if (pthread_mutex_init(&(ctx->log_mutex), NULL) != 0){
    perror("mutex init failed");
    return 1;
  }

  if(mkdir_p(dir)){
    printf("failed to create dir to log\n");
    return 1;
  }
  int len = strlen(dir) + strlen(filename) + 2;

  char * path = malloc(sizeof(char) * len);
  if(path == NULL){
    printf("out of memory\n");
    return 1;
  }
  strcpy(path, dir);
  strcat(path, "/");
  strcat(path, filename);
  path[len-1] = 0;
  
  ctx->path = path;

  ctx->level = level;

  ctx->fd = fopen (ctx->path, "a"); 
  if(ctx->fd == NULL){
    printf("could not open log file <%s>\n", ctx->path);
    goto err;
  }

  return 0;

err:

  if(ctx->path != NULL){
    free(ctx->path);
  }

  return 1;
}
void log_destroy(){

  log_ctx_t * ctx = _log_get_context();

  if(ctx->path != NULL){
    free(ctx->path);
  }
  if(ctx->fd != NULL){
    fclose(ctx->fd);
  }

  pthread_mutex_destroy(&(ctx->log_mutex));
}

log_ctx_t * _log_get_context(){

  return &__log_ctx;
}
