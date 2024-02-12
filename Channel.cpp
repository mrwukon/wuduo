#include "Channel.h"
#include "Reactor.h"
#include "Epoll.h"
namespace wu{
    wu::Channel::Channel(Reactor* reactor,int fd)
    :_ownerreactor(reactor),
     _fd(fd),
     _events(0),
     _revents(0),
     _index(KNew)
     {
         _tied=false;
     }
     wu::Channel::~Channel(){
         int y=1;
     }
     void wu::Channel::update(){
        _ownerreactor->updateChannel(this);
     }
    
     void wu::Channel::tie(const std::shared_ptr<void>&obj){
      _tie=obj;
      _tied=true;
     }
     void wu::Channel::handleEvent(){
       std::shared_ptr<void> guard;
       if(_tied){
         guard=_tie.lock();
         if(guard){
            handleEventWithGuard();
         }
       }
        else{
         handleEventWithGuard();
        }
     }
     void wu::Channel::handleEventWithGuard(){
         //_eventHandling=true;
         if(_revents&(EPOLLIN|EPOLLPRI)){
            if(_readCallback){
               _readCallback();
            }
         }
         if(_revents&(EPOLLOUT)){
            _writeCallback();
         }
         //_eventHandling=false;
     }
}