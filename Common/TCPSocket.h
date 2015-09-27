#pragma once

#ifdef _WIN64
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
static WSADATA wsaData;
#define SOCKET_TYPENAME SOCKET
#endif
#ifdef __gnu_linux__
#endif

#include <string>

class TCPSocket
{
public:
	TCPSocket(const std::string &ip, const std::string &port, bool server, bool blocking);
	~TCPSocket();
	int				connect(void);
	int				disconnect(void);
	TCPSocket		*accept(void);
	int				send(void *data, int size);
	int				receive(void *buff, int size);
	bool			isGood(void);
	std::string		getIP(void);
	std::string		getPort(void);
	static int		init_all();
	static int		clean_all();
	SOCKET_TYPENAME	getSock(void);
protected:
	bool				server, good;
	SOCKET_TYPENAME		socket;
#ifdef _WIN64
	TCPSocket(SOCKET socket);
	struct addrinfo		*addr;
#endif
};

