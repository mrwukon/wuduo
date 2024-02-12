#include <set>
#include<memory>
#pragma once
#include"TcpConnection.h"
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
namespace wu{
    class Acceptor;
    class Reactor;
    class TcpServer
    {
        public:
        //typedef std::function<void(Reactor* reactor)> ThreadInitCallback;
        typedef std::shared_ptr<wu::TcpConnection>TcpConnectionPtr;
        typedef std::function<void (const TcpConnectionPtr&)> ConnectionCallback;
        typedef std::function<void (const TcpConnectionPtr&,const char *data,int len)> MessageCallback;
        TcpServer(Reactor* reactor,int port);
        ~TcpServer();
        void start();
        void setConnectionCallback(ConnectionCallback &cb){
            _connectcallback=cb;
        }
        void setMessageCallback(MessageCallback &cb){
            _messagecallback=cb;
        }
      
        private:
        std::set<TcpConnectionPtr> _connset;
        void newConnection(int sockfd,struct sockaddr_in *clientaddr);
        Reactor * _reactor;
        Acceptor * _acceptor;
        //ReactorThreadPool _threadpool;
        ConnectionCallback _connectcallback;
        MessageCallback _messagecallback;
        //WriteCompleteCallback _writecompletecallback;
        //ThreadInitCallback _threadinitcallback;   
    };
}