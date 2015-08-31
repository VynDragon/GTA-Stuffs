#pragma once
#include "..\Common\script.h"
#include "Inventory.h"
#include "..\Common\Kamikaze_Menu.h"

class InventoryMenu : public Kamikaze_Menu
{
public:
	InventoryMenu(Inventory *inv, PedStatus *player);
	~InventoryMenu();
	typedef struct {
		PedStatus								*player;
		std::vector<InventoryObject*>::iterator	index;
		Inventory								*inv;
		InventoryMenu							*menu;
	} data;
	static void		use(void *data, const void *cdata);
};

