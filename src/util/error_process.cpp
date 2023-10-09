#include "util/error_process.h"

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <errno.h>
#include <cstring>

namespace xtc{
  
void errif(bool condition, const char* errmsg) {
  if(condition) {
    perror(errmsg);
    printf("%s\n",strerror(errno));
    exit(EXIT_FAILURE);
  }
}
} // namespace xtc