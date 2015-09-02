#pragma once

#include <string>
#include <vector>

typedef struct {
	std::string	weapon;
	std::string	name;
} ammoType;

static const std::vector<ammoType> ammoTypes = {
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
{ "WEAPON_MUSKET","Musket Ammo" },
{ "WEAPON_COMBATPISTOL", "Combat Pistol Ammo"},
{ "WEAPON_APPISTOL","AppPistol Ammo"},
{ "WEAPON_VINTAGEPISTOL","Vintage Pistol Ammo" },
{ "WEAPON_PISTOL50","50 Pistol Ammo" },
{ "WEAPON_MICROSMG","Micro SMG Ammo" },
{ "WEAPON_ASSAULTSMG","Assault SMG Ammo" },
{ "WEAPON_ASSAULTRIFLE","Assault Rifle Ammo" },
{ "WEAPON_CARBINERIFLE","Carbine Rifle Ammo" },
{ "WEAPON_ADVANCEDRIFLE","Advanced Rifle Ammo" },
{ "WEAPON_COMBATPDW","PDW Ammo" },
{ "WEAPON_COMBATMG","Combat MachineGun Ammo" },
{ "WEAPON_PUMPSHOTGUN","Pump Shotgun Ammo" },
{ "WEAPON_SAWNOFFSHOTGUN","Sawn Off Shotgun Ammo" },
{ "WEAPON_ASSAULTSHOTGUN","Assault Shotgun Ammo" },
{ "WEAPON_BULLPUPSHOTGUN","Bullpup Shotgun Ammo" },
{ "WEAPON_HEAVYSNIPER","Heavy Sniper Ammo" },
{ "WEAPON_GRENADELAUNCHER_SMOKE","Smoke Grenade launcher Ammo" },
{ "WEAPON_STICKYBOMB","C4" },
{ "WEAPON_SMOKEGRENADE","Smoke Grenade" },
{ "WEAPON_BZGAS","Gas Grenade" },
{ "WEAPON_MOLOTOV","Molotov" },
{ "WEAPON_FIREEXTINGUISHER","Fire Extinguisher Unit" },
{ "WEAPON_PETROLCAN","Petrol Can Unit" },
{ "WEAPON_SNSPISTOL","SN Pistol Ammo" },
{ "WEAPON_SPECIALCARBINE","Special Carbine Ammo" },
{ "WEAPON_HEAVYPISTOL","Heavy Pistol Ammo" },
{ "WEAPON_BULLPUPRIFLE","Bullpup Rifle Ammo" },
{ "WEAPON_PROXMINE","Proximity Mine" },
{ "WEAPON_SNOWBALL","Snowball" },
{ "WEAPON_FIREWORK","Firework" },
{ "WEAPON_MARKSMANRIFLE","Marksman Rifle Ammo" },
{ "WEAPON_HEAVYSHOTGUN","Heavy Shotgun Ammo" },
{ "WEAPON_GUSENBERG","Gusenberg Ammo" }
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