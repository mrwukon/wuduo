#include<memory>
#include<functional>
#include "TcpServer.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include<sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include"Socket.h"
#include"Reactor.h"
#include"Channel.h"
#include"Acceptor.h"

    
wu::TcpServer::TcpServer(Reactor* reactor,int port)
    :_reactor(reactor){
        int listenfd=socket(AF_INET,SOCK_STREAM| SOCK_CLOEXEC,IPPROTO_TCP);
    //_listenfd=listenfd;
        wu::Socket mysock(listenfd);
        mysock.bind(port);
        mysock.setReuseAddr(1);
        mysock.setReusePort(1);
        mysock.setTcpNoDelay(1);
        int fd=mysock.getfd();
        printf("socket fd is%d\n",fd);
        _acceptor=new wu::Acceptor(reactor,fd);
         //std::function<void (int sockfd,struct sockaddr_in *clientaddr)> cb1=
        _acceptor->setNewConnectionCallback(std::bind(&wu::TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2));
    }
wu::TcpServer::~TcpServer(){
        int lo;   
    }
void wu::TcpServer::start(){
       // _threadpool->start(_threadinitcallback);
        //assert(!_acceptor->listening());
        //_reactor->runInLoop(std::bind(&wu::Acceptor::listen,_acceptor));
        _acceptor->listen();
        _reactor->loop();
    }
    
void wu::TcpServer::newConnection(int sockfd,struct sockaddr_in *clientaddr){
        //assert(_reactor->isInReactorThread());
        //reactor* ioreactor=_threadpool->getNextReactor();
        Reactor* ioreactor=_reactor;
        //InetAddress localAddr(wu::sockets::getLocalAddr(socketfd));
        TcpConnectionPtr conn(new TcpConnection(ioreactor,sockfd,clientaddr));
        conn->setConnectionCallback(_connectcallback);
        conn->setMessageCallback(_messagecallback);
        //conn->setWriteCompleteCallback(_writecompletecallback);
        //close cb

        //ioreactor->runInLoop(std::bind(&wu::TcpConnection::connectEstablished,conn));
        conn->connectEstablished();
        _connset.insert(conn);
    }
