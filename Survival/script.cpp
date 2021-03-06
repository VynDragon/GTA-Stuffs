#include "..\Common\script.h"
#include "..\Common\keyboard.h"
#include "PedStatus.h"
#include "PlayerStatus.h"
#include "hud.h"
#include "..\Common\Weapons.h"
#include "..\Common\Menu.h"
#include "..\Common\KeyManager.h"
#include "OptionManager.h"
#include <string>
#include <ctime>
#include <fstream>
#include "..\Common\Utilities.h"

#pragma warning(disable : 4244 4305) // double <-> float conversions

/* Some function use static buffers because the game freak out when using new or malloc, for some reasons.
* Yes i know this is horribly unsecure.
*/

void					add_weapons(Menu *menu, Ped pedId)
{
	int					n;
	DWORD				hash;
	char				buff[100]; // no weapon name is as long as this.

	while (weapon_list[n] != "")
	{
		memcpy(buff, weapon_list[n].c_str(), weapon_list[n].size());
		buff[weapon_list[n].size()] = 0;
		hash = GAMEPLAY::GET_HASH_KEY(buff);
		if (WEAPON::HAS_PED_GOT_WEAPON(pedId, hash, false))
		{
			menu->add_entry(weapon_list[n], &Hud::weapon_submenu, NULL, &weapon_list[n]);
			Utilities::notify(buff);
		}
		n++;
	}
}

void	setup_options(Menu *menu)
{

}

void main()
{	
	PlayerStatus		*player = new PlayerStatus(PLAYER::PLAYER_ID());
	Hud					*hud = new Hud(player);
	ContainerManager	containerManager;
	DWORD				longTick = GetTickCount() + 600;

	WAIT(0);
	containerManager.load();
	ContainerManager::setCurrent(&containerManager);
	while (true)
	{
		//OutputDebugString("start tick");
		// pre tick setup
		PLAYER::_SET_PLAYER_HEALTH_REGENERATION_RATE(PLAYER::PLAYER_ID(), 0.0f);
		// menu switch
		if (Menu::currentMenu != NULL)
		{
			Menu::currentMenu->tick();
			if (KeyManager::keys.justPressed(VK_NUMPAD0))
				Menu::hide(Menu::currentMenu);
		}
		// thest
		auto plcoords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID()), true);
		if (OptionManager::options.devMode)
			if (KeyManager::keys.justPressed(VK_F6))
			{
				//OBJECT::PLACE_OBJECT_ON_GROUND_PROPERLY(Utilities::create_object(containers[(rand() % 9)], plcoords));
				//GAMEPLAY::CLEAR_AREA_OF_VEHICLES(plcoords.x, plcoords.y, plcoords.z, 1000.0, 0, 0, 0, 0, 0);
				containerManager.addCrate_permanent(false, Inventory::emptyInventory, plcoords);
			}
		// all else go here
		OptionManager::options.tick();
		KeyManager::keys.tick();
		//OutputDebugString("start player tick");
		player->tick();
		//OutputDebugString("end player tick");
		hud->draw();
		if (GetTickCount() > longTick)
		{
			containerManager.tick(player->getPos());
			longTick = GetTickCount() + 600;
		}
		// look like that WAIT is important
		//OutputDebugString("end tick");
		WAIT(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	GRAPHICS::CLEAR_DRAW_ORIGIN();
	/*GAMEPLAY::_DISABLE_AUTOMATIC_RESPAWN(true);
	GAMEPLAY::SET_FADE_IN_AFTER_DEATH_ARREST(false);*/
	while (!ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
		WAIT(0);
	while (!PLAYER::IS_PLAYER_CONTROL_ON(PLAYER::PLAYER_ID()))
		WAIT(0);
	main();
}
