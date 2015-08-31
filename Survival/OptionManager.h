#pragma once

#include <string>

class OptionManager
{
public:
	OptionManager();
	~OptionManager();
	static OptionManager	options;
	void					save(void);
	void					tick(void);
	std::string				openCrateKey;
	std::string				openInventoryKey;
	std::string				openWeaponListKey;
	bool					devMode;
	unsigned int			crateRespawnTime;
private:
	bool					saved;
};

