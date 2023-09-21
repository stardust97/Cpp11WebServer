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

#define MAX_EVENTS 1024
#define MAX_BUF_SIZE 1024


void set_fd_no_blocked(int32_t fd) {
  int flags = fcntl(fd,F_GETFL,0);
  flags |= O_NONBLOCK;
  fcntl(fd,F_SETFL,flags);
}


int main() {
  xtc::Socket* socket = new xtc::Socket();
  xtc::InetAddress addr("127.0.0.1", 8888);
  socket->bind(addr);
  socket->listen();

  xtc::Epoll *epoll = new xtc::Epoll();
  epoll->AddToEpoll(socket->GetFd(), EPOLLIN); //负责监听的文件描述符设置为LT，阻塞

  while (true) {
    printf("start to wait\n");

    std::vector<epoll_event> active_events(MAX_EVENTS);
    epoll->Wait(active_events);
    for (auto& ev:active_events) {
      if (ev.data.fd == socket->GetFd()) {
        xtc::InetAddress addr;
        int32_t client_fd = socket->accept(addr); //API不够优雅
        auto& client_addr = addr.Getaddr();
        printf("new client fd %d! IP: %s Port: %d\n", client_fd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        set_fd_no_blocked(client_fd);
        epoll->AddToEpoll(client_fd, EPOLLIN | EPOLLET);
      } else if (ev.events & EPOLLIN) {
        // TODO 读取任务封装
        printf("client has new msg\n");
        char buf[MAX_BUF_SIZE];
        int32_t client_fd = ev.data.fd;
        
        // TODO 封装fd为channel，记录其IP 端口等信息，此处posix接口无法获取IP 端口等信息。
        while (true) { // ET模式需要一次性读出所有数据
          bzero(buf, MAX_BUF_SIZE);
          int32_t read_bytes = read(client_fd, buf, MAX_BUF_SIZE);
          if(read_bytes > 0) {
            printf("client fd: %d, recv msg: %s\n", client_fd, buf);
          } else if(read_bytes == 0){  //EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", client_fd);
            close(client_fd);
            epoll->RemoveFromEpoll(client_fd);
            break;
           } else if(read_bytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示本次数据全部读取完毕
            printf("client %d read finished!\n", client_fd);
            break;
          } else if(read_bytes == -1 && errno == EINTR){  //客户端正常中断、继续读取
            printf("need next read\n");
            break;
          } 
        }
      } else {
        // printf("events %d TODO\n", ev.events); //TODO 其他事件的处理
      }
    }
  }

  return 0;
}
