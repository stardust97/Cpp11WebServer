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
#include <sys/socket.h>
#include <fcntl.h>

#include "util/error_process.h"

#define MAX_EVENTS 1024
#define MAX_BUF_SIZE 1024

void read_client_message(int fd, void *buf, size_t nbytes) {
  auto read_bytes = read(fd, buf, nbytes);
  if(read_bytes > 0 ){
    printf("recv msg: %s\n", (char*) buf); // FIXME printf非线程安全
  } else if (read_bytes == 0) {
    printf("client socket disconnected!\n");
  } else {
    xtc::errif(true, "read error!\n");
  }
}

void add_fd_to_epoll(int epoll_fd, int fd, bool ET) {
  epoll_event ev;
  ev.events = EPOLLIN ;
  if (ET) {
    ev.events |= EPOLLET;
    int flags = fcntl(fd,F_GETFL,0);;    //ET需要搭配非阻塞式socket使用
    fcntl(fd,F_SETFL,flags);
  }
  ev.data.fd = fd;
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev);    //添加事件到epoll
}

int main() {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  xtc::errif(sockfd == -1,"create socket error!\n");

  struct sockaddr_in  serv_addr;
  bzero(&serv_addr,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET; // IPV4
  serv_addr.sin_port = htons(8888);
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  int bind_res = bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  xtc::errif(bind_res == -1,"bind error!\n");

  int listen_res = listen(sockfd, SOMAXCONN);
  xtc::errif(listen_res == -1,"listen error!\n");

  // 将socketfd加入epoll，设置为LT，监听其可读事件
  struct epoll_event ev;
  ev.data.fd = sockfd;
  ev.events = EPOLLIN;  
  int epoll_fd = epoll_create1(0);
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sockfd, &ev);

  struct sockaddr_in client_addr;
  bzero(&client_addr, sizeof(client_addr));
  socklen_t client_len;
  char buf[MAX_BUF_SIZE];
  struct epoll_event events[MAX_EVENTS];

  while(true) {
    printf("start to wait\n");
    int nfds =  epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    for (int i = 0; i < nfds; i++) {
      if (events[i].data.fd == sockfd) {
        int client_fd = accept(sockfd, (struct sockaddr*) &client_addr, &client_len);
        xtc::errif(client_fd == -1, "accept error!\n");
        printf("new client fd %d! IP: %s Port: %d\n", client_fd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        add_fd_to_epoll(epoll_fd, client_fd, true);

      } else if (events[i].events & EPOLLIN) {
        int client_fd = events[i].data.fd;
        bzero(buf,sizeof(buf));
        printf(" client msg : fd %d! IP: %s Port: %d\n", client_fd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        read_client_message(client_fd, buf, sizeof(buf));      
      }
    }
  }
  return 0;
}
