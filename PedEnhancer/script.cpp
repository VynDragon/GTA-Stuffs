
#include "..\Common\script.h"
#include "..\Common\keyboard.h"
#include <sstream>
#include <vector>
#include "PedManager.h"
#include "..\Common\Kamikaze_menu.h"
#include "..\Common\KeyManager.h"


void main(bool devMode, float range)
{
	DWORD		longTick = GetTickCount();
	PedManager	manager(".\\Peds.ini");

	while (true)
	{
		Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
		if (longTick < GetTickCount())
		{
			manager.tick(pos, range);
			longTick = GetTickCount() + 600;
		}
		if (devMode)
		{
			if (Menu::currentMenu != NULL)
			{
				Menu::currentMenu->tick();
				if (KeyManager::keys.justPressed(VK_NUMPAD0))
					Menu::hide(Menu::currentMenu);
			}
			if (KeyManager::keys.justPressed(VK_PRIOR))
			{
				Menu	*men = new Kamikaze_Menu();


				manager.add(new BPed(pos.x, pos.y, pos.z, "u_m_y_militarybum"), true);
				Menu::show(men);	
			}
			KeyManager::keys.tick();
		}
		WAIT(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	float pedSpawnDistance = Utilities::getFromFile<float>(".\\Options.ini", "Options", "pedSpawnDistance", 150.0f);
	if (pedSpawnDistance == 150.0f)
		Utilities::writeToFile<float>(".\\Options.ini", "Options", "pedSpawnDistance", 150.0f);
	main(Utilities::getFromFile<bool>(".\\Options.ini", "Options", "devMode", false), pedSpawnDistance);
}
