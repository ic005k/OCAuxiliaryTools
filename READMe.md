[English](https://github.com/ic005k/QtOpenCoreConfig/blob/master/READMe.md) | [简体中文](https://github.com/ic005k/QtOpenCoreConfig/blob/master/READMe-cn.md)
## OC Auxiliary Tools-Open source cross-platform OpenCore auxiliary tools


| [Release][release-link]|[Download][download-link]|[Issues][issues-link]|
|-----------------|-----------------|-----------------|
|[![release-badge](https://img.shields.io/github/release/ic005k/QtOpenCoreConfig.svg?style=flat-square "Release status")](https://github.com/ic005k/QtOpenCoreConfig/releases "Release status") | [![download-badge](https://img.shields.io/github/downloads/ic005k/QtOpenCoreConfig/total.svg?style=flat-square "Download status")](https://github.com/ic005k/QtOpenCoreConfig/releases/latest "Download status")|[![issues-badge](https://img.shields.io/badge/github-issues-red.svg?maxAge=60 "Issues")](https://github.com/ic005k/QtOpenCoreConfig/issues "Issues")|

[![Windows](https://github.com/ic005k/QtOpenCoreConfig/actions/workflows/windows.yml/badge.svg)](https://github.com/ic005k/QtOpenCoreConfig/actions/workflows/windows.yml)      [![MacOS](https://github.com/ic005k/QtOpenCoreConfig/actions/workflows/macos.yml/badge.svg)](https://github.com/ic005k/QtOpenCoreConfig/actions/workflows/macos.yml)       [![MacOS Classical](https://github.com/ic005k/QtOpenCoreConfig/actions/workflows/macos1012.yml/badge.svg)](https://github.com/ic005k/QtOpenCoreConfig/actions/workflows/macos1012.yml)  [![Ubuntu](https://github.com/ic005k/QtOpenCoreConfig/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/ic005k/QtOpenCoreConfig/actions/workflows/ubuntu.yml)    

[download-link]: https://github.com/ic005k/QtOpenCoreConfig/releases/latest "Download status"
[download-badge]: https://img.shields.io/github/downloads/ic005k/QtOpenCoreConfig/total.svg?style=flat-square "Download status"

[release-link]: https://github.com/ic005k/QtOpenCoreConfig/releases "Release status"
[release-badge]: https://img.shields.io/github/release/ic005k/QtOpenCoreConfig.svg?style=flat-square "Release status"

[issues-link]: https://github.com/ic005k/QtOpenCoreConfig/issues "Issues"
[issues-badge]: https://img.shields.io/badge/github-issues-red.svg?maxAge=60 "Issues"

[discourse-link]: https://www.insanelymac.com/forum/topic/344752-open-source-cross-platform-opencore-auxiliary-tools/



OC auxiliary tools（OCAT or ocat for short） = Config.plist editor + Config.plist database + ...

OC development version can be downloaded [here](https://github.com/acidanthera/OpenCorePkg/actions).

---

### How to configure a new EFI?
* Just click "Database" on the toolbar and double click "Sample.plist" in the database list (this file is an example of the official OC configuration file), then a EFI folder on the desktop, the process is complete.
* After editing and modifying this "Sample.plist" (mainly to match with your own machine hardware, which is a continuous debugging and testing process), you can always select the "Edit" menu in the "Generate EFI folder on desktop" to refresh the previously generated EFI directory.
* After that, you can copy the EFI folder on the desktop to a boot partition such as ESP for boot testing.

### How to upgrade OC automatically? (Very simple, only two steps)
* Drag and drop the configuration file onto the OCAT interface and click "Save" on the toolbar.
* Then click "Upgrade OC Main Program" on the toolbar, a "Synchronize OC" interface will appear, Click the "Start Sync" button below to finish the OC upgrade.

### Why does the OC verification icon turn red when I open or save the configuration file?
* This is because there is a problem with the configuration file, usually it will be normal after saving the configuration file. If there is still a problem prompted after saving the configuration file, it is a very simple process to troubleshoot according to the OC verify prompt. 

### What are the convenient operations of OCAT?
* All files that need to be opened can be opened by dragging and dropping them to the corresponding interface (single or multiple selection is supported).
* Search box on the toolbar (it is highly recommended to use the search portal when modifying configuration files instead of clicking on the tabs one by one, it supports fuzzy matching and is very powerful).
* Left click and drag the mouse inside the data table to make multiple selections.
* Open two or more different profiles to compare or copy rows or paste rows between data tables (supports single or multiple selection).
* ...

### About the "Presets" feature
* This is an open interface, The preset file is located in "Database/preset/PreSet.plist" (Open database directory" from the "Edit" menu), and you can edit it at any time using various Plist editors, just like you can edit Config.plist. It's simple, it's actually stripped from Config.plist.

### How does OCAT get OpenCore?
* OCAT currently only gets OpenCore officially from OC, it comes from here: https://github.com/acidanthera/OpenCorePkg/actions which is the "OpenCore Factory" in the "Help" menu of OCAT. Other than that, OCAT does not get any files for OpenCore from any other source.
* Please note: All raw materials used by OCAT come from official OpenCore sources, OCAT does not get any content from any third party, not now, not ever.

### I can't download the Kext update Or is OCAT slow to automatically download updates?
* It's simple, please select the appropriate web proxy in the "Misc" section of the "Database" screen to improve the download speed of GitHub.

### Why does OCAT follow the development version of OC? Is it always consistent with the development version of OC?
* Normally, OCAT is maintained on the stable version of OC, and once something is added to the configuration file of the OC development version, that's when OCAT switches to the OC development version. The advantage of this is that you can put these additions into OCAT in advance, and when OC releases a new stable version, OCAT will not seem to be lagging behind.
* No, OCAT only updates the development version of OC periodically. If you need to update OC daily, please do it manually, OCAT can't do that. This is something you need to please be aware of.

### Why doesn't OCAT use an online approach to automatically obtain OC?
* Because OC is only released in stable version almost once a month, and the OC update is inevitably accompanied by an OCAT update, when OCAT has enough time to put the OC into OCAT, there is absolutely no need to use the online fetching method.

### Can OCAT make a switch option for stable and development versions of OC?
* No. OCAT does not consider this feature at the moment, the goal of OCAT is to be simple, clear and easy to use. If you do not want to use the development version of OC, please download a specific version of OCAT and turn off "Automatic update checking" (in the "Help" menu).

---

### Thanks for the support of the following open source software!

[OpenCore](https://github.com/acidanthera/OpenCorePkg)&nbsp; &nbsp; &nbsp; &nbsp;
[qtplist](https://github.com/reillywatson/qtplist)&nbsp; &nbsp; &nbsp; &nbsp;
[FindESP](https://github.com/bluer007/FindESP)&nbsp; &nbsp; &nbsp; &nbsp;
[winfile](https://github.com/microsoft/winfile)&nbsp; &nbsp; &nbsp; &nbsp;
[PlistCpp](https://github.com/animetrics/PlistCpp)&nbsp; &nbsp; &nbsp; &nbsp;
[pugixml](https://github.com/zeux/pugixml)&nbsp;&nbsp; &nbsp; &nbsp;
[aria2](https://github.com/aria2/aria2)&nbsp; &nbsp; &nbsp;&nbsp;
[wget](http://wget.addictivecode.org/)&nbsp; &nbsp; &nbsp;&nbsp;

### Finally, a big thank you to all the contributors together, not to be listed here, for your participation and support!

---

### Note: This project started in August 2020
