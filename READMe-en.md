[English](https://github.com/ic005k/QtOpenCoreConfig/blob/master/READMe.md) | [简体中文](https://github.com/ic005k/QtOpenCoreConfig/blob/master/READMe-en.md)
# OC Auxiliary Tools-开源跨平台的OpenCore辅助工具

| [最新发布][release-link]|[下载][download-link]|[问题反馈][issues-link]|[讨论区][discourse-link]|
|-----------------|-----------------|-----------------|-----------------|
|[![release-badge](https://img.shields.io/github/release/ic005k/QtOpenCoreConfig.svg?style=flat-square "Release status")](https://github.com/ic005k/QtOpenCoreConfig/releases "Release status") | [![download-badge](https://img.shields.io/github/downloads/ic005k/QtOpenCoreConfig/total.svg?style=flat-square "Download status")](https://github.com/ic005k/QtOpenCoreConfig/releases/latest "Download status")|[![issues-badge](https://img.shields.io/badge/github-issues-red.svg?maxAge=60 "Issues")](https://github.com/ic005k/QtOpenCoreConfig/issues "Issues")|[![discourse](https://img.shields.io/badge/forum-discourse-orange.svg)](https://www.insanelymac.com/forum/topic/344752-open-source-cross-platform-opencore-auxiliary-tools/)|


[![Windows](https://github.com/ic005k/QtOpenCoreConfig/actions/workflows/windows.yml/badge.svg)](https://github.com/ic005k/QtOpenCoreConfig/actions/workflows/windows.yml)      [![MacOS](https://github.com/ic005k/QtOpenCoreConfig/actions/workflows/macos.yml/badge.svg)](https://github.com/ic005k/QtOpenCoreConfig/actions/workflows/macos.yml)       [![MacOS Classical](https://github.com/ic005k/QtOpenCoreConfig/actions/workflows/macos1012.yml/badge.svg)](https://github.com/ic005k/QtOpenCoreConfig/actions/workflows/macos1012.yml)  [![Ubuntu](https://github.com/ic005k/QtOpenCoreConfig/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/ic005k/QtOpenCoreConfig/actions/workflows/ubuntu.yml)    

[download-link]: https://github.com/ic005k/QtOpenCoreConfig/releases/latest "Download status"
[download-badge]: https://img.shields.io/github/downloads/ic005k/QtOpenCoreConfig/total.svg?style=flat-square "Download status"

[release-link]: https://github.com/ic005k/QtOpenCoreConfig/releases "Release status"
[release-badge]: https://img.shields.io/github/release/ic005k/QtOpenCoreConfig.svg?style=flat-square "Release status"

[issues-link]: https://github.com/ic005k/QtOpenCoreConfig/issues "Issues"
[issues-badge]: https://img.shields.io/badge/github-issues-red.svg?maxAge=60 "Issues"

[discourse-link]: https://www.insanelymac.com/forum/topic/344752-open-source-cross-platform-opencore-auxiliary-tools/

OC辅助工具（简称OCAT或ocat） = Config.plist编辑器 + Config.plist数据库 + ...

OC开发版可以[在此](https://github.com/acidanthera/OpenCorePkg/actions)下载

## 如何配置一个全新的EFI？
* 这个过程非常简单，只需点击工具栏上的“数据库”，在数据库列表里面双击“Sample.plist”（这个文件是OC官方提供的配置文件示例），此时会自动在桌面产生一个EFI文件夹，过程完成。
* 编辑修改这个“Sample.plist”之后（主要是要和自己的机器硬件相匹配，这是一个不断调试和测试的过程），可以随时点击工具栏上的“在桌面上生成EFI文件夹“，以刷新之前生成的EFI目录。
* 之后就可以将桌面上的EFI文件夹拷贝到ESP等引导分区进行引导测试。

## 如何自动升级OC？（非常简单，只需两步）
* 将配置文件拖拽到OCAT的界面上，点击工具栏上的“保存”。
* 之后再点击工具栏上的“升级OC主程序”，此时会出现一个“同步OC”的界面，点击下面的“开始同步”按钮，OC升级结束。

## 为什么打开配置文件或保存配置文件时，OC验证的图标会变红？
* 这是因为配置文件存在问题，一般在保存配置文件之后就正常了。如果保存配置文件之后，还是提示有问题，则需要根据OC验证的提示来进行排查，这是一个非常简单的过程。如果您不懂英语，那很简单，只需将OC验证的提示内容复制到搜狗或谷歌等翻译上，分分钟您就明白了：）。

## OCAT有哪些便利的操作？
* 所有需要打开的文件均可以通过拖拽到相应的界面进行打开（支持单选或多选）。
* 工具栏上的搜索框（强烈建议修改配置文件内容时采用搜索入口而不用去一个一个地点击标签页，支持模糊匹配，功能非常强大）。
* 鼠标在数据表里面按下左键并拖动，可进行多选。
* 打开两个或多个不同的配置文件，进行对比或数据表之间的复制行、粘贴行（支持单选或多选）。
* ...

## 关于“预置”功能
* 这是一个开放性的接口，预置文件位于数据库目录下preset里面的PreSet.plist（通过工具栏上的“打开数据库目录”），您可以随时采用各种Plist编辑器来编辑它，就像编辑Config.plist一样。很简单，它其实就是从Config.plist里面剥离出来的。

### 感谢以下开源软件的支持！

[OpenCore](https://github.com/acidanthera/OpenCorePkg)&nbsp; &nbsp; &nbsp; &nbsp;
[qtplist](https://github.com/reillywatson/qtplist)&nbsp; &nbsp; &nbsp; &nbsp;
[FindESP](https://github.com/bluer007/FindESP)&nbsp; &nbsp; &nbsp; &nbsp;
[winfile](https://github.com/microsoft/winfile)&nbsp; &nbsp; &nbsp; &nbsp;
[PlistCpp](https://github.com/animetrics/PlistCpp)&nbsp; &nbsp; &nbsp; &nbsp;
[pugixml](https://github.com/zeux/pugixml)


### 备注：此项目开始于2020年8月

