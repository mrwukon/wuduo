CC=g++
CFLAGS=-g
all:Channel.o Reactor.o Epoll.o Socket.o  Acceptor.o TcpConnection.o TcpServer.o tstcsrtv.o
	$(CC) Channel.o Reactor.o Epoll.o Socket.o  Acceptor.o  TcpConnection.o TcpServer.o tstcsrtv.o -o test8
Channel.o:Channel.cpp 
	$(CC) $(CFLAGS) -c Channel.cpp
Reactor.o:Reactor.cpp
	$(CC) $(CFLAGS) -c Reactor.cpp
Epoll.o:Epoll.cpp
	$(CC) $(CFLAGS) -c Epoll.cpp
Socket.o:Socket.cpp
	$(CC) $(CFLAGS) -c Socket.cpp
Acceptor.o:Acceptor.cpp
	$(CC) $(CFLAGS) -c Acceptor.cpp
TcpConnection.o:TcpConnection.cpp
	$(CC) $(CFLAGS) -c TcpConnection.cpp
TcpServer.o:TcpServer.cpp
	$(CC) $(CFLAGS) -c TcpServer.cpp
tstcsrtv.o:tstcsrtv.cpp
	$(CC) $(CFLAGS) -c tstcsrtv.cpp