
#include "..\Common\script.h"
#include "CarManager.h"
#include "..\Common\Utilities.h"
#include "..\Common\Keys.h"
#include <sstream>
#include <vector>


void main(float fuelConsumptionMultiplier, DWORD fuelKey)
{
	CarManager	manager(FILE);
	Vehicle		lastVehicle;
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
		else if (saved == false)
		{
			CarManager::managed i = manager.getManagedByVehicle(lastVehicle);
			i.that->updateLoaded(lastVehicle);
			i.that->writeToFile(Utilities::xToString<unsigned int>(i.id), FILE);
			saved = true;
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
	main(fm, fuelKey);
}
