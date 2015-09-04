#include "PedStatus.h"
#include "ContainerManager.h"
#include "..\Common\Weapons.h"
#include "..\Common\Utilities.h"



PedStatus::PedStatus(Ped playerId)
{
	food = STATLIMIT;
	water = STATLIMIT;
	this->pedId = pedId;
	time = GetTickCount();
}


PedStatus::~PedStatus()
{
}

void	PedStatus::addFood(float f)
{
	food += f;
	if (food > STATLIMIT)
		food = STATLIMIT;
}

void	PedStatus::addWater(float w)
{
	water += w;
	if (water > STATLIMIT)
		water = STATLIMIT;
}

float	PedStatus::getFood(void)
{
	return (food);
}

float	PedStatus::getWater(void)
{
	return (water);
}

Ped		PedStatus::getId(void)
{
	return (this->pedId);
}

int PedStatus::doTick(void)
{
	if (PED::_IS_PED_DEAD(pedId, true) && !dead)
	{
		food = STATLIMIT;
		water = STATLIMIT;
		Inventory	in;
		in = this->inventory;
		this->inventory = Inventory::emptyInventory;
		int	n = 1;
		while (n < weapon_list.size())
		{
			if (WEAPON::HAS_PED_GOT_WEAPON(this->getId(), Utilities::get_hash(weapon_list[n]), false))
			{
				in.addObject(new InventoryObject_Weapon(weapon_list[n]));
				if (WEAPON::GET_PED_AMMO_BY_TYPE(this->getId(), WEAPON::_GET_PED_AMMO_TYPE(this->getId(), Utilities::get_hash(weapon_list[n]))) > 0)
					in.addObject(new InventoryObject_Ammo(get_ammo_by_weapon(weapon_list[n]).name, weapon_list[n], WEAPON::GET_PED_AMMO_BY_TYPE(this->getId(), WEAPON::_GET_PED_AMMO_TYPE(this->getId(), Utilities::get_hash(weapon_list[n])))));
				WEAPON::REMOVE_WEAPON_FROM_PED(this->getId(), Utilities::get_hash(weapon_list[n]));
				WEAPON::SET_PED_AMMO_BY_TYPE(this->getId(), WEAPON::_GET_PED_AMMO_TYPE(this->getId(), Utilities::get_hash(weapon_list[n])), 0);
			}
			n++;
		}
		ContainerManager::current->addCrate(true, in, this->getPos());
		in = Inventory::emptyInventory; // this actually deallocate the stuffs;
		dead = true;
	}
	else if (!PED::_IS_PED_DEAD(pedId, true))
		dead = false;
	ENTITY::SET_ENTITY_MAX_HEALTH(pedId, 200);
	food -= FOODRATE;
	water -= WATERRATE;
	food = food < 0 ? 0 : food;
	water = water < 0 ? 0 : water;
	if (food > REGENLIMIT && water > REGENLIMIT)
		ENTITY::SET_ENTITY_HEALTH(pedId, ENTITY::GET_ENTITY_HEALTH(pedId) + REGENRATE);
	else if (food < LOSSLIMIT || water < LOSSLIMIT)
		ENTITY::SET_ENTITY_HEALTH(pedId, ENTITY::GET_ENTITY_HEALTH(pedId) -LOSSRATE);
	time = GetTickCount() + TICK_LENGTH;
	return (0);
}

int			PedStatus::tick(void)
{
	if (GetTickCount() > time)
		doTick();
	return (0);
}