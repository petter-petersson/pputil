#ifndef __LOG_H
#define __LOG_H

#include <stdio.h>
#include <pthread.h>

#define MAX_FMT_SIZE 1024

//test
typedef enum log_color_s {
  NOC,
  RED,
  GRN,
  YEL,
  BLU,
  MAG,
  CYN,
  WHT,
  RESET
} log_color_t;


typedef enum log_level_e {
  NONE,
  TRACE,
  DEBUG,
  INFO,
  WARN,
  FATAL
} log_level_t;

typedef struct log_ctx_s {
  
  FILE * fd;
  char * path;
  log_level_t level;
  pthread_mutex_t log_mutex;

} log_ctx_t;

//TODO can we have this in a static variable inside _log_get_context instead
extern log_ctx_t __log_ctx;

void log_message(log_level_t level, const char * func, const int line_no, const char * fmt, ...);

int log_init(const char * dir, const char * filename, log_level_t level);

void log_destroy();

log_ctx_t * _log_get_context();


#define LOG(level, fmt, ...) do { \
  log_message(level, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__); \
} while(0)

#define LOG_TRACE(fmt, ...) LOG(TRACE, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) LOG(DEBUG, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) LOG(INFO, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) LOG(WARN, fmt, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) LOG(FATAL, fmt, ##__VA_ARGS__)

#endif
