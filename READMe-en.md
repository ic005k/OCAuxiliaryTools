[English](https://github.com/ic005k/QtOpenCoreConfig/blob/master/READMe.md) | [简体中文](https://github.com/ic005k/QtOpenCoreConfig/blob/master/READMe-en.md)
# 开源跨平台的OpenCore辅助工具

| [最新发布][release-link]|[下载][download-link]|[问题反馈][issues-link]|[讨论区][discourse-link]|
|-----------------|-----------------|-----------------|-----------------|
|[![release-badge](https://img.shields.io/github/release/ic005k/QtOpenCoreConfig.svg?style=flat-square "Release status")](https://github.com/ic005k/QtOpenCoreConfig/releases "Release status") | [![download-badge](https://img.shields.io/github/downloads/ic005k/QtOpenCoreConfig/total.svg?style=flat-square "Download status")](https://github.com/ic005k/QtOpenCoreConfig/releases/latest "Download status")|[![issues-badge](https://img.shields.io/badge/github-issues-red.svg?maxAge=60 "Issues")](https://github.com/ic005k/QtOpenCoreConfig/issues "Issues")|[![discourse](https://img.shields.io/badge/forum-discourse-orange.svg)](https://www.insanelymac.com/forum/topic/344752-open-source-cross-platform-opencore-auxiliary-tools/)|


[![Windows](https://github.com/ic005k/QtOpenCoreConfig/actions/workflows/windows.yml/badge.svg)](https://github.com/ic005k/QtOpenCoreConfig/actions/workflows/windows.yml)      [![MacOS](https://github.com/ic005k/QtOpenCoreConfig/actions/workflows/macos.yml/badge.svg)](https://github.com/ic005k/QtOpenCoreConfig/actions/workflows/macos.yml)         [![Ubuntu](https://github.com/ic005k/QtOpenCoreConfig/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/ic005k/QtOpenCoreConfig/actions/workflows/ubuntu.yml)

[download-link]: https://github.com/ic005k/QtOpenCoreConfig/releases/latest "Download status"
[download-badge]: https://img.shields.io/github/downloads/ic005k/QtOpenCoreConfig/total.svg?style=flat-square "Download status"

[release-link]: https://github.com/ic005k/QtOpenCoreConfig/releases "Release status"
[release-badge]: https://img.shields.io/github/release/ic005k/QtOpenCoreConfig.svg?style=flat-square "Release status"

[issues-link]: https://github.com/ic005k/QtOpenCoreConfig/issues "Issues"
[issues-badge]: https://img.shields.io/badge/github-issues-red.svg?maxAge=60 "Issues"

[discourse-link]: https://www.insanelymac.com/forum/topic/344752-open-source-cross-platform-opencore-auxiliary-tools/




OC辅助工具（简称OCAT或ocat） = Config.plist编辑器 + Config.plist数据库 + ...

OC开发版可以在此下载：https://github.com/acidanthera/OpenCorePkg/actions

非官方简体中文参考手册：https://github.com/SukkaW/OpenCore-Document-zh_Hans  感谢@SukkaW！

以上链接已放入软件的“帮助”菜单。

关于软件各功能的解读（翻译）: 由于时间关系，没有细致地去润色及排版，在此表示歉意，同时建议大家研读原版OC说明书。

## 特点包括但不限于：

* 写入plist文件采用空格进行缩进，以适应所有的编辑器或编辑环境，比如Shell编辑环境

* 强大的关键词搜索功能，包括检索不可视的数据表

* 所有的标签和检查框文本均支持复制（右键菜单）

* 无限级别的撤消和重做（打开新文件时自动清除）

* 多进程和单进程之间复制行和粘贴行

* 内置良好的数据结构，以适应OpenCore的迅速迭代

* 内置配置文件数据库（[分享配置文件](https://github.com/ic005k/QtOpenCoreConfigDatabase/issues))

* 根据配置文件在桌面上生成EFI文件夹

* 各功能选项采用右键菜单“显示提示”

* 双击打开文件、拖拽文件到软件界面打开文件，打开文件的历史记录显示

* 软件UI自适应各种分辨率，包括winpe和linux livecd等等

* 内置更新检查

......

![Screenshots](https://github.com/ic005k/QtOpenCoreConfig/blob/master/Screenshots/ocat1.png)

![Screenshots](https://github.com/ic005k/QtOpenCoreConfig/blob/master/Screenshots/ocat2.png)

![Screenshots](https://github.com/ic005k/QtOpenCoreConfig/blob/master/Screenshots/ocat3.png)

![Screenshots](https://github.com/ic005k/QtOpenCoreConfig/blob/master/Screenshots/ocat4.png)

![Screenshots](https://github.com/ic005k/QtOpenCoreConfig/blob/master/Screenshots/ocat5.png)

![Screenshots](https://github.com/ic005k/QtOpenCoreConfig/blob/master/Screenshots/ocat6.png)

![Screenshots](https://github.com/ic005k/QtOpenCoreConfig/blob/master/Screenshots/ocat7.png)

![Screenshots](https://github.com/ic005k/QtOpenCoreConfig/blob/master/Screenshots/ocat8.png)

![Screenshots](https://github.com/ic005k/QtOpenCoreConfig/blob/master/Screenshots/ocat9.png)

![Screenshots](https://github.com/ic005k/QtOpenCoreConfig/blob/master/Screenshots/ocat10.png)

### [国内github下载文件推荐](https://toolwa.com/github/)

### 感谢以下开源软件的支持！

[qtplist](https://github.com/reillywatson/qtplist)&nbsp; &nbsp; &nbsp; &nbsp;
[FindESP](https://github.com/bluer007/FindESP)&nbsp; &nbsp; &nbsp; &nbsp;
[winfile](https://github.com/microsoft/winfile)&nbsp; &nbsp; &nbsp; &nbsp;
[PlistCpp](https://github.com/animetrics/PlistCpp)&nbsp; &nbsp; &nbsp; &nbsp;
[pugixml](https://github.com/zeux/pugixml)

## 讨论区：

[insanelymac](https://www.insanelymac.com/forum/topic/344752-open-source-cross-platform-opencore-configuration-file-editor/)


### 备注：此项目开始于2020年8月

