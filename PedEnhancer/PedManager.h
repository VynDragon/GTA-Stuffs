#pragma once

#include "..\Common\script.h"

class BPed
{
public:
	BPed(const std::string &appName, const std::string &file);
	BPed(float x, float y, float z, float heading, const std::string &model, int type = 26);
	~BPed() {}
	float				getX(void) const { return (x); }
	float				getY(void) const { return (y); }
	float				getZ(void) const { return (z); }
	int					getType(void) const { return (type); }
	std::string			getModel(void) const { return (model); }
	void				loadFromFile(const std::string &appName, const std::string &file);
	void				writeToFile(const std::string &appName, const std::string &file);
	virtual void		fileManageProxy(const std::string &appName, const std::string &file, bool write);
	virtual void		pedAttributes(Ped ped);
	virtual Ped			spawnFunction(void) const;
	virtual std::string	getClassName(void) const { return ("BPed"); }
protected:
	float				x, y, z, heading;
	std::string			model;
	int					type;
};

class Guard_Ped : public BPed
{
public:
	Guard_Ped(const std::string &appName, const std::string &file);
	Guard_Ped(float x, float y, float z, float heading, const std::string &model, float range = 25.0f, int type = 26);
	void				loadFromFile(const std::string &appName, const std::string &file);
	void				writeToFile(const std::string &appName, const std::string &file);
	virtual void		pedAttributes(Ped ped);
	virtual std::string	getClassName(void) const { return ("Guard_Ped"); }
	virtual Ped			spawnFunction(void) const;
	virtual void		fileManageProxy(const std::string &appName, const std::string &file, bool write);
protected:
	float	range;
};

class Wanderer_Ped : public BPed
{
public:
	Wanderer_Ped(const std::string &appName, const std::string &file);
	Wanderer_Ped(float x, float y, float z, float heading, const std::string &model, float range = 25.0f, int type = 26);
	void				loadFromFile(const std::string &appName, const std::string &file);
	void				writeToFile(const std::string &appName, const std::string &file);
	virtual void		pedAttributes(Ped ped);
	virtual std::string	getClassName(void) const { return ("Wanderer_Ped"); }
	virtual Ped			spawnFunction(void) const;
	virtual void		fileManageProxy(const std::string &appName, const std::string &file, bool write);
protected:
	float	range;
};

class PedManager
{
public:
	struct managed {
		BPed			*that;
		Ped				ped;
		unsigned int	id;
	};
	PedManager(const std::string file);
	~PedManager();
	void					clean(void);
	void					tick(Vector3 pos, float range);
	void					add(BPed *that, bool writeTofile);
	std::vector<managed>	*getLoaded(void) { return (&loaded); }
	std::vector<managed>	*getUnloaded(void) { return (&unloaded); }
private:
	std::vector<managed>	loaded, unloaded;
	unsigned int			total;
	std::string				file;
};

