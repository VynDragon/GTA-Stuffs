#pragma once
#include <vector>
#include <string>
#include "..\Common\script.h"
#include <limits>

#define	NO_MODEL "NO_MODEL"

class Decoration
{
public:
	Decoration(const std::string &appName, const std::string &fileName);
	Decoration(const std::string &modelName, float x, float y, float z, float heading, const std::string &prettyName = "", bool forceFreeze = false);
	Decoration(Hash hash, float x, float y, float z, float heading, const std::string &prettyName = "", bool forceFreeze = false);
	~Decoration();
	Decoration					&operator=(const Decoration &other);
	std::string					getModelName() const { return(modelName); }
	std::string					getName() const { return(prettyName); }
	Vector3						getCoords() const { return(coords); }
	float						getHeading(void) const { return (heading); }
	Hash						getHash(void) const { return (hash); }
	bool						getFreeze(void) const { return (forceFreeze); }
	void						setCoords(Vector3 c) { coords = c; }
	void						setHeading(float h) { heading = h; }
	virtual	void				action(void);
	virtual void				tick(void);
	virtual	void				delaction(void);
	virtual void				writeToFile(const std::string &appName, const std::string &fileName);
protected:
	std::string	modelName;
	std::string	prettyName;
	Hash		hash;
	Vector3		coords;
	float		heading;
	bool		forceFreeze;
};

template<typename T>
class Manager
{
public:
	struct managed {
		T				*that;
		Entity			object;
		unsigned int	fileId;
		Blip			blip;
	};
	Manager(const std::string &file, bool exact = true) 
	{
		this->file = file;
		total = 0;
		this->exact = exact;
		while (true)
		{
			T*	that = new T(Utilities::xToString<unsigned int>(total), file);
			if (that->getModelName() == "")
			{
				delete that;
				break;
			}
			this->add(that, false);
			total++;
		}
	}
	~Manager()
	{
		this->clean();
	}
	void							updateFile(managed it)
	{
		it.that->writeToFile(Utilities::xToString<unsigned int>(it.fileId), file);
	}
	managed							updateLoaded(Object id)
	{
		auto	it = loaded.begin();

		while (loaded.end() != it)
		{
			if ((*it).object == id)
			{
				(*it).that->setCoords(ENTITY::GET_ENTITY_COORDS(id, true));
				(*it).that->setHeading(ENTITY::GET_ENTITY_HEADING(id));
				return (*it);
			}
			it++;
		}
		managed mp = { NULL, 0, 0 };
		return (mp);
	}
	void							add(T *that, bool writeTofile) 
	{ 
		managed kek = { that, 0, total }; 
		
		unloaded.push_back(kek); 
		if (writeTofile)
		{
			kek.that->writeToFile(Utilities::xToString<unsigned int>(total), file);
			total++;
		}
	}
	void		clean(void)
	{
		auto	it = loaded.begin();

		while (loaded.end() != it)
		{
			OBJECT::DELETE_OBJECT(&(*it).object);
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
	void		tick(Vector3 position, float range, bool devMode = false)
	{
		auto	it = unloaded.begin();

		while (it != unloaded.end())
		{
			if (GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(position.x, position.y, position.z, (*it).that->getCoords().x, (*it).that->getCoords().y, (*it).that->getCoords().z, true) < range)
			{
				(*it).object = 0;
				if ((*it).that->getHash() == 0)
				{
					DWORD attachhash = Utilities::get_hash((*it).that->getModelName());
					if (STREAMING::IS_MODEL_VALID(attachhash)) {
						STREAMING::REQUEST_MODEL(attachhash);
						/*while (!STREAMING::HAS_MODEL_LOADED(attachhash))
							WAIT(0);*/
						(*it).object = OBJECT::CREATE_OBJECT(attachhash, (*it).that->getCoords().x, (*it).that->getCoords().y, (*it).that->getCoords().z, 1, false, false);
					}
				}
				else
				{
					if (STREAMING::IS_MODEL_VALID((*it).that->getHash())) {
						STREAMING::REQUEST_MODEL((*it).that->getHash());
						/*while (!STREAMING::HAS_MODEL_LOADED((*it).that->getHash()))
							WAIT(0);*/
						(*it).object = OBJECT::CREATE_OBJECT((*it).that->getHash(), (*it).that->getCoords().x, (*it).that->getCoords().y, (*it).that->getCoords().z, 1, false, false);
					}
				}
				if ((*it).object != 0)
				{
					ENTITY::SET_ENTITY_HEADING((*it).object, (*it).that->getHeading());
					if (!exact)
						OBJECT::PLACE_OBJECT_ON_GROUND_PROPERLY((*it).object);
					if ((*it).that->getFreeze())
						ENTITY::FREEZE_ENTITY_POSITION((*it).object, true);
					else
						ROPE::ACTIVATE_PHYSICS((*it).object);
					if (devMode)
						(*it).blip = UI::ADD_BLIP_FOR_ENTITY((*it).object);
				}
				else
					Utilities::notify("invalid model");
				loaded.push_back(*it);
				unloaded.erase(it);
				break;
			}
			it++;
		}
		it = loaded.begin();
		while (it != loaded.end())
		{
			if ((GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(position.x, position.y, position.z, (*it).that->getCoords().x, (*it).that->getCoords().y, (*it).that->getCoords().z, true) > range
				|| !ENTITY::DOES_ENTITY_EXIST((*it).object))
				&& !CAM::IS_SPHERE_VISIBLE((*it).that->getCoords().x, (*it).that->getCoords().y, (*it).that->getCoords().z, 1.0))
			{
				if (devMode)
					UI::REMOVE_BLIP(&(*it).blip);
				OBJECT::DELETE_OBJECT(&(*it).object);
				unloaded.push_back(*it);
				loaded.erase(it);
				break;
			}
			it++;
		}
	}
	std::vector<managed>	*getLoaded(void) { return (&loaded); }
	std::vector<managed>	*getUnloaded(void) { return (&unloaded); }
	managed					getNearest(Vector3 position)
	{
		auto				it = unloaded.begin();
		managed				nearest = { 0,0 };
		float				nearestDistance = (std::numeric_limits<float>::max)();

		while (it != unloaded.end())
		{
			float distance = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(position.x, position.y, position.z, (*it).that->getCoords().x, (*it).that->getCoords().y, (*it).that->getCoords().z, true);
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
			float distance = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(position.x, position.y, position.z, (*it).that->getCoords().x, (*it).that->getCoords().y, (*it).that->getCoords().z, true);
			if (distance < nearestDistance)
			{
				nearestDistance = distance;
				nearest = (*it);
			}
			it++;
		}
		return (nearest);
	}
private:
	unsigned int			total;
	std::string				file;
	std::vector<managed>	loaded, unloaded;
	bool					exact;
};

