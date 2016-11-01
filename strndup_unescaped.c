#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "strndup_unescaped.h"

char * strndup_unescaped(const char *s1, size_t n) {

  char * res;
  const char * reset;
  char * p;
  int i;
  char prescan;
  char hexstr[5];
  wchar_t wc; 
  
  reset = s1;

  if (n == 0) {
    return NULL;
  }

  res = malloc(sizeof(*res) * (n + 1));

  if(res == NULL){
    perror("malloc");
    return NULL;
  }

  p = res;
  i = 0;

  while(i < n && *s1 != 0){

    prescan = *s1;

    if(*s1=='\\'){

      prescan = *(s1 + 1);

      switch(prescan){
        case 0:
        break;

        case '"':
        case '/':
        s1++;
        i++;
        goto cont;
        break;

        case 'n':
        *p='\n';
        s1 += 2;
        i += 2;
        p++;
        goto cont;
        break;
        
        case 't':
        *p='\t';
        s1 += 2;
        i += 2;
        p++;
        goto cont;
        break;

        case 'u':
        prescan = *(s1 + 2);
        for(int j=2; j < 6; j++){
          if(prescan == 0){
            printf("Unexpected EOF\n");
            goto endwhile;
          }
          prescan = *(s1 + j);
        }
        strncpy(hexstr, (s1 + 2), 4);
        hexstr[4] = 0;
        wc = strtol(hexstr, NULL, 16);

        if (wc < 0x80) {
          *p = (char) wc;
          p++;
        } else {
          *p = (wc>>6) | 0xC0;
          p++;
          *p = (wc & 0x3F) | 0x80;
          p++;
        }
        i += 6;
        s1 += 6;
        
        goto cont;
        break;
      }

    }

    *p = *s1;

    p++;
    s1++;
    i++;

cont: ;

  }

endwhile: ;

  *p = 0;

  s1 = reset;

  return res;
}
