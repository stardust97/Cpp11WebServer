/*
 * @Author: stardust97 641765169@qq.comls
 * @Date: 2023-09-18 06:59:24
 * @LastEditors: stardust97 641765169@qq.comls
 * @LastEditTime: 2023-09-18 09:08:21
 * @FilePath: /30daywebserver/Cpp11WebServer/service.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */


#include <cstring>
#include <cstdio>
#include <cstdlib>

#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include "util/error_process.h"

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

  struct sockaddr_in client_addr;
  bzero(&client_addr, sizeof(client_addr));
  socklen_t client_len;
  int client_fd = accept(sockfd, (struct sockaddr*) &client_addr, &client_len);
  xtc::errif(client_fd == -1, "accept error!\n");
  printf("new client fd %d! IP: %s Port: %d\n", client_fd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
  
  char buf[1024];
  while(true) {
    bzero(buf,sizeof(buf));
    auto read_bytes = read(client_fd, buf,sizeof(buf));
    if(read_bytes > 0 ){
     printf("recv msg: %s\n", buf);
    } else if (read_bytes == 0) {
      printf("client socket disconnected!\n");
      break;
    } else {
      xtc::errif(true, "read error!\n");
      break;
    }
  }
  return 0;
}
