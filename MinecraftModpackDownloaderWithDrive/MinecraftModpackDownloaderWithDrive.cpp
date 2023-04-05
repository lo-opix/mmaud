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

const string MINECRAFT_LAUNCH_COMMAND("explorer.exe shell:appsFolder\\Microsoft.4297127D64EC6_8wekyb3d8bbwe!Minecraft");

string base_url("https://www.googleapis.com/drive/v3/files");
string folder_id("");
string api_key("API_KEY");
string mods_folder("");
string language("");

int setSettings() {
	#pragma warning(suppress : 4996)
	string user_folder = getenv("USERPROFILE");

	string** settings = Settings::ReadSettingsFile(user_folder);
	for (int i = 0; i < sizeof(settings)/sizeof(settings[0]); i++) {
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

void check_files_to_dl(string remote_index_file_name) {

	ifstream remote_index_stream(remote_index_file_name);
	json parsed_remote_index = json::parse(remote_index_stream);

	size_t local_jar_files_size(0);
	vector<string> local_jar_files = get_local_files("C:/Users/Max/AppData/Roaming/.minecraft/mods/");
	local_jar_files_size = local_jar_files.size();

	cout << "Local files count: " << local_jar_files_size << endl;
	//compare
	cout << "Remote files count: " << parsed_remote_index["files"].size() << endl;
	
	for (size_t i = 0; i < parsed_remote_index["files"].size(); i++) {
	
		string filename = parsed_remote_index["files"][i].value("name", "Not Found");
		string file_id = parsed_remote_index["files"][i].value("id", "Not Found");

		cout << "File name: " + filename + "... ";

		//search in all the local files the current remote file
		bool is_file_present(0);

		for (size_t j = 0; j < local_jar_files_size; j++) { // ERROR: local_jar_files_size is 0
			if (filename == local_jar_files[j]) {
				is_file_present = true;
				cout << "\033[1;32m" << "Found!" << "\033[0m" << endl;
				break;
			}
		}

		if (!is_file_present) {
			cout << "\033[1;31m" << "Not found" << "\033[0m" << endl;
			try {
				std::string appdata_path = getAppDataPath();
				dl_file(appdata_path + "/.minecraft/mods/" + filename, file_id);
			}
			catch (const std::exception& ex) {
				std::cerr << "Error: " << ex.what() << std::endl;
				break;
			}
			
		}
		
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

	check_files_to_dl(fullPath.string());

	remove("temp_data.json");

	// launch minecraft
	cout << "\033[1;32m" << "Launching Minecraft... " << "\033[0m" << endl;
	system(MINECRAFT_LAUNCH_COMMAND.c_str());

	int timeout_seconds = 5;

	cout << "\033[1;32m" << "This window will close in 5 sec ..." << "\033[0m" << endl;

	std::this_thread::sleep_for(std::chrono::seconds(timeout_seconds));

	return 0;
}

//For show the Ui after opening settings
int ShowConsoleUI(bool clear_at_start) {
	int result = UI::RunConsoleUi(clear_at_start);
	switch (result) {
	case 0: // exit
		return 0;
	case 1: // Download and launch
		cout << "" << endl;// Line Break
		DownloadMinecaftModsCore();
		return 0;
	case 2: // Settings
		cout << "\033[1;31m" << "Please restart the program to apply the changes" << "\033[0m" << endl;

		int timeout_seconds = 10;
		std::this_thread::sleep_for(std::chrono::seconds(timeout_seconds));

		return 0;
	}
}


int main() {
	int result = setSettings();

	if (result == 0) {
		ShowConsoleUI(1);
	}
	else if (result == 1) {
		ShowConsoleUI(0);
	}
	else {
		cerr << "\033[1;31m" << "Error while setting settings" << "\033[0m" << endl;
	}

	return 0;
}