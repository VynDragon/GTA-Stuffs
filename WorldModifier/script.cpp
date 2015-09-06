
#include "..\Common\script.h"
#include "..\Common\keyboard.h"
#include "..\Common\Utilities.h"
#include <sstream>
#include <fstream>
#include <vector>
#include <Psapi.h>
#define PI 3.14f


/* COPYRIGHT FOR THE SNOW SIGNATURES:
	* Copyright (C) GTA:Multiplayer Team (https://wiki.gta-mp.net/index.php/Team)
	*
	* Redistribution and use in source and binary forms, with or without
	* modification, are permitted provided that the following conditions are
	* met:
	*
	*     * Redistributions of source code must retain the above copyright
	*		notice, this list of conditions and the following disclaimer.
	*     * Redistributions in binary form must reproduce the above
	*		copyright notice, this list of conditions and the following disclaimer
	*		in the documentation and/or other materials provided with the
	*		distribution.
	*     * Neither the name of GTA-Network nor the names of its
	*		contributors may be used to endorse or promote products derived from
	*		this software without specific prior written permission.
	*
	* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES INCLUDING, BUT NOT
	* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	* DATA, OR PROFITS; OR BUSINESS INTERRUPTION HOWEVER CAUSED AND ON ANY
	* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	* INCLUDING NEGLIGENCE OR OTHERWISE ARISING IN ANY WAY OUT OF THE USE
	* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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

void main(bool snow, bool xmasWeather)
{
	DWORD		actualizeWeatherTime = GetTickCount();
	DWORD		actualizeVehicules = GetTickCount();
	GAMEPLAY::CLEAR_OVERRIDE_WEATHER();
	/*uintptr_t	ptr = (uintptr_t)GetModuleHandle(nullptr) + (unsigned int)0x01E60020;
	uintptr_t	tmp = 0;
	/*std::string	txt = "ptr is:" + Utilities::xToString<unsigned long long>(ptr) + "\n Ptr at ptr is :" + Utilities::xToString<unsigned long long>(*(uintptr_t*)ptr) + "\n";
	log_text(txt);
	ReadProcessMemory(GetCurrentProcess(), (void*)(*(uintptr_t*)ptr + 0x90), &l, sizeof(float), NULL);
	txt = "value is:";
	txt += Utilities::xToString<float>(l);
	txt += "\n";
	log_text(txt);*/
	//tmp = *((uintptr_t*)ptr + 0x2C8);
	//ptr = *((uintptr_t*)tmp + 0x90);
	/*float	fov;
	ReadProcessMemory(GetCurrentProcess(), (void*)(tmp + 0x90), &fov, sizeof(float), NULL);
	std::string	txt = Utilities::xToString<unsigned long long>(ptr) + "\n";
	log_text(txt);
	txt = Utilities::xToString<float>(fov) + "\n";
	log_text(txt);*/
	//VirtualProtect(fov, sizeof(float), PAGE_EXECUTE_READWRITE, nullptr);
	/*nativeInit(0x10FAB35428CCC9D7);
	uintptr_t	ptr = (uintptr_t)nativeCall;
	Utilities::log_text(Utilities::xToString<void*>((void*)(ptr)) + "\n");
	for (uintptr_t i = (uintptr_t)ptr; i < (uintptr_t)ptr + 800; i += 8)
		Utilities::log_text(Utilities::xToString<void*>((void*)(*(uintptr_t*)i)));*/
	UI::DISPLAY_RADAR(true);
	UI::DISPLAY_HUD(true);
	while (true)
	{
		//*fov = 70.0f;
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
		STREAMING::SET_VEHICLE_POPULATION_BUDGET(0);
		PLAYER::CLEAR_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID());
		PLAYER::SET_MAX_WANTED_LEVEL(0);
		GRAPHICS::DISABLE_VEHICLE_DISTANTLIGHTS(true);
		if (actualizeWeatherTime < GetTickCount())
		{
			if (snow)
			{
				GRAPHICS::_SET_BLACKOUT(true);
				if (xmasWeather)
				{
					GAMEPLAY::SET_OVERRIDE_WEATHER("XMAS");
					GAMEPLAY::SET_WEATHER_TYPE_NOW_PERSIST("XMAS");
				}
				else
				{
					GAMEPLAY::SET_OVERRIDE_WEATHER("BLIZZARD");
					GAMEPLAY::SET_WEATHER_TYPE_NOW_PERSIST("BLIZZARD");
				}
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
		//Utilities::putText(Utilities::xToString<float>(CAM::GET_GAMEPLAY_CAM_FOV()), 0.5f, 0.5f);
		Vehicle vehicles[1024];
		int count = worldGetAllVehicles(vehicles, 1024);
		for (int i = 0; i < count; i++)
		{
			Vector3	pos = ENTITY::GET_ENTITY_COORDS(vehicles[i], true);
			Vector3 forward = ENTITY::GET_ENTITY_FORWARD_VECTOR(vehicles[i]);
			Any		on, rail;
			Vector3	left;
			left.x = forward.y * 0.8f;
			left.y = -forward.x * 0.8f;
			left.z = forward.z * 0.8f;
			Vector3	newforward = forward;
			newforward.z = newforward.z - 0.1f;
			VEHICLE::GET_VEHICLE_LIGHTS_STATE(vehicles[i], &on, &rail);
			if (!VEHICLE::_IS_HEADLIGHT_L_BROKEN(vehicles[i]) && on)
				GRAPHICS::DRAW_SPOT_LIGHT(pos.x - left.x + forward.x, pos.y - left.y + forward.y, pos.z - left.z + forward.z, newforward.x, newforward.y, newforward.z, 255, 255, 255, 25.0f, 5.0f, 5.0f, 15.0f, 1.0f);
			if (!VEHICLE::_IS_HEADLIGHT_R_BROKEN(vehicles[i]) && on)
				GRAPHICS::DRAW_SPOT_LIGHT(pos.x + left.x + forward.x, pos.y + left.y + forward.y, pos.z + left.z + forward.z, newforward.x, newforward.y, newforward.z, 255, 255, 255, 25.0f, 5.0f, 5.0f, 15.0f, 0.5f);
			/*VEHICLE::SET_VEHICLE_LIGHT_MULTIPLIER(vehicles[i], 1.0f);
			VEHICLE::SET_VEHICLE_LIGHTS(vehicles[i], 2);*/
		}
		/*Entity	entity;
		if (PLAYER::_GET_AIMED_ENTITY(PLAYER::PLAYER_ID(), &entity))
		{
			Utilities::putText(Utilities::xToString<Hash>(ENTITY::GET_ENTITY_MODEL(entity)), 0.5f, 0.5f);
		}*/
		//Utilities::putText(Utilities::xToString<float>(plcoords.x) + "," + Utilities::xToString<float>(plcoords.y) + "," + Utilities::xToString<float>(plcoords.z), 0.5f, 0.5f);
		WAIT(0);
	}
}

void ScriptMain()
{
	bool		snow = false, xmasWeather = false;
	memory_data	data;
	srand(GetTickCount());
	snow = Utilities::getFromFile<bool>(".\\Options.ini", "Options", "snow", true);
	if (snow)
		WritePrivateProfileString("Options", "snow", "1", ".\\options.ini");
	xmasWeather = Utilities::getFromFile<bool>(".\\Options.ini", "Options", "xmasWeather", false);
	if (!xmasWeather)
		Utilities::writeToFile<bool>(".\\Options.ini", "Options", "xmasWeather", false);
	GET_THE_FUCKING_SNOW_MATE(&data, snow);
	main(snow, xmasWeather);
}
