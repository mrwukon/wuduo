#include <stdio.h>
#include <pthread.h>
#include "Reactor.h"
#include <unistd.h>
using namespace wu;
Reactor* reactor;
void* threadFunc(void * data){
    reactor->loop();
}
int main(){
    printf("main thread tid=%d\n",pthread_self());
    Reactor y;
    reactor=&y;
    pthread_t pthid1=0;
    int clientfd=0;
    pthread_create(&pthid1,NULL,threadFunc,(void*)(long)clientfd);
    sleep(2);
}