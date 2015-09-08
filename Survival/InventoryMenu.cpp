#include "InventoryMenu.h"
#include "Defines.h"
#include "ContainerManager.h"
#include "PedStatus.h"

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

void		use_thing(void *dat, const void *cdata)
{
	InventoryMenu::data	*data = static_cast<InventoryMenu::data*>(dat);

	data->inv->use(data->index, data->player);
	delete data;
	Menu::hide(Menu::currentMenu);
}

void		drop(void *dat, const void *cdata)
{
	InventoryMenu::data	*data = static_cast<InventoryMenu::data*>(dat);
	InventoryMenu		*m = data->menu;
	Inventory			inv;

	inv.addObject((*data->index)->clone());
	ContainerManager::getCurrent()->addCrate(true, inv, data->player->getPos());
	data->inv->remove(data->index);
	delete data;
	Menu::hide(Menu::currentMenu);
}

void				InventoryMenu::use(void *dat, const void *cdata) 
{
	InventoryMenu::data	*data = static_cast<InventoryMenu::data*>(dat);
	InventoryMenu	*m = data->menu;
	Kamikaze_Menu	*sub = new Kamikaze_Menu();

	sub->add_entry(USE_STRING, &use_thing, dat, cdata);
	sub->add_entry(DROP_STRING, &drop, dat, cdata);
	Menu::hide(Menu::currentMenu);
	Menu::show(sub);
}