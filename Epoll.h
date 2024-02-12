#include "Reactor.h"
#include<map>
#include<vector>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include<sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#pragma once
namespace wu{
    class Channel;
    class Epoller{
        public:
        typedef std::vector<Channel*>Channellist;
        Epoller(Reactor* reactor);
        ~Epoller();
        void updateChannel(Channel* channel);
        void update(int opration,Channel* channel);
        //int isinReactorThread(){return _ownnerReactor->isInReactorThread();}
        void epollwait(Channellist* activateChannel);
        typedef std::map<int,Channel*>ChannelMap;
        ChannelMap _mapChannels;
        int _epollfd;
        private:
        Reactor *_ownnerReactor;
        void fillActivateChannel(int numEvents,Channellist*activateChannel)const;
        
        struct epoll_event eventlist[100];
        //typedef std::vector<struct epoll_event> Eventlist;
        //Eventlist _events;
    };
}