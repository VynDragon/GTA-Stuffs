#include "Inventory.h"
#include "..\Common\Food.h"
#include "..\Common\Weapons.h"
#include "..\Common\Ammos.h"

const Inventory					Inventory::emptyInventory;

Inventory::Inventory()
{
}


Inventory::~Inventory()
{
	auto it = list.begin();

	while (it != list.end())
	{
		delete (*it);
		it++;
	}
}

int		Inventory::use(std::vector<InventoryObject*>::iterator it, PedStatus *ped)
{
	(*it)->effect(ped);
	delete (*it);
	list.erase(it);
	return (0);
}

std::string								Inventory::toString(void)
{
	std::string							str = "";
	auto								it = list.begin();

	while (it != list.end())
	{
		str += (*it)->getName();
		it++;
		if (it != list.end())
			str += ",";
	}
	return (str);
}

int										Inventory::fromString(const std::string &str)
{
	std::string							item;
	unsigned int						i = 0, last = 0;

	while (str.c_str()[i] != 0)
	{
		item = "";
		while (str.c_str()[i] != 0 && str.c_str()[i] != ',')
			i++;
		item = str.substr(last, i - last);
		if (str.c_str()[i] != 0)
			i++;
		if (str.c_str()[i] != 0)
			last = i;
		int	k = 0;
		while (foodTypes[k].name!= item && foodTypes[k].name!= "")
			k++;
		if (foodTypes[k].name != "")
			this->addObject(new InventoryObject_Food(foodTypes[k].food, foodTypes[k].water, foodTypes[k].name));
		k = 0;
		while (k < weapon_list.size() && weapon_list[k] != item)
			k++;
		if (k < weapon_list.size())
			this->addObject(new InventoryObject_Weapon(weapon_list[k]));
		if (item.find('x') != std::string::npos)
		{
			std::string tmp = item.substr(0, item.rfind('x'));
			k = 0;
			while (k < ammoTypes.size() && ammoTypes[k].name != tmp)
				k++;
			if (k < ammoTypes.size())
				this->addObject(new InventoryObject_Ammo(ammoTypes[k].name, ammoTypes[k].weapon, atoi(item.substr(item.rfind('x') + 1).c_str())));
		}
	}
	return (0);
}

Inventory								&Inventory::operator=(const Inventory &inv)
{
	auto								it = inv.begin();
	auto								its = list.begin();

	while (its != list.end())
	{
		delete (*its);
		its++;
	}
	list.clear();
	while (it != inv.end())
	{
		this->addObject((*it)->clone());
		it++;
	}
	return (*this);
}

Inventory::Inventory(const Inventory &inv)
{
	auto								it = inv.begin();

	while (it != inv.end())
	{
		list.push_back((*it)->clone());
		it++;
	}
}