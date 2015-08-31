#pragma once

#include "script.h"
#include <map>

class KeyManager
{
public:
	KeyManager();
	~KeyManager();
	static KeyManager	keys;
	void	tick();
	bool	justPressed(DWORD);
	bool	isPressed(DWORD);
protected:
	std::map<DWORD, bool> justPresseds;
	std::map<DWORD, bool> isPresseds;
};

