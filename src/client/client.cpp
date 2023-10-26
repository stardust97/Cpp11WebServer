/*
 * @Author: stardust97 641765169@qq.comls
 * @Date: 2023-09-18 07:01:38
 * @LastEditors: stardust97 641765169@qq.comls
 * @LastEditTime: 2023-09-19 08:42:02
 * @FilePath: /30daywebserver/Cpp11WebServer/client.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */


#include <thread>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>

#include "base/Connection.h"
#include "base/Socket.h"
#include "base/InetAddress.h"
#include "util/Logger.h"
int main(){
  xtc::Logger logger = xtc::Logger::GetInstance();
  logger.Init("conf/log4cxx.properties");
  LOG4CXX_INFO(xtc::Logger::GetLogger(), "server start ");
  xtc::InetAddress *addr= new xtc::InetAddress("127.0.0.1", 8888);
  xtc::Socket* socket = new xtc::Socket();
  socket -> Connect(addr);
  xtc::Connection* conn = new xtc::Connection(nullptr, socket);
  try {
    for(int32_t i = 0; i < 200; ++i) {
      conn ->SetWriteBuf("hello " + std::to_string(i));
      conn ->Write();
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      // conn->Read();
    }
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}