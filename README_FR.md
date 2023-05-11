# Mise à jour automatique des mods Minecraft avec lecteur (MMAUD)
Il s'agit d'un programme en ligne de commande qui télécharge et met à jour
un modpack minecraft à partir d'un dossier Google Drive distant.<br>
Il compare les fichiers de mod locaux avec les fichiers distants et télécharge tous les fichiers manquants ou
les obsolètes.
<h>

## Exigences
- Windows 10 64 bits ou supérieur
- Java 8 ou supérieur
- Lanceur Minecraft installé par Microsoft Store

## Utilisation
### En tant que joueur

1. Téléchargez la dernière version depuis la [page des versions](https://github.com/lo-opix/mmaud/releases/)
2. Exécutez le fichier .exe
3. Windows vous montrera un avertissement, cliquez sur "Plus d'infos" puis sur "Exécuter quand même".
> Si vous ne voulez pas faire cela, vous pouvez compiler le programme vous-même

4. Ouvrez les paramètres en entrant '2' dans la console
5. À la ligne `google_drive_folder_id=` entrez l'identifiant fourni par l'administrateur du serveur
4. Le programme téléchargera tous les mods de Google Drive et les placera dans le dossier `Appdata/.minecraft/mods`
5. Le programme lancera Minecraft
<h>

### En tant que serveur d'administrateur de seveur minecraft

  * Vous devez fournir l'identifiant de votre dossier Google Drive qui contient les mods.
Vous pouvez obtenir l'identifiant à partir de l'url du dossier :
  * Dans cette URL `https://drive.google.com/drive/folders/1Z2X3C4V5B6N7M8K9J0H1G2F3D4S5A6Q7?usp=sharing` l'identifiant est `1Z2X3C4V5B6N7M8K9J0H1G2F3D4S5A6Q7`
  * Renommez le dossier au nom de votre serveur
>Veuillez noter que votre dossier doit être défini sur: <br>"Tous ceux qui ont le lien peuvent voir le dossier"


## Contribution

Si vous souhaitez contribuer, vous pouvez le faire en ouvrant une pull request ou un ticket.

## Outils tiers utilisés
- [Json](https://github.com/nlohmann/json) pour analyser les réponses HTTP de Google Drive

## Problèmes connus
- Si l'API_KEY est invalide, le programme sautera la vérification des mods et lancera minecraft
- Le paramètre de langue ne fonctionne pas encore