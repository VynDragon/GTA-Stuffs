#pragma once
#include "PedStatus.h"
#include "InventoryObject.h"
#include "PlayerKeys.h"


class PlayerStatus : public PedStatus
{
public:
	PlayerStatus(Player playerId);
	~PlayerStatus();
	Player				getPlayer(void) { return (this->playerId); }
	int					tick(void);
protected:
	PlayerKeys			pkeys;
	Player				playerId;
	DWORD				timesave;
};

