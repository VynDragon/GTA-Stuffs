#include "Manager.h"
#include "..\Common\Utilities.h"

Decoration::Decoration(const std::string &appName, const std::string &fileName)
{
	this->modelName = Utilities::getFromFile<std::string>(fileName, appName, "modelName", "");
	this->prettyName = Utilities::getFromFile<std::string>(fileName, appName, "prettyName", "");
	if (this->prettyName == "")
		this->prettyName = this->modelName;
	this->coords.x = Utilities::getFromFile<float>(fileName, appName, "x", 0);
	this->coords.y = Utilities::getFromFile<float>(fileName, appName, "y", 0);
	this->coords.z = Utilities::getFromFile<float>(fileName, appName, "z", 0);
	this->heading = Utilities::getFromFile<float>(fileName, appName, "heading", 0);
	this->hash = Utilities::getFromFile<Hash>(fileName, appName, "hash", 0);
	this->forceFreeze = Utilities::getFromFile<bool>(fileName, appName, "forceFreeze", false);
}

Decoration::Decoration(const std::string &modelName, float x, float y, float z, float heading, const std::string &prettyName, bool forceFreeze)
{
	this->modelName = modelName;
	if (prettyName != "")
		this->prettyName = prettyName;
	else
		this->prettyName = modelName;
	this->coords.x = x;
	this->coords.y = y;
	this->coords.z = z;
	this->heading = heading;
	this->forceFreeze = forceFreeze;
	this->hash = 0;
}

Decoration::Decoration(Hash hash, float x, float y, float z, float heading, const std::string &prettyName, bool forceFreeze)
{
	this->hash = hash;
	this->modelName = Utilities::xToString<Hash>(hash);
	if (prettyName != "")
		this->prettyName = prettyName;
	else
		this->prettyName = Utilities::xToString<Hash>(hash);
	this->coords.x = x;
	this->coords.y = y;
	this->coords.z = z;
	this->heading = heading;
	this->forceFreeze = forceFreeze;
}

Decoration::~Decoration()
{
}

Decoration	&Decoration::operator=(const Decoration &other)
{
	this->modelName = other.getModelName();
	this->hash = other.getHash();
	this->forceFreeze = other.getFreeze();
	this->prettyName = other.getName();
	this->coords.x = other.getCoords().x;
	this->coords.y = other.getCoords().y;
	this->coords.z = other.getCoords().z;
}

void		Decoration::action(void)
{
	return;
}
void		Decoration::delaction(void)
{
	return;
}

void		Decoration::tick(void)
{
	return;
}

void				Decoration::writeToFile(const std::string &appName, const std::string &fileName)
{
	WritePrivateProfileString(appName.c_str(), "modelName", this->modelName.c_str(), fileName.c_str());
	WritePrivateProfileString(appName.c_str(), "prettyName", this->prettyName.c_str(), fileName.c_str());
	WritePrivateProfileString(appName.c_str(), "x", Utilities::xToString<float>(this->coords.x).c_str(), fileName.c_str());
	WritePrivateProfileString(appName.c_str(), "y", Utilities::xToString<float>(this->coords.y).c_str(), fileName.c_str());
	WritePrivateProfileString(appName.c_str(), "z", Utilities::xToString<float>(this->coords.z).c_str(), fileName.c_str());
	WritePrivateProfileString(appName.c_str(), "heading", Utilities::xToString<float>(this->heading).c_str(), fileName.c_str());
	WritePrivateProfileString(appName.c_str(), "hash", Utilities::xToString<Hash>(this->hash).c_str(), fileName.c_str());
	WritePrivateProfileString(appName.c_str(), "forceFreeze", Utilities::xToString<bool>(this->forceFreeze).c_str(), fileName.c_str());
}
