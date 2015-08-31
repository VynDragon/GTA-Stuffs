#pragma once

#include "..\Common\script.h"
#include "PedStatus.h"
#include "PlayerStatus.h"
#include "..\Common\Menu.h"
#include "..\Common\Kamikaze_Menu.h"
#include "..\Common\Weapons.h"

class Hud
{
public:
	Hud(PlayerStatus *player);
	~Hud();
	void			draw();
	void			actualize_weapons();
	void			tick();
	static void		weapon_submenu(void *, const void*);
protected:
	PlayerStatus	*player;
	float			x;
	float			y;
	Menu			tab_menu;
	DWORD			weapon_timer;
};

