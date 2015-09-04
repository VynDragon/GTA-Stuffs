#include "PedManager.h"
#include "..\Common\Ammos.h"
#include "..\Common\Groups.h"
#include "..\Common\Utilities.h"


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
		else if (classe == "Guard_Ped")
			that = new Guard_Ped(Utilities::xToString<unsigned int>(total), file);
		else if (classe == "Wanderer_Ped")
			that = new Wanderer_Ped(Utilities::xToString<unsigned int>(total), file);
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

BPed::BPed(float x, float y, float z, float heading, const std::string &model, int type)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->model = model;
	this->type = type;
	this->heading = heading;
}

void BPed::loadFromFile(const std::string & appName, const std::string & file)
{
	x = Utilities::getFromFile<float>(file, appName, "x", 0);
	y = Utilities::getFromFile<float>(file, appName, "y", 0);
	z = Utilities::getFromFile<float>(file, appName, "z", 0);
	heading = Utilities::getFromFile<float>(file, appName, "heading", 0);
	model = Utilities::getFromFile<std::string>(file, appName, "model", "");
	type = Utilities::getFromFile<int>(file, appName, "type", 26);
}

void BPed::writeToFile(const std::string & appName, const std::string & file)
{
	Utilities::writeToFile<float>(file, appName, "x", x);
	Utilities::writeToFile<float>(file, appName, "y", y);
	Utilities::writeToFile<float>(file, appName, "z", z);
	Utilities::writeToFile<float>(file, appName, "heading", heading);
	Utilities::writeToFile<std::string>(file, appName, "model", model);
	Utilities::writeToFile<std::string>(file, appName, "class", getClassName());
	Utilities::writeToFile<int>(file, appName, "type", type);
}

void	BPed::fileManageProxy(const std::string &appName, const std::string &file, bool write)
{
	if (write)
		BPed::writeToFile(appName, file);
	else
		BPed::loadFromFile(appName, file);
}

void BPed::pedAttributes(Ped ped)
{
}

Ped	BPed::spawnFunction() const
{
	return (PED::CREATE_PED(type, Utilities::get_hash(model), x, y, z, heading, true, false));
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
			(*it).ped = (*it).that->spawnFunction();
			PED::SET_PED_RANDOM_PROPS((*it).ped);
			(*it).that->pedAttributes((*it).ped);
			ENTITY::SET_ENTITY_MAX_HEALTH((*it).ped, ENTITY::GET_ENTITY_MAX_HEALTH((*it).ped));
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
		kek.that->fileManageProxy(Utilities::xToString<unsigned int>(total), file, true);
		total++;
	}
}

Guard_Ped::Guard_Ped(const std::string & appName, const std::string & file) : BPed(appName, file)
{
	loadFromFile(appName, file);
}

Guard_Ped::Guard_Ped(float x, float y, float z, float heading, const std::string &model, float range, int type) : BPed(x, y, z, heading, model, type)
{
	this->range = range;
}

void Guard_Ped::loadFromFile(const std::string & appName, const std::string & file)
{
	BPed::loadFromFile(appName, file);
	range = Utilities::getFromFile<float>(file, appName, "range", 10);
}

void Guard_Ped::writeToFile(const std::string & appName, const std::string & file)
{
	BPed::writeToFile(appName, file);
	Utilities::writeToFile<float>(file, appName, "range", range);
}

void Guard_Ped::pedAttributes(Ped ped)
{
	//PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, 1);
	PED::SET_PED_FLEE_ATTRIBUTES(ped, 0, 0);
	PED::SET_PED_COMBAT_ATTRIBUTES(ped, 17, 1);
	PED::SET_PED_RELATIONSHIP_GROUP_HASH(ped, Groups::guard);
	PED::SET_PED_RANDOM_PROPS(ped);
	AI::CLEAR_PED_TASKS(ped);
	AI::CLEAR_PED_SECONDARY_TASK(ped);
	PED::SET_PED_KEEP_TASK(ped, false);
	AI::TASK_GUARD_CURRENT_POSITION(ped, range, range, true);
	PED::SET_PED_KEEP_TASK(ped, true);
	int	wep = rand() % 3, tot = 0;
	for (int i = 0; i < ammoTypes.size(); i++)
	{
		ammoType  type = ammoTypes[i];
		if (type.weapon.find("ASSAULT") != std::string::npos)
		{
			if (tot == wep)
			{
				WEAPON::GIVE_WEAPON_TO_PED(ped, Utilities::get_hash(type.weapon), -1, true, true);
				break;
			}
			tot += 1;
		}
	}
	PED::SET_PED_ACCURACY(ped, 80);
	PED::SET_PED_COMBAT_ABILITY(ped, 2);
	PED::SET_PED_CAN_COWER_IN_COVER(ped, true);
	PED::SET_PED_CAN_PEEK_IN_COVER(ped, true);
	PED::SET_PED_TO_LOAD_COVER(ped, true);
}

Ped Guard_Ped::spawnFunction(void) const
{
	return(PED::CREATE_PED(type, Utilities::get_hash(model), x, y, z, heading, true, false));
}

void	Guard_Ped::fileManageProxy(const std::string &appName, const std::string &file, bool write)
{
	if (write)
		Guard_Ped::writeToFile(appName, file);
	else
		Guard_Ped::loadFromFile(appName, file);
}

Wanderer_Ped::Wanderer_Ped(const std::string & appName, const std::string & file) : BPed(appName, file)
{
	loadFromFile(appName, file);
}

Wanderer_Ped::Wanderer_Ped(float x, float y, float z, float heading, const std::string &model, float range, int type) : BPed(x, y, z, heading, model, type)
{
	this->range = range;
}

void Wanderer_Ped::loadFromFile(const std::string & appName, const std::string & file)
{
	BPed::loadFromFile(appName, file);
	range = Utilities::getFromFile<float>(file, appName, "range", 10);
}

void Wanderer_Ped::writeToFile(const std::string & appName, const std::string & file)
{
	BPed::writeToFile(appName, file);
	Utilities::writeToFile<float>(file, appName, "range", range);
}

void Wanderer_Ped::pedAttributes(Ped ped)
{
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, 1);
	PED::SET_PED_FLEE_ATTRIBUTES(ped, 0, 0);
	PED::SET_PED_COMBAT_ATTRIBUTES(ped, 17, 1);
	PED::SET_PED_COMBAT_ATTRIBUTES(ped, 46, TRUE);
	PED::SET_PED_RELATIONSHIP_GROUP_HASH(ped, Groups::wanderer);
	PED::SET_PED_RANDOM_PROPS(ped);
	PED::SET_PED_COMBAT_ABILITY(ped, 2);
	PED::SET_PED_COMBAT_MOVEMENT(ped, 3);
	AI::CLEAR_PED_TASKS(ped);
	AI::CLEAR_PED_SECONDARY_TASK(ped);
	PED::SET_PED_KEEP_TASK(ped, false);
	AI::TASK_WANDER_STANDARD(ped, 0x471c4000, 0);
	PED::SET_PED_KEEP_TASK(ped, true);
	int	wep = rand() % 7, tot = 0;
	for (int i = 0; i < ammoTypes.size(); i++)
	{
		ammoType  type = ammoTypes[i];
		if (type.weapon.find("PISTOL") != std::string::npos)
		{
			if (tot == wep)
			{
				WEAPON::GIVE_WEAPON_TO_PED(ped, Utilities::get_hash(type.weapon), -1, true, false);
				break;
			}
			tot += 1;
		}
	}
	PED::SET_PED_ACCURACY(ped, 70);
	PED::SET_PED_COMBAT_ABILITY(ped, 1);
	PED::SET_PED_CAN_COWER_IN_COVER(ped, true);
	PED::SET_PED_CAN_PEEK_IN_COVER(ped, true);
	PED::SET_PED_TO_LOAD_COVER(ped, true);
}

Ped Wanderer_Ped::spawnFunction(void) const
{
	if (model == "")
		return (PED::CREATE_RANDOM_PED(x, y, z));
	else
		return(PED::CREATE_PED(type, Utilities::get_hash(model), x, y, z, heading, true, false));
}

void	Wanderer_Ped::fileManageProxy(const std::string &appName, const std::string &file, bool write)
{
	if (write)
		Wanderer_Ped::writeToFile(appName, file);
	else
		Wanderer_Ped::loadFromFile(appName, file);
}