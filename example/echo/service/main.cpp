
#include "echo_service.h"

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
