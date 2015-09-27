#pragma once

#include "..\inc\natives.h"
#include "..\inc\nativeCaller.h"
#include <vector>

#define	FUEL_CONSUMPTION_TICK 0.0001f
#define FILE ".\\PlayerCars.ini"

class Veh
{
public:
	Veh(const std::string &appName, const std::string &file);
	Veh(float x, float y, float z, float heading, Hash modelHash);
	Veh(float x, float y, float z, float heading, const std::string &model);
	~Veh() {}
	float				getX(void) const { return (x); }
	float				getY(void) const { return (y); }
	float				getZ(void) const { return (z); }
	bool				getBroken(void) const { return (broken); }
	float				getFuel(void) const { return (fuel); }
	float				getMaxFuel(void) const { return (maxFuel); }
	void				setFuel(float fuel) { this->fuel = fuel; }
	void				setBroken(bool i) { this->broken = i; }
	std::string			getModel(void) const { return (model); }
	void				updateLoaded(Vehicle veh);
	void				loadFromFile(const std::string &appName, const std::string &file);
	void				writeToFile(const std::string &appName, const std::string &file);
	virtual void		fileManageProxy(const std::string &appName, const std::string &file, bool write);
	virtual Vehicle		spawnFunction(void) const;
	virtual std::string	getClassName(void) const { return ("Veh"); }
protected:
	float				x, y, z, heading;
	std::string			model;
	float				fuel, maxFuel;
	bool				broken;
};


class CarManager
{
public:
	struct managed
	{
		Veh				*that;
		Vehicle			vehicle;
		unsigned int	id;
	};
	CarManager(const std::string &file);
	~CarManager();
	void					clean(void);
	void					tick(Vector3 pos, float range);
	void					add(Veh *that, bool writeTofile, Vehicle loaded = 0);
	std::vector<managed>	*getLoaded(void) { return (&loaded); }
	std::vector<managed>	*getUnloaded(void) { return (&unloaded); }
	managed					getNearest(Vector3 position);
	managed					getManagedByVehicle(Vehicle i);
private:
	std::vector<managed>	unloaded, loaded;
	unsigned int			total;
	std::string				file;
};

