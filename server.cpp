#include "server.hpp"

Server::Server(void) {}

Server::Server(unsigned int port) : clntAddrSize(sizeof(servAddr_))
{
	servSock_ = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // ipv4, tcp
	if (servSock_ == -1)
	{
		std::cerr << "socket error!" << std::endl;
		exit(1);
	}

	// 강제종료나 비정상 종료가 된 경우에 bind 된 데이터를 재사용 할 수 있도록 한다.
	int on = 0;
	if(setsockopt(servSock_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
	{
		printf("socket option set error\n");
		exit(1);
	}

	servAddr_.sin_family = PF_INET; // ipv4
	servAddr_.sin_addr.s_addr = INADDR_ANY; // ip address
	servAddr_.sin_port = htons(port); // port

	// dummy data는 필수적으로 0으로 초기화 되어 있어야 한다.
	memset(servAddr_.sin_zero, 0, sizeof(servAddr_.sin_zero));

	// 서버의 주소와 소켓 디스크립터를 결합한다.
	if (bind(servSock_, (struct sockaddr*)&servAddr_, sizeof(servAddr_)) == -1)
	{
		std::cerr << "bind error!" << std::endl;
		exit(1);
	}

	// 연결 대기열을 생성한다.
	if (listen(servSock_, 10) == -1)
	{
		std::cerr << "listen error!" << std::endl;
		exit(1);
	}

	// 제일 처음 연결된 연결을 가져와서 새로운 연결된 소켓을 만든다.
	clntSock_ = accept(servSock_, (struct sockaddr*)&clntAddr_, (socklen_t*)&clntAddrSize);
	if (clntSock_ == -1)
	{
		std::cerr << "accept error!" << std::endl;
		exit(1);
	}

	int	flag = fcntl(clntSock_, F_GETFL, 0); // 파일의 속성을 가져온다.
	fcntl(clntSock_, F_SETFL, flag | O_NONBLOCK); // 기존 속성에 NONBLOCK 옵션을 추가한다.

	char	buf[1024] = {0};
	int		readVal;

	while (1)
	{
		sleep(240);
		std::cout << "Waiting for new connection" << std::endl;
		// 제일 처음 연결된 연결을 가져와서 새로운 연결된 소켓을 만든다.
		// clntSock_ = accept(servSock_, (struct sockaddr*)&clntAddr_, (socklen_t*)&clntAddrSize);
		// if (clntSock_ == -1)
		// {
		// 	std::cerr << "accept error!" << std::endl;
		// 	exit(1);
		// }

		// 데이터를 계속 입력 받고 엔터 2번이 들어오면 멈춰야 한다.
		readVal = read(clntSock_, buf, sizeof(buf));
		if (readVal < 0)
		{
			// 일반적으로 read 함수는 유용한 입력 없을 시 기다리는데 non-block 모드에서는 즉시 반환 후 에러를 보고한다.
			if (errno == EAGAIN)
			{
				std::cout << "read EAGAIN" << std::endl;
			}
			else
			{
				std::cerr << "read error!" << std::endl;
				std::cout << "client socket close!" << std::endl;
				close(clntSock_);
				break ;
			}
		}
		else if (readVal == 0)
		{
			std::cout << "client socket close!" << std::endl;
			close(clntSock_);
			break ;
		}
		std::cout << buf << std::endl;
		char	conMsg[] = "Connected to server!\n";
		write(clntSock_, conMsg, strlen(conMsg));
	}
}

Server::~Server(void) {}
