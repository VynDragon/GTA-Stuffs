#include "PlayerStatus.h"
#include "..\Common\Utilities.h"



PlayerStatus::PlayerStatus(Player playerId) : PedStatus(PLAYER::GET_PLAYER_PED(playerId))
{
	//OutputDebugString("Entering player constructor");
	food = Utilities::getFromFile<float>(PLAYER_FILE, "Player", "food", 1.0f);
	water = Utilities::getFromFile<float>(PLAYER_FILE, "Player", "water", 1.0f);
	inventory.fromString(Utilities::getFromFile<std::string>(PLAYER_FILE, "Player", "inventory", ""));
	targetPos.x = Utilities::getFromFile<float>(PLAYER_FILE, "Player", "x", 0.0f);
	targetPos.y = Utilities::getFromFile<float>(PLAYER_FILE, "Player", "y", 0.0f);
	targetPos.z = Utilities::getFromFile<float>(PLAYER_FILE, "Player", "z", 0.0f);
	if (targetPos.x == 0.0f && targetPos.y == 0.0f && targetPos.z == 0.0f)
		moved = false;
	else
		moved = true;
	timesave = GetTickCount() + 5000;
	this->playerId = playerId;
	//OutputDebugString("Exiting player constructor");
}


PlayerStatus::~PlayerStatus()
{
}

int			PlayerStatus::tick(void)
{
	pedId = PLAYER::GET_PLAYER_PED(playerId);
	/*OutputDebugString(Utilities::xToString<int>(playerId).c_str());
	OutputDebugString((Utilities::xToString<int>(pedId) + "pedId").c_str());
	OutputDebugString((Utilities::xToString<int>(PLAYER::PLAYER_ID()) + " orplId").c_str());
	OutputDebugString((Utilities::xToString<int>(PLAYER::PLAYER_PED_ID()) + " orpedId").c_str());*/
	if (GetTickCount() > time)
		doTick();
	if (GetTickCount() > timesave)
	{
		WritePrivateProfileString("Player", "food", Utilities::floatToString(food).c_str(), PLAYER_FILE);
		WritePrivateProfileString("Player", "water", Utilities::floatToString(water).c_str(), PLAYER_FILE);
		WritePrivateProfileString("Player", "inventory", this->inventory.toString().c_str(), PLAYER_FILE);
		Vector3 pos = this->getPos();
		Utilities::writeToFile<std::string>(PLAYER_FILE, "Player", "x", Utilities::xToString<float>(pos.x));
		Utilities::writeToFile<std::string>(PLAYER_FILE, "Player", "y", Utilities::xToString<float>(pos.y));
		Utilities::writeToFile<std::string>(PLAYER_FILE, "Player", "z", Utilities::xToString<float>(pos.z));
		timesave = GetTickCount() + 5000;
	}
	pkeys.tick(this);
	return (0);
}