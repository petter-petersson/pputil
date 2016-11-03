#ifndef __APPEND_BUF_H
#define __APPEND_BUF_H

#include <stdint.h>

struct append_buf_s {
  uint8_t * buf;
  uint32_t pos;
  uint32_t buf_size;
  uint32_t mem_chunk_size;
};

typedef struct append_buf_s append_buf_t;

append_buf_t * append_buf_create(uint32_t mem_chunk_size);
void append_buf_destroy(append_buf_t * buf);
int append_to_buf(append_buf_t * buf, uint8_t * append, uint32_t append_size);

#endif
