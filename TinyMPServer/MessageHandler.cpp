#include "..\Common\TCPSocket.h"
#include "..\Common\MPManager.h"
#include <iostream>

void	HandleEntity(TCPSocket *sock, t_nwmsg msg)
{

}

void	HandleRequestAll(TCPSocket *sock)
{
	std::cout << "client requested synchronization" << std::endl;
}

void	HandleDestroyed(TCPSocket *sock, t_nwmsg msg)
{

}