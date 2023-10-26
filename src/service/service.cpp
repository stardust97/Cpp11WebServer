
#include <filesystem>
#include <iostream>

#include "base/EventLoop.h"
#include "base/Server.h"
#include "base/Connection.h"
#include "util/Logger.h"

int main() {
  xtc::Logger logger = xtc::Logger::GetInstance();
  logger.Init("conf/log4cxx.properties");
  LOG4CXX_INFO(xtc::Logger::GetLogger(), "server start ");
  xtc::EventLoop* loop = new xtc::EventLoop();
  xtc::Server* server = new xtc::Server(loop);
  server ->SetConnectionCallback(
    [](xtc::Connection* conn) {
      conn -> Read();
      if(conn -> GetState() == xtc::Connection::ConnState::CLOSED) {
        printf("Connection closed\n");
        conn -> Close();
        return;
      }
      // conn -> Write(); // TODO
    }
  );
  loop -> Loop();
  delete server;
  delete loop;
  return 0;
}
