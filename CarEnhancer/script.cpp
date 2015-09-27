
#include "..\Common\script.h"
#include "CarManager.h"
#include "..\Common\Utilities.h"
#include "..\Common\Keys.h"
#include "..\Common\VehicleList.h"
#include <sstream>
#include <vector>


void main(float fuelConsumptionMultiplier, DWORD fuelKey)
{
	CarManager	manager(FILE);
	Vehicle		lastVehicle = 0;
	DWORD		longTick = GetTickCount();
	bool		saved = false;

	while (true)
	{
		Vector3 plCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
		{
			CarManager::managed	i;
			Vehicle				veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);

			i = manager.getManagedByVehicle(veh);
			if (i.that != NULL)
			{
				float	torm = i.that->getFuel() - FUEL_CONSUMPTION_TICK * fuelConsumptionMultiplier * (ENTITY::GET_ENTITY_SPEED(veh) / 100.0f);
				i.that->setFuel(torm > 0 ? torm : 0);
				Utilities::putText("fuel: " + Utilities::xToString<float>(i.that->getFuel()), 0.8f, 0.8f);
				if (i.that->getFuel() <= 0)
					VEHICLE::SET_VEHICLE_ENGINE_ON(veh, false, false);
				else if (!VEHICLE::_IS_VEHICLE_ENGINE_ON(veh))
					VEHICLE::SET_VEHICLE_ENGINE_ON(veh, true, false);
			}
			else
			{
				Vector3	pos = ENTITY::GET_ENTITY_COORDS(veh, true);
				manager.add(new Veh(pos.x, pos.y, pos.z, ENTITY::GET_ENTITY_HEADING(veh), ENTITY::GET_ENTITY_MODEL(veh)), true, veh);
			}
			lastVehicle = veh;
			saved = false;
		}
		else if (saved == false && lastVehicle != 0 && ENTITY::DOES_ENTITY_EXIST(lastVehicle))
		{
			CarManager::managed i = manager.getManagedByVehicle(lastVehicle);
			if (ENTITY::DOES_ENTITY_EXIST(i.vehicle) ? !VEHICLE::IS_VEHICLE_DRIVEABLE(i.vehicle, false) : true)
				i.that->setBroken(true);
			i.that->updateLoaded(lastVehicle);
			i.that->writeToFile(Utilities::xToString<unsigned int>(i.id), FILE);
			saved = true;
			lastVehicle = 0;
		}
		Hash	wep;
		WEAPON::GET_CURRENT_PED_WEAPON(PLAYER::PLAYER_PED_ID(), &wep, true);
		if (wep == Utilities::get_hash("WEAPON_PETROLCAN") && IsKeyDown(fuelKey))
		{
			auto	it = manager.getLoaded()->begin();
			Vector3	pos = ENTITY::GET_ENTITY_COORDS((*it).vehicle, true);
			while (it != manager.getLoaded()->end())
			{
				if (GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(plCoords.x, plCoords.y, plCoords.z, pos.x, pos.y, pos.z, true) < 5.0f)
				{
					float toput = (*it).that->getFuel() + 0.5f;
					(*it).that->setFuel(toput > (*it).that->getMaxFuel() ? (*it).that->getMaxFuel() : toput);
					WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), wep);
					break;
				}
				it++;
			}
		}
		if (GetTickCount() > longTick && manager.getLoaded()->size() < 2)
		{
			Vector3	outb;
			if (PATHFIND::GET_SAFE_COORD_FOR_PED(plCoords.x + rand() % 180 - 90, plCoords.y + rand() % 180 - 90, plCoords.z, false, &outb, 0))
			{
				if (PATHFIND::IS_POINT_ON_ROAD(outb.x, outb.y, outb.z, 0) && !CAM::IS_SPHERE_VISIBLE(outb.x, outb.y, outb.z + 2.0f, 10.0f) && GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(outb.x, outb.y, outb.z, plCoords.x, plCoords.y, plCoords.z, true) > 50.0f)
				{
					std::string veh = vehicleList[rand() % vehicleList.size()];
					int vh = VEHICLE::GET_VEHICLE_CLASS_FROM_NAME(Utilities::get_hash(veh));
					do
					{
						veh = vehicleList[rand() % vehicleList.size()];
						vh = VEHICLE::GET_VEHICLE_CLASS_FROM_NAME(Utilities::get_hash(veh));
						
					} while (vh == 15 || vh == 16);
					manager.add(new Veh(outb.x, outb.y, outb.z, (float)(rand() % 360), veh), true);
				}
			}
			longTick = GetTickCount() + 100;
		}
		//Utilities::putText("Petrolcan: " + Utilities::xToString<Hash>(Utilities::get_hash("WEAPON_PETROLCAN")) + " Weapon: " + Utilities::xToString<Hash>(wep), 0.5f, 0.7f);
		manager.tick(plCoords, 150.0f);
		WAIT(0);
	}
}

void			ScriptMain()
{
	float		fm;
	DWORD		fuelKey;
	std::string	fKey;

	fm = Utilities::getFromFile<float>(".\\Options.ini", "Options", "fuelConsumptionMultiplier", 1.0f);
	if (fm == 1.0f)
		Utilities::writeToFile<float>(".\\Options.ini", "Options", "fuelConsumptionMultiplier", 1.0f);
	fKey = Utilities::getFromFile<std::string>(".\\Options.ini", "Options", "fuelKey", "E key");
	if (fKey == "E key")
		Utilities::writeToFile<std::string>(".\\Options.ini", "Options", "fuelKey", fKey);
	fuelKey = Keys::find_key_by_name(fKey).vkey;
	while (!ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
		WAIT(0);
	while (!PLAYER::IS_PLAYER_CONTROL_ON(PLAYER::PLAYER_ID()))
		WAIT(0);
	main(fm, fuelKey);
}
