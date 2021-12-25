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



OC Auxiliary Tool (OCAT or ocat for short) is a very simple utility for OC that is feature-rich and easy to use. OCAT maintains its structural nature of being fully transparent to the user level, with a completely open design, it is still evolving and welcomes any questions or suggestions.

* [Updating OpenCore and Kexts with OCAT (by @5T33Z0)](https://github.com/5T33Z0/OC-Little-Translated/blob/main/D_Updating_OpenCore/README.md)

* Pre-selected plugin content for Quirks is provided and maintained by @5T33Z0.  [Details](https://github.com/5T33Z0/OC-Little-Translated/tree/main/F_Desktop_EFIs/preset)

* The BaseConfigs plugin package is provided and maintained by @5T33Z0. [Details](https://github.com/5T33Z0/OC-Little-Translated/tree/main/F_Desktop_EFIs) 

---

### Special thanks to
* [@vit9696](https://github.com/vit9696) for providing suggestions on OCAT functionality.
* [@5T33Z0](https://github.com/5T33Z0) for providing and maintaining the OCAT plugin package.
* [@LucasMucGH](https://github.com/LucasMucGH) Native UI design suggestions based on the Mac version.

---

### How to configure a new EFI?
* The process is very simple, just click on "Database" on the toolbar, double click on the template file that matches your model in the database list (thanks to @5T33Z0 for providing and maintaining these files), an EFI folder will be automatically created on the desktop and the process is complete.

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

### How does OCAT get OpenCore?
* OCAT currently only gets OpenCore officially from OC, it comes from here: https://github.com/acidanthera/OpenCorePkg/actions which is the "OpenCore Factory" in the "Help" menu of OCAT. Other than that, OCAT does not get any files for OpenCore from any other source.
* Please note: All raw materials used by OCAT come from official OpenCore sources, OCAT does not get any content from any third party, not now, not ever.

### Why doesn't OCAT use an online approach to automatically obtain OC?
* Because OC is only released in stable version almost once a month, and the OC update is inevitably accompanied by an OCAT update, when OCAT has enough time to put the OC into OCAT, there is absolutely no need to use the online fetching method.

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
API: https://api.github.com/repos/ic005k/QtOpenCoreConfig/releases/latest
