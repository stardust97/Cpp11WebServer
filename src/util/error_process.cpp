#include "util/error_process.h"

#include <cassert>
#include <cstdlib>
#include <cstdio>

namespace xtc{
  
void errif(bool condition, const char* errmsg) {
  if(condition) {
    perror(errmsg);
    exit(EXIT_FAILURE);
  }
}
} // namespace xtc