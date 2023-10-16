#include "util/Logger.h"

#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>

namespace xtc{
log4cxx::LoggerPtr Logger::logger_ptr_{nullptr};

  
Logger::Logger() {

}

Logger::~Logger() {
  
}

void Logger::Init(std::string conf_file) {
  log4cxx::PropertyConfigurator::configure(conf_file.c_str());
  log4cxx::LoggerPtr logger(log4cxx::Logger::getRootLogger());
  logger_ptr_ = logger;
}

Logger& Logger::GetInstance() {
  static Logger instance;
  return instance;
}




} // namespace xtc