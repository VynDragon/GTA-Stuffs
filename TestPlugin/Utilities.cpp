#include "script.h"

void			notify(char *str)
{
	UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(str);
	UI::_DRAW_NOTIFICATION(false, false);
}

void			writeCoordinate(std::fstream *file, Vector3 v)
{
	*file << v.x;
	*file << " ";
	*file << v.y;
	*file << " ";
	*file << v.z;
	*file << std::endl;
	*file << std::flush;
}

Vector3			readCoordinate(std::fstream *file)
{
	Vector3		coord;

	*file >> coord.x;
	*file >> coord.y;
	*file >> coord.z;
	return (coord);
}

const					std::string floatToString(float f)
{
	std::ostringstream	buffer;
	buffer << f;
	std::string str = buffer.str();
	return (str);
}

Vector3	getPlayerCoords(void)
{
	return(ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true));
}

void	putText(const std::string &text, float x, float y)
{
	GRAPHICS::CLEAR_DRAW_ORIGIN();
	UI::SET_TEXT_FONT(0);
	UI::SET_TEXT_SCALE(0.0, 0.25f);
	UI::SET_TEXT_COLOUR(255, 255, 255, 255);
	UI::SET_TEXT_CENTRE(true);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
	UI::_SET_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)text.c_str());
	UI::_DRAW_TEXT(x, y);
}
DWORD					get_hash(const std::string &str)
{
	DWORD				hash;
	char				buff[100];

	memcpy(&buff[0], str.c_str(), str.size());
	buff[str.size()] = 0;
	hash = GAMEPLAY::GET_HASH_KEY(&buff[0]);
	return (hash);
}
