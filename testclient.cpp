#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#pragma pack (1)
enum CMD{
    CMD_LOGIN=1,
    CMD_LOGOUT,
    CMD_ERROR
};
struct Dataheader{
  long length;
  int cmd;
};
struct loginmsg:public Dataheader{
  loginmsg(){
    cmd=CMD_LOGIN;
    length=sizeof(loginmsg);
  }
  char name[20];
  long id;
  long password;
};
struct loginresponmsg:public Dataheader{
  loginresponmsg(){
    cmd=CMD_LOGIN;
    length=sizeof(loginresponmsg);
  }
  long succes;
};
struct logoutmsg:public Dataheader{
  logoutmsg(){
    cmd=CMD_LOGOUT;
    length=sizeof(logoutmsg);
  }
  
  long id;
};
struct logoutresponmsg:public Dataheader{
  logoutresponmsg(){
    cmd=CMD_LOGOUT;
    length=sizeof(logoutresponmsg);
  }
  long succes;
};
void Send(long clientfd,char* buf,int size,int flag){
    int iret;
    //struct Dataheader varheader;
    //memset(buffer,0,sizeof(buffer));
    if ( (iret=send(clientfd,(char *)buf,size,flag))>=0) // 接收客户端的请求报文。
    {
       printf("iret=%d\n",iret); 
    }
}
void Recv(long clientfd,char * buf,int size,int flag){
    int iret;
    //struct Dataheader varheader;
    //memset(buffer,0,sizeof(buffer));
    if ( (iret=recv(clientfd,( char *)buf,size,flag))<=0) // 接收客户端的请求报文。
    {
       printf("iret=%d\n",iret);
    }
} 
int main(int argc,char *argv[])
{
  if (argc!=3)
  {
    printf("Using:./client ip port\nExample:./client 127.0.0.1 5005\n\n"); return -1;
  }
  if(sizeof(argv[1])>10||sizeof(argv[2])>10){
    printf("uid or password too long\n");
    return 1;
  }
  
  int sockfd;
  if ( (sockfd = socket(AF_INET,SOCK_STREAM,0))==-1) { perror("socket"); return -1; }
 
  
  struct hostent* h;
  if ( (h = gethostbyname(argv[1])) == 0 )   // 指
  { printf("gethostbyname failed.\n"); close(sockfd); return -1; }
  struct sockaddr_in servaddr;
  memset(&servaddr,0,sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(atoi(argv[2])); // 指定服务端的通信端口。
  memcpy(&servaddr.sin_addr,h->h_addr,h->h_length);
  if (connect(sockfd, (struct sockaddr *)&servaddr,sizeof(servaddr)) != 0)  // 向服务端发起连接清求。
  { perror("connect"); close(sockfd); return -1; }
 
  char buffer[1024];
  int echo=1;
  int j=1;
  while(j){
    scanf("%s",buffer);
    if(echo){
      Send(sockfd,buffer,strlen(buffer),0);
      j--;
      continue;
    }
    if(strcmp("login",buffer)==0){
      struct loginmsg varloginmsg;
      strcpy(varloginmsg.name,"alice");
      varloginmsg.id=1001;
      varloginmsg.password=1233;
      for(int i=1;i<=100;i++){
        varloginmsg.id=i;
        Send(sockfd,(char*)&varloginmsg,sizeof(varloginmsg),0);
      }
      struct loginresponmsg varloginresponmsg;
      //Recv(sockfd,(char*)&varloginresponmsg,sizeof(varloginresponmsg),0);
      printf("%d\n",varloginresponmsg.succes);

    }
    if(strcmp("logout",buffer)==0){
      struct logoutmsg varlogoutmsg;
      varlogoutmsg.id=19999;
      Send(sockfd,(char*)&varlogoutmsg,sizeof(varlogoutmsg),0);
      struct logoutresponmsg varlogoutresponmsg;

      //Recv(sockfd,(char*)&varlogoutresponmsg,sizeof(varlogoutresponmsg),0);
      printf("%d\n",varlogoutresponmsg.succes);

    }
    
  }
  //
  
  // 第4步：关闭socket，释放资源。
  close(sockfd);
}
