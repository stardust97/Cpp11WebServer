#ifndef __XTC__LOGGER__H__
#define __XTC__LOGGER__H__


#include <log4cxx/logger.h>


namespace xtc{
class Logger {
public:
  Logger();
  ~Logger();
  void Init(std::string conf_file);
  static Logger& GetInstance();

 static log4cxx::LoggerPtr GetLogger() { return logger_ptr_;}

private:
 static log4cxx::LoggerPtr logger_ptr_;
};

} // namespace xtc


#endif // __XTC__LOGGER__H__






