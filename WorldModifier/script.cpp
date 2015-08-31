
#include "..\Common\script.h"
#include "..\Common\keyboard.h"
#include <sstream>
#include <fstream>
#include <vector>
#include <Psapi.h>


/* heavily 'inspired' from here: https://development.crix-dev.com/gtamp-mods/sp-snow-mod/blob/master/src/Main.cpp
*	Thanks to whoever wrote that.
*/

void		log_text(const std::string &str)
{
	std::fstream file("log.txt", std::ios_base::out | std::ios_base::in | std::ios_base::app);

	file << str;
	file.close();
}

intptr_t						get_address_by_pattern_text(uint8_t *pattern, uint8_t *mask, uint32_t length) // function that scan the memory for a pattern, it scan by uint8_t (byte by byte)
{
	uintptr_t					current = NULL;
	uintptr_t					base_address = (uintptr_t)GetModuleHandle(nullptr);
	uintptr_t					size = NULL;
	MODULEINFO					info;

	GetModuleInformation(GetCurrentProcess(), (HMODULE)base_address, &info, sizeof(MODULEINFO));
	size = info.SizeOfImage;
	current = base_address;
	while (current < base_address + size)
	{
		uint32_t	i = 0;
		while (i < length && (((uint8_t*)(current))[i] == pattern[i] || mask[i] == 0))
			i++;
		if (i >= length)
			return (current);
		current = current + 1;
	}
	return (0);
}

intptr_t							get_address_by_pattern_heap(uint8_t *pattern, uint8_t *mask, uint32_t length) // function that scan the memory for a pattern, it scan by uint8_t (byte by byte)
{
	uintptr_t						current = NULL;
	uintptr_t						base_address = (uintptr_t)GetProcessHeap();
	uintptr_t						size = NULL;
	PROCESS_HEAP_ENTRY				Entry;

	Entry.lpData = NULL;
	while (HeapWalk((HANDLE)base_address, &Entry) != FALSE)
	{
		if ((Entry.wFlags & PROCESS_HEAP_ENTRY_BUSY) != 0)
		{
			current = (uintptr_t)Entry.lpData;
			while (current < (uintptr_t)Entry.lpData + (uintptr_t)Entry.cbData)
			{
				uint32_t	i = 0;
				while (i < length && (((uint8_t*)(current))[i] == pattern[i] || mask[i] == 0))
					i++;
				if (i >= length)
					return (current);
				current = current + 1;
			}
		}
	}
	return (0);
}


void	GET_THE_FUCKING_SNOW_MATE(memory_data *data, bool enable) // MAKE IT RAIN
{
	intptr_t	a, b;

	a = get_address_by_pattern_text((uint8_t*)"\x80\x3D\x00\x00\x00\x00\x00\x74\x27\x84\xC0", (uint8_t*)"\xFF\xFF\x00\x00\x00\x00\x00\xFF\xFF\xFF\xFF", 11); // shamelessly stole addresses :/
	b = get_address_by_pattern_text((uint8_t*)"\x44\x38\x3D\x00\x00\x00\x00\x74\x0F", (uint8_t*)"\xFF\xFF\xFF\x00\x00\x00\x00\xFF\xFF", 9);

	if (a && b)
	{
		VirtualProtect((void*)a, 13, PAGE_EXECUTE_READWRITE, nullptr);
		VirtualProtect((void*)b, 14, PAGE_EXECUTE_READWRITE, nullptr);
		data->a = a;
		data->b = b;
		memcpy(data->mem_a, (void*)a, 13); // gotta save that stuffs, may want to restore them later
		memcpy(data->mem_b, (void*)b, 14);
		// and now for the magic
		if (enable)
		{
			memset((void*)a, 0x90, 13);
			memset((void*)b, 0x90, 14);
			GRAPHICS::_TOGGLE_SNOW_VEHICLE_TRACKS(true);
			GRAPHICS::_TOGGLE_SNOW_PED_FOOT_TRACKS(true);
		}
		else
		{
			memcpy((void*)a, data->mem_a, 13);
			memcpy((void*)b, data->mem_b, 13);
		}
	}
}

void main(bool snow)
{
	DWORD		actualizeWeatherTime = GetTickCount();
	DWORD		actualizeVehicules = GetTickCount();
	GAMEPLAY::CLEAR_OVERRIDE_WEATHER();
	while (true)
	{
		/*STREAMING::REQUEST_NAMED_PTFX_ASSET("core_snow");
		GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("core_snow");
		GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("core_snow", PLAYER::PLAYER_PED_ID(), 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, false, false, false);*/
		VEHICLE::SET_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(0.0f); // disable traffic
		PED::SET_PED_DENSITY_MULTIPLIER_THIS_FRAME(0.0f); // disable random peds
		//VEHICLE::SET_ALL_VEHICLE_GENERATORS_ACTIVE_IN_AREA(plcoords.x - 500.0f, plcoords.y - 500.0f, plcoords.z - 500.0f, plcoords.x + 500.0f, plcoords.y + 500.0f, plcoords.z + 500.0f, false, false); // seems to be disabling vehicle generators
		Vector3 plcoords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID()), true);
		VEHICLE::REMOVE_VEHICLES_FROM_GENERATORS_IN_AREA(plcoords.x - 500.0f, plcoords.y - 500.0f, plcoords.z - 500.0f, plcoords.x + 500.0f, plcoords.y + 500.0f, plcoords.z + 500.0f);
		PLAYER::SET_POLICE_IGNORE_PLAYER(PLAYER::PLAYER_ID(), true);
		PED::SET_CREATE_RANDOM_COPS(false);
		VEHICLE::SET_RANDOM_TRAINS(false);
		VEHICLE::SET_RANDOM_BOATS(false);
		VEHICLE::SET_GARBAGE_TRUCKS(false);
		PLAYER::CLEAR_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID());
		GRAPHICS::DISABLE_VEHICLE_DISTANTLIGHTS(true);
		if (actualizeWeatherTime < GetTickCount())
		{
			if (snow)
			{
				GRAPHICS::_SET_BLACKOUT(true);
				GAMEPLAY::SET_OVERRIDE_WEATHER("BLIZZARD");
				GAMEPLAY::SET_WEATHER_TYPE_NOW_PERSIST("BLIZZARD");
				GAMEPLAY::_SET_CLOUD_HAT_TRANSITION("Snowy 01", 0);
			}
			else
			{
				GAMEPLAY::SET_OVERRIDE_WEATHER("FOGGY");
				GAMEPLAY::SET_WEATHER_TYPE_NOW_PERSIST("FOGGY");
				GAMEPLAY::_SET_CLOUD_HAT_TRANSITION("Wispy", 0);
				GRAPHICS::_SET_BLACKOUT(false);
			}
			GAMEPLAY::SET_WIND(1.0);
			GAMEPLAY::SET_WIND_SPEED(25.0f);
			GAMEPLAY::SET_WIND_DIRECTION((float)(rand() % 360));
			actualizeWeatherTime = GetTickCount() + 30000;
		}
		/*if (actualizeVehicules < GetTickCount())
		{
			Vehicle vehicles[1024];
			int count = worldGetAllVehicles(vehicles, 1024);
			for (int i = 0; i < count; i++)
			{
				Vector3	pos = ENTITY::GET_ENTITY_COORDS(vehicles[i], true);
				Vector3 forward = ENTITY::GET_ENTITY_FORWARD_VECTOR(vehicles[i]);
				//GRAPHICS::DRAW_SPOT_LIGHT(pos.x, pos.y, pos.z + 1.0f, forward.x, forward.y, forward.z, 255, 255, 255, 100.0f, 1.0f, 1.0f, 13.0f, 1.0f);
				VEHICLE::SET_VEHICLE_LIGHT_MULTIPLIER(vehicles[i], 1.0f);
				VEHICLE::SET_VEHICLE_LIGHTS(vehicles[i], 2);
			}
			actualizeVehicules = GetTickCount() + 500;
		}*/
		WAIT(0);
	}
}

void ScriptMain()
{
	bool		snow = false;
	memory_data	data;
	srand(GetTickCount());
	char	buff[10];
	GetPrivateProfileString("Options", "snow", "1", buff, 9, ".\\options.ini");
	if (!strcmp("1", buff))
	{
		WritePrivateProfileString("Options", "snow", "1", ".\\options.ini");
		snow = true;
	}
	GET_THE_FUCKING_SNOW_MATE(&data, snow);
	main(snow);
}
