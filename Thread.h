

namespace wu{
    class Thread{
        public:
        typedef std::function<void()> ThreadFunc;
        Thread(ThreadFunc);
        ~Thread();
        void start();
        int join();
        bool started() const {return _started;}
        pthread_t pthreadId() const{return _pthreadId;}
        pid_t tid() const {return _tid;}
        private:
        bool _started;
        bool _joined;
        pthread_t _pthreadId;
        pid_t tid;
        ThreadFunc _func;
        
    }
}