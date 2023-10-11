/*
 * @Author: stardust97 641765169@qq.comls
 * @Date: 2023-09-18 06:59:24
 * @LastEditors: stardust97 641765169@qq.comls
 * @LastEditTime: 2023-09-19 08:41:28
 * @FilePath: /30daywebserver/Cpp11WebServer/service.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */


#include "base/EventLoop.h"
#include "base/Server.h"


int main() {
  xtc::EventLoop* loop = new xtc::EventLoop();
  xtc::Server* server = new xtc::Server(loop);
  server ->Start();
  loop -> Loop();
  return 0;
}
