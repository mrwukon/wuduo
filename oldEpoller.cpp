#include"Epoll.h"
#include "Channel.h"
#include <assert.h>
#include <errno.h>
#include<map>
#include <sys/epoll.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>

#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <arpa/inet.h>
namespace wu{
    wu::Epoller::Epoller(Reactor *reactor)
    :_ownnerReactor(reactor),
    _epollfd(::epoll_create(1)),
    _events(16){

    }
    wu::Epoller::~Epoller(){
        ::close(_epollfd);
    }
    void wu::Epoller::updateChannel(Channel*channel){
        //assert(isinReactorThread());
        int index=channel->_index;
        if(index==KNew||index==KDeleted){
            int fd=channel->getfd();
            if(index==KNew){
                assert(_mapChannels.find(fd)==_mapChannels.end());
                _mapChannels[fd]=channel;

            }
            else{
                assert(_mapChannels.find(fd)!=_mapChannels.end());
                assert(_mapChannels[fd]==channel);
            }
            channel->setIndex(KAdded);
            update(EPOLL_CTL_ADD,channel);
        }
        else{
            int fd=channel->getfd();
            assert(_mapChannels.find(fd)!=_mapChannels.end());
            assert(_mapChannels[fd]==channel);
            assert(index==KAdded);
            if(channel->ChannelisNonevent()){
                update(EPOLL_CTL_DEL,channel);
                channel->setIndex(KDeleted);
            }
            else{
                update(EPOLL_CTL_MOD,channel);
            }
        }
    }
    void wu::Epoller::fillActivateChannel(int numEvents,Channellist*activateChannel)const{
        //assert(numEvents<=_events.size());
        for(int i=0;i<numEvents;i++){
            Channel* channel=static_cast<Channel*>(_events[i].data.ptr);
            int fd=channel->getfd();

            //printf("fd is %d\n");
            auto it=_mapChannels.find(fd);
            assert(it!=_mapChannels.end());
            assert(it->second==channel);
            printf("fd is %d\n",it->second->getfd());
            channel->setrevents(_events[i].events);
            activateChannel->push_back(channel);
        }
    }
    void wu::Epoller::epollwait(Channellist* activateChannel ){
        int numEvents=::epoll_wait(_epollfd,
                                   &*_events.begin(),
                                   (int)_events.size(),
                                   -1);
        int savedErrno=errno;
        //printf("active fd is %d",)
        if(numEvents>0){
            fillActivateChannel(numEvents,activateChannel);   
        }
        else if(numEvents==0){

        }
        else{
            if(savedErrno!=EINTR){
                //
            }
        }
        
    }    
    void wu::Epoller::update(int opration,Channel* channel){
        struct epoll_event newevent;
        bzero(&newevent,sizeof(newevent));
        newevent.events=channel->getevent();
        newevent.data.ptr=channel;
        int fd=channel->getfd();
        printf("EpollerupdateChannel fd is %d",channel->getfd());
        ::epoll_ctl(_epollfd,opration,fd,&newevent);
    }
}