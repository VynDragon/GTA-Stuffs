#include "hud.h"
#include "ContainerManager.h"
#include "..\Common\Keys.h"
#include "OptionManager.h"
#include "..\Common\Utilities.h"

Hud::Hud(PlayerStatus *player)
{
	this->player = player;
	x = 0.5f;
	y = 0.9f;
	weapon_timer = GetTickCount() + WEAPON_ACTUALIZATION_DELAY;
}


Hud::~Hud()
{
}
void		Hud::draw()
{
	float	hp, maxhp;

	GRAPHICS::CLEAR_DRAW_ORIGIN();
	Vector3		color;
	color.x = 0.0f;
	color.y = 0.0f;
	color.z = 0.0f;
	hp = static_cast<float>(ENTITY::GET_ENTITY_HEALTH(player->getId()));
	maxhp = static_cast<float>(ENTITY::GET_ENTITY_MAX_HEALTH(player->getId()));
	Utilities::drawRectangle(x, y, 0.2f, 0.02f, color, 0.5f);
	Utilities::drawRectangle(x, y + 0.021f, 0.2f, 0.02f, color, 0.5f);
	Utilities::drawRectangle(x, y + 0.042f, 0.2f, 0.02f, color, 0.5f);
	color.x = 1.0f;
	Utilities::drawRectangle(x, y + 0.0010f, 0.2f * (hp - 100.0f) / (maxhp - 100.0f), 0.0150f, color, 1.0f);
	color.x = 0.0f;
	color.z = 1.0f;
	Utilities::drawRectangle(x, y + 0.022f, 0.2f * player->getWater(), 0.0150f, color, 1.0f);
	color.z = 0.0f;
	color.y = 1.0f;
	Utilities::drawRectangle(x, y + 0.043f, 0.2f * player->getFood(), 0.0150f, color, 1.0f);
	color.y = 0.0f;
	Utilities::drawRectangle(x, y + 0.022f, 0.001f, 0.0530f, color, 1.0f);
	Hash wep;
	WEAPON::GET_CURRENT_PED_WEAPON(player->getId(), &wep, true);
	if (!(PLAYER::IS_PLAYER_FREE_AIMING(PLAYER::PLAYER_ID()) &&
		(wep == Utilities::get_hash("WEAPON_MARKSMANRIFLE") ||
			wep == Utilities::get_hash("WEAPON_SNIPERRIFLE") ||
			wep == Utilities::get_hash("WEAPON_HEAVYSNIPER")
			)))
	{
		UI::HIDE_HUD_AND_RADAR_THIS_FRAME();
	}
	else
		UI::_DISABLE_RADAR_THIS_FRAME();
	if (GetTickCount() > weapon_timer)
	{
		actualize_weapons();
		weapon_timer = GetTickCount() + WEAPON_ACTUALIZATION_DELAY;
	}
	if (KeyManager::keys.justPressed(Keys::find_key_by_name(OptionManager::options.openWeaponListKey).vkey))
	{
		if (Menu::currentMenu == NULL)
			Menu::show(&tab_menu);
		else
			Menu::hide(Menu::currentMenu);
	}
}

void		Hud::actualize_weapons(void)
{
	int					n = 0;

	tab_menu.clear();
	while (n < weapon_list.size())
	{
		if (WEAPON::HAS_PED_GOT_WEAPON(player->getId(), Utilities::get_hash(weapon_list[n]), false))
			tab_menu.add_entry(weapon_list[n], &Hud::weapon_submenu, player, &weapon_list[n]);
		n++;
	}
}

void		equip_weapon(void *data, const void *cdata)
{
	PedStatus			*player = static_cast<PedStatus*>(data);
	const std::string	*str = static_cast<const std::string*>(cdata);
	WEAPON::SET_CURRENT_PED_WEAPON(player->getId(), Utilities::get_hash(*str), true);
	Menu::hide(Menu::currentMenu);
}

void		drop_weapon(void *data, const void *cdata)
{
	PedStatus			*player = static_cast<PedStatus*>(data);
	const std::string	*str = static_cast<const std::string*>(cdata);
	Inventory			inv;

	WEAPON::REMOVE_WEAPON_FROM_PED(player->getId(), Utilities::get_hash(*str));
	inv.addObject(new InventoryObject_Weapon(*str));
	ContainerManager::getCurrent()->addCrate(true, inv, player->getPos());
	Menu::hide(Menu::currentMenu);
}

void		ammo_weapon(void *data, const void *cdata)
{
	PedStatus			*player = static_cast<PedStatus*>(data);
	const std::string	*str = static_cast<const std::string*>(cdata);
	Inventory			inv;
	Any					type = WEAPON::_GET_PED_AMMO_TYPE(player->getId(), Utilities::get_hash(*str));

	inv.addObject(new InventoryObject_Ammo(get_ammo_by_weapon(*str).name, *str, WEAPON::GET_PED_AMMO_BY_TYPE(player->getId(), WEAPON::_GET_PED_AMMO_TYPE(player->getId(), Utilities::get_hash(*str)))));
	WEAPON::SET_PED_AMMO(player->getId(), Utilities::get_hash(*str), 0);
	ContainerManager::getCurrent()->addCrate(true, inv, player->getPos());
	Menu::hide(Menu::currentMenu);
}

void					Hud::weapon_submenu(void *data, const void *cdata)
{
	Menu				*submenu = new Kamikaze_Menu();
	PedStatus			*player = static_cast<PedStatus*>(data);
	const std::string	*str = static_cast<const std::string*>(cdata);

	submenu->add_entry(DROP_STRING, &drop_weapon, data, cdata);
	submenu->add_entry(get_ammo_by_weapon(*str).name 
			+ "x" + Utilities::xToString<int>(WEAPON::GET_PED_AMMO_BY_TYPE(player->getId(), WEAPON::_GET_PED_AMMO_TYPE(player->getId(), Utilities::get_hash(*str)))),
			&ammo_weapon, data, cdata);
	submenu->add_entry(EQUIP_STRING, &equip_weapon, data, cdata);
	Menu::show(submenu);
}
