#include "LightManager.h"
#include "..\Common\Utilities.h"

LightManager::LightManager(const std::string & file)
{
	char	buff[100];

	this->file = file;
	while (true)
	{
		managed light;
		light.id = (unsigned int)list.size();
		GetPrivateProfileString(Utilities::xToString<unsigned int>(list.size()).c_str(), "r", "0", buff, 99, file.c_str());
		light.r = atoi(buff);
		GetPrivateProfileString(Utilities::xToString<unsigned int>(list.size()).c_str(), "g", "0", buff, 99, file.c_str());
		light.g = atoi(buff);
		GetPrivateProfileString(Utilities::xToString<unsigned int>(list.size()).c_str(), "b", "0", buff, 99, file.c_str());
		light.b = atoi(buff);
		GetPrivateProfileString(Utilities::xToString<unsigned int>(list.size()).c_str(), "x", "0", buff, 99, file.c_str());
		light.pos.x = (float)atof(buff);
		GetPrivateProfileString(Utilities::xToString<unsigned int>(list.size()).c_str(), "y", "0", buff, 99, file.c_str());
		light.pos.y = (float)atof(buff);
		GetPrivateProfileString(Utilities::xToString<unsigned int>(list.size()).c_str(), "z", "0", buff, 99, file.c_str());
		light.pos.z = (float)atof(buff);
		GetPrivateProfileString(Utilities::xToString<unsigned int>(list.size()).c_str(), "range", "0", buff, 99, file.c_str());
		light.range = (float)atof(buff);
		GetPrivateProfileString(Utilities::xToString<unsigned int>(list.size()).c_str(), "intensity", "0", buff, 99, file.c_str());
		light.intensity = (float)atof(buff);
		light.spot = false;
		if (light.range == 0 && light.intensity == 0)
			break;
		this->add(false, light);
	}
}

LightManager::~LightManager()
{
}

void LightManager::add(bool writeToFile, int r, int g, int b, float x, float y, float z, float range, float intensity, bool spot, float dirx, float diry, float dirz, float roundness, float radius, float fadeout)
{
	managed light;
	light.r = r;
	light.g = g;
	light.b = b;
	light.pos.x = x;
	light.pos.y = y;
	light.pos.z = z;
	light.range = range;
	light.intensity = intensity;
	light.spot = false;
	this->add(writeToFile, light);
}

void LightManager::add(bool writeToFile, managed light)
{
	if (writeToFile)
		this->write(light);
	list.push_back(light);
}

void LightManager::write(managed light)
{
	WritePrivateProfileString(Utilities::xToString<unsigned int>(list.size()).c_str(), "r", Utilities::xToString<int>(light.r).c_str(), file.c_str());
	WritePrivateProfileString(Utilities::xToString<unsigned int>(list.size()).c_str(), "g", Utilities::xToString<int>(light.g).c_str(), file.c_str());
	WritePrivateProfileString(Utilities::xToString<unsigned int>(list.size()).c_str(), "b", Utilities::xToString<int>(light.b).c_str(), file.c_str());
	WritePrivateProfileString(Utilities::xToString<unsigned int>(list.size()).c_str(), "x", Utilities::xToString<float>(light.pos.x).c_str(), file.c_str());
	WritePrivateProfileString(Utilities::xToString<unsigned int>(list.size()).c_str(), "y", Utilities::xToString<float>(light.pos.y).c_str(), file.c_str());
	WritePrivateProfileString(Utilities::xToString<unsigned int>(list.size()).c_str(), "z", Utilities::xToString<float>(light.pos.z).c_str(), file.c_str());
	WritePrivateProfileString(Utilities::xToString<unsigned int>(list.size()).c_str(), "range", Utilities::xToString<float>(light.range).c_str(), file.c_str());
	WritePrivateProfileString(Utilities::xToString<unsigned int>(list.size()).c_str(), "intensity", Utilities::xToString<float>(light.intensity).c_str(), file.c_str());
}

void				LightManager::tick(Vector3 pos, float distance) const
{
	auto			it = list.begin();

	while (it != list.end())
	{
		if (!(*it).spot && GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS((*it).pos.x, (*it).pos.y, (*it).pos.z, pos.x, pos.y, pos.z, true) < distance)
		{
			//GRAPHICS::DRAW_LIGHT_WITH_RANGE((*it).pos.x, (*it).pos.y, (*it).pos.z, (*it).r, (*it).g, (*it).b, (*it).range, (*it).intensity);
			GRAPHICS::DRAW_SPOT_LIGHT((*it).pos.x, (*it).pos.y, (*it).pos.z - 0.2f, 0, 0, 1, (*it).r, (*it).g, (*it).b, (*it).range * 2.0f, (*it).intensity * 2.0f, 30.0f, 200.0f, (*it).range * 2.0f);
			GRAPHICS::DRAW_SPOT_LIGHT((*it).pos.x, (*it).pos.y, (*it).pos.z + 0.2f, 0, 0, -1, (*it).r, (*it).g, (*it).b, (*it).range * 2.0f, (*it).intensity * 2.0f, 30.0f, 200.0f, (*it).range * 2.0f);
		}
		it++;
	}
}
