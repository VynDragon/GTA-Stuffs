#include "TCPSocket.h"
#include <sstream>
#include "..\Common\Utilities.h"

TCPSocket::TCPSocket(const std::string &ip, const std::string &port, bool server, bool blocking)
{
	this->server = server;
#ifdef _WIN64
	if (!server)
	{
		struct addrinfo *result = NULL, hints;
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		int le_result = getaddrinfo(ip.c_str(), port.c_str(), &hints, &result);
		if (le_result != 0) {
			good = false;
			return;
		}
		socket = INVALID_SOCKET;
		addr = result;
		this->socket = ::socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
		if (socket == INVALID_SOCKET)
		{
			freeaddrinfo(result);
			good = false;
			return;
		}
		if (!blocking)
		{
			unsigned long mode = blocking ? 0 : 1;
			le_result = ioctlsocket(socket, FIONBIO, &mode);
			if (le_result != 0)
			{
				freeaddrinfo(result);
				closesocket(socket);
				good = false;
				return;
			}
		}
		good = true;
	}
	else
	{
		struct addrinfo *result = NULL, hints;

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;
		int le_result = getaddrinfo(ip.c_str(), port.c_str(), &hints, &result);
		if (le_result != 0) {
			good = false;
			return;
		}
		addr = result;
		socket = INVALID_SOCKET;
		this->socket = ::socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
		if (socket == INVALID_SOCKET)
		{
			freeaddrinfo(result);
			good = false;
			return;
		}
		if (!blocking)
		{
			unsigned long mode = blocking ? 0 : 1;
			le_result = ioctlsocket(socket, FIONBIO, &mode);
			if (le_result != 0)
			{
				freeaddrinfo(result);
				closesocket(socket);
				good = false;
				return;
			}
		}
		le_result = bind(socket, result->ai_addr, (int)result->ai_addrlen);
		if (le_result == SOCKET_ERROR)
		{
			freeaddrinfo(result);
			closesocket(socket);
			good = false;
			return;
		}
		if (listen(socket, SOMAXCONN) == SOCKET_ERROR)
		{
			closesocket(socket);
			good = false;
			return;
		}
		good = true;
	}
#endif
}


TCPSocket::~TCPSocket()
{
}

int TCPSocket::connect(void)
{
	if (good && !server)
	{
#ifdef _WIN64
		int result = ::connect(socket, addr->ai_addr, (int)addr->ai_addrlen);
		if (result == SOCKET_ERROR)
		{
			closesocket(socket);
			socket = INVALID_SOCKET;
			good = false;
			return -1;
		}
		return (0);
#endif
	}
	return -1;
}

int TCPSocket::disconnect(void)
{
	closesocket(socket);
	return 0;
}

TCPSocket * TCPSocket::accept(void)
{
	if (good && server)
	{
#ifdef _WIN64
		SOCKET clsock = INVALID_SOCKET;
		clsock = ::accept(socket, NULL, NULL);
		if (clsock == INVALID_SOCKET)
		{
			if (!(WSAGetLastError() == WSAEWOULDBLOCK))
				good = false;
		}
		else
			return (new TCPSocket(clsock));
#endif
	}
	return (NULL);
}

int TCPSocket::send(void * data, int size)
{
	if (good && !server)
	{
#ifdef _WIN64
		int result = ::send(socket, (char*)data, size, 0);
		if (result == SOCKET_ERROR)
			good = false;
		return result;
#endif
	}
	return -1;
}

int TCPSocket::receive(void * buff, int size)
{
	if (good && !server)
	{
#ifdef _WIN64
		int result = recv(socket, (char*)buff, size, 0);
		if (result == SOCKET_ERROR)
		{
			if (!(WSAGetLastError() == WSAEWOULDBLOCK))
			{
				good = false;
				return -1;
			}
			else
				return (0);
		}
		return result;
#endif
	}
	return -1;
}

bool TCPSocket::isGood(void)
{
	return good;
}

std::string TCPSocket::getIP(void)
{
	if (good)
	{
#ifdef _WIN64
		sockaddr_in	addr;
		int size = sizeof(addr);
		getsockname(socket, (sockaddr*)&addr, &size);
		char buff[100];
		inet_ntop(addr.sin_family, (void*)&addr.sin_addr, buff, 100);
		return (buff);
#endif
	}
	return std::string();
}

std::string TCPSocket::getPort(void)
{
	if (good)
	{
#ifdef _WIN64
		sockaddr_in	addr;
		int size = sizeof(addr);
		getsockname(socket, (sockaddr*)&addr, &size);
		std::stringstream ss;
		ss << ntohs(addr.sin_port);
		return (ss.str());
#endif
	}
	return std::string();
}

int TCPSocket::init_all()
{
#ifdef _WIN64
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
		return result;
#endif
	return 0;
}

int TCPSocket::clean_all()
{
#ifdef _WIN64
	WSACleanup();
#endif
	return 0;
}

SOCKET_TYPENAME TCPSocket::getSock(void)
{
	return this->socket;
}

#ifdef _WIN64
TCPSocket::TCPSocket(SOCKET socket)
{
	this->socket = socket;
	server = false;
	good = true;
}
#endif
