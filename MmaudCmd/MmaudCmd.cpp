/*TODO:
* Remove temp files
*/


#include <cstdlib>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <chrono>
#include <thread>
#include "ConsoleMenus.h"
#include "Settings.h"

using namespace std;
using json = nlohmann::json;
namespace fs = std::filesystem;

#if defined(_WIN64)
bool ON_WINDOWS = true;
#else
bool ON_WINDOWS = false;
#endif


const string MINECRAFT_LAUNCH_COMMAND("explorer.exe shell:appsFolder\\Microsoft.4297127D64EC6_8wekyb3d8bbwe!Minecraft");

string base_url("https://www.googleapis.com/drive/v3/files");
string folder_id(""); 
string api_key("AIzaSyC4qR1jMIs8yxe8WoHSYvKJzYuv4r1EfFQ");
string mods_folder(""); // Equal to C:\Users\USERNAME\AppData\Roaming\.minecraft\mods
string language("");

#pragma warning(suppress : 4996)
string user_folder = getenv("USERPROFILE"); // Equal to C:\Users\USERNAME

string retrieve_remote_folder_name() {
	string folder_name_file = "folder_name.json";
	string args = folder_id + "?key=" + api_key + "&fields=name";

	string request_command = "curl -s -o " + folder_name_file + " \"" + base_url + "/" + args + "\"";

	int result = system(request_command.c_str());

	if (result == 0) {
		ifstream folder_name_stream(folder_name_file);
		json parsed_folder_name = json::parse(folder_name_stream);
		remove("folder_name.json");
		return parsed_folder_name.value("name", "Not Found");
	}
	else {
		remove("folder_name.json");
		return "Not Found";
	}
}

int setSettings() {
	string** settings = Settings::ReadSettingsFile(user_folder);

	for (int i = 0; i < (sizeof(settings) / 2.0) - 1; i++) { // diveided by 2 because they are 2 columns by row; -1 because the last blank line in counted

		if (settings[i][0] == "mods_folder") 
			mods_folder = settings[i][1];
		else if (settings[i][0] == "language") 
			language = settings[i][1];
		else if (settings[i][0] == "google_drive_folder_id") 
			folder_id = settings[i][1];

	}

	if (folder_id == "") {
		cerr << "\033[1;31m" << "ERROR: Please set google_drive_folder_id in settings.txt" << "\033[0m" << endl;
		return 1;
	}

	return 0;
}


std::string getAppDataPath() {
	#pragma warning(suppress : 4996)
	const char* appdata = std::getenv("APPDATA");
	if (appdata == nullptr) {
		throw std::runtime_error("Could not get APPDATA environment variable");
	}
	return std::filesystem::path{ appdata }.string();
}

int dl_file(string filepath, string file_id) { //return 0 if success, 1 if error

	string request_command = "curl -s -o "+ filepath + " \"" + base_url + "/" + file_id + "?alt=media&key=" + api_key + "\"";
	
	//cout << request_command << endl;
	cout << "\033[1;34m" << "Downloading " << "\033[0m";
	cout << filepath + "... ";
	int result = system(request_command.c_str());

	//Executing script
	if (result == 0) {
		cout << "\033[1;32m" << "Downloaded" << "\033[0m" << endl;
	}
	else {
		cout << "Error while downloading file: " + filepath << endl;
	}
	return result;
}

vector<string> get_local_files(std::string folder_path) {
	vector<string> jar_files;


	for (const auto& entry : fs::directory_iterator(folder_path))
	{
		string file_name = entry.path().generic_string();
		

		file_name.erase(0, folder_path.length());

		string file_extention = file_name;
		if (file_extention.substr(file_extention.find_last_of(".") + 1) == "jar") //file has the extention asked
		{
			jar_files.push_back(file_name);
		}
	}
	return jar_files;
}


void check_mods(string remote_index_file_name) {
	

	ifstream remote_index_stream(remote_index_file_name);
	json parsed_remote_index = json::parse(remote_index_stream);

	vector<string> local_jar_files = get_local_files(mods_folder);

	cout << "Local files count: " << local_jar_files.size() << endl;
	cout << "Remote files count: " << parsed_remote_index["files"].size() << endl;


	for (size_t i = 0; i < parsed_remote_index["files"].size(); i++) {
		cout << "------------------------" << endl;

		string filename = parsed_remote_index["files"][i].value("name", "Not Found");
		string file_id = parsed_remote_index["files"][i].value("id", "Not Found");

		string temp_filename = filename;


		if (temp_filename.erase(5, temp_filename.length()) == "forge") {
			cout << "File detected is forge" << endl;
			continue;
		}

		cout << "File name " << i+1 << "/" << parsed_remote_index["files"].size() << ": " + filename + "... ";

		//search in all the local files the current remote file
		bool is_file_present = false;

		for (size_t j = 0; j < local_jar_files.size(); j++) {
			if (local_jar_files[j].at(0) == '/' || local_jar_files[j].at(0) == '\\') {
				local_jar_files[j].erase(0, 1);
			}

			if (filename == local_jar_files[j]) {
				is_file_present = true;
				cout << "\033[1;32m" << "Found!" << "\033[0m" << endl;
				break;
			}
		}

		if (is_file_present == false) {
			cout << "\033[1;31m" << "Not found" << "\033[0m" << endl;
			try {
				std::string appdata_path = getAppDataPath();
				dl_file(appdata_path + "\\.minecraft\\mods\\" + filename, file_id);
			}
			catch (const std::exception& ex) {
				std::cerr << "Error: " << ex.what() << std::endl;
				break;
			}

		}

	}
	
	local_jar_files.clear();

}


// Returns: 0 -> up to date; 1 -> forge just installed
int check_forge(string remote_index_file_name) {
	ifstream remote_index_stream(remote_index_file_name);
	json parsed_remote_index = json::parse(remote_index_stream);


	//Check if the correct version for forge is installed
	for (size_t i = 0; i < parsed_remote_index["files"].size(); i++) {
		string filename = parsed_remote_index["files"][i].value("name", "Not Found");
		string file_id = parsed_remote_index["files"][i].value("id", "Not Found");
		string temp_filename = filename;

		// Check if the 5 first letters of the file name are "forge"
		if (temp_filename.erase(5, temp_filename.length()) == "forge") {

			temp_filename = filename;
			string forge_version = temp_filename.erase(0, 6).erase(temp_filename.find_last_of("-"), temp_filename.length());

			cout << "Forge version needed: " << forge_version << endl; // Sould look like "1.16.5-36.1.0"
			temp_filename = filename;
			string temp_forge_v = forge_version;
			string forge_id_version = temp_forge_v.erase(0, temp_forge_v.find_last_of("-") + 1);
			temp_filename = filename;
			temp_forge_v = forge_version;
			string forge_minecraft_version = temp_forge_v.erase(temp_forge_v.find_last_of("-"), temp_forge_v.length());

			//now in versions/ if 1.18.2-forge-40.2.0 folder exists, then forge is up to date (dl not needed)
			if (fs::exists(user_folder + "\\AppData\\Roaming\\.minecraft\\versions\\" + forge_minecraft_version + "-forge-" + forge_id_version)) {
				cout << "Forge is up to date" << endl;
				return 0;
			}
			else {
				dl_file(mods_folder + "\\" + filename, file_id);

				// Execute .jar installer
				cout << "Please install forge by the windows which will apear..." << endl;
				string command = "java -jar " + mods_folder + "\\" + filename;
				system(command.c_str());
				return 1;
			}

		}
	}

	return 2; // Error Unhandled
}

int check_mods_folder(string remote_index_file_name) {

	ifstream remote_index_stream(remote_index_file_name);
	json parsed_remote_index = json::parse(remote_index_stream);

	if (fs::exists(mods_folder)) {
		cout << "Mods folder exist" << endl;
		return 0;		
	}
	else {
		cout << "Mods folder not found" << endl;
		return 1;
	}
}

/*
Return remote index file name
*/
string retrieve_remote_files_index()
{
	string text;
	string remote_index_file_name("temp_data.json");
	string args = "?q='" + folder_id + "'+in+parents&key=" + api_key;


	string request_command = "curl -s -o " + remote_index_file_name +" \"" + base_url + args + "\"";
	
	cout << "\033[1;34m" << "Downloading " << "\033[0m";

	cout << "Remote Index File... ";
	int result = system(request_command.c_str());//Executing script
	if (result == 0) {
		cout << "\033[1;32m" << "Downloaded!" << "\033[0m" << endl;
	}
	else {
		cout << "Error while downloading file: " + remote_index_file_name << endl;
	}
	return remote_index_file_name;
}

//Download_Minecaft_mods core
int DownloadMinecaftModsCore() {
	string remote_index_filename = retrieve_remote_files_index();
	fs::path fullPath = fs::current_path() / remote_index_filename;

	int result = check_mods_folder(remote_index_filename);
	
	if (result == 0) {
		check_forge(remote_index_filename);
		check_mods(remote_index_filename);
		// launch minecraft
		cout << "\033[1;32m" << "Launching Minecraft... " << "\033[0m" << endl;
		system(MINECRAFT_LAUNCH_COMMAND.c_str());
	}
	else {
		fs::create_directory(mods_folder);
		if (check_forge(remote_index_filename) == 1) {
			cout << "\033[1;32m" << "Please launch minecraft with forge configuration once to generate forge files" << "\033[0m" << endl;
			std::this_thread::sleep_for(std::chrono::seconds(2));
			cout << "\033[1;32m" << "Launching Minecraft... " << "\033[0m" << endl;
			system(MINECRAFT_LAUNCH_COMMAND.c_str());
		}
		else {
			check_mods(remote_index_filename);
			cout << "\033[1;32m" << "Launching Minecraft... " << "\033[0m" << endl;
			system(MINECRAFT_LAUNCH_COMMAND.c_str());
		}
	}

	remove("temp_data.json");

	int timeout_seconds = 5;

	cout << "\033[1;32m" << "This window will close in 5 sec ..." << "\033[0m" << endl;

	std::this_thread::sleep_for(std::chrono::seconds(timeout_seconds));

	return 0;
}

//For show the Ui after opening settings
int ShowConsoleUI(bool clear_at_start) {
	int result = UI::RunConsoleUi(clear_at_start);
	int timeout_seconds = 10;

	switch (result) {
		case 0: // exit
			return 0;
		case 1: // Download and launch
			cout << "" << endl;// Line Break
			DownloadMinecaftModsCore();
			return 0;
		case 2: // Settings
			cout << "\033[1;31m" << "Please restart the program to apply the changes" << "\033[0m" << endl;
			std::this_thread::sleep_for(std::chrono::seconds(timeout_seconds));
			return 0;
		default:
			return 0;
	}
}


int main() {
	if (ON_WINDOWS) {
		int result = setSettings();
		string remote_folder_name = retrieve_remote_folder_name();

		cout << "Configured for: " << remote_folder_name << endl;

		if (result == 0) {
			ShowConsoleUI(0);
		}
		else if (result == 1) {
			ShowConsoleUI(0);
		}
		else {
			cerr << "\033[1;31m" << "Error while setting settings" << "\033[0m" << endl;
		}
	}
	else {
		cout << "Please run this program on a Windows X64 system" << endl;
	}
	return 0;
}