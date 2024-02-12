#include <functional>
//#include <pthread.h>
#include <assert.h>
//#include "Mutex.h"
#include <memory>

#pragma once

namespace wu{
    class Channel;
    class Epoller;

    class Reactor{
        public:
        typedef std::function<void()> Functor;
        Reactor();
        ~Reactor();
        void loop();
        void runInLoop(const Functor &cb);
        void queueInLoop(const Functor &cb);
        void wakeup();
        void updateChannel(Channel* channel);
        void removeChannel(Channel * channel);
        bool hasChannel(Channel* channel);
        //bool callingPendingFunctors()const{return _callingPendingFunctors;}

        bool isInReactorThread() const{return _threadId==pthread_self();}
        private:
        const pthread_t _threadId;
        bool _looping;
        int _quit;
        int _wakeupfd;
        Reactor* getEventReactorInThisThread();
        void handleRead();
        void doPendingFunctors();
        typedef std::vector<Channel*>ChannelList;
        ChannelList _activateChannels;
        Channel* _currentActiveChannel;
        //Mutex _FunctorListmutex;
        Epoller* _epoller;
        std::vector<Functor>_pendingFunctors;
    };
    
}