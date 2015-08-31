#include "OptionManager.h"
#include <Windows.h>
#include "Defines.h"
#include "..\Common\script.h"

OptionManager OptionManager::options;

OptionManager::OptionManager()
{
	char	buff[100];

	GetPrivateProfileString("Options", "openCrateKey", "E key", buff, 99, OPTIONS_FILE);
	openCrateKey = buff;
	GetPrivateProfileString("Options", "openInventoryKey", "VK_F5", buff, 99, OPTIONS_FILE);
	openInventoryKey = buff;
	GetPrivateProfileString("Options", "openWeaponListKey", "VK_TAB", buff, 99, OPTIONS_FILE);
	openWeaponListKey = buff;
	GetPrivateProfileString("Options", "devMode", "0", buff, 99, OPTIONS_FILE);
	if (!strcmp("1", buff))
		devMode = true;
	else
		devMode = false;
	GetPrivateProfileString("Options", "crateRespawnTime", "600000", buff, 99, OPTIONS_FILE);
	crateRespawnTime = strtoul(buff, NULL, 10);
	saved = false;
}


OptionManager::~OptionManager()
{
}


void		OptionManager::save(void)
{
	WritePrivateProfileString("Options", "openCrateKey", openCrateKey.c_str(), OPTIONS_FILE);
	WritePrivateProfileString("Options", "openInventoryKey", openInventoryKey.c_str(), OPTIONS_FILE);
	WritePrivateProfileString("Options", "openWeaponListKey", openWeaponListKey.c_str(), OPTIONS_FILE);
	if (devMode)
		WritePrivateProfileString("Options", "devMode", "1", OPTIONS_FILE);
	else
		WritePrivateProfileString("Options", "devMode", "0", OPTIONS_FILE);
	WritePrivateProfileString("Options", "crateRespawnTime", Utilities::xToString<unsigned int>(crateRespawnTime).c_str(), OPTIONS_FILE);
}

void					OptionManager::tick(void)
{
	if (!saved)
	{
		OptionManager::save();
		saved = true;
	}
}