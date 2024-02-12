#include "Channel.h"
#include "Socket.h"
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
namespace wu{
    class Reactor;
    //class InetAddress;
    class Acceptor{
        public:
        typedef std::function<void(int sockfd,struct sockaddr_in *peerAddress)>NewConnectionCallback;
        Acceptor(Reactor* reactor,int listenfd);
        void setNewConnectionCallback(const NewConnectionCallback &cb){
            _newConnectionCallback=cb;
        }
        int getlistenfd(){return _listenfd;}
        //bool listenning const{return _listenning;}
        void listen();
        private:
        void handleRead();
        Reactor * _reactor;
        int  _listenfd;
        Channel _acceptChannel;
        NewConnectionCallback _newConnectionCallback;
        //bool _listenning;
        //struct sockaddr_in _listenAddr;
    };
}