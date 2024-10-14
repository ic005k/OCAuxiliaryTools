
# OpenCore Auxiliary Tools (OCAT)

![GUI](https://user-images.githubusercontent.com/76865553/165901706-abbccb4a-89bc-4b03-b6e5-f52dcb10b53c.png)

## About
OpenCore Auxiliary Tools is a GUI-based Configurator for editing `config.plist` files for Acidanthera's OpenCore Boot Manager.

Unlike other Configurator apps, OCAT doesn't mess up the config if the Devs of OpenCore add new features/keys to the config file structure. Instead, it adapts and integrates them in the interface automatically.

## Features

Besides beeing a plist Configurator it can do much more:

* Mount the EFI partion and automatically open the config.plist
* Update and migrate configs to the latest specs simply by clicking on the "Save" button. No more copying of keys and manually validtaing the config is required (huge timesaver).
* Automatically performs config validatation and points to possible conflicts/configuration issues.
* Sync Feature: check for and apply updates for OpenCore, Resources and Kext with a few clicks
* Select between Official/Dev Release and Debug builds of OpenCore
* Dropdown Menus with suggested Quirks for Intel and AMD CPUs
* Preset Menus for ACPI, Kernel and other lists.
* Editable list with URLs of Kext Repos
* Integrated ASCII < > HEX Converter 
* Database with complete base configs for Intel and AMD CPUs based on Dortania's OpenCore Install Guide
* Ability to generate EFI folders from a config with a single click
* Ability to Create EFI folder Backups

## Guides

* [Updating OpenCore and Kexts with OCAT (by 5T33Z0)](https://github.com/5T33Z0/OC-Little-Translated/blob/main/D_Updating_OpenCore/README.md)
* Pre-selected plug-in content for Intel CPU Quirks is provided and maintained by 5T33Z0. [Details](https://github.com/5T33Z0/OC-Little-Translated/tree/main/F_Desktop_EFIs/preset)
* Intel CPU BaseConfigs plug-in package provided and maintained by 5T33Z0. [Details](https://github.com/5T33Z0/OC-Little-Translated/tree/main/F_Desktop_EFIs)
* [OpenCore Auxiliary Tools User's Guide (by chriswayg)](https://chriswayg.gitbook.io/opencore-visual-beginners-guide/oc_auxiliary_tools)


## About adapt OpenCore version  （关于OCAT支持的OpenCore版本的问题）
[#311](https://github.com/ic005k/OCAuxiliaryTools/issues/311) [#361](https://github.com/ic005k/OCAuxiliaryTools/issues/361)

OCAT is Smart adapt to openCore version,if you need update  openCore version，you can get new OpenCore like this 
（OCAT 是智能适应OC的新版本的，如果你需要更新OC版本,你可以按照下图操作）

First,  click this button （第一步，点击这个按钮）
![image](https://github.com/ic005k/OCAuxiliaryTools/assets/7905253/b4c94679-cd8c-4243-a85f-c70c1c304665)

Second, select the latest version or the other version from the drop-down list（第二步，在下拉列表里选择最新的版本或者其他的OC版本）

![image](https://github.com/ic005k/OCAuxiliaryTools/assets/7905253/240a7831-fcb5-411e-842d-bffd22cc2982)

Third, click to get the latest version, or get the OC version of your choice（第三步，点击获取最新版，或者获取你选择的OC版本）
![image](https://github.com/ic005k/OCAuxiliaryTools/assets/7905253/e3331a23-4744-4075-b820-e58878c36dc8)


## Credits
* [vit9696](https://github.com/vit9696) OCAT feature suggestions etc.
* [5T33Z0](https://github.com/5T33Z0) Intel CPU plug-in package for OCAT, suggestions for user interaction and functionality, Write Readme, etc.
* [LucasMucGH](https://github.com/LucasMucGH) Native UI design suggestions for Mac-based versions, etc.
* [chriswayg](https://github.com/chriswayg) OpenCore Auxiliary Tools User's Guide.
* APP Icon Design: Mirone (Brazil).
* [OpenCore](https://github.com/acidanthera/OpenCorePkg)&nbsp; &nbsp; &nbsp; &nbsp;
[qtplist](https://github.com/reillywatson/qtplist)&nbsp; &nbsp; &nbsp; &nbsp;
[FindESP](https://github.com/bluer007/FindESP)&nbsp; &nbsp; &nbsp; &nbsp;
[winfile](https://github.com/microsoft/winfile)&nbsp; &nbsp; &nbsp; &nbsp;
[PlistCpp](https://github.com/animetrics/PlistCpp)&nbsp; &nbsp; &nbsp; &nbsp;
[pugixml](https://github.com/zeux/pugixml)&nbsp;&nbsp; &nbsp; &nbsp;
[aria2](https://github.com/aria2/aria2)&nbsp; &nbsp; &nbsp;&nbsp;
[wget](http://wget.addictivecode.org/)&nbsp; &nbsp; &nbsp;&nbsp;
[DirectionalToolTip](https://github.com/scondratev/DirectionalToolTip)&nbsp; &nbsp; &nbsp;&nbsp;
[dortania build-repo](https://github.com/dortania/build-repo)&nbsp; &nbsp; &nbsp;&nbsp;
[HackinPlugins](https://github.com/bugprogrammer/HackinPlugins)&nbsp; &nbsp; &nbsp;&nbsp;

---

API: https://api.github.com/repos/ic005k/OCAuxiliaryTools/releases/latest
