#include <string.h>
#include <limits.h>
#include <sys/stat.h>
#include <errno.h>

#include "mkdir_p.h"
/*
  https://gist.github.com/JonathonReinhart/8c0d90191c38af2dcadb102c4e202950   
*/
int mkdir_p(const char *path) {

  const size_t len = strlen(path);
  char _path[PATH_MAX];
  char *p; 

  errno = 0;

  if (len > sizeof(_path)-1) {
    errno = ENAMETOOLONG;
    return -1; 
  }   
  strcpy(_path, path);

  for (p = _path + 1; *p; p++) {
    if (*p == '/') {
      *p = '\0';

      if (mkdir(_path, S_IRWXU) != 0) {
        if (errno != EEXIST)
          return -1; 
      }

      *p = '/';
    }
  }   

  if (mkdir(_path, S_IRWXU) != 0) {
    if (errno != EEXIST)
      return -1; 
  }   

  return 0;
}
