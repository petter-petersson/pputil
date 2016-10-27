#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "append_buf.h"

append_buf_t * http_util_append_buf_create(uint8_t mem_chunk_size){

  append_buf_t * ret;

  ret = malloc(sizeof(*ret));

  if (ret == NULL){
    perror("EOM http_util_append_buf_create");
    return NULL;
  }
  ret->buf = NULL;
  ret->pos = 0;
  ret->buf_size = 0;
  ret->mem_chunk_size = mem_chunk_size;

  return ret;
}

void http_util_append_buf_destroy(append_buf_t * buf){
  if(buf == NULL){
    return;
  }
  if(buf->buf != NULL){
    free(buf->buf);
  }
  free(buf);
  buf = NULL;
}

int http_util_append_to_buf(append_buf_t * buf, uint8_t * append, uint32_t append_size){
  uint8_t * tmp;

  if(buf->buf == NULL){

    if(append_size < buf->mem_chunk_size){
      buf->buf = malloc( buf->mem_chunk_size * sizeof(char) );
      buf->buf_size = buf->mem_chunk_size;
    } else {
      buf->buf = malloc( (append_size + 1) * sizeof(char) );
      buf->buf_size = append_size;
    }
    if(buf->buf == NULL){
      perror("malloc failure");
      return 1;
    }
    memcpy(buf->buf, append, append_size);
    buf->pos = append_size;
  } else {
    
    int required = buf->pos + append_size + 1;
    if(required > buf->buf_size){
      if(required > (buf->buf_size) + buf->mem_chunk_size){
        tmp = realloc( buf->buf, required * sizeof(char) );
        buf->buf_size = required;
      } else {
        tmp = realloc( buf->buf, (buf->buf_size + buf->mem_chunk_size) * sizeof(char) );
        buf->buf_size = buf->buf_size + buf->mem_chunk_size;
      }
      if(tmp == NULL){
        perror("malloc failure");
        return 1;
      }
      buf->buf = tmp;
      memcpy(buf->buf + buf->pos, append, append_size);
      buf->pos = buf->pos + append_size;
    } else {
      memcpy(buf->buf + buf->pos, append, append_size);
      buf->pos = buf->pos + append_size;
    }
  }
  buf->buf[buf->pos] = 0;

  return 0;
}
