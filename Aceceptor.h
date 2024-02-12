#pragma once
#include"Channel.h"
#include"Socket.h"
#include <functional>
#include <memory>
namespace wu{
    class Reactor;
    class Acceptor{
        public:
        typedef std::function<void (int sockfd)> NewConnectionCallback;
        Acceptor(Reactor* reactor,int listenfd);
         ~Acceptor();
        void setNewConnectionCallback(const NewConnectionCallback& cb)
        { _newConnectionCallback = cb; }

    //bool listenning() const { return listenning_; }
        void listen();
        private:
        void handleRead();
        Reactor * _reactor;
        Socket _acceptSocket;
        Channel _acceptChannel;
        NewConnectionCallback _newConnectionCallback;
    };
}