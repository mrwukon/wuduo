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
#include "Acceptor.h"
#include "Reactor.h"
wu::Acceptor::Acceptor(Reactor* reactor,int listenfd)
    :_reactor(reactor),
     _listenfd(listenfd),
     _acceptChannel(reactor,_listenfd)
     //_listenAddr(listenAddr)
     //listenning(false),
     {
        //_acceptSocket.setReuseAddr(true);
        //_acceptSocket.setReusePort(true);
        //_acceptSocket.bindAddress(listenAddr);
        //bind(_listenfd,(struct sockaddr *)&_listenAddr,sizeof(_listenAddr))
        
        _acceptChannel.setReadCallback(std::bind(&Acceptor::handleRead,this));
     }
void wu::Acceptor::listen(){
    assert(_reactor->isInReactorThread());
    //_listenning=true;
    //_acceptSocket->listen();
    ::listen(_listenfd,55);
    _acceptChannel.enableReading();
    printf("_acceptChannel fd is %d",_acceptChannel.getfd());
}
void wu::Acceptor::handleRead(){
   assert(_reactor->isInReactorThread());
   //InetAddress peerAddr(0);
   //int connfd=_acceptSocket.accept(&peerAddr);
   int  socklen=sizeof(struct sockaddr_in); // struct sockaddr_in的大小
   struct sockaddr_in *iclientaddr=(struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));  // 客户端的地址信息。
   int connfd=::accept(_listenfd,(struct sockaddr *)iclientaddr,(socklen_t*)&socklen);
   assert(connfd>=0);
   printf("客户端（%s）,%d已连接。\n",inet_ntoa(iclientaddr->sin_addr),connfd);
   _newConnectionCallback(connfd,iclientaddr);
   /*if(_newConnectionCallback){
      _newConnectionCallback(connfd,clientaddr);
   }
   else{
      //sockets::close(connfd);
   }*/
}