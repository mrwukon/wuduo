int sockets::createNonblockingOrDie(){
    int sockfd=::socket(AF_INET,SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,IPPROTO_TCP);
}
void sockets::bindOrDie(int sockfd,const struct *addr)
{
    int ret=::bind(sockfd,addr,(sizeof(struct sockaddr_in));
}