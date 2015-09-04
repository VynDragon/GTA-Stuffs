
#include "..\Common\script.h"
#include "..\Common\keyboard.h"
#include "..\Common\Keys.h"
#include "..\Common\Utilities.h"
#include <sstream>



void	putText(const std::string &text, float x, float y)
{
	GRAPHICS::CLEAR_DRAW_ORIGIN();
	UI::SET_TEXT_FONT(0);
	UI::SET_TEXT_SCALE(0.0, 0.25f);
	UI::SET_TEXT_COLOUR(255, 255, 255, 255);
	UI::SET_TEXT_CENTRE(true);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
	UI::_SET_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)text.c_str());
	UI::_DRAW_TEXT(x, y);
}

std::string floatToString(float f)
{
	std::ostringstream	buffer;
	buffer << f;
	std::string str = buffer.str();
	return (str);
}

float		clampFloat(float x, float max, float min)
{
	x = x > max ? max : x;
	x = x < min ? min : x;
	return (x);
}

void		poseEntity(Entity entity, Vector3 *angles, bool devMode)
{
	if (ENTITY::IS_AN_ENTITY(entity))
	{
		if (ENTITY::DOES_ENTITY_HAVE_PHYSICS(entity))
		{
			Vector3	targetPoint = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
			Vector3	rot = CAM::GET_GAMEPLAY_CAM_ROT(2);
			Vector3	forward = ENTITY::GET_ENTITY_FORWARD_VECTOR(PLAYER::PLAYER_PED_ID());
			if (!devMode)
			{
				targetPoint.x = targetPoint.x + forward.x * 1.2f;
				targetPoint.y = targetPoint.y + forward.y * 1.2f;
				targetPoint.z = targetPoint.z + SYSTEM::SIN(rot.x) * 1.2f + 0.6f;
			}
			else
			{
				targetPoint.x = targetPoint.x + forward.x * 2.0f;
				targetPoint.y = targetPoint.y + forward.y * 2.0f;
				targetPoint.z = targetPoint.z + SYSTEM::SIN(rot.x) * 2.0f + 0.6f;
			}
			Vector3 entityPos = ENTITY::GET_ENTITY_COORDS(entity, true);
			Vector3	entityVelocity = ENTITY::GET_ENTITY_SPEED_VECTOR(entity, false);
			Vector3	applyForce;
			Vector3	entityRotationVelocity = ENTITY::GET_ENTITY_ROTATION_VELOCITY(entity);
			applyForce.x = clampFloat(1.0f / (targetPoint.x - entityPos.x), 1.0f, -1.0f) * 0.15f - clampFloat(entityVelocity.x, 1.0f, -1.0f) * 0.05f;
			applyForce.y = clampFloat(1.0f / (targetPoint.y - entityPos.y), 1.0f, -1.0f) * 0.15f - clampFloat(entityVelocity.y, 1.0f, -1.0f) * 0.05f;
			applyForce.z = clampFloat(1.0f / (targetPoint.z - entityPos.z), 1.0f, -1.0f) * 0.15f - clampFloat(entityVelocity.z, 1.0f, -1.0f) * 0.05f;
			Vector3	playerRotation = ENTITY::GET_ENTITY_ROTATION(entity, 2);
			if ((targetPoint.z - entityPos.z > 0.15f || targetPoint.x - entityPos.x > 0.15f || targetPoint.x - entityPos.x > 0.15f) && !devMode)
			{
				ENTITY::APPLY_FORCE_TO_ENTITY(entity, 1, applyForce.x, applyForce.y, applyForce.z, 0.0f, 0.0f, 0.0f, true, false, true, true, true, true);
				OBJECT::SET_ACTIVATE_OBJECT_PHYSICS_AS_SOON_AS_IT_IS_UNFROZEN(entity, true);
				ENTITY::FREEZE_ENTITY_POSITION(entity, false);
				ROPE::ACTIVATE_PHYSICS(entity);
			}
			else
			{
				ENTITY::FREEZE_ENTITY_POSITION(entity, true);
				ENTITY::SET_ENTITY_COORDS(entity, targetPoint.x, targetPoint.y, targetPoint.z, true, true, true, true);
				//ENTITY::SET_ENTITY_ROTATION(entity, playerRotation.x, playerRotation.y, playerRotation.z, 2, true);
				ENTITY::SET_ENTITY_ROTATION(entity, angles->x, angles->y, angles->z, 2, true);
				if (IsKeyDown(VK_NUMPAD7))
					angles->z++;
				else if (IsKeyDown(VK_NUMPAD9))
					angles->z--;
				if (IsKeyDown(VK_NUMPAD4))
					angles->y++;
				else if (IsKeyDown(VK_NUMPAD6))
					angles->y--;
				if (IsKeyDown(VK_NUMPAD8))
					angles->x++;
				else if (IsKeyDown(VK_NUMPAD2))
					angles->x--;

				if (angles->x > 180.0f)
					angles->x = -180.0f;
				else if (angles->x < -180.0f)
					angles->x = 180.0f;
				if (angles->y > 180.0f)
					angles->y = -180.0f;
				else if (angles->y < -180.0f)
					angles->y = 180.0f;
				if (angles->z > 180.0f)
					angles->z = -180.0f;
				else if (angles->z < -180.0f)
					angles->z = 180.0f;
			}
		}
	}
}

void main(DWORD key, bool devMode)
{
	Entity	entity;
	Vector3	angles;
	bool	locked = false;
	bool	keylock = false;
	DWORD	waitlock = GetTickCount();

	while (true)
	{
		if (devMode)
			putText(Utilities::xToString(entity), 0.8f, 0.5f);
		if (IsKeyDown(key))
		{
			if (!locked && !keylock)
			{
				PLAYER::SET_PLAYER_FORCED_AIM(PLAYER::PLAYER_ID(), true);
				if (PLAYER::_GET_AIMED_ENTITY(PLAYER::PLAYER_ID(), &entity) || PLAYER::GET_PLAYER_TARGET_ENTITY(PLAYER::PLAYER_ID(), &entity))
				{
					if (ENTITY::IS_AN_ENTITY(entity))
					{
						if (ENTITY::DOES_ENTITY_HAVE_PHYSICS(entity) || devMode)
							if (ENTITY::IS_ENTITY_AN_OBJECT(entity) || devMode)
							{
								locked = true;
								angles = ENTITY::GET_ENTITY_ROTATION(entity, 2);
								angles.z = angles.z;
								angles.x = ENTITY::GET_ENTITY_PITCH(entity);
								angles.y = ENTITY::GET_ENTITY_ROLL(entity);
							}
					}
				}
			}
			else
			{
				poseEntity(entity, &angles, devMode);
				if (IsKeyDown(VK_CONTROL) && waitlock < GetTickCount())
					{
						keylock = !keylock;
						waitlock = GetTickCount() + 800;
					}
			}
		}
		else if (!keylock)
		{
			PLAYER::SET_PLAYER_FORCED_AIM(PLAYER::PLAYER_ID(), false);
			locked = false;
			if (ENTITY::IS_AN_ENTITY(entity))
			{
				OBJECT::SET_ACTIVATE_OBJECT_PHYSICS_AS_SOON_AS_IT_IS_UNFROZEN(entity, true);
				ENTITY::FREEZE_ENTITY_POSITION(entity, false);
				ROPE::ACTIVATE_PHYSICS(entity);
			}
			entity = 0;
		}
		else if (keylock && locked)
		{
			poseEntity(entity, &angles, devMode);
		}

		WAIT(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	char	buff[50];
	bool	devMode = false;
	GetPrivateProfileString("Options", "devMode", "0", buff, 49, "./options.ini");
	if (!strcmp("1", buff))
		devMode = true;
	else
		devMode = false;
	GetPrivateProfileString("Options", "handKey", "X key", buff, 49, ".\\options.ini");
	if (!strcmp("X key", buff))
		WritePrivateProfileString("Options", "handKey", "X key", ".\\options.ini");
	
	main(Keys::find_key_by_name(buff).vkey, devMode);
}
