#include "InventoryMenu.h"

// this is so insecure... if the inventory is modified while the menu is open, we're in for problems.

InventoryMenu::InventoryMenu(Inventory *inv, PedStatus *player)
{
	auto	it = inv->begin();

	while (it != inv->end())
	{
		this->add_entry((*it)->getName(), InventoryMenu::use, &((*new InventoryMenu::data) = { player, it, inv, this }), NULL);
		it++;
	}
}


InventoryMenu::~InventoryMenu()
{
}

void				InventoryMenu::use(void *dat, const void *cdata) 
{
	InventoryMenu::data	*data = static_cast<InventoryMenu::data*>(dat);
	InventoryMenu	*m = data->menu;
	data->inv->use(data->index, data->player);
	delete data;
	Menu::hide(m);
}