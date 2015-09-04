#include "Menu.h"
#include "Utilities.h"

Menu	*Menu::currentMenu;

Menu::Menu()
{
	selected = "";
	start = 0;
	wait = GetTickCount();
}


Menu::~Menu()
{
}

void		Menu::add_entry(const std::string &name, void (*function)(void *, const void*), void *data, const void *cdata)
{
	std::string	namee = name;

	while (map.find(namee) != map.end())
	{
		namee += " ";
	}
	std::get<0>(map[namee]) = function;
	std::get<1>(map[namee]) = data;
	std::get<2>(map[namee]) = cdata;
}

int			Menu::remove_entry(const std::string &name)
{
	return (static_cast<int>(map.erase(name)));
}

void								Menu::tick(void)
{
	MenuEntries::iterator			it;
	MenuEntries::reverse_iterator	rit;
	std::string						last;
	int								line = 0;
	float							x = 0.08f, y = 0.05f;

	if (map.size() < 1)
	{
		Menu::hide(this);
		return;
	}
	it = map.begin();
	if (selected == "")
		selected = (*it).first;
	last = "";
	while (it != map.end() && line != start)
	{
		last = (*it).first;
		it++;
		line++;
	}
	line = 0;
	while (it != map.end() && line != MAX_LINES)
	{
		if ((*it).first == selected)
		{
			Vector3	color;
			color.x = 1.0f;
			color.y = 0.0f;
			color.z = 0.0f;
			Utilities::drawRectangle(x, y + 0.01f, 0.15f, 0.01f, color, 0.8f);
			if (KeyManager::keys.isPressed(VK_NUMPAD8) && last != "" && wait < GetTickCount())
			{
				selected = last;
				wait = GetTickCount() + 50;
			}
		}
		Utilities::putText((*it).first, x, y, 0.20f);
		y += 0.01f;
		last = (*it).first;
		it++;
		line++;
	}
	Utilities::putText(Utilities::xToString<int>(start / MAX_LINES), x, y + 0.02f, 0.20f);
	if (line == MAX_LINES && KeyManager::keys.justPressed(VK_NUMPAD6))
	{
		selected = last;
		start += MAX_LINES;
	}
	if (KeyManager::keys.justPressed(VK_NUMPAD4))
	{
		start = 0;
		selected = "";
	}
	rit = map.rbegin();
	last = "";
	while (rit != map.rend())
	{
		if ((*rit).first == selected && KeyManager::keys.isPressed(VK_NUMPAD2) && last != "" && wait < GetTickCount())
		{
			selected = last;
			wait = GetTickCount() + 50;
		}
		last = (*rit).first;
		rit++;
	}
	{
		Vector3	color;
		color.x = 0.0f;
		color.y = 0.0f;
		color.z = 0.0f;
		Utilities::drawRectangle(0.08f, 0.05f + (y - 0.045f) / 2.0f, 0.15f, y - 0.045f, color, 0.7f);
	}
	if (KeyManager::keys.justPressed(VK_NUMPAD5))
	{
		if (std::get<0>(map[selected]) != NULL)
		{
			std::get<0>(map[selected])(std::get<1>(map[selected]), std::get<2>(map[selected]));
		}
	}
}

void	Menu::show(Menu *menu)
{
	if (Menu::currentMenu != NULL)
		Menu::hide(Menu::currentMenu);
	Menu::currentMenu = menu;
}

void	Menu::hide(Menu *menu)
{
	Menu::currentMenu = NULL;
	menu->setSelected("");
	menu->hideAction();
}

void				Menu::clear(void)
{
	map.clear();
}

void				Menu::hideAction(void)
{
}


