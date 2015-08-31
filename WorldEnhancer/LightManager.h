#pragma once

#include <vector>
#include <string>
#include "..\Common\script.h"

class LightManager
{
public:
	struct managed
	{
		unsigned int	id;
		int	r, g, b;
		Vector3	pos;
		float range;
		float intensity;
		bool	spot;
		Vector3	dir;
		float	roundness;
		float	radius;
		float	fadeout;
	};
	LightManager(const std::string &file);
	~LightManager();
	void	add(bool writeToFile, int r, int g, int b, float x, float y, float z, float range, float intensity, bool spot = false, float dirx = 0, float diry = 0, float dirz = 0, float roundness = 0, float radius = 0, float fadeout = 0);
	void	add(bool writeToFile, managed light);
	void	write(managed light);
	void	tick(Vector3 pos, float distance) const;
private:
	std::vector<managed>	list;
	std::string				file;
};

