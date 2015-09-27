#pragma once

#include <string>
#include <sstream>
#include "..\inc\types.h"

namespace Utilities
{
	int				notify(const std::string &str);
	std::string			floatToString(float f);
	template<typename T>
	std::string				xToString(T x)
	{
		std::ostringstream	ss;
		ss << x;
		std::string str = ss.str();
		return (str);
	}
	template<typename T>
	static T						getFromFile(const std::string &file, const std::string &appName, const std::string &keyName, T default)
	{
		char				buff[500];
		std::stringstream	ss;
		T					tmp;

		GetPrivateProfileString(appName.c_str(), keyName.c_str(), xToString<T>(default).c_str(), buff, 499, file.c_str());
		ss << buff;
		ss >> tmp;
		return (tmp);
	}

	template<>
	static std::string			getFromFile<std::string>(const std::string &file, const std::string &appName, const std::string &keyName, std::string default)
	{
		char				buff[500];
		std::string			str;

		GetPrivateProfileString(appName.c_str(), keyName.c_str(), default.c_str(), buff, 499, file.c_str());
		str = buff;
		return (buff);
	}
	
	template<typename T>
	void					writeToFile(const std::string &file, const std::string &appName, const std::string &keyName, T value)
	{
		WritePrivateProfileString(appName.c_str(), keyName.c_str(), xToString<T>(value).c_str(), file.c_str());
	}
	Vector3				getPlayerCoords(void);
	void				drawRectangle(float x1, float y1, float x2, float y2, Vector3 color, float alpha);
	void				putText(const std::string &text, float x, float y, float size = 0.25f);
	DWORD				get_hash(const std::string &str);
	Object				create_object(const std::string &name, Vector3 pos);
	void				log_text(const std::string &str);
	std::string			input_text();
}