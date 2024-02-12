#include <stdio.h>
#include <pthread.h>
#include "Reactor.h"
using namespace wu;
void* threadFunc(void* data){
    pthread_detach(pthread_self());
    printf("threadFunc():tid=%d\n",pthread_self());
    Reactor  reactor;
    reactor.loop();
    return NULL;
}
int main(){
    printf("main thread tid=%d\n",pthread_self());
    pthread_t pthid1=0;
    int clientfd=0;
    Reactor reactor;
    pthread_create(&pthid1,NULL,threadFunc,(void*)(long)clientfd);
    reactor.loop();

}