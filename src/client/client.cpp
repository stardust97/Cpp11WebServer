/*
 * @Author: stardust97 641765169@qq.comls
 * @Date: 2023-09-18 07:01:38
 * @LastEditors: stardust97 641765169@qq.comls
 * @LastEditTime: 2023-09-19 08:42:02
 * @FilePath: /30daywebserver/Cpp11WebServer/client.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

//C++ 标准库
#include <thread>
#include <chrono>
// C标准库
#include <cstring>
// 系统库
#include <arpa/inet.h>  //这个头文件包含了<netinet/in.h>，不用再次包含了
#include <unistd.h>
// 其他
#include "util/error_process.h"


#include <cstdio>


int main(){
  int socketfd = socket(AF_INET, SOCK_STREAM, 0);
  xtc::errif(socketfd == -1,"create socket error!\n");

  struct sockaddr_in serv_addr;
  bzero(&serv_addr,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(8888);
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  int res = connect(socketfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  xtc::errif(socketfd == -1,"connect error!\n");

  char buf[1024];
  bzero(buf,sizeof(buf));
  for(int i = 0; i < 200; ++i){
    snprintf(buf,sizeof(buf), "hello id: %d", i);
    int write_bytes = write(socketfd, buf,sizeof(buf));
    if(write_bytes > 0) {
      printf("message send succeed : %s\n", buf);
    } else if(write_bytes == 0){      //read返回0，表示EOF，通常是服务器断开链接，等会儿进行测试
      printf("server socket disconnected!\n");
      break;
    } else {
      xtc::errif(true, "client send error");
    }
     std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }
  close(socketfd);
  return 0;
}