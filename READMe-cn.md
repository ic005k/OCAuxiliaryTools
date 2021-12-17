[English](https://github.com/ic005k/QtOpenCoreConfig/blob/master/READMe.md) | [简体中文](https://github.com/ic005k/QtOpenCoreConfig/blob/master/READMe-cn.md)
## OC Auxiliary Tools-开源跨平台的OpenCore辅助工具

| [最新发布][release-link]|[下载][download-link]|[问题反馈][issues-link]|
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

OC辅助工具（简称OCAT或ocat） 是一个针对OC的非常简单的实用工具，功能丰富、简洁易用。OCAT保持对用户层面全透明的结构特性，采用完全开放式的设计，它仍在不断发展，欢迎任何问题或建议。

OC开发版可以[在此](https://github.com/acidanthera/OpenCorePkg/actions)下载

* Quirks的预选插件内容由 @5T33Z0 提供并维护。[详情](https://github.com/5T33Z0/OC-Little-Translated/tree/main/F_Desktop_EFIs/preset)
 
* BaseConfigs（基本配置文件）插件包由 @5T33Z0 提供并维护。[详情](https://github.com/5T33Z0/OC-Little-Translated/tree/main/F_Desktop_EFIs)

* OCAT的功能提示采用的是开放式接口，如果您对枯燥的翻译有兴趣，请帮助完善toolTip.txt文件（数据库preset目录中）的内容，期待您的加入！文件地址：https://github.com/ic005k/QtOpenCoreConfig/blob/master/Database/preset/toolTip.txt

* OCAT的预置功能采用的是开放式接口，如果您对ACPI补丁等条目的预置感兴趣，请帮助完善PreSet.plist文件的内容，期待您的加入！

* [如何使用OCAT升级OpenCore和Kexts(by @5T33Z0)](https://github.com/5T33Z0/OC-Little-Translated/blob/main/D_Updating_OpenCore/README.md)

* [OpenCore Quirks for Intel and AMD CPUs(by @5T33Z0)](https://github.com/5T33Z0/OC-Little-Translated/tree/main/08_Quirks)


---

### 如何配置一个全新的EFI？
* 这个过程非常简单，只需点击工具栏上的“数据库”，在数据库列表里面双击与你的机型相匹配的模版文件（感谢 @5T33Z0 提供并维护这些文件），此时会自动在桌面产生一个EFI文件夹，过程完成。

### 如何自动升级OC？（非常简单，只需两步）
* 将配置文件拖拽到OCAT的界面上，点击工具栏上的“保存”。
* 之后再点击工具栏上的“升级OC主程序”，此时会出现一个“同步OC”的界面，点击下面的“开始同步”按钮，OC升级结束。

### 为什么打开配置文件或保存配置文件时，OC验证的图标会变红？
* 这是因为配置文件存在问题，一般在保存配置文件之后就正常了。如果保存配置文件之后，还是提示有问题，则需要根据OC验证的提示来进行排查，这是一个非常简单的过程。如果您不懂英语，那很简单，只需将OC验证的提示内容复制到搜狗或谷歌等翻译上，分分钟您就明白了：）。

### OCAT有哪些便利的操作？
* 所有需要打开的文件均可以通过拖拽到相应的界面进行打开（支持单选或多选）。
* 工具栏上的搜索框（强烈建议修改配置文件内容时采用搜索入口而不用去一个一个地点击标签页，支持模糊匹配，功能非常强大）。
* 鼠标在数据表里面按下左键并拖动，可进行多选。
* 打开两个或多个不同的配置文件，进行对比或数据表之间的复制行、粘贴行（支持单选或多选）。
* ...

### 关于“预置”功能
* 这是一个开放性的接口，预置文件位于数据库目录下preset里面的PreSet.plist（通过“编辑”菜单的“打开数据库目录”），您可以随时采用各种Plist编辑器来编辑它，就像编辑Config.plist一样。很简单，它其实就是从Config.plist里面剥离出来的。

### OCAT获取OpenCore的途径？
* OCAT目前只从OC官方获取OpenCore，它来自这里：https://github.com/acidanthera/OpenCorePkg/actions 也就是OCAT“帮助“菜单中的”OpenCore工厂“。除此之外，OCAT不从任何其它途径获取OpenCore的任何文件。
* 请注意：OCAT使用的所有原材料，均来自OpenCore官方，OCAT不从任何第三方获取任何内容，现在不会，将来也不会。

### 我无法下载Kext的升级或者自动更新OCAT时非常缓慢？
* 很简单，请在“数据库”界面的“杂项”里面选择合适的网络代理，以提高GitHub的下载速度。

### OCAT为什么不自动在线获取OC？
* 因为OC几乎是每月才发布一次稳定版，而且OC更新的时候必然会伴随着OCAT的更新，此时OCAT有足够的时间将OC放入OCAT中，完全没有必要采用在线获取的方式。

### 为什么不为OCAT编写一个完整的文档？
* 因为OCAT被设计得足够简单和清晰，用户几乎可以在没有任何文档的情况下使用OCAT。
* 还有，我几乎可以随时在这里回答用户的任何问题：https://github.com/ic005k/QtOpenCoreConfig/issues

---

### 感谢以下开源软件的支持！

[OpenCore](https://github.com/acidanthera/OpenCorePkg)&nbsp; &nbsp; &nbsp; &nbsp;
[qtplist](https://github.com/reillywatson/qtplist)&nbsp; &nbsp; &nbsp; &nbsp;
[FindESP](https://github.com/bluer007/FindESP)&nbsp; &nbsp; &nbsp; &nbsp;
[winfile](https://github.com/microsoft/winfile)&nbsp; &nbsp; &nbsp; &nbsp;
[PlistCpp](https://github.com/animetrics/PlistCpp)&nbsp; &nbsp; &nbsp; &nbsp;
[pugixml](https://github.com/zeux/pugixml)&nbsp; &nbsp; &nbsp; &nbsp;
[aria2](https://github.com/aria2/aria2)&nbsp; &nbsp; &nbsp;&nbsp;
[wget](http://wget.addictivecode.org/)&nbsp; &nbsp; &nbsp;&nbsp;

### 最后，对所有的贡献者一并表示感谢，在此不再一一列出，谢谢你们的参与和支持！

---

### 备注：此项目开始于2020年8月
API: https://api.github.com/repos/ic005k/QtOpenCoreConfig/releases/latest
