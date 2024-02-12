#include "Reactor.h"
#include <stdio.h>
#include "Channel.h"
#include <signal.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include "Epoll.h"
namespace wu{
    //using namespace wu;
    __thread Reactor* _reactorInThisThread=0;
    wu::Reactor::Reactor():_looping(false),_threadId(pthread_self()){
        if(_reactorInThisThread){
            printf("reactor exist error\n");
        }
        else{
            _reactorInThisThread=this;
        }
        _epoller=new Epoller(this);
    }
    wu::Reactor::~Reactor(){
        assert(!_looping);
        _reactorInThisThread=NULL;
    }
    void wu::Reactor::loop(){
        //printf("reactor thread tid=%d\n",pthread_self());
        //fflush(stdout);
        assert(!_looping);
        assert(isInReactorThread());
        _looping=true;
        _quit=7;
        
        while(_quit>0){
            printf("loop %d\n",_quit);
            _activateChannels.clear();
            _epoller->epollwait(&_activateChannels);
            //printf("epollsize()%d\n",_activateChannels.size());
            for(auto it:_activateChannels){
                it->handleEvent();
            }
            _quit=_quit-1;;
            //doPendingFunctors();
        }
        _looping=false;
    }
    Reactor* wu::Reactor::getEventReactorInThisThread(){
        return _reactorInThisThread;
    }
    /*void wu::Reactor::runInLoop(Functor cb){
        if(isInReactorThread()){
            cb();
        }
        else{
            queueInLoop(cb);
        }
    }
    /*void wu::Reactor::queueInLoop(Functor cb){
        _FunctorListmutex.lock();
        _pendingFunctors.push_back(cb);
        _FunctorListmutex.unlock();
        if((!isInReactorThread())||_callingPendingFunctors){
            wakeup();
        }
    }*/
    /*void wu::Reactor::wakeup(){
        uint64_t one=1;
        int n=wu::sockets::write(_wakeupfd,&one,sizeof(one));
    }
    void wu::Reactor::handleRead(){
        uint64_t one =1;
        int n=wu::sockets::read(_wakeupfd,&one,sizeof(one));;
    }*/
    void wu::Reactor::updateChannel(Channel*  channel){
        assert(channel->getownerReactor()==this);
        //assert(isinReactorThread());
        printf("ReactorudateChannel fd is %d",channel->getfd());
        _epoller->updateChannel(channel);
    }
    /*void wu::Reactor::doPendingFunctors(){
        std::vector<Functor>functors;
        _callingPendingFunctors=true;
        {
            _FunctorListmutex.lock();
            functors.swap(_pendingFunctors);
        }
        for(int i=0;i<functors.size();i++){
            functors[i]();
        }
        _callingPendingFunctors=false;
    }*/
}
