#pragma once

#include "script.h"
#include "KeyManager.h"
#include <map>
#include <functional>

#define MAX_LINES 80

typedef std::map<std::string, std::tuple<void(*)(void*, const void*), void*, const void *>, std::greater<std::string>> MenuEntries;

class Menu
{
public:
	Menu();
	~Menu();
	static Menu				*currentMenu;
	void					add_entry(const std::string &name, void (*function)(void*, const void*), void* data, const void *cdata);
	int						remove_entry(const std::string &name);
	void					tick(void);
	static void				show(Menu *menu);
	static void				hide(Menu *menu);
	void					setSelected(const std::string &str) { selected = str; }
	const std::string		&getSelected(void) const { return (selected); }
	void					clear(void);
	virtual void			hideAction(void);
protected:
	MenuEntries			map;
	int					start;
	DWORD				wait;
	std::string			selected;
};

