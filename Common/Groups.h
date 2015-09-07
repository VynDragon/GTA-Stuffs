#pragma once

#include "..\inc\natives.h"
#include "..\inc\types.h"
#include "..\inc\enums.h"
#include "Utilities.h"
#include <string>

class Groups
{
public:
	static Hash	wanderer, guard, zombie;
	static void	init(void)
	{
		Hash		hash;

		//Zombie
		PED::ADD_RELATIONSHIP_GROUP("ZOMBIE", &hash);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(RelationshipHate, hash, Utilities::get_hash("PLAYER"));
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(RelationshipHate, Utilities::get_hash("PLAYER"), hash);
		zombie = hash;


		//Wanderer
		PED::ADD_RELATIONSHIP_GROUP("WANDERER", &hash);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(RelationshipHate, hash, Utilities::get_hash("COUGAR"));
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(RelationshipHate, Utilities::get_hash("COUGAR"), hash);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(RelationshipHate, hash, zombie);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(RelationshipHate, zombie, hash);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(RelationshipCompanion, hash, Utilities::get_hash("PLAYER"));
		wanderer = hash;

		//Guard
		PED::ADD_RELATIONSHIP_GROUP("GUARD", &hash);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(RelationshipCompanion, wanderer, hash);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(RelationshipCompanion, hash, wanderer);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(RelationshipHate, hash, Utilities::get_hash("COUGAR"));
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(RelationshipHate, Utilities::get_hash("COUGAR"), hash);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(RelationshipHate, hash, zombie);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(RelationshipHate, zombie, hash);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(RelationshipCompanion, hash, Utilities::get_hash("PLAYER"));
		guard = hash;

		//Player
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(RelationshipCompanion, Utilities::get_hash("PLAYER"), guard);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(RelationshipCompanion, Utilities::get_hash("PLAYER"), wanderer);
	}
};