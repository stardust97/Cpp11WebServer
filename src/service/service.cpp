/*
 * @Author: stardust97 641765169@qq.comls
 * @Date: 2023-09-18 06:59:24
 * @LastEditors: stardust97 641765169@qq.comls
 * @LastEditTime: 2023-09-19 08:41:28
 * @FilePath: /30daywebserver/Cpp11WebServer/service.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#include <filesystem>
#include <iostream>

#include "base/EventLoop.h"
#include "base/Server.h"
#include "util/Logger.h"

int main() {
  std::cout << std::filesystem::current_path() << std::endl;
  xtc::Logger logger = xtc::Logger::GetInstance();
  logger.Init("conf/log4cxx.properties");
  LOG4CXX_INFO(xtc::Logger::GetLogger(), "server start ");
  xtc::EventLoop* loop = new xtc::EventLoop();
  xtc::Server* server = new xtc::Server(loop);
  loop -> Loop();
  delete server;
  delete loop;
  return 0;
}
