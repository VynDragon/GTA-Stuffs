/*
	THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2015
*/

#pragma once

#include "inc\natives.h"
#include "inc\types.h"
#include "inc\enums.h"

#include "inc\main.h"

#include "keyboard.h"

#include <string>
#include <ctime>
#include <sstream>
#include <vector>
#include <fstream>

void			ScriptMain();

void			writeCoordinate(std::fstream *file, Vector3 v);
Vector3			readCoordinate(std::fstream *file);
const			std::string floatToString(float f);
Vector3			getPlayerCoords(void);
void			putText(const std::string &text, float x, float y);
void			notify(char *str);
DWORD					get_hash(const std::string &str);