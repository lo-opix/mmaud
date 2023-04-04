#include "ConsoleMenus.h"
#include "Settings.h"
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

using namespace std;

// Prints the main menu of the program to the console
void UI::PrintMainMenu() {
	system("cls");
	cout << "Minecraft Modpack Auto Updater and Downloader" << endl;
	cout << "================================================" << endl;
	cout << "1. Download mods" << endl;
	cout << "2. Settings" << endl;
	cout << "3. Exit" << endl;
	cout << "================================================" << endl;
}

// Prompts the user to enter a menu choice and returns the choice as a string
string GetMenuChoice() {
	string choice;
	cout << "Enter your choice: ";
	cin >> choice;
	return choice;
}

/*
Runs the main menu of the program.
Returns: 0=exit, 1=download mods, 2=settings
*/
int UI::RunConsoleUi() {
	#pragma warning(suppress : 4996)
	string user_folder = getenv("USERPROFILE");

	PrintMainMenu();
	string choice = GetMenuChoice();
	if (choice == "1") {
		cout << "Start downloading process" << endl;
		return 1;
	}
	else if (choice == "2") {
		if (Settings::CheckSettingsFile(user_folder) == 0) {
			Settings::OpenSettingsFile(user_folder);
		}
		else {
			Settings::CreateSettingsFolder(user_folder);
			Settings::CreateSettingsFile(user_folder);
			Settings::OpenSettingsFile(user_folder);
		}
		return 2;
	}
	else if (choice == "3") {
		return 0;
	}
	else {
		cout << "Invalid choice" << endl;
	}
	return 0;
}


