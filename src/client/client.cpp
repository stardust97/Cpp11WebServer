
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
      std::string msg = "hello "+ std::to_string(i);
      conn ->SetWriteBuf( msg );
      conn ->Write();
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      // conn->Read();
    }
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}