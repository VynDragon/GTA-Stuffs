#include "ContainerManager.h"
#include "..\Common\Food.h"
#include "Defines.h"
#include "Container.h"
#include "..\Common\Weapons.h"
#include "..\Common\Ammos.h"
#include "OptionManager.h"
#include "..\Common\Utilities.h"

ContainerManager		*ContainerManager::current;

ContainerManager::ContainerManager()
{
	last = 0;
}


ContainerManager::~ContainerManager()
{
}

void		ContainerManager::openCrate(Object crate, Inventory *inv)
{
	auto	it = loaded.begin();

	while (it != loaded.end() && (*it).object != crate)
		it++;
	if ((*it).object == crate)
	{
		if ((*it).inventoried)
		{
			auto iti = (*it).inventory.begin();
			while (iti != (*it).inventory.end())
			{
				inv->addObject((*iti)->clone());
				iti++;
			}
		}
		else
		{
			int	length = rand() % 5 + 1;
			for (int x = 0; x < length; x++)
			{
				FoodType food = foodTypes[(rand() % NUMBER_DIFFERENT_FOOD)];
				inv->addObject(new InventoryObject_Food(food.food, food.water, food.name));
			}
			if (rand() % 7 < 1)
				inv->addObject(new InventoryObject_Weapon(weapon_list[(rand() % (weapon_list.size() - 1) + 1)]));
			length = rand() % 2 + 1;
			for (int x = 0; x < length; x++)
			{
				ammoType ammo = ammoTypes[rand() % ammoTypes.size()];
				inv->addObject(new InventoryObject_Ammo(ammo.name, ammo.weapon, rand() % 20 + 1));
			}
			if (rand() % 4 == 1)
			{
				inv->addObject(new InventoryObject_Weapon("WEAPON_PETROLCAN"));
			}
			if (rand() % 4 == 1)
			{
				inv->addObject(new InventoryObject_RepairKit());
			}
		}
		
		if (OptionManager::options.devMode)
			(*it).respawnTimer = GetTickCount();
		else
			(*it).respawnTimer = GetTickCount() + OptionManager::options.crateRespawnTime;
		if (!(*it).respawn)
			loaded.erase(it);
	}
	OBJECT::DELETE_OBJECT(&crate);
}


ContainerManager::Crate			ContainerManager::readFromFile(const std::string file, const std::string &sect)
{
	char		buff[500];
	Crate		crate;

	crate.inventoried = false;
	crate.object = 0;
	crate.position = { 0, 0, 0 };
	GetPrivateProfileString(sect.c_str(), "inventoried", NULL, buff, 499, file.c_str());
	if (!strcmp("", buff))
		return (crate);
	if (!strcmp("1", buff))
		crate.inventoried = true;
	GetPrivateProfileString(sect.c_str(), "inventory", NULL, buff, 499, file.c_str());
	crate.inventory.fromString(buff);
	GetPrivateProfileString(sect.c_str(), "x", NULL, buff, 499, file.c_str());
	crate.position.x = (float)atof(buff);
	GetPrivateProfileString(sect.c_str(), "y", NULL, buff, 499, file.c_str());
	crate.position.y = (float)atof(buff);
	GetPrivateProfileString(sect.c_str(), "z", NULL, buff, 499, file.c_str());
	crate.position.z = (float)atof(buff);
	crate.object = 1;
	crate.respawn = true;
	crate.respawnTimer = GetTickCount();
	return (crate);
}
int				ContainerManager::writeToFile(const std::string file, const std::string &sect, ContainerManager::Crate crate)
{
	if (crate.inventoried)
		WritePrivateProfileString(sect.c_str(), "inventoried", "1", file.c_str());
	else
		WritePrivateProfileString(sect.c_str(), "inventoried", "0", file.c_str());
	WritePrivateProfileString(sect.c_str(), "inventory", crate.inventory.toString().c_str(), file.c_str());
	WritePrivateProfileString(sect.c_str(), "x", Utilities::floatToString(crate.position.x).c_str(), file.c_str());
	WritePrivateProfileString(sect.c_str(), "y", Utilities::floatToString(crate.position.y).c_str(), file.c_str());
	WritePrivateProfileString(sect.c_str(), "z", Utilities::floatToString(crate.position.z).c_str(), file.c_str());
	return (0);
}

void				ContainerManager::load(void)
{
	Crate			crate;
	unsigned int	i = 0;

	crate = readFromFile(CRATE_FILE, Utilities::xToString<unsigned int>(i));
	while (crate.object != 0)
	{
		unloaded.push_back(crate);
		i++;
		crate = readFromFile(CRATE_FILE, Utilities::xToString<unsigned int>(i));
	}
	last = i;
}

void		ContainerManager::addCrate(bool inventoried, Inventory inventory, Vector3 position)
{
	Crate	crate;

	crate.inventoried = inventoried;
	crate.inventory = inventory;
	crate.position = position;
	crate.respawn = false;
	crate.respawnTimer = GetTickCount();
	crate.object = 0;
	last++;
	unloaded.push_back(crate);
}

void		ContainerManager::addCrate_permanent(bool inventoried, Inventory inventory, Vector3 position)
{
	Crate	crate;

	crate.inventoried = inventoried;
	crate.inventory = inventory;
	crate.position = position;
	crate.respawn = true;
	crate.object = 0;
	crate.respawnTimer = GetTickCount();
	writeToFile(CRATE_FILE, Utilities::xToString<unsigned int>(last), crate);
	last++;
	unloaded.push_back(crate);
}

void			ContainerManager::tick(Vector3 pos)
{
	auto		it = unloaded.begin();
	bool		bloaded = false, bunloaded = false;

	while (it != unloaded.end() && !bloaded)
	{
		if (GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(pos.x, pos.y, pos.z, (*it).position.x, (*it).position.y, (*it).position.z, true) < CONTAINER_LOAD_DISTANCE && (*it).respawnTimer < GetTickCount())
		{
			(*it).object = Utilities::create_object(containers[(rand() % containers.size())], (*it).position);
			OBJECT::PLACE_OBJECT_ON_GROUND_PROPERLY((*it).object);
			ROPE::ACTIVATE_PHYSICS((*it).object);
			if (OptionManager::options.devMode)
				(*it).blip = UI::ADD_BLIP_FOR_ENTITY((*it).object);
			loaded.push_back((*it));
			unloaded.erase(it);
			bloaded = true;
		}
		it++;
	}
	it = loaded.begin();
	while (it != loaded.end() && !bunloaded)
	{
		if (GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(pos.x, pos.y, pos.z, (*it).position.x, (*it).position.y, (*it).position.z, true) > CONTAINER_LOAD_DISTANCE || !ENTITY::DOES_ENTITY_EXIST((*it).object))
		{
			OBJECT::DELETE_OBJECT(&(*it).object);
			if (OptionManager::options.devMode)
			{
				UI::REMOVE_BLIP(&(*it).blip);
			}
			unloaded.push_back((*it));
			loaded.erase(it);
			bunloaded = true;
		}
		it++;
	}

}