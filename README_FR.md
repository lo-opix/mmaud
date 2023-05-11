# Mise � jour automatique des mods Minecraft avec lecteur (MMAUD)
Il s'agit d'un programme en ligne de commande qui t�l�charge et met � jour
un modpack minecraft � partir d'un dossier Google�Drive distant.<br>
Il compare les fichiers de mod locaux avec les fichiers distants et t�l�charge tous les fichiers manquants ou
les obsol�tes.
<h>

## Exigences
- Windows 10 64 bits ou sup�rieur
- Java 8 ou sup�rieur
- Lanceur Minecraft install� par Microsoft Store

## Utilisation
### En tant que joueur

1. T�l�chargez la derni�re version depuis la [page des versions](https://github.com/lo-opix/mmaud/releases/)
2. Ex�cutez le fichier .exe
3. Windows vous montrera un avertissement, cliquez sur "Plus d'infos" puis sur "Ex�cuter quand m�me".
> Si vous ne voulez pas faire cela, vous pouvez compiler le programme vous-m�me

4. Ouvrez les param�tres en entrant '2' dans la console
5. � la ligne `google_drive_folder_id=` entrez l'identifiant fourni par l'administrateur du serveur
4. Le programme t�l�chargera tous les mods de Google Drive et les placera dans le dossier `Appdata/.minecraft/mods`
5. Le programme lancera Minecraft
<h>

### En tant que serveur d'administrateur de seveur minecraft

  * Vous devez fournir l'identifiant de votre dossier Google Drive qui contient les mods.
Vous pouvez obtenir l'identifiant � partir de l'url du dossier�:
  * Dans cette URL `https://drive.google.com/drive/folders/1Z2X3C4V5B6N7M8K9J0H1G2F3D4S5A6Q7?usp=sharing` l'identifiant est `1Z2X3C4V5B6N7M8K9J0H1G2F3D4S5A6Q7`
  * Renommez le dossier au nom de votre serveur
>Veuillez noter que votre dossier doit �tre d�fini sur: <br>"Tous ceux qui ont le lien peuvent voir le dossier"


## Contribution

Si vous souhaitez contribuer, vous pouvez le faire en ouvrant une pull request ou un ticket.

## Outils tiers utilis�s
- [Json](https://github.com/nlohmann/json) pour analyser les r�ponses HTTP de Google Drive

## Probl�mes connus
- Si l'API_KEY est invalide, le programme sautera la v�rification des mods et lancera minecraft
- Le param�tre de langue ne fonctionne pas encore