#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
// socket(), accept(), listen(), send(), recv(), bind(), connect(), inet_addr(), setsockopt(), getsockname()
#include <sys/socket.h>
// struct sockaddr_in
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

class Server
{
	public:
		Server(void);
		Server(unsigned int port);
		~Server(void);
	private:
		int					servSock_;
		int					clntSock_;
		struct sockaddr_in	servAddr_;
		struct sockaddr_in	clntAddr_;
		socklen_t			clntAddrSize;
		//std::string			reqData_;
};

#endif
