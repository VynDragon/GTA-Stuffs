
#include "..\Common\script.h"
#include "..\Common\Utilities.h"
#include "..\Common\keyboard.h"
#include <sstream>
#include <vector>

#define	TPREGENTIME 5000
#define PAINDELAY 150;

Ped		setupPed(Vector3 pos, int maxHealth = 2000)
{
	DWORD attachhash = Utilities::get_hash("a_c_mtlion");
	if (STREAMING::IS_MODEL_VALID(attachhash)) {
		STREAMING::REQUEST_MODEL(attachhash);
		while (!STREAMING::HAS_MODEL_LOADED(attachhash))
			WAIT(0);

		Ped	ped = PED::CREATE_PED(26, attachhash, pos.x, pos.y, pos.z + 1.0f, (float)(rand() % 360), true, false);
		PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, 1);
		PED::SET_PED_RELATIONSHIP_GROUP_HASH(ped, Utilities::get_hash("COUGAR"));
		PED::SET_PED_FLEE_ATTRIBUTES(ped, 0, 0);
		PED::SET_PED_COMBAT_ATTRIBUTES(ped, 17, 1);
		PED::SET_PED_COMBAT_ATTRIBUTES(ped, 46, TRUE);
		PED::SET_PED_COMBAT_ABILITY(ped, 2);
		PED::SET_PED_COMBAT_MOVEMENT(ped, 3);
		AI::CLEAR_PED_TASKS(ped);
		AI::CLEAR_PED_SECONDARY_TASK(ped);
		PED::SET_PED_KEEP_TASK(ped, false);
		Vector3	tPos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
		AI::TASK_GO_STRAIGHT_TO_COORD(ped, tPos.x, tPos.y, tPos.z, 2.0f, -1, (float)(rand() % 360), 0.0f);
		PED::SET_PED_KEEP_TASK(ped, true);
		PED::SET_PED_COMBAT_ABILITY(ped, 2);
		PED::SET_PED_AS_ENEMY(ped, true);
		//PED::SET_PED_CAN_SMASH_GLASS(ped, true, true);
		//PED::REGISTER_HATED_TARGETS_AROUND_PED(ped, 50.0f);
		PED::SET_PED_ARMOUR(ped, 100);
		ENTITY::SET_ENTITY_MAX_HEALTH(ped, maxHealth);
		ENTITY::SET_ENTITY_HEALTH(ped, maxHealth);
		ENTITY::SET_ENTITY_ALPHA(ped, 52, true);
		OutputDebugString("Spawned Beast");
		return (ped);
	}
	return (0);
}

Ped	teleportTick(DWORD *timer, Ped ped)
{
	if (PLAYER::IS_PLAYER_FREE_AIMING_AT_ENTITY(PLAYER::PLAYER_ID(), ped) && *timer < GetTickCount())
	{
		Vector3	togo;
		Vector3	ppos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
		Vector3	fp = ENTITY::GET_ENTITY_FORWARD_VECTOR(PLAYER::PLAYER_PED_ID());
		togo.x = ppos.x - 30.0f * fp.x;
		togo.y = ppos.y - 30.0f * fp.y;
		togo.z = ppos.z;
		//PATHFIND::GET_SAFE_COORD_FOR_PED(togo.x, togo.y, ppos.z, false, &out, 0);
		//ENTITY::SET_ENTITY_COORDS(ped, out.x, out.y, out.z, true, true, true, true);
		Ped newe = setupPed(togo, ENTITY::GET_ENTITY_HEALTH(ped));
		PED::DELETE_PED(&ped);
		ped = newe;
		return (ped);
		*timer = GetTickCount() + TPREGENTIME;
	}	
	return (ped);
}

void main(void)
{
	Ped		ped = 0;
	DWORD	timer = GetTickCount(), damageTimer = GetTickCount();

	while (true)
	{
		Vector3	ppos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
		if (INTERIOR::GET_INTERIOR_FROM_COLLISION(ppos.x, ppos.y, ppos.z) && ped == 0)
		{
			Vector3	dec = ppos;
			dec.x += rand() % 40 - 20;
			dec.y += rand() % 40 - 20;
			ped = setupPed(dec);
		}
		if (ped != 0)
		{
			Vector3	bpos = ENTITY::GET_ENTITY_COORDS(ped, true);
			if (!PED::_IS_PED_DEAD(ped, true))
			{
				if (GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(bpos.x, bpos.y, bpos.z, ppos.x, ppos.y, ppos.z, true) > 40.0f)
				{
					Vector3	togo;
					Vector3	ppos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
					Vector3	fp = ENTITY::GET_ENTITY_FORWARD_VECTOR(PLAYER::PLAYER_PED_ID());
					togo.x = ppos.x - 30.0f * fp.x;
					togo.y = ppos.y - 30.0f * fp.y;
					togo.z = ppos.z;
					//PATHFIND::GET_SAFE_COORD_FOR_PED(togo.x, togo.y, ppos.z, false, &out, 0);
					//ENTITY::SET_ENTITY_COORDS(ped, out.x, out.y, out.z, true, true, true, true);
					Ped newe = setupPed(togo, ENTITY::GET_ENTITY_HEALTH(ped));
					PED::DELETE_PED(&ped);
					ped = newe;
				}
				else
				{
					ped = teleportTick(&timer, ped);
				}
				if (PED::IS_PED_STOPPED(ped))
					AI::TASK_GO_STRAIGHT_TO_COORD(ped, ppos.x, ppos.y, ppos.z, 2.0f, -1, (float)(rand() % 360), 0.0f);
				if (GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(bpos.x, bpos.y, bpos.z, ppos.x, ppos.y, ppos.z, true) < 1.5f && damageTimer < GetTickCount())
				{
					ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), ENTITY::GET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID()) - 1);
					AUDIO::PLAY_PAIN(PLAYER::PLAYER_PED_ID(), 6, 0);
					damageTimer = GetTickCount() + PAINDELAY;
				}
			}
			else
			{
				PED::DELETE_PED(&ped);
				ped = 0;
			}
			if (GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(bpos.x, bpos.y, bpos.z, ppos.x, ppos.y, ppos.z, true) > 100.0f || !INTERIOR::GET_INTERIOR_FROM_COLLISION(bpos.x, bpos.y, bpos.z))
			{
				PED::DELETE_PED(&ped);
				ped = 0;
			}
		}
		
		WAIT(0);
	}
}

void ScriptMain()
{
	main();
}
