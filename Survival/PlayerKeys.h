#pragma once

#include "..\Common\script.h"
#include "InventoryMenu.h"
#include "Container.h"
#include "ContainerManager.h"

class PedStatus;

class PlayerKeys
{
public:
	PlayerKeys();
	~PlayerKeys();
	int		tick(PedStatus *player);
};