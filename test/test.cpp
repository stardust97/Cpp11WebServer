#include <thread>
#include <chrono>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdio>
#include <functional>

#include "util/error_process.h"
#include "base/ThreadPool.h"


void oneClient(int32_t msgs, int32_t wait) {
  int socketfd = socket(AF_INET, SOCK_STREAM, 0);
  xtc::errif(socketfd == -1,"create socket error!\n");
  struct sockaddr_in serv_addr;
  bzero(&serv_addr,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(8888);
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  int res = connect(socketfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  xtc::errif(res == -1,"connect error!\n");
  sleep(wait);
  char buf[1024];
  bzero(buf,sizeof(buf));
  for(int i = 0; i < msgs; ++i){
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
}

int main() {
  int threads = 100;
  int msgs = 100;
  int wait = 0;
  // int o;

  xtc::ThreadPool *poll = new xtc::ThreadPool(threads);
  std::function<void()> func = std::bind(oneClient, msgs, wait);
  for(int i = 0; i < threads; ++i){
      poll->AddTask(func);
  }
  delete poll;
  return 0;
}