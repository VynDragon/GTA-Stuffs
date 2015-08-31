#pragma once

#include "..\Common\script.h"
#include "..\Common\Ammos.h"
class PedStatus;

class InventoryObject
{
public:
	virtual ~InventoryObject() { }
	virtual int			effect(PedStatus *ped) = 0;
	virtual const std::string	&getName(void) const = 0;
	virtual InventoryObject		*clone() const = 0;
};

class InventoryObject_Food : public InventoryObject
{
public:
	InventoryObject_Food(float amountFood, float amountWater, const std::string &name = "Food");
	~InventoryObject_Food();
	int						effect(PedStatus *ped);
	const std::string		&getName(void) const { return (name); }
	InventoryObject			*clone(void) const { return (new InventoryObject_Food(food, water, name)); }
private:
	std::string	name;
	float		food, water;
};

class InventoryObject_Weapon : public InventoryObject
{
public:
	InventoryObject_Weapon(const std::string &id);
	~InventoryObject_Weapon();
	const std::string		&getName(void) const { return (name); }
	InventoryObject			*clone(void) const { return (new InventoryObject_Weapon(name)); }
	int						effect(PedStatus *ped);
private:
	std::string	name;
};

class InventoryObject_Ammo : public InventoryObject
{
public:
	InventoryObject_Ammo(const std::string &name, const std::string &weapon, int amount);
	~InventoryObject_Ammo();
	const std::string		&getName(void) const { return (amountedName); }
	InventoryObject			*clone(void) const { return (new InventoryObject_Ammo(name, weapon, amount)); }
	int						effect(PedStatus *ped);
private:
	std::string	name;
	std::string	amountedName;
	std::string weapon;
	int amount;
};