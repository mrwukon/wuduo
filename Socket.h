#pragma once
namespace wu{
    class Socket{
        public:
        explicit Socket(int sockfd)
        : _sockfd(sockfd)
        { }
        int getfd() const { return _sockfd; }
        void listen();
        void bind(int port);
        int accept();
        void setTcpNoDelay(bool on);
        void setReuseAddr(bool on);
        void setReusePort(bool on);
        private:
        const int _sockfd;
    };
}