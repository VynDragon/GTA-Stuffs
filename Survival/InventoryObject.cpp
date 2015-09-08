#include "InventoryObject.h"
#include "PedStatus.h"
#include "..\Common\Utilities.h"



/*InventoryObject::InventoryObject(const std::string &name) : name(name)
{
}*/

InventoryObject_Food::InventoryObject_Food(float amountFood, float amountWater, const std::string &name)
{
	food = amountFood;
	water = amountWater;
	this->name = name;
}


InventoryObject_Food::~InventoryObject_Food()
{
}

int		InventoryObject_Food::effect(PedStatus *ped)
{
	ped->addFood(food);
	ped->addWater(water);
	return (0);
}
InventoryObject_Weapon::InventoryObject_Weapon(const std::string &id)
{
	this->name = id;
}
InventoryObject_Weapon::~InventoryObject_Weapon()
{

}
int			InventoryObject_Weapon::effect(PedStatus *ped)
{
	int		ammo;
	Any		ammoType;
	ammoType = WEAPON::_GET_PED_AMMO_TYPE(ped->getId(), Utilities::get_hash(name));
	ammo = WEAPON::GET_PED_AMMO_BY_TYPE(ped->getId(), ammoType);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped->getId(), Utilities::get_hash(name), 1000, 0);
	WEAPON::SET_PED_AMMO_BY_TYPE(ped->getId(), ammoType, ammo);
	return (0);
}

InventoryObject_Ammo::InventoryObject_Ammo(const std::string &name, const std::string &weapon, int amount)
{
	this->name = name;
	this->weapon = weapon;
	this->amount = amount;
	this->amountedName = name + "x" + Utilities::xToString<int>(amount);
}
InventoryObject_Ammo::~InventoryObject_Ammo()
{
}

int						InventoryObject_Ammo::effect(PedStatus *ped)
{
	WEAPON::ADD_AMMO_TO_PED(ped->getId(), Utilities::get_hash(weapon), amount);
	return (0);
}

int InventoryObject_RepairKit::effect(PedStatus * ped)
{
	Vector3	pos = ped->getPos();
	int		entries[20 * 2 + 2] = { 0 };
	entries[0] = 20;
	int		ct = PED::GET_PED_NEARBY_VEHICLES(PLAYER::PLAYER_PED_ID(), entries);
	int		closest = 0;
	float	lastdistance = (std::numeric_limits<float>::max)();
	for (int i = 2; i < ct * 2; i += 2)
	{
		if (ENTITY::DOES_ENTITY_EXIST(entries[i]))
		{
			Vector3	a = ENTITY::GET_ENTITY_COORDS(entries[i], true), b = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
			float dist = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(a.x, a.y, a.z, b.x, b.y, b.z, true);
			if (dist < lastdistance)
			{
				lastdistance = dist;
				closest = entries[i];
			}
		}
	}
	if (closest != 0)
	{
		ENTITY::SET_ENTITY_HEALTH(closest, ENTITY::GET_ENTITY_MAX_HEALTH(closest));
		VEHICLE::SET_VEHICLE_ENGINE_HEALTH(closest, 1000);
		VEHICLE::SET_VEHICLE_BODY_HEALTH(closest, 1000);
		VEHICLE::SET_VEHICLE_PETROL_TANK_HEALTH(closest, 1000);
		VEHICLE::SET_VEHICLE_FIXED(closest);
		VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(closest);
	}
	else
	{
		Utilities::notify("No vehicle to repair found");
		return (1);
	}
}
