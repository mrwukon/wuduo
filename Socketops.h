namespace wu{
    namespace sockets{
        int createNonblockingOrDie();
        int connect(int sockfd,const struct sockaddr* addr);
        void bindOrDie(int sockfd,const struct sockaddr* addr);
        void listenOrDie(int sockfd);
        int accept(int sockfd,struct sockaddr_in6*addr);
        
    }
}