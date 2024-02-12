#include"TcpConnection.h"
#include"Socket.h"
#include"Reactor.h"
#include"Channel.h"
#include<fcntl.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
    wu::TcpConnection::TcpConnection(Reactor* reactor,
                                     int sockfd,
                                     struct sockaddr_in *clientaddr)
        :_reactor(reactor),
        _state(KConnecting),
        _socket(sockfd),
        _channel(new Channel(reactor,sockfd)),
        _clientaddr(clientaddr)
    {
        _channel->setReadCallback(
            std::bind(&TcpConnection::handleRead,this));
    }
    void wu::TcpConnection::connectEstablished()
    {
        //assert(_reactor->isInReactorThread);
        setState(kConnected);
        //_channel->tie(shared_from_this());
        _channel->enableReading();
        _connectioncallback(shared_from_this());
    }
    void wu::TcpConnection::handleRead()
    {
        //assert(_reactor->isInReactorThread);
        int saveErrno=0;
        
        char buf[65536];
        memset(buf,0,sizeof(buf));
        int n=::read(_channel->getfd(),buf,10);
        printf("%d,%s\n",n,buf);
        _messagecallback(shared_from_this(),buf,n);
        n=::read(_channel->getfd(),buf,10);
        printf("%d\n",n);
    }

