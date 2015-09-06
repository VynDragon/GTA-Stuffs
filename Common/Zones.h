#pragma once

#include <string>
#include <vector>
#include "script.h"
#include "Utilities.h"

typedef struct {
	std::string		name;
	float			zombieMul;
} zone;

static const std::vector<zone> zones = { { "", 0.20f },
{ "PBOX", 3 },
{ "DOWNT", 3 },
{ "ALTA", 3 },
{ "HAWICK", 3 },
{ "DTVINE", 3 },
{ "ROCKF", 3 },
{ "golf", 3 },
{ "MOVIE", 3 },
{ "DELPE", 3 },
{ "KOREAT", 3 },
{ "LOSPUER", 3 },
{ "DELSOL", 3 },
{ "VCANA", 3 },
{ "STRAW", 3 },
{ "CHAMH", 3 },
{ "DAVIS", 3 },
{ "RANCHO", 3 },
{ "CYPRE", 3 },
{ "ELYSIAN", 3 },
{ "BANNING", 3 },
{ "STAD", 3 },
{ "MIRR", 3 },
{ "LMESA", 3 },
{ "MURRI", 3 },
{ "RICHM", 3 },
{ "SanAnd", 3 },
{ "EAST_V", 3 },
{ "BURTON", 3 },
{ "TEXTI", 3 },
{ "CHIL", 0.45f },
{ "DELBE", 1 },
{ "PBLUFF", 1 },
{ "CHU", 1 },
{ "EBURO", 1 },
{ "BHAMCA", 1.1f },
{ "SANDY", 2 },
{ "GRAPES", 2 },
{ "PALETO", 2 },
{ "ARMYB", 2 },
{ "HARMO", 2 },
{ "TONGVAH", 0.35f },
{ "LAGO", 0.35f },
{ "BAYTRE", 0.35f },
{ "GALLI", 0.35f },
{ "BEACH", 0.35f },
{ "GREATC", 0.35f },
{ "PALHIGH", 0.35f },
{ "TATAMO", 0.35f },
{ "WINDF", 0.35f },
{ "DESRT", 0.35f },
{ "SANCHIA", 0.35f },
{ "MTCHIL", 0.35f },
{ "MTGORDO", 0.35f },
{ "BRADP", 0.35f },
{ "PROCOB", 0.35f },
{ "PALFOR", 0.35f },
{ "CMSW", 0.35f },
{ "PALCOV", 0.35f },
{ "CANNY", 0.35f },
{ "CCREAK", 0.35f },
{ "ALAMO", 0.35f },
{ "MTJOSE", 0.35f },
{ "NCHU", 0.35f },
{ "ZANCUDO", 0.35f },
{ "RTRAK", 0.35f },
{ "LACT", 0.35f },
{ "AIRP", 1.35f },
{ "TERMINA", 1.35f },
{ "VINE", 3 },
{ "SKID", 3 },
{ "WVINE", 3 },
{ "CALAFB", 0.35f},
{ "SLAB", 0.35f },
{ "MORN", 0.35f },
{ "VESP", 0.35f },
{ "RGLEN", 0.35f },
{ "TONGVAV", 0.35f },
{ "OCEANA", 0.15f},
{ "OBSERV", 0.5f},
{ "JAIL" , 3 },
{"LEGSQU", 3}
};

zone		get_zone_by_name(const std::string &name)
{
	auto	it = zones.begin();

	while (it != zones.end() && (*it).name != name)
		it++;
	if (it == zones.end() ? false : (*it).name == name)
		return (*it);
	std::string	str = "Missing Zone: ";
	str += name;
	Utilities::notify(str);
	return (zones[0]);
}