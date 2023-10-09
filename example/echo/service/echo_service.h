/*
 * @Author: stardust97 641765169@qq.comls
 * @Date: 2023-09-18 06:59:24
 * @LastEditors: stardust97 641765169@qq.comls
 * @LastEditTime: 2023-09-19 08:41:28
 * @FilePath: /30daywebserver/Cpp11WebServer/service.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */


#include "base/InetAddress.h"
#include "base/Epoll.h"
#include "base/Channel.h"


namespace xtc{

class EchoService {
public:
  EchoService(InetAddress addr);
  ~EchoService();
  void StartUp();

private:
  static constexpr int32_t MAX_EVENTS{10024};
  static constexpr int32_t MAX_BUF_SIZE{1024};

  void on_recv_msg(xtc::Channel* client_channel);
  void on_new_connect();

  Epoll *epoll_;
  InetAddress addr_;
};



} // namespace xtc













