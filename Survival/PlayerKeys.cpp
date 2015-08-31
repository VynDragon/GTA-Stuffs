#include "PlayerKeys.h"
#include "PedStatus.h"
#include "Defines.h"
#include "..\Common\Keys.h"
#include "OptionManager.h"



PlayerKeys::PlayerKeys()
{
}


PlayerKeys::~PlayerKeys()
{
}

int					PlayerKeys::tick(PedStatus *player)
{
	unsigned int	i = 0;
	Vector3			pos = player->getPos();

	if (KeyManager::keys.justPressed(Keys::find_key_by_name(OptionManager::options.openInventoryKey).vkey))
		Menu::show(new InventoryMenu(player->getInventory(), player));
	while (containers[i] != "")
	{
		Object obj = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(pos.x, pos.y, pos.z, 0.7f, Utilities::get_hash(containers[i]), false, false, true);
		if (obj)
		{
			if (player->getInventory()->size() >= 20)
			{
				Utilities::notify("Inventory Full");
				return (0);
			}
			std::string buff = "Press ";
			Keys::hhb_vkey kek = Keys::find_key_by_name(OptionManager::options.openCrateKey);
			buff += kek.description;
			buff += " to open Crate";
			Utilities::notify(buff);
			if (KeyManager::keys.justPressed(kek.vkey))
				ContainerManager::getCurrent()->openCrate(obj, player->getInventory());
		}
		i++;
	}
	return (0);
}