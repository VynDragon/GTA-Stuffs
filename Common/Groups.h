#pragma once

#include "..\inc\natives.h"
#include "..\inc\types.h"
#include "..\inc\enums.h"
#include "Utilities.h"
#include <string>

class Groups
{
public:
	static Hash	wanderer, guard;
	static void	init(void)
	{
		Hash		hash;

		//Wanderer
		PED::ADD_RELATIONSHIP_GROUP("WANDERER", &hash);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(RelationshipHate, hash, Utilities::get_hash("COUGAR"));
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(RelationshipHate, Utilities::get_hash("COUGAR"), hash);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(RelationshipCompanion, hash, Utilities::get_hash("GUARD"));
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(RelationshipCompanion, hash, Utilities::get_hash("PLAYER"));
		wanderer = hash;

		//Guard
		PED::ADD_RELATIONSHIP_GROUP("GUARD", &hash);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(RelationshipCompanion, hash, Utilities::get_hash("WANDERER"));
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(RelationshipHate, hash, Utilities::get_hash("COUGAR"));
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(RelationshipHate, Utilities::get_hash("COUGAR"), hash);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(RelationshipCompanion, hash, Utilities::get_hash("PLAYER"));
		guard = hash;

		//Player
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(RelationshipCompanion, Utilities::get_hash("PLAYER"), Utilities::get_hash("GUARD"));
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(RelationshipCompanion, Utilities::get_hash("PLAYER"), Utilities::get_hash("WANDERER"));
	}
};