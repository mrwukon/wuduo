#pragma once
#include <memory>
#include"Socket.h"
#include <functional>
namespace wu{

    class Channel;
    class Reactor;
    class TcpConnection:public std::enable_shared_from_this<TcpConnection>
    {
        public:
        typedef std::shared_ptr<wu::TcpConnection>TcpConnectionPtr;
        typedef std::function<void (const TcpConnectionPtr&)> ConnectionCallback;
        typedef std::function<void (const TcpConnectionPtr&,const char *data,int len)> MessageCallback;
        enum StateE{KConnecting,kConnected,};
        TcpConnection(Reactor* reactor,int sockfd,struct sockaddr_in *clientaddr);
        void handleRead();
        //void handleWrite();
        //void handleClose();
        //void handleError();
        void connectEstablished();
        //void sendInLoop(const char* message,int len);
        void setState(StateE s){_state=s;}
        //void startReadInLoop();
        Reactor* _reactor;
        StateE _state;
        ConnectionCallback _connectioncallback;
        MessageCallback _messagecallback;
        void setConnectionCallback(const ConnectionCallback& cb)
        { _connectioncallback = cb; }

        void setMessageCallback(const MessageCallback& cb)
        { _messagecallback = cb; }
    //bool _reading;
        Socket _socket;
        Channel* _channel;
        //InetAddress _localAddr;
        struct sockaddr_in *_clientaddr;

    };
    
}