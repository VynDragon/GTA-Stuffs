#include "..\Common\TCPSocket.h"
#include "..\Common\MPManager.h"
#include "..\Common\script.h"
#include "..\Common\keyboard.h"
#include "..\Common\Utilities.h"
#include <sstream>
#include <vector>


void main(void)
{
	TCPSocket	*socket = NULL;
	MPManager	manager(true);
	DWORD		longTick = GetTickCount();

	while (true)
	{
		if (socket != NULL)
		{
			manager.setSocket(socket);
			socket = NULL;
		}
		if (IsKeyJustUp(VK_INSERT))
		{
			std::string ip = Utilities::input_text();
			std::string port = Utilities::input_text();
			socket = new TCPSocket(ip, port, false, true);
			if (!socket->isGood())
				socket = NULL;
			socket->connect();
		}
		if (longTick < GetTickCount())
		{
			Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
			manager.tick(pos, 100.0f);
			MPEntity *pl = new MPEntity;
			pl->exist = true;
			pl->id = PLAYER::PLAYER_PED_ID();
			pl->model = ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID());
			pl->x = pos.x;
			pl->y = pos.y;
			pl->z = pos.z;
			pl->type = MPEntity::PLAYER;
			manager.serverObject(pl, PLAYER::PLAYER_PED_ID());
			longTick = GetTickCount() + 1000;
		}
		WAIT(0);
	}
}

void ScriptMain()
{
	while (!ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
		WAIT(0);
	while (!PLAYER::IS_PLAYER_CONTROL_ON(PLAYER::PLAYER_ID()))
		WAIT(0);
	main();
}
