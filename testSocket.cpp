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

//using namespace wu;
int main(){
    int listenfd=socket(AF_INET,SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,IPPROTO_TCP);
    wu::Socket mysock(listenfd);
    mysock.bind(5005);
    mysock.setReuseAddr(1);
    mysock.setReusePort(1);
    mysock.setTcpNoDelay(1);

    mysock.listen();
    int epollfd;
    epollfd=epoll_create(1);
    struct epoll_event ev;
    ev.data.fd=listenfd;
    ev.events=EPOLLIN;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,listenfd,&ev);
    int newpipefd[2];
    if (pipe(newpipefd) < 0) {printf("pipe error"); }
    struct epoll_event ev2;
    ev.data.fd=newpipefd[0];
    ev.events=EPOLLIN;
    char buf[12];
    buf[0]='s';
  while(1)
  {
    //fd_set tmpfdset=readfdset;
    //int infds =select(maxfd+1,&tmpfdset,NULL,NULL,NULL);
    struct epoll_event events[100];
    printf("adfds");
    int infds=epoll_wait(epollfd,events,100,-1);
    printf("infds =%d \n",infds);
    if(infds<0)
    {
        perror("select error\n");
        break;
    }
    if(infds==0)
    {
        printf("select timeout");
        continue;
    }
    for(int i=0;i<infds;i++)
    {
   
        if((events[i].data.fd==listenfd)&&(events[i].events&EPOLLIN)){
            int  clientfd;                  // 客户端的socket。
            int  socklen=sizeof(struct sockaddr_in); // struct sockaddr_in的大小
            struct sockaddr_in clientaddr;  // 客户端的地址信息。
            clientfd=mysock.accept();//accept(listenfd,(struct sockaddr *)&clientaddr,(socklen_t*)&socklen);
            if(clientfd<0)
            {
              perror("accept fail\n");
              continue;
            }
            printf("客户端（%s）,%d已连接。\n",inet_ntoa(clientaddr.sin_addr),clientfd);
            //FD_SET(clientfd,&readfdset);
            memset(&ev,0,sizeof(struct epoll_event));
            ev.data.fd=clientfd;
            ev.events=EPOLLIN;
            epoll_ctl(epollfd,EPOLL_CTL_ADD,clientfd,&ev);
            continue;      
        }
        else if(events[i].events&EPOLLIN)
        {
          char buffer[1024];
          memset(buffer,0,sizeof(buffer));
          ssize_t isize=read(events[i].data.fd,buffer,sizeof(buffer));
          if(isize<=0)
          {
            printf("clientfd eventfd =%d disconnect \n",events[i].data.fd);
            close(events[i].data.fd);
            memset(&ev,0,sizeof(struct epoll_event));
            ev.data.fd=events[i].data.fd;
            ev.events=EPOLLIN;
            epoll_ctl(epollfd,EPOLL_CTL_DEL,events[i].data.fd,&ev);
            //FD_CLR(eventfd,&readfdset);
            
            continue;
          }
          else
          {
            printf("接收：%s\n",buffer);
          //  strcpy(buffer,"ok");
            if ( (send(events[i].data.fd,buffer,strlen(buffer),0))<=0) // 向客户端发送响应结果。
            { perror("send"); break; }
             printf("发送：%s\n",buffer); 
          }
        }
    }
  
}
}