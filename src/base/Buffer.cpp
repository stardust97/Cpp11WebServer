
#include "base/Buffer.h"

namespace xtc{

Buffer::Buffer() {

}

Buffer::~Buffer() {

}

char const* Buffer::GetStr() {
  return buf_.c_str();
}

ssize_t Buffer::GetSize() {
  return buf_.size();
}


void Buffer::Append(const char* str, ssize_t len) {
  for (int i = 0; i < len; ++i) {
    if(str[i] == '\0') break;
    buf_.push_back(str[i]);
  }
}

void Buffer::Clear() {
  buf_.clear();
}



} // namespace xtc
