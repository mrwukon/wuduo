#include <stdio.h>
#include <netinet/tcp.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include<sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "Socket.h"
using namespace wu;

void Socket::bind(int port){
  struct sockaddr_in servaddr;    // 服务端地址信息的数据结构。
  memset(&servaddr,0,sizeof(servaddr));
  servaddr.sin_family = AF_INET;  // 协议族，在socket编程中只能是AF_INET。
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);          // 任意ip地址。
  //servaddr.sin_addr.s_addr = inet_addr("192.168.190.134"); // 指定ip地址。
  servaddr.sin_port = htons(port);  // 指定通信端口。
  ::bind(_sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
}
void Socket::listen(){
    ::listen(_sockfd,15);
}
int Socket::accept(){
    int  clientfd;                  // 客户端的socket。
    int  socklen=sizeof(struct sockaddr_in); // struct sockaddr_in的大小
    struct sockaddr_in clientaddr;  // 客户端的地址信息。
    clientfd=::accept(_sockfd,(struct sockaddr *)&clientaddr,(socklen_t*)&socklen);
    return clientfd;
}
void Socket::setTcpNoDelay(bool on)
{
  int optval = on ? 1 : 0;
  ::setsockopt(_sockfd, IPPROTO_TCP, TCP_NODELAY,
               &optval, static_cast<socklen_t>(sizeof optval));
  // FIXME CHECK
}

void Socket::setReuseAddr(bool on)
{
  int optval = on ? 1 : 0;
  ::setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR,
               &optval, static_cast<socklen_t>(sizeof optval));
  // FIXME CHECK
}

void Socket::setReusePort(bool on)
{

  int optval = on ? 1 : 0;
  int ret = ::setsockopt(_sockfd, SOL_SOCKET, SO_REUSEPORT,
                         &optval, static_cast<socklen_t>(sizeof optval));
 
}
