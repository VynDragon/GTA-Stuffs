#pragma once

#include <string>
#include <vector>

typedef struct {
	std::string	weapon;
	std::string	name;
} ammoType;

static const std::vector<ammoType> ammoTypes = { { "WEAPON_PUMPSHOTGUN","Shotgun Ammo" },
{ "WEAPON_MG","Machinegun Ammo" },
{ "WEAPON_PISTOL","Pistol Ammo" },
{ "WEAPON_SMG","Submachine Gun Ammo" },
{ "WEAPON_ASSAULTRIFLE","Rifle Ammo" },
{ "WEAPON_STUNGUN","Stungun Ammo" },
{ "WEAPON_SNIPERRIFLE","Sniper Rifle Ammo" },
{ "WEAPON_GRENADELAUNCHER","Grenade Launcher Ammo" },
{ "WEAPON_RPG","Rocket Launcher Ammo" },
{ "WEAPON_MINIGUN","Minigun Ammo" },
{ "WEAPON_RAILGUN","Railgun Ammo" },
{ "WEAPON_HOMINGLAUNCHER","Homing Rocket Launcher Ammo" },
{ "WEAPON_MUSKET","Musket Ammo" }
};

static ammoType		get_ammo_by_weapon(const std::string &weapon)
{
	auto			it = ammoTypes.begin();
	ammoType		dummy = { "", "" };

	while (it != ammoTypes.end() && (*it).weapon != weapon)
		it++;
	if (it == ammoTypes.end())
		return (dummy);
	return (*it);
}