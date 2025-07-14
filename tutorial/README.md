# Tutorial to create your own 

To create a new radio station, you need to do the following:

- [Tutorial to create your own](#tutorial-to-create-your-own)
  - [Project creation](#project-creation)
  - [Duplicating audio project file](#duplicating-audio-project-file)
  - [Adding music tracks](#adding-music-tracks)
  - [Override file SystemsConfig.conf](#override-file-systemsconfigconf)
    - [Getting a list of track lengths using a powershell script (Windows)](#getting-a-list-of-track-lengths-using-a-powershell-script-windows)
  - [Testing](#testing)
  - [Publishing](#publishing)

## Project creation

You need to create a new Project in Arma Reforger Tools

<picture><img src="./images/1.png" alt="image-1.png" style="width:auto; height: 300px"></picture>
<picture><img src="./images/2.png" alt="image-2.png" style="width:auto; height: 300px"></picture>

> [!IMPORTANT]
> Add ProjectSonarRadio as a dependency.

> [!TIP]
> You can also see more information about creating a project at [BI wiki](https://community.bohemia.net/wiki/Arma_Reforger:Mod_Project_Setup).


## Duplicating audio project file

Duplicate CustomRadioBroadcast.acp to your project from \ProjectSonar\Sounds\RadioBroadcast\

<picture><img src="./images/3.png" alt="image-3.png" style="width:auto; height: 350px"></picture>
<picture><img src="./images/4.png" alt="image-4.png" style="width:auto; height: 350px"></picture>
<picture><img src="./images/5.png" alt="image-5.png" style="width:auto; height: 350px"></picture>

Name the file as you like.

<picture><img src="./images/6.png" alt="image-6.png" style="width:auto; height: 150px"></picture>


## Adding music tracks

Find the newly created file in your system, create a Samples folder next to it and place your tracks there. If you want to add DJ tracks, you can create a subfolder for DJ tracks inside the Samples folder.

<picture><img src="./images/9.png" alt="image-9.png" style="width:auto; height: 200px"></picture>

<picture><img src="./images/10.png" alt="image-10.png" style="width:auto; height: 200px"></picture>


Open created audio project file (.acp) and find Bank Music (or DJ) there.

<picture><img src="./images/7.png" alt="image-7.png" style="width:auto; height: 300px"></picture>

Scroll to Samples category. Clear the existing placeholder tracks there.

<picture><img src="./images/8.png" alt="image-8.png" style="width:auto; height: 300px"></picture>

Drag your tracks from the Samples folder to Bank Music or DJ node.

<picture><img src="./images/11.png" alt="image-11.png" style="width:auto; height: 300px"></picture>
<picture><img src="./images/12.png" alt="image-12.png" style="width:auto; height: 300px"></picture>
<picture><img src="./images/13.png" alt="image-13.png" style="width:auto; height: 300px"></picture>

> [!IMPORTANT]
> Bank files **Music** and **DJ** **MUST** have atleast 1 track.


## Override file SystemsConfig.conf

Override SystemsConfig.conf in your project from \ProjectSonar\Configs\Systems\SystemsConfig.conf

<picture><img src="./images/14.png" alt="image-14.png" style="width:auto; height: 350px"></picture>
<picture><img src="./images/15.png" alt="image-15.png" style="width:auto; height: 350px"></picture>

Expand Systems > RT_PS_CustomRadioAntennaSystem > Radiostations.
Add a new radio station by clicking on +.
Expand added RT_PS_CustomRadioStation.

<picture><img src="./images/16.png" alt="image-16.png" style="width:auto; height: 400px"></picture>

<picture><img src="./images/17.png" alt="image-17.png" style="width:auto; height: 300px"></picture>

Fill **Radiostation Name**, **Station Audio Project** (select the previously created .acp file).

Fill **Track Length** with length of your tracks (in seconds).

> [!NOTE]
> To get a list of all lengths you can use [powershell script](#getting-a-list-of-track-lengths-using-a-script-windows)

> [!TIP]
> If **not** need DJ tracks. You can leave **DJ Tracks Length** empty or set **DJ Probability** to 0

> [!TIP] 
> If you need DJ tracks.
> Fill **DJ Tracks Length** with length of your tracks.
> Fill **DJ Probability** to launch dj tracks with some probability.


<picture><img src="./images/18.png" alt="image-18.png" style="width:auto; height: 300px"></picture>
<picture><img src="./images/19.png" alt="image-19.png" style="width:auto; height: 300px"></picture>


### Getting a list of track lengths using a powershell script (Windows)

0. if you know how to execute scripts see point 4.
1. Open PowerShell as administrator.
2. Check execution policy by ```Get-ExecutionPolicy``` and remember printed value (for further reverting).
3. Set new execution policy by ```Set-ExecutionPolicy RemoteSigned``` and approve it by type ```A```.
4. Download ```getTimes.ps1``` file.
5. Go to directory with file ```getTimes.ps1``` by command ```cd <Path>```
6. execute ```./getTimes.ps1``` with first argument of directory with music tracks **in brackets**

> [!IMPORTANT]
>  Quotes around path and backslash at the end of the path are important

7. If you need - revert old execution policy by ```Set-ExecutionPolicy <remembered-policy>```.


<picture><img src="./images/25.png" alt="image-19.png" style="width:100%"></picture>

## Testing

To test your radio station, you can open the existing world in the ProjectSonarRadio mod (\ProjectSonar\worlds\Empty.ent)

<picture><img src="./images/20.png" alt="image-20.png" style="width:auto; height: 300px"></picture>
<picture><img src="./images/21.png" alt="image-21.png" style="width:auto; height: 300px"></picture>

Go to any vehicle or radio and check the functionality of your radio station.

<picture><img src="./images/22.png" alt="image-22.png" style="width:auto; height: 300px"></picture>

## Publishing

> [!TIP]
> The publishing process is best described at [BI wiki](https://community.bistudio.com/wiki/Arma_Reforger:Mod_Publishing_Process)

<picture><img src="./images/23.png" alt="image-23.png" style="width:auto; height: 350px"></picture>
<picture><img src="./images/24.png" alt="image-24.png" style="width:auto; height: 350px"></picture>