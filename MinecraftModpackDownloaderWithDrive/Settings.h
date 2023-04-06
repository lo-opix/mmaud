// Created by: lo-opix
#ifndef Settings_H
#define Settings_H
#include <iostream>
#include <fstream>
#include <filesystem>

namespace Settings {
	const int max_settings(3);
	// Creates a settings file in the local Appdata directory if it doesn't already exist and writes default settings to it. Returns 0 on success, 1 on failure.
	int CreateSettingsFile(std::string user_folder);
	// Creates a settings folder in the local Appdata directory if it doesn't already exist. Returns 0 on success, 1 on failure.
	int CreateSettingsFolder(std::string user_folder);
	// Checks if the settings file exists in the local Appdata directory. Returns 0 if the file exists, 1 if it doesn't.
	int CheckSettingsFile(std::string user_folder);
	// Opens the settings file in notepad.exe. Returns the status code of the system() call.
	int OpenSettingsFile(std::string user_folder);
	// Reads the settings file and returns the value of the specified setting. Returns an empty string if the setting doesn't exist.
	std::string** ReadSettingsFile(std::string user_folder);
}
#endif

