#pragma once
#include "..\Common\script.h"
#include "InventoryObject.h"

class Inventory
{
public:
	Inventory();
	Inventory(const Inventory &inv);
	~Inventory();
	static const Inventory							emptyInventory;
	void											addObject(InventoryObject *object) { list.push_back(object); }
	void											remove(std::vector<InventoryObject*>::iterator it) { delete (*it); list.erase(it); }
	std::vector<InventoryObject*>::iterator			begin() { return(list.begin()); }
	std::vector<InventoryObject*>::iterator			end() { return(list.end()); }
	std::vector<InventoryObject*>::const_iterator	begin() const { return(list.cbegin()); }
	std::vector<InventoryObject*>::const_iterator	end() const { return(list.cend()); }
	int												use(std::vector<InventoryObject*>::iterator it, PedStatus *ped);
	std::string										toString(void);
	int												fromString(const std::string &str);
	Inventory										&operator=(const Inventory &inv);
	unsigned int									size(void) const { return((unsigned int)list.size()); }
private:
	std::vector<InventoryObject*>	list;
};

