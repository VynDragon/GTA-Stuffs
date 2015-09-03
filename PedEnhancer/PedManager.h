#pragma once

#include "..\Common\script.h"

class BPed
{
public:
	BPed(const std::string &appName, const std::string &file);
	BPed(float x, float y, float z, const std::string &model, int type = 26);
	~BPed() {}
	float				getX(void) const { return (x); }
	float				getY(void) const { return (y); }
	float				getZ(void) const { return (z); }
	int					getType(void) const { return (type); }
	std::string			getModel(void) const { return (model); }
	void				loadFromFile(const std::string &appName, const std::string &file);
	void				writeToFile(const std::string &appName, const std::string &file);
	virtual void		pedAttributes(Ped ped);
	virtual std::string	getClassName(void) const { return ("BPed"); }
protected:
	float				x, y, z;
	std::string			model;
	int					type;
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

