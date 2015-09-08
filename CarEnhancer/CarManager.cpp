#include "CarManager.h"
#include "..\Common\Utilities.h"
#include "..\Common\VehicleClasses.h"
#include "..\Common\VehicleList.h"


CarManager::CarManager(const std::string &file)
{
	this->file = file;
	total = 0;
	while (true)
	{
		std::string	classe = Utilities::getFromFile<std::string>(file, Utilities::xToString<unsigned int>(total), "class", "");
		Veh*	that;

		if (classe == "Veh")
			that = new Veh(Utilities::xToString<unsigned int>(total), file);
		else
			break;
		this->add(that, false);
		total++;
	}
}


CarManager::~CarManager()
{
	clean();
}

void		CarManager::clean(void)
{
	auto	it = loaded.begin();

	while (loaded.end() != it)
	{
		VEHICLE::DELETE_VEHICLE(&(*it).vehicle);
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

void CarManager::tick(Vector3 position, float range)
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
			(*it).vehicle = (*it).that->spawnFunction();
			ENTITY::SET_ENTITY_MAX_HEALTH((*it).vehicle, ENTITY::GET_ENTITY_MAX_HEALTH((*it).vehicle));
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
		if (ENTITY::DOES_ENTITY_EXIST((*it).vehicle))
			coords = ENTITY::GET_ENTITY_COORDS((*it).vehicle, true);
		if ((GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(position.x, position.y, position.z, coords.x, coords.y, coords.z, true) > range
			&& !CAM::IS_SPHERE_VISIBLE(coords.x, coords.y, coords.z, 1.0)) || !ENTITY::DOES_ENTITY_EXIST((*it).vehicle))
		{
			VEHICLE::DELETE_VEHICLE(&(*it).vehicle);
			unloaded.push_back(*it);
			loaded.erase(it);
			break;
		}
		it++;
	}
}

void						CarManager::add(Veh *that, bool writeTofile, Vehicle loaded)
{
	managed kek = { that, loaded, total };

	if (loaded != 0)
		this->loaded.push_back(kek);
	else
		unloaded.push_back(kek);
	if (writeTofile)
	{
		kek.that->fileManageProxy(Utilities::xToString<unsigned int>(total), file, true);
		total++;
	}
}

Veh::Veh(const std::string & appName, const std::string & file)
{
	Veh::loadFromFile(appName, file);
}

Veh::Veh(float x, float y, float z, float heading, Hash modelHash)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->model = getModelNameByHash(modelHash);
	this->maxFuel = getVehicleClassById(VEHICLE::GET_VEHICLE_CLASS_FROM_NAME(modelHash)).maxFuel;
	this->heading = heading;
	this->fuel = ((float)(rand() % 100)) / 100.0f * maxFuel;
}

Veh::Veh(float x, float y, float z, float heading, const std::string &model)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->model = model;
	this->maxFuel = getVehicleClassById(VEHICLE::GET_VEHICLE_CLASS_FROM_NAME(Utilities::get_hash(model))).maxFuel;
	this->heading = heading;
	this->fuel = ((float)(rand() % 100)) / 100.0f * maxFuel;
}

void Veh::updateLoaded(Vehicle veh)
{
	Vector3	pos = ENTITY::GET_ENTITY_COORDS(veh, true);
	x = pos.x;
	y = pos.y;
	z = pos.z;
	heading = ENTITY::GET_ENTITY_HEADING(veh);
}

void Veh::loadFromFile(const std::string & appName, const std::string & file)
{
	x = Utilities::getFromFile<float>(file, appName, "x", 0);
	y = Utilities::getFromFile<float>(file, appName, "y", 0);
	z = Utilities::getFromFile<float>(file, appName, "z", 0);
	heading = Utilities::getFromFile<float>(file, appName, "heading", 0);
	model = Utilities::getFromFile<std::string>(file, appName, "model", "");
	fuel = Utilities::getFromFile<float>(file, appName, "fuel", 0);
	maxFuel = getVehicleClassById(VEHICLE::GET_VEHICLE_CLASS_FROM_NAME(Utilities::get_hash(model))).maxFuel;
}

void Veh::writeToFile(const std::string & appName, const std::string & file)
{
	Utilities::writeToFile<float>(file, appName, "x", x);
	Utilities::writeToFile<float>(file, appName, "y", y);
	Utilities::writeToFile<float>(file, appName, "z", z);
	Utilities::writeToFile<float>(file, appName, "heading", heading);
	Utilities::writeToFile<std::string>(file, appName, "model", model);
	Utilities::writeToFile<std::string>(file, appName, "class", getClassName());
	Utilities::writeToFile<float>(file, appName, "fuel", fuel);
}

void	Veh::fileManageProxy(const std::string &appName, const std::string &file, bool write)
{
	if (write)
		Veh::writeToFile(appName, file);
	else
		Veh::loadFromFile(appName, file);
}

Vehicle		Veh::spawnFunction() const
{
	OutputDebugString("spawning car");
	return (VEHICLE::CREATE_VEHICLE(Utilities::get_hash(model), x, y, z, heading, true, false));
}

CarManager::managed		CarManager::getNearest(Vector3 position)
{
	auto				it = unloaded.begin();
	managed				nearest = { 0, 0, 0 };
	float				nearestDistance = (std::numeric_limits<float>::max)();

	while (it != unloaded.end())
	{
		float distance = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(position.x, position.y, position.z, (*it).that->getX(), (*it).that->getY(), (*it).that->getZ(), true);
		if (distance < nearestDistance)
		{
			nearestDistance = distance;
			nearest = (*it);
		}
		it++;
	}
	it = loaded.begin();
	while (it != loaded.end())
	{
		float distance = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(position.x, position.y, position.z, (*it).that->getX(), (*it).that->getY(), (*it).that->getZ(), true);
		if (distance < nearestDistance)
		{
			nearestDistance = distance;
			nearest = (*it);
		}
		it++;
	}
	return (nearest);
}

CarManager::managed	CarManager::getManagedByVehicle(Vehicle i)
{
	managed			dummy = { NULL, 0, 0 };
	auto			it = loaded.begin();

	while (it != loaded.end())
	{
		if (i == (*it).vehicle)
			return (*it);
		it++;
	}
	it = unloaded.begin();
	while (it != unloaded.end())
	{
		if (i == (*it).vehicle)
			return (*it);
		it++;
	}
	return dummy;
}
