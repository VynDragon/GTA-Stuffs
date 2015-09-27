
#include "..\Common\script.h"
#include "..\Common\keyboard.h"
#include "..\Common\Utilities.h"
#include "..\Common\Keys.h"
#include <sstream>
#include <vector>

#define NBENTRIES 20
#define TIMEOUT 2000

void main(DWORD	key)
{
	int	lastseat = 0;
	while (true)
	{
		if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), true) && IsKeyJustUp(key))
		{
			Vehicle vehicles[512];
			int		count = worldGetAllVehicles(vehicles, 512);
			int		closest = 0;
			float	lastdistance = (std::numeric_limits<float>::max)();
			for (int i = 0; i < count; i++)
			{
				if (ENTITY::DOES_ENTITY_EXIST(vehicles[i]))
				{
					Vector3	a = ENTITY::GET_ENTITY_COORDS(vehicles[i], true), b = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
					float dist = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(a.x, a.y, a.z, b.x, b.y, b.z, true);
					if (dist < lastdistance)
					{
						lastdistance = dist;
						closest = vehicles[i];
					}
				}
			}
			if (closest != 0)
			{
				int		i = 0;
				bool	done = false;
				while (i < VEHICLE::GET_VEHICLE_MAX_NUMBER_OF_PASSENGERS(closest) && !done)
				{
					if (VEHICLE::IS_VEHICLE_SEAT_FREE(closest, i))
					{

						AI::TASK_ENTER_VEHICLE(PLAYER::PLAYER_PED_ID(), closest, TIMEOUT, i, 2.0f, 1, 0);
						lastseat = i;
						done = true;
					}
					i++;
				}
				//PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), closest, -2);
			}
		}
		else if (IsKeyJustUp(key))
		{
			Vehicle		veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
			if (ENTITY::DOES_ENTITY_EXIST(veh))
			{
				int		i = lastseat;
				bool	done = false;
				while (i < VEHICLE::GET_VEHICLE_MAX_NUMBER_OF_PASSENGERS(veh) && !done)
				{
					if (VEHICLE::IS_VEHICLE_SEAT_FREE(veh, i))
					{
						PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, i);
						lastseat = i;
						done = true;
					}
					i++;
				}
				if (i >= VEHICLE::GET_VEHICLE_MAX_NUMBER_OF_PASSENGERS(veh))
				{
					int		i = -1;
					while (i < VEHICLE::GET_VEHICLE_MAX_NUMBER_OF_PASSENGERS(veh) && !done)
					{
						if (VEHICLE::IS_VEHICLE_SEAT_FREE(veh, i))
						{
							PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, i);
							lastseat = i;
							done = true;
						}
						i++;
					}
				}
			}
		}
		WAIT(0);
	}
}

void			ScriptMain()
{
	std::string	key;
	DWORD		keyCode;

	key = Utilities::getFromFile<std::string>(".\\Options.ini", "Options", "passengerKey", "W key");
	if (key == "W key")
		Utilities::writeToFile<std::string>(".\\Options.ini", "Options", "passengerKey", "W key");
	keyCode = Keys::find_key_by_name(key).vkey;
	main(keyCode);
}
