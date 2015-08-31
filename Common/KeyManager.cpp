#include "KeyManager.h"

KeyManager	KeyManager::keys;

KeyManager::KeyManager()
{
}


KeyManager::~KeyManager()
{
}

void		KeyManager::tick()
{
	auto	kjp = justPresseds.begin();
	auto	kip = isPresseds.begin();

	while (kjp != justPresseds.end())
	{
		kjp->second = IsKeyJustUp(kjp->first);
		kjp++;
	}

	while (kip != isPresseds.end())
	{
		kip->second = IsKeyDown(kip->first);
		kip++;
	}
}

bool	KeyManager::justPressed(DWORD key)
{
	if (justPresseds[key])
	{
		justPresseds[key] = false;
		return (true);
	}
	return (false);
}

bool	KeyManager::isPressed(DWORD key)
{
	if (isPresseds[key])
	{
		isPresseds[key] = false;
		return (true);
	}
	return (false);
}