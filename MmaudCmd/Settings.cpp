#include "Settings.h"
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

using namespace std;



// Creates a settings file in the local Appdata directory if it doesn't already exist and writes default settings to it. Returns 0 on success, 1 on failure.
int Settings::CreateSettingsFile(string user_folder) {
	string settings_file_path = user_folder + "\\Appdata\\Local\\Mmaud\\settings.txt";
	ofstream settings_file(settings_file_path);
	if (settings_file.is_open()) {
		settings_file << "mods_folder=" + user_folder + "\\Appdata\\Roaming\\.minecraft\\mods\n";
		settings_file << "language=en\n";
		settings_file << "google_drive_folder_id=";
		settings_file.close();
		return 0;
	}
	else {
		return 1;
	}
}

// Creates a settings folder in the local Appdata directory if it doesn't already exist. Returns 0 on success, 1 on failure.
int Settings::CreateSettingsFolder(string user_folder) {
	string settings_folder = user_folder + "\\Appdata\\Local\\Mmaud";
	return fs::create_directory(settings_folder);
}

// Checks if the settings file exists in the local Appdata directory. Returns 0 if the file exists, 1 if it doesn't.
int Settings::CheckSettingsFile(string user_folder) {
	string settings_file_path = user_folder + "\\Appdata\\Local\\Mmaud\\settings.txt";
	string settings_folder = user_folder + "\\Appdata\\Local\\Mmaud";
	if (fs::exists(settings_folder)) {
		ifstream settings_file(settings_file_path);
		if (settings_file.is_open()) {
			return 0;
		}
		else {
			return 1;
		}
	}
	else {
		return 1;
	}
}

// Opens the settings file in notepad.exe. Returns the status code of the system() call.
int Settings::OpenSettingsFile(string user_folder) {
	string settings_file_path = user_folder + "\\Appdata\\Local\\Mmaud\\settings.txt";
	string command = "notepad.exe " + settings_file_path;
	return system(command.c_str());
}

/*
Reads the settings file and returns an array of strings containing the settings.
Each string is in the format "setting=value".
It return a 2d array of strings, where the first dimension is the setting and the second dimension is the value.
*/
string** Settings::ReadSettingsFile(string user_folder) {
	string settings_file_path = user_folder + "\\Appdata\\Local\\Mmaud\\settings.txt";
	ifstream settings_file(settings_file_path);
	string** settings = new string*[Settings::max_settings];
	for (int i = 0; i < Settings::max_settings; i++) {
		settings[i] = new string[2];
	}
	if (settings_file.is_open()) {
		string line;
		int i = 0;
		while (getline(settings_file, line)) {
			settings[i][0] = line.substr(0, line.find("="));
			settings[i][1] = line.substr(line.find("=") + 1);
			i++;
		}
		settings_file.close();
	}
	return settings;
}