// Created by: lo-opix

#ifndef CONSOLE_MENUS_H
#define CONSOLE_MENUS_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include "ConsoleMenus.h"



namespace UI {
	void PrintMainMenu(bool clear_at_start);
	int RunConsoleUi(bool clear_at_start);
}

#endif // CONSOLE_MENUS_H