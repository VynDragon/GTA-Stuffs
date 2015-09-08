#include "PedStatus.h"
#include "ContainerManager.h"
#include "..\Common\Weapons.h"
#include "..\Common\Utilities.h"


float	get_random_mul(void)
{
	int i = rand() % 3 - 1;

	while (i == 0)
		i = rand() % 3 - 1;
	return ((float)i);
}

PedStatus::PedStatus(Ped playerId)
{
	food = STATLIMIT;
	water = STATLIMIT;
	this->pedId = pedId;
	lastPedId = 0;
	moved = false;
	time = GetTickCount();
	respawnTimer = 0;
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
	//OutputDebugString("entering player dotick");
	if (moved && respawnTimer < GetTickCount())
	{
		Vector3	po = this->getPos();
		if (GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(targetPos.x, targetPos.y, targetPos.z, po.x, po.y, po.z, true) < 10.0f)
		{
			moved = false;
			respawnTimer = 0;
		}
		else
			ENTITY::SET_ENTITY_COORDS(pedId, targetPos.x, targetPos.y, targetPos.z, true, true, true, true);
	}
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
		Vector3	pos = this->getPos();
		Vector3 out;
		do
		{
			float	x = pos.x + (float)(get_random_mul() * (rand() % 120 + 100.0f)), y = pos.y + (float)(get_random_mul() * (rand() % 120 + 100.0f)), z = 0;
			GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(x, y, pos.z + 10.0f, &z);
			WAIT(100);
			if (z <= 0)
				GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(x, y, pos.z + 1000.0f, &z);
			targetPos.z = z + 1.0f;
			targetPos.x = x;
			targetPos.y = y;
		} while (!PATHFIND::GET_SAFE_COORD_FOR_PED(targetPos.x, targetPos.y, targetPos.z, false, &out, 0));
		targetPos.x = out.x;
		targetPos.y = out.y;
		targetPos.z = out.z;
		moved = true;
		respawnTimer = GetTickCount() + 20000;
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
	//OutputDebugString("exiting player dotick");
	return (0);
}

int			PedStatus::tick(void)
{
	if (GetTickCount() > time)
		doTick();
	return (0);
}