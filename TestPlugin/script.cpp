/*
	THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
				http://dev-c.com			
			(C) Alexander Blade 2015
*/

/*
	F4					activate
	NUM2/8/4/6			navigate thru the menus and lists (numlock must be on)
	NUM5 				select
	NUM0/BACKSPACE/F4 	back
	NUM9/3 				use vehicle boost when active
	NUM+ 				use vehicle rockets when active
*/

#include "script.h"

/*void	appartment()
{
	if (ENTITY::IS_ENTITY_AT_COORD(PLAYER::PLAYER_PED_ID(), 328.47f, -1258.45f, 31.53f, 5.0f, 5.0f, 5.0f, 0, 1, 0))
	{
		AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET", 0);
	}
}*/

std::fstream	*init(void)
{
	std::fstream	*file = new std::fstream("houses.nfs", std::fstream::in | std::fstream::out | std::fstream::app);

	if (!file->good())
		file->open("houses.nfs", std::fstream::out | std::fstream::app);
	return (file);
}

void	drawPlayerCoordinates()
{
	std::string coordinates;

	Vector3 p = getPlayerCoords();
	coordinates = floatToString(p.x);
	coordinates += " ";
	coordinates += floatToString(p.y);
	coordinates += " ";
	coordinates += floatToString(p.z);
	GRAPHICS::CLEAR_DRAW_ORIGIN();
	UI::SET_TEXT_FONT(0);
	UI::SET_TEXT_SCALE(0.0, 0.25f);
	UI::SET_TEXT_COLOUR(255, 255, 255, 255);
	UI::SET_TEXT_CENTRE(false);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
	UI::_SET_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)coordinates.c_str());
	UI::_DRAW_TEXT(0.001f, 0.001f);
}

void mainish(std::fstream *file)
{	
	std::string caption(""), dir("");

	/*while (file->good())
	{
		Vector3 tmp;
		std::string str;
		char ch[100];

		tmp = readCoordinate(file);
		str = floatToString(tmp.x);
		str += " ";
		str += floatToString(tmp.y);
		str += " ";
		str += floatToString(tmp.z);
		if (str.size() < 100)
		{
			memcpy(ch, str.data(), str.length());
			ch[str.length()] = 0;
			notify(ch);
		}
	}
	file->close();
	delete file;
	file = init();*/

	while (true)
	{
		drawPlayerCoordinates();
		{
			int	peds[100];
			int	i = 1;
			peds[0] = 50;
			int nbped = PED::GET_PED_NEARBY_PEDS(PLAYER::PLAYER_PED_ID(), peds, -1);
			while (i < nbped)
			{
				Vector3 pedp;
				pedp = ENTITY::GET_ENTITY_COORDS(peds[i], true);
				GRAPHICS::SET_DRAW_ORIGIN(pedp.x, pedp.y, pedp.z, 0);
				caption = floatToString(pedp.x);
				caption += " ";
				caption += floatToString(pedp.y);
				caption += " ";
				caption += floatToString(pedp.z);
				UI::SET_TEXT_FONT(0);
				UI::SET_TEXT_SCALE(0.0, 0.25f);
				UI::SET_TEXT_COLOUR(255, 255, 255, 255);
				UI::SET_TEXT_CENTRE(true);
				UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
				UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
				UI::_SET_TEXT_ENTRY("STRING");
				UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)caption.c_str());
				UI::_DRAW_TEXT(0.0f, 0.0f);
				i++;
			}
			putText(floatToString(PED::_IS_PED_DEAD(PLAYER::PLAYER_PED_ID(), true)), 0.5f, 0.62f);
			Vector3 p = getPlayerCoords();
			/*if (IsKeyJustUp(VK_F5))
			{
				INTERIOR::_0x2CA429C029CCF247(69122);
				ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1273.58f, -1718.93f, 54.7715f, true, true, true, true);
			}
			if (IsKeyJustUp(VK_NUMPAD3))
			{
				writeCoordinate(file, getPlayerCoords());
				notify("Added new coordinate");
			}*/
			caption = floatToString(INTERIOR::GET_INTERIOR_FROM_COLLISION(p.x, p.y, p.z));
			putText(caption, 0.5f, 0.5f);
			putText(dir, 0.5f, 0.56f);
			if (INTERIOR::IS_VALID_INTERIOR(INTERIOR::GET_INTERIOR_FROM_COLLISION(p.x, p.y, p.z)))
				caption = "true";
			else
				caption = "false";
			putText(caption, 0.5f, 0.53f);
			putText(floatToString(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID())), 0.5f, 0.59f);
			putText(floatToString(OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(p.x, p.y, p.z, 1.0f, get_hash("prop_suitcase_02"), false, false, true)), 0.5f, 0.70f);
		}
		WAIT(0);
	}
}


void ScriptMain()
{
	std::fstream	*file;

	srand(GetTickCount());
	file = init();
	if (file->good())
		mainish(file);
	else
		notify("TestPlugin failed to initialize");
}

/*
	Lester's House : 69122
	Franklin's safehouse 2 : 4866
	garage : 146433 = 234.26 -1005.88 -98.9999
	appart: 146689 = - 611.819 58.5026 98.2353

*/