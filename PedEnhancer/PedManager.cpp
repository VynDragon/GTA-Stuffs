#include "PedManager.h"



PedManager::PedManager(const std::string file)
{
	this->file = file;
	total = 0;
	while (true)
	{
		std::string	classe = Utilities::getFromFile<std::string>(file, Utilities::xToString<unsigned int>(total), "class", "");
		BPed*	that;

		if (classe == "BPed")
			that = new BPed(Utilities::xToString<unsigned int>(total), file);
		else
			break;
		this->add(that, false);
		total++;
	}
}

PedManager::~PedManager()
{
	clean();
}

BPed::BPed(const std::string & appName, const std::string & file)
{
	BPed::loadFromFile(appName, file);
}

BPed::BPed(float x, float y, float z, const std::string &model, int type)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->model = model;
	this->type = type;
}

void BPed::loadFromFile(const std::string & appName, const std::string & file)
{
	x = Utilities::getFromFile<float>(file, appName, "x", 0);
	y = Utilities::getFromFile<float>(file, appName, "y", 0);
	z = Utilities::getFromFile<float>(file, appName, "z", 0);
	model = Utilities::getFromFile<std::string>(file, appName, "model", "");
	type = Utilities::getFromFile<int>(file, appName, "type", 26);
}

void BPed::writeToFile(const std::string & appName, const std::string & file)
{
	Utilities::writeToFile<float>(file, appName, "x", x);
	Utilities::writeToFile<float>(file, appName, "y", y);
	Utilities::writeToFile<float>(file, appName, "z", z);
	Utilities::writeToFile<std::string>(file, appName, "model", model);
	Utilities::writeToFile<std::string>(file, appName, "class", getClassName());
	Utilities::writeToFile<int>(file, appName, "type", type);
}

void BPed::pedAttributes(Ped ped)
{
}

void		PedManager::clean(void)
{
	auto	it = loaded.begin();

	while (loaded.end() != it)
	{
		OBJECT::DELETE_OBJECT(&(*it).ped);
		delete (*it).that;
		it++;
	}
	it = unloaded.begin();
	while (unloaded.end() != it)
	{
		delete (*it).that;
		it++;
	}
}

void PedManager::tick(Vector3 position, float range)
{
	auto	it = unloaded.begin();

	while (it != unloaded.end())
	{
		if (GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(position.x, position.y, position.z, (*it).that->getX(), (*it).that->getY(), (*it).that->getZ(), true) < range)
		{
			DWORD attachhash = Utilities::get_hash((*it).that->getModel());
			if (STREAMING::IS_MODEL_VALID(attachhash)) {
				STREAMING::REQUEST_MODEL(attachhash);
				while (!STREAMING::HAS_MODEL_LOADED(attachhash))
					WAIT(0);
			}
			(*it).ped = PED::CREATE_PED((*it).that->getType(), attachhash, (*it).that->getX(), (*it).that->getY(), (*it).that->getZ(), (float)(rand() % 360), true, false);
			(*it).that->pedAttributes((*it).ped);
			loaded.push_back(*it);
			unloaded.erase(it);
			break;
		}
		it++;
	}
	it = loaded.begin();
	while (it != loaded.end())
	{
		Vector3	coords;
		if (ENTITY::DOES_ENTITY_EXIST((*it).ped))
			coords = ENTITY::GET_ENTITY_COORDS((*it).ped, true);
		if ((GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(position.x, position.y, position.z, coords.x, coords.y, coords.z, true) > range
			&& !CAM::IS_SPHERE_VISIBLE(coords.x, coords.y, coords.z, 1.0)) || !ENTITY::DOES_ENTITY_EXIST((*it).ped))
		{
			PED::DELETE_PED(&(*it).ped);
			unloaded.push_back(*it);
			loaded.erase(it);
			break;
		}
		it++;
	}
}

void						PedManager::add(BPed *that, bool writeTofile)
{
	managed kek = { that, 0, total };

	unloaded.push_back(kek);
	if (writeTofile)
	{
		kek.that->writeToFile(Utilities::xToString<unsigned int>(total), file);
		total++;
	}
}

