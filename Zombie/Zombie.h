#pragma once

#include "..\Common\script.h"

#define MAX_ZOMBIE_BASE 110.0f
#define ZOMBIE_CLEANING_DISTANCE 150.0f
#define MAX_ZOMBIES_PER_FRAME 30
#define MAX_ZOMBIE_DELETION_PER_FRAME 30
#define	HEAR_RANGE ZOMBIE_CLEANING_DISTANCE / 3.0f

namespace Zombie
{
	void			zombify_nearby_ped(Ped id);
	void			actualize_attacks(void);
	void			place_random_zombie_near(Vector3 point, float mulZombie);
	unsigned int	getZombieNumber(void);
	void			ZombieMain(void);
}