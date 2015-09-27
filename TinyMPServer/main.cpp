#include "..\Common\TCPSocket.h"
#include "..\Common\MPManager.h"
#include <iostream>
#include <vector>

typedef struct s_data
{
	TCPSocket	*sock;
	MPManager	*manager;
} t_data;

DWORD handleClient(void *data)
{
	t_data	*dat = (t_data*)data;
	std::cout << "client connected on: " << dat->sock->getIP() << ":" << dat->sock->getPort() << std::endl;
	while (1)
	{
		t_nwmsg msg;
		if (dat->sock->receive(&msg, sizeof(msg)) <= 0)
			return (0);
		dat->manager->handleMsg(dat->sock, &msg);
		if (msg.type == ENTITYMSG)
			std::cout << msg.serverid << " id of type " << msg.type << " with model " << msg.entity.model << std::endl;
	}
	return (0);
}

int		main(int ac, char **av)
{
	TCPSocket::init_all();
	std::vector<TCPSocket *>			clients; //TODO: client tracking and disconnected deletion
	MPManager							manager(true);
	if (ac == 3)
	{
		TCPSocket sock(av[1], av[2], true, true);
		if (!sock.isGood())
		{
			std::cout << "Socket not good: " << WSAGetLastError() << std::endl;
			return (0);
		}
		else
			std::cout << "Socket good" << std::endl;
		fd_set	setread;
		FD_ZERO(&setread);
		FD_SET(sock.getSock(), &setread);
		while (1)
		{
			t_data dat = { sock.accept(), &manager };
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)handleClient, &dat, 0, NULL);
		}
	}
	else
		std::cout << "usage: ./server <ip> <port>" << std::endl;
	return (0);
}