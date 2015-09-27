#include "MPManager.h"

#ifdef GAME
#include "script.h"
#endif


MPEntity::MPEntity()
{
}

MPEntity::~MPEntity()
{
}

MPEntity::MPEntity(const MPEntity & other)
{
	exist = other.exist;
	x = other.x;
	y = other.y;
	z = other.z;
	model = other.model;
	id = other.id;
	type = other.type;
}

MPEntity & MPEntity::operator=(const MPEntity & other)
{
	exist = other.exist;
	x = other.x;
	y = other.y;
	z = other.z;
	model = other.model;
	id = other.id;
	type = other.type;
	return (*this);
}

void MPEntity::spawn(void)
{
}

void MPEntity::remove(void)
{
#ifdef GAME	
	ENTITY::DELETE_ENTITY(&id);		
#endif
}

MPManager::MPManager(bool server)
{
	this->server = server;
	this->socket = NULL;
}


MPManager::~MPManager()
{
}

void MPManager::clear(void)
{
	auto it = entities.begin();

	while (it != entities.end())
	{
		delete it->second;
	}
	entities.clear();
}

void MPManager::handleMsg(TCPSocket *sock, s_nwmsg * msg)
{
	if (msg->type == REQUESTALL)
	{
		auto it = entities.begin();
		while (it != entities.end())
		{
			t_nwmsg msg = { it->first, ENTITYMSG, *(it->second) };
			sock->send(&msg, sizeof(msg));
			it++;
		}
	}
	else if (msg->type == ENTITYMSG && !server)
	{
		entities[msg->serverid] = new MPEntity(msg->entity);
	}
	else if (msg->type == DESTROYEDMSG)
	{
		auto t = entities.find(msg->serverid);
		if (!(t == entities.end()))
		{
			t->second->remove();
			entities.erase(t);
		}
	}
}

void MPManager::tick(Vector3 pos, float maxDist)
{
	if (server && socket)
	{
		auto it = entities.begin();
		while (it != entities.end())
		{
			t_nwmsg msg = { it->first, ENTITYMSG, *(it->second) };
			socket->send(&msg, sizeof(msg));
			it++;
		}
	}
	else if (socket)
	{
#ifdef GAME
		auto it = entities.begin();
		while (it != entities.end())
		{
			if (it->second->exist)
			{
				if (ENTITY::IS_AN_ENTITY(it->second->id))
				{
					if (GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(it->second->x, it->second->y, it->second->z, pos.x, pos.y, pos.z, true) > maxDist)
					{
						it->second->remove();
						it->second->exist = false;
					}
				}
				else
					it->second->exist = false;
			}
			else
			{
				if (GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(it->second->x, it->second->y, it->second->z, pos.x, pos.y, pos.z, true) < maxDist)
				{
					it->second->spawn();
					it->second->exist = true;
				}
			}
			it++;
		}
#endif
		s_nwmsg msg = { 0, REQUESTALL };
		sendMsg(&msg);
	}
}

TCPSocket * MPManager::getSocket(void)
{
	return socket;
}

DWORD		msgHandler(void * data)
{
	MPManager *manager = (MPManager*)data;
	while (1)
	{
		t_nwmsg msg;
		if (manager->getSocket()->receive(&msg, sizeof(msg)) <= 0)
			return (0);
		manager->handleMsg(manager->getSocket(), &msg);
	}
}


int MPManager::sendMsg(s_nwmsg * msg)
{
	return (socket->send(msg, sizeof(s_nwmsg)));
}

void							MPManager::setSocket(TCPSocket *socket)
{
	this->socket = socket;
	s_nwmsg msg = { 0, REQUESTALL };
	sendMsg(&msg);
	clear();
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)msgHandler, this, 0, NULL);
}