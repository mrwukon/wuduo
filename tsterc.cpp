#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include<sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include<signal.h> 
#include"Socket.h"
#include"Reactor.h"
#include"Channel.h"
#include <functional>
//using namespace wu;
int _listenfd=0;
void handleRead(){
   struct sockaddr_in clientaddr;  // 客户端的地址信息。
   int  socklen=sizeof(struct sockaddr_in); // struct sockaddr_in的大小
   int connfd=accept(_listenfd,(struct sockaddr *)&clientaddr,(socklen_t*)&socklen);
   assert(connfd>=0);
   printf("客户端（%s）,%d已连接。\n",inet_ntoa(clientaddr.sin_addr),connfd);

}
int main(){
    int listenfd=socket(AF_INET,SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,IPPROTO_TCP);
    _listenfd=listenfd;
    wu::Socket mysock(listenfd);
    mysock.bind(5005);
    mysock.setReuseAddr(1);
    mysock.setReusePort(1);
    mysock.setTcpNoDelay(1);

    mysock.listen();
    wu::Reactor reactor;
    wu::Channel listenchannel(&reactor,mysock.getfd());
    listenchannel.enableReading();
    std::function<void()> f =handleRead;
    listenchannel.setReadCallback(f);
    reactor.loop();
    return 0;
}