#pragma once

#include "..\Common\script.h"
#include "Inventory.h"

class ContainerManager
{
public:
	 ContainerManager();
	 ~ContainerManager();
	typedef struct {
		bool		inventoried;
		Inventory	inventory;
		Vector3		position;
		Object		object;
		int			blip;
		bool		respawn;
		DWORD		respawnTimer;
	} Crate;
	 static ContainerManager	*current;
	 static void				setCurrent(ContainerManager *cm) { current = cm; }
	 static ContainerManager	*getCurrent(void) { return (current); }
	 void					openCrate(Object crate, Inventory *inv);
	//static void				openInventoriedCrate(Crate crate, Inventory *inv);
	 void					addCrate(bool inventoried, Inventory inventory, Vector3 position);
	 void					addCrate_permanent(bool inventoried, Inventory inventory, Vector3 position);
	 Crate					readFromFile(const std::string file, const std::string &sect);
	 int						writeToFile(const std::string file, const std::string &sect, ContainerManager::Crate crate);
	 void					load(void);
	 void					tick(Vector3 pos);
protected:
	std::vector<Crate>		unloaded;
	std::vector<Crate>		loaded;
	unsigned int			last;
};

