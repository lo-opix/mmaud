# Minecraft Mods Auto-Updater With Drive (MMAUD)
This is a command-line program that downloads and updates the 
Minecraft mods from a remote Google Drive folder.<br>
It compares the local mod files with the remote ones and downloads any missing or 
outdated ones.
<hr>

### Aussi disponible en [Francais](README_FR.md)

<hr>


## Requirements
- Windows 10 64 bit or higher
- Java 8 or higher
- Minecraft launcher installed by MS Store

## Usage
### As a Player

1. Download the latest release from the [releases page](https://github.com/lo-opix/mmaud/releases/)
2. Execute the .exe file
3. Windows will show you a warning, click "More info" and then "Run anyway".
>If you don't want to do this, you can compile the program yourself

4. Open settings with entering `2` in the console
5. At the row `google_drive_folder_id=` enter the id prowided by the admin of the server
4. The program will download all the mods from google drive and put them in the `Appdata/.minecraft/mods` folder
5. The program will launch Minecraft
<hr>

### As an Admin minecraft server

 * You need to provide the id of your google drive folder which contains the mods.
You can get the id from the url of the folder: 
 * In this url `https://drive.google.com/drive/folders/1Z2X3C4V5B6N7M8K9J0H1G2F3D4S5A6Q7?usp=sharing` the id is `1Z2X3C4V5B6N7M8K9J0H1G2F3D4S5A6Q7`
 * Rename the folder to the name of your server
>Please note that your folder have to be set on enveryone that have the link can view the folder

## ToDo
- [x] Add a Menu page at first start
- [x] Add settings page
- [ ] Add languages
- [x] Add option for the google drive remote id

## Contributing

If you want to contribute, you can do so by opening a pull request or an issue.

## Used third-party tools
- [Json](https://github.com/nlohmann/json) for parsing Google Drive HTTP responses 

## Known issues
- If the API_KEY is invalid, the program will skip the verification of mods and launch minecraft
- The language setting don't work yet
