/*
	THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2015
*/

#pragma once

#include "..\inc\natives.h"
#include "..\inc\types.h"
#include "..\inc\enums.h"

#include "..\inc\main.h"
#include "keyboard.h"


#include <string>
#include <ctime>
#include <vector>

#define ABS(x) x > 0 ? x : -x

void				ScriptMain();


typedef struct
{
	uint8_t		mem_a[14], mem_b[15];
	intptr_t	a, b;
} memory_data;