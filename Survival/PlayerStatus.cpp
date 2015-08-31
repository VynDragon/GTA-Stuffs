#include "PlayerStatus.h"



PlayerStatus::PlayerStatus(Player playerId) : PedStatus(PLAYER::GET_PLAYER_PED(playerId))
{
	char	buff[500];
	this->playerId = playerId;
	GetPrivateProfileString("Player", "food", NULL, buff, 99,PLAYER_FILE);
	food = (float)atof(buff);
	GetPrivateProfileString("Player", "water", NULL, buff, 99,PLAYER_FILE);
	water = (float)atof(buff);
	GetPrivateProfileString("Player", "inventory", NULL, buff, 499, PLAYER_FILE);
	inventory.fromString(buff);
	timesave = GetTickCount() + 5000;
}


PlayerStatus::~PlayerStatus()
{
}

int			PlayerStatus::tick(void)
{
	pedId = PLAYER::GET_PLAYER_PED(playerId);
	if (GetTickCount() > time)
		doTick();
	if (GetTickCount() > timesave)
	{
		WritePrivateProfileString("Player", "food", Utilities::floatToString(food).c_str(), PLAYER_FILE);
		WritePrivateProfileString("Player", "water", Utilities::floatToString(water).c_str(), PLAYER_FILE);
		WritePrivateProfileString("Player", "inventory", this->inventory.toString().c_str(), PLAYER_FILE);
		timesave = GetTickCount() + 5000;
	}
	pkeys.tick(this);
	return (0);
}