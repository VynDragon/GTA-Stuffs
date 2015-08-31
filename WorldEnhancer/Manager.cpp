#include "Manager.h"

Decoration::Decoration(const std::string &appName, const std::string &fileName)
{
	char	buff[100];

	GetPrivateProfileString(appName.c_str(), "modelName", "", buff, 99, fileName.c_str());
	this->modelName = buff;
	GetPrivateProfileString(appName.c_str(), "prettyName", "", buff, 99, fileName.c_str());
	if (!strcmp(buff, ""))
		this->prettyName = this->modelName;
	else
		this->prettyName = buff;
	GetPrivateProfileString(appName.c_str(), "x", "0", buff, 99, fileName.c_str());
	this->coords.x = (float)atof(buff);
	GetPrivateProfileString(appName.c_str(), "y", "0", buff, 99, fileName.c_str());
	this->coords.y = (float)atof(buff);
	GetPrivateProfileString(appName.c_str(), "z", "0", buff, 99, fileName.c_str());
	this->coords.z = (float)atof(buff);
	GetPrivateProfileString(appName.c_str(), "heading", "0", buff, 99, fileName.c_str());
	this->heading = (float)atof(buff);
}

Decoration::Decoration(const std::string &modelName, float x, float y, float z, float heading, const std::string &prettyName)
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
}

Decoration::~Decoration()
{
}

Decoration	&Decoration::operator=(const Decoration &other)
{
	this->modelName = other.getModelName();
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
}
