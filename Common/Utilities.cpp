#include <fstream>
#include "Utilities.h"
#include "..\inc\natives.h"

namespace Utilities
{

	int			notify(const std::string &str)
	{
		char		buff[500];

		memcpy(buff, str.c_str(), str.length());
		buff[str.length()] = 0;
		UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
		UI::_ADD_TEXT_COMPONENT_STRING(buff);
		return (UI::_DRAW_NOTIFICATION(false, false));
	}

	std::string				floatToString(float f)
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

	void	putText(const std::string &text, float x, float y, float size)
	{
		UI::SET_TEXT_FONT(0);
		UI::SET_TEXT_SCALE(0.0, size);
		UI::SET_TEXT_COLOUR(255, 255, 255, 255);
		UI::SET_TEXT_CENTRE(true);
		UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
		UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
		UI::_SET_TEXT_ENTRY("STRING");
		UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)text.c_str());
		UI::_DRAW_TEXT(x, y);
	}

	void	drawRectangle(float x1, float y1, float width, float height, Vector3 color, float alpha)
	{
		GRAPHICS::DRAW_RECT(x1, y1, width, height, (int)(color.x * 255.0f), (int)(color.y * 255.0f), (int)(color.z * 255.0f), (int)(alpha * 255.0f));
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

	Object					create_object(const std::string &name, Vector3 pos)
	{
		DWORD attachhash = get_hash(name);
		if (STREAMING::IS_MODEL_VALID(attachhash)) {
			STREAMING::REQUEST_MODEL(attachhash);
			while (!STREAMING::HAS_MODEL_LOADED(attachhash)) {
				WAIT(0);
			}
			Object o = OBJECT::CREATE_OBJECT(attachhash, pos.x, pos.y, pos.z + 0.1f, true, true, false);
			return (o);
		}
		return (false);
	}

	void		log_text(const std::string &str)
	{
		std::fstream file("log.txt", std::ios_base::out | std::ios_base::in | std::ios_base::app);

		file << str;
		file.close();
	}
}