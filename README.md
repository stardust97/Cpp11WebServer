# Cpp11WebServer

23.9.27： //TODO 解决Epoll与Channel的循环包含
23.10.9:  解决BUG：accept第三个参数需要初始化，否则会返回-1
23.10.11: 增加Server、EventLoop，引入Reactor模式，进一步抽象模型。更改Channel旧接口
