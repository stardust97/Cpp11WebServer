#ifndef __XTC__BUFFER__H__
#define __XTC__BUFFER__H__

#include <string>

namespace xtc{
class Buffer{
public:
  Buffer();
  ~Buffer();

  ssize_t GetSize();
  void Append(const char* str, ssize_t len);
  void Clear();
  char const* GetStr();
private:
  std::string buf_; // 缓冲区容器

};


  
} // namespace xtc



#endif