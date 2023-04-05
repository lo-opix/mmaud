# Minecraft Mods Auto-Updater With Drive (MMAUD)
This is a command-line program that downloads and updates the 
Minecraft mods from a remote Google Drive folder.<br>
It compares the local mod files with the remote ones and downloads any missing or 
outdated ones.

## Usage

1. Download the latest release from the [releases page](releases/latest)
2. Execute the .exe file
3. Windows will show you a warning, click "More info" and then "Run anyway".
>If you don't want to do this, you can compile the program yourself

4. The program will download all the mods from google drive and put them in the `Appdata/.minecraft/mods` folder

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
