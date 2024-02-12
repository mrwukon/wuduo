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
#include"Acceptor.h"
#include"TcpServer.h"
#include"TcpConnection.h"
#include <functional>
typedef std::shared_ptr<wu::TcpConnection>TcpConnectionPtr;
void onconnection(const TcpConnectionPtr& conn){
    printf("connection is established\n");
}
void onMessage(const TcpConnectionPtr& conn,
               const char *data,
               int len){
    printf("onMessage(): received data\n");
}
int main(){
    wu::Reactor reactor;
    int o;
    wu::TcpServer server(&reactor,5005);
    std::function<void (const TcpConnectionPtr&)> cb1=onconnection;
    std::function<void (const TcpConnectionPtr&,const char *data,int len)> cb2=onMessage;
    server.setConnectionCallback(cb1);
    server.setMessageCallback(cb2);
    server.start();
}