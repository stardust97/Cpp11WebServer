/*
 * @Author: stardust97 641765169@qq.comls
 * @Date: 2023-09-18 06:59:24
 * @LastEditors: stardust97 641765169@qq.comls
 * @LastEditTime: 2023-09-19 08:41:28
 * @FilePath: /30daywebserver/Cpp11WebServer/service.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */


#include <cstring>
#include <cstdio>
#include <cstdlib>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>

#include "util/error_process.h"
#include "base/Socket.h"
#include "base/InetAddress.h"
#include "base/Epoll.h"
#include "base/Channel.h"


#define MAX_EVENTS 1024
#define MAX_BUF_SIZE 1024

xtc::Epoll *epoll;

void on_recv_msg(xtc::Channel* client_channel) {
  printf("client has new msg\n");
  char buf[MAX_BUF_SIZE];
  while (true) { // ET模式需要一次性读出所有数据
    bzero(buf, MAX_BUF_SIZE);
    int32_t client_fd =  client_channel -> GetFd();
    int32_t read_bytes = read(client_fd, buf, MAX_BUF_SIZE);
    if(read_bytes > 0) {
      printf("client fd: %d, recv msg: %s\n", client_fd, buf);
    } else if(read_bytes == 0){  //EOF，客户端断开连接
      printf("EOF, client fd %d disconnected\n", client_fd);
      close(client_fd); // TODO 和removefromepoll封装到Channel中
      epoll->RemoveFromEpoll(client_channel); // BUG 当前存在内存泄漏
      break;
    } 
      //非阻塞IO，这个条件表示本次数据全部读取完毕
      else if(read_bytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){
      printf("client %d read finished!\n", client_fd);
      break;
    } else if(read_bytes == -1 && errno == EINTR){  //客户端正常中断、继续读取
      printf("need next read\n");
      break;
    } 
  }
}
int main() {
  xtc::Socket* socket = new xtc::Socket();
  xtc::InetAddress addr("127.0.0.1", 8888);
  socket->bind(addr);
  socket->listen();

  epoll = new xtc::Epoll();
  xtc::Channel* service_channel = new xtc::Channel(epoll, socket->GetFd());
  service_channel->EnableReading(); //阻塞 LT模式
  printf("start to wait\n");
  while (true) {
    std::vector<xtc::Channel*> active_events;
    epoll->Wait(active_events);
    for (auto& ev : active_events) { // TODO 新连接用回调函数注册，不写在main中
      if (ev -> GetFd() == socket -> GetFd()) {
        xtc::InetAddress addr;
        int32_t client_fd = socket->accept(addr); 
        auto& client_addr = addr.GetAddr();
        printf("new client fd %d! IP: %s Port: %d\n", client_fd, 
            inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        xtc::Channel* client_channel = new xtc::Channel(epoll, client_fd);
        client_channel -> EnableETReading();
      } else if (ev -> GetActiveEvents() & EPOLLIN) { //客户端发生可读事件
        // TODO 读取任务封装
          on_recv_msg(ev);
      } else {
        // printf("events %d TODO\n", ev.events); //TODO 其他事件的处理
      }
    }
  }

  return 0;
}
