#pragma once

#include "..\Common\TCPSocket.h"
#include "..\inc\types.h"
#include "..\Common\Utilities.h"
#include <map>

#define GAME

enum NwMsgType
{
	ENTITYMSG = 0,
	DESTROYEDMSG = 1,
	REQUESTALL = 2
};

class MPEntity
{
public:
	enum Type
	{
		ENTITY = 0,
		PLAYER = 1
	};
	MPEntity();
	~MPEntity();
	MPEntity(const MPEntity &other);
	MPEntity &operator=(const MPEntity &other);
	void	spawn(void);
	void	remove(void);
	//data
	bool	exist;
	float	x, y, z;
	Hash	model;
	int		id;
	Type	type;
};

typedef struct s_nwmsg
{
	unsigned int	serverid;
	NwMsgType		type;
	MPEntity		entity;
} t_nwmsg;


class MPManager
{
public:
	MPManager(bool server);
	~MPManager();
	void							setSocket(TCPSocket *socket);
	void							clear(void);
	void							handleMsg(TCPSocket *sender, s_nwmsg *msg);
	void							tick(Vector3 pos, float maxDist);
	TCPSocket						*getSocket(void);
	void							serverObject(MPEntity *it, unsigned int id) { if (entities.find(id) != entities.end()) delete entities[id]; entities[id] = it; }
protected:
	int								sendMsg(s_nwmsg *msg);
	TCPSocket						*socket;
	bool							server;
	std::map<unsigned int, MPEntity*>	entities;
};

