
#include "..\Common\script.h"
#include "..\Common\keyboard.h"
#include "..\Common\Wrecks.h"
#include "Manager.h"
#include <sstream>
#include <vector>
#include "..\Common\Menu.h"
#include "..\Common\Kamikaze_Menu.h"
#include "..\Common\Props.h"
#include "..\Common\Keys.h"
#include "..\Common\Utilities.h"
#include "LightManager.h"

static Manager<Decoration>::managed	*currentMovable = NULL;

#define MOVEMENT_TICK 0.02f
#define MOVEMENT_TICK_HEADING 2.0f

// Menu functions

void					spawnProp(void *baseManager, const void *str)
{
	Manager<Decoration>	*base = (Manager<Decoration>*)baseManager;
	const std::string	*name = (const std::string*)str;
	Vector3	plcoords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
	base->add(new Decoration(*name, plcoords.x, plcoords.y, plcoords.z + 1.0f, (float)(rand() % 360)), true);
}

void					spawnHashedProp(void *baseManager, const void *str)
{
	Manager<Decoration>	*base = (Manager<Decoration>*)baseManager;
	Hash				*name = (Hash*)str;
	Vector3	plcoords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
	base->add(new Decoration(*name, plcoords.x, plcoords.y, plcoords.z + 1.0f, (float)(rand() % 360)), true);
}

void								setCurrentMovable(void *object, const void *str)
{
	currentMovable = (Manager<Decoration>::managed*)object;
	
}

// end

void main(bool devMode, float enhancerRenderDistance, unsigned int wreckDensity)
{
	Manager<Decoration>	carManager(".\\Cars.ini", false);
	Manager<Decoration>	baseManager(".\\Bases.ini");
	LightManager		lightManager(".\\Lights.ini");
	DWORD				longTick = GetTickCount(), longTickb = GetTickCount();

	while (true)
	{
		Vector3	plcoords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
		// random wrecks
		if (GetTickCount() > longTick)
		{
			Vector3	out, outb;
			float	outh;
			/*if (PATHFIND::GET_RANDOM_VEHICLE_NODE(plcoords.x, plcoords.y, plcoords.z, 100.0f, true, true, true, &out, &outh))
			{*/
				if (PATHFIND::GET_SAFE_COORD_FOR_PED(plcoords.x + rand() % 180 - 90, plcoords.y + rand() % 180 - 90, plcoords.z, false, &outb, 0))
				{
					if (PATHFIND::IS_POINT_ON_ROAD(outb.x, outb.y, outb.z, 0) && !CAM::IS_SPHERE_VISIBLE(outb.x, outb.y, out.z + 2.0f, 10.0f) && GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(outb.x, outb.y, out.z, plcoords.x, plcoords.y, plcoords.z, true) > 50.0f)
					{
						if (carManager.getLoaded()->size() < wreckDensity)
						{
							carManager.add(new Decoration(carWrecks[rand() % carWrecks.size()], outb.x, outb.y, outb.z, (float)(rand() % 360)), true);
							carManager.tick(plcoords, enhancerRenderDistance + 20.0f, devMode);
						}
					}
				}
			//}
			longTick = GetTickCount() + 100;
		}
		// bases
		if (GetTickCount() > longTickb)
		{
			carManager.tick(plcoords, enhancerRenderDistance + 20.0f, devMode);
			baseManager.tick(plcoords, enhancerRenderDistance, devMode);
			longTickb = GetTickCount() + 10;
		}
		lightManager.tick(plcoords, enhancerRenderDistance);
		//devmode
		if (devMode)
		{
			if (KeyManager::keys.justPressed(VK_PRIOR))
			{
				lightManager.add(true, 255, 255, 255, plcoords.x, plcoords.y, plcoords.z + 0.8f, 10.0f, 10.0f);
			}
			if (Menu::currentMenu != NULL)
			{
				Menu::currentMenu->tick();
				if (KeyManager::keys.justPressed(VK_NUMPAD0))
					Menu::hide(Menu::currentMenu);
			}
			if (KeyManager::keys.justPressed(VK_INSERT))
			{
				Kamikaze_Menu	*men = new Kamikaze_Menu();
				auto			it = props.begin();
				while (it != props.end())
				{
					men->add_entry(*it, spawnProp, &baseManager, &(*it));
					it++;
				}
				auto			itb = hashedProps.begin();
				while (itb != hashedProps.end())
				{
					men->add_entry(Utilities::xToString<Hash>(*itb), spawnHashedProp, &baseManager, &(*itb));
					itb++;
				}
				Menu::show(men);
			}
			if (KeyManager::keys.justPressed(VK_DELETE))
			{
				Kamikaze_Menu	*men = new Kamikaze_Menu();
				auto			it = baseManager.getLoaded()->begin();
				while (it != baseManager.getLoaded()->end())
				{
					men->add_entry((*it).that->getName() + Utilities::xToString<int>((*it).object), setCurrentMovable, &(*it), NULL);
					it++;
				}
				Menu::show(men);
			}
			
			if (currentMovable != NULL)
			{
				ENTITY::FREEZE_ENTITY_POSITION(currentMovable->object, true);
				Vector3 pls = ENTITY::GET_ENTITY_COORDS(currentMovable->object, true);
				GRAPHICS::SET_DRAW_ORIGIN(pls.x, pls.y, pls.z, 0);
				Utilities::putText(Utilities::xToString<unsigned int>(currentMovable->fileId), 0.0f, 0.0f);
				GRAPHICS::CLEAR_DRAW_ORIGIN();
				float	head = ENTITY::GET_ENTITY_HEADING(currentMovable->object);
				if (KeyManager::keys.isPressed(VK_NUMPAD4))
				{
					pls.y += MOVEMENT_TICK;
				}
				else if (KeyManager::keys.isPressed(VK_NUMPAD6))
				{
					pls.y -= MOVEMENT_TICK;
				}
				if (KeyManager::keys.isPressed(VK_NUMPAD8))
				{
					pls.x += MOVEMENT_TICK;
				}
				else if (KeyManager::keys.isPressed(VK_NUMPAD2))
				{
					pls.x -= MOVEMENT_TICK;
				}
				if (KeyManager::keys.isPressed(VK_NUMPAD7))
				{
					pls.z += MOVEMENT_TICK;
				}
				else if (KeyManager::keys.isPressed(VK_NUMPAD1))
				{
					pls.z -= MOVEMENT_TICK;
				}
				if (KeyManager::keys.isPressed(VK_NUMPAD9))
				{
					head += MOVEMENT_TICK_HEADING;
				}
				else if (KeyManager::keys.isPressed(VK_NUMPAD3))
				{
					head -= MOVEMENT_TICK_HEADING;
				}
				ENTITY::SET_ENTITY_COORDS(currentMovable->object, pls.x, pls.y, pls.z, true, true, true, true);
				ENTITY::SET_ENTITY_ROTATION(currentMovable->object, 0, 0, 0, 2, 1);
				ENTITY::SET_ENTITY_HEADING(currentMovable->object, head);
				if (KeyManager::keys.justPressed(VK_END))
				{
					baseManager.updateFile(baseManager.updateLoaded(currentMovable->object));
					currentMovable = NULL;
				}
			}
			KeyManager::keys.tick();
		}
		//end devmode
		WAIT(0);
	}
}

void				ScriptMain()
{
	bool			devMode;
	float			enhancerRenderDistance;
	unsigned int	wreckDensity;

	{
		char	buff[100];
		GetPrivateProfileString("Options", "devMode", "0", buff, 99, "./options.ini");
		if (!strcmp("1", buff))
			devMode = true;
		else
			devMode = false;
		GetPrivateProfileString("Options", "enhancerRenderDistance", "150", buff, 99, "./options.ini");
		enhancerRenderDistance = (float)atof(buff);
		if (enhancerRenderDistance == 150.0f)
			WritePrivateProfileString("Options", "enhancerRenderDistance", "150", "./options.ini");
		GetPrivateProfileString("Options", "wreckDensity", "50", buff, 99, "./options.ini");
		wreckDensity = std::stoul(buff);
		if (wreckDensity == 100)
			WritePrivateProfileString("Options", "wreckDensity", "50", "./options.ini");
	}
	srand(GetTickCount());
	main(devMode, enhancerRenderDistance, wreckDensity);
}
