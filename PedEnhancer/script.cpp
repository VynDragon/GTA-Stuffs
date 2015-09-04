
#include "..\Common\script.h"
#include "..\Common\keyboard.h"
#include <sstream>
#include <vector>
#include "PedManager.h"
#include "..\Common\Kamikaze_menu.h"
#include "..\Common\KeyManager.h"
#include "..\Common\MilitaryModels.h"
#include "..\Common\Groups.h"

namespace DevMode
{
	void			spawn_guardian(void *data, const void *cdata)
	{
		PedManager	*manager = (PedManager*)data;
		Vector3		pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
		float		heading = ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID());

		manager->add(new Guard_Ped(pos.x, pos.y, pos.z, heading, militaryPeds[rand() % militaryPeds.size()]), true);
	}
	void			spawn_basic(void *data, const void *cdata)
	{
		PedManager	*manager = (PedManager*)data;
		Vector3		pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
		float		heading = ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID());

		manager->add(new BPed(pos.x, pos.y, pos.z, heading, "u_m_y_militarybum"), true);
	}
	void			spawn_wanderer(void *data, const void *cdata)
	{
		PedManager	*manager = (PedManager*)data;
		Vector3		pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
		float		heading = ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID());

		manager->add(new Wanderer_Ped(pos.x, pos.y, pos.z, heading, ""), true);
	}
}

void main(bool devMode, float range)
{
	DWORD		longTick = GetTickCount(), longTickb = GetTickCount();
	PedManager	manager(".\\Peds.ini");

	Groups::init();
	while (true)
	{
		Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
		if (longTickb < GetTickCount())
		{
			auto it = manager.getLoaded()->begin();
			while (it != manager.getLoaded()->end())
			{
				if ((*it).that->getClassName() == "Wanderer_Ped")
				{
					if (ENTITY::GET_ENTITY_HEALTH((*it).ped) < ENTITY::GET_ENTITY_MAX_HEALTH((*it).ped) / 2.0f)
					{
						AI::CLEAR_PED_TASKS((*it).ped);
						AI::CLEAR_PED_SECONDARY_TASK((*it).ped);
						PED::SET_PED_KEEP_TASK((*it).ped, false);
					}
				}
				it++;
			}
			longTickb = GetTickCount() + 1200;
		}
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
			if (KeyManager::keys.justPressed(VK_NEXT))
			{
				Menu	*men = new Kamikaze_Menu();

				men->add_entry("Guardian Ped", DevMode::spawn_guardian, &manager, NULL);
				men->add_entry("Basic Ped", DevMode::spawn_basic, &manager, NULL);
				men->add_entry("Wandering Ped", DevMode::spawn_wanderer, &manager, NULL);
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
