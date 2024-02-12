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
//#include "Acceptor.h"
int main(int argc,char *argv[])
{
  int pipefd[2];
  int ret=pipe(pipefd);
  signal(SIGCHLD,SIG_IGN);
  if (argc!=2)
  {
    printf("Using:./server port\nExample:./server 5005\n\n"); return -1;
  }
 
  // 第1步：创建服务端的socket。
  int listenfd;
  if ( (listenfd = socket(AF_INET,SOCK_STREAM,0))==-1) { perror("socket"); return -1; }
  int opt=1;unsigned int len=sizeof(opt);
  setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,len);
  setsockopt(listenfd,SOL_SOCKET,SO_KEEPALIVE,&opt,len);
  // 第2步：把服务端用于通信的地址和端口绑定到socket上。
  struct sockaddr_in servaddr;    // 服务端地址信息的数据结构。
  memset(&servaddr,0,sizeof(servaddr));
  servaddr.sin_family = AF_INET;  // 协议族，在socket编程中只能是AF_INET。
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);          // 任意ip地址。
  //servaddr.sin_addr.s_addr = inet_addr("192.168.190.134"); // 指定ip地址。
  servaddr.sin_port = htons(atoi(argv[1]));  // 指定通信端口。
  if (bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) != 0 )
  { perror("bind"); close(listenfd); return -1; }
  
  // 第3步：把socket设置为监听模式。
  if (listen(listenfd,15) != 0 ) { perror("listen"); close(listenfd); return -1; }
  //fd_set readfdset;
  int maxfd=listenfd;
  //FD_ZERO(&readfdset);
  //FD_SET(listenfd,&readfdset);
  // 第4步：接受客户端的连接。
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
  epoll_ctl(epollfd,EPOLL_CTL_ADD,newpipefd[0],&ev2);
  //write(newpipefd[1],buf,sizeof(buf));

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
            clientfd=accept(listenfd,(struct sockaddr *)&clientaddr,(socklen_t*)&socklen);
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
            
          }
        }
    }
  
}
}