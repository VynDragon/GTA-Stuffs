#pragma once

#include <vector>

struct vehClass
{
	int		id;
	float	maxFuel;
};

static const std::vector<vehClass>	vehicleClasses = 
{
	{-1, 1.0f},
	{0, 1.0f},
	{1, 1.1f},
	{2, 1.3f},
	{3, 1.1f},
	{4, 0.9f},
	{5, 0.8f},
	{6, 1.0f},
	{7, 0.8f},
	{8, 0.5f},
	{9, 1.1f},
	{10, 1.5f},
	{11, 1.4f},
	{12, 1.05f},
	{13, 0.0f},
	{14, 1.0f},
	{15, 1.8f},
	{16, 2.0f},
	{17, 1.1f},
	{18, 1.2f},
	{19, 1.5f},
	{20, 1.0f}
};

vehClass	getVehicleClassById(int id)
{
	auto	it = vehicleClasses.begin();

	while (it != vehicleClasses.end() && (*it).id != id)
		it++;
	if (it != vehicleClasses.end())
		return (*it);
	return (vehicleClasses[0]);
}