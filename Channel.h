//#include <boost/function.hpp>

//#include <boost/noncopyable.hpp>
//#include <boost/shared_ptr.hpp>
//#include <bo/weak_ptr.hpp>
#pragma once
#include <functional>
#include <memory>
#include <sys/epoll.h>
#define kreadevent EPOLLIN|EPOLLPRI
#define kwriteevent EPOLLIOUT
#define kerrorevent EPOLLIERR
#define knonevent 0
#define KNew 0
#define KDeleted 1
#define KAdded 2
namespace wu{
    class Reactor;
    
    class Channel{
    public:
    int _index;
    typedef std::function<void()> EventCallback();
    Channel(Reactor* reactor,int fd);
    ~Channel();
    void tie(const std::shared_ptr<void>&obj);
    void handleEventWithGuard();
    void setIndex(int nindex){_index=nindex;}
    void handleEvent();
    int getevent() const{return _events;}
    void setrevents(int revt){_revents=revt;}
    int getrevents()const{return _revents;}
    void setReadCallback(const std::function<void()> &cb){
        _readCallback=cb;
    }
    bool ChannelisNonevent()const{return _events==knonevent;}
    void setWriteCallback(const std::function<void()> &cb){
        _writeCallback=cb;
    }
    void setErrorCallback(const std::function<void()> &cb){
        _errorCallback=cb;
    }
    int getfd()const{return _fd;}

    void enableReading(){_events|=kreadevent;update();}
    Reactor* getownerReactor()const{return _ownerreactor;}
    private:
    void update();
    std::weak_ptr<void>_tie;
    int _tied;
    std::function<void()>  _readCallback;
    std::function<void()> _writeCallback;
    std::function<void()> _errorCallback;
    EventCallback _closeCallback;
    int _fd;
    Reactor* _ownerreactor;
    int _events;
    int _revents;
    int _epollindex;
};
}