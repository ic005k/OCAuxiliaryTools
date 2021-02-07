[English](https://github.com/ic005k/QtOpenCoreConfig/blob/master/READMe.md) | [简体中文](https://github.com/ic005k/QtOpenCoreConfig/blob/master/READMe-en.md)
# 开源跨平台的OpenCore辅助工具

OC辅助工具 = Config.plist编辑器 + Config.plist数据库 + ...

OC开发版可以在此下载：https://github.com/acidanthera/OpenCorePkg/actions

非官方简体中文参考手册：https://github.com/SukkaW/OpenCore-Document-zh_Hans  感谢@SukkaW！

以上链接已放入软件的“帮助”菜单。

关于软件各功能的解读（翻译）: 由于时间关系，没有细致地去润色及排版，在此表示歉意，同时建议大家研读原版OC说明书。

## 特点包括但不限于：

* 稳健的plist文件底层读写代码，确保数据读写的正确性

* 内置配置文件数据库（[分享配置文件](https://github.com/ic005k/QtOpenCoreConfigDatabase/issues))

* 根据配置文件在桌面上生成EFI文件夹

* 多语言支持（目前支持中文和英文界面），自动识别操作系统语言并根据语种加载相应本土化的功能

* 开源、跨平台（win、mac、linux均提供二进制文件的下载）

* 各功能选项采用悬浮提示和状态栏提示

* 完全可视化对Config.plist进行编辑

* 紧跟OpenCore最新开发版，完全与之同步

* 一键挂载ESP分区（mac、win）

* 模块的导入与导出

* 双击打开文件、拖拽文件到软件界面打开文件，打开文件的历史记录显示

* 三个平台均采用OC官方的macserial来生成序列号

* 软件采用全键值写入，完美配合OC的ocvalidate数据检查

* 软件UI自适应各种分辨率，包括winpe和linux livecd等等

* 内置更新检查

......

![截图](https://github.com/ic005k/QtOpenCoreConfig/blob/master/ocat2.png)

### [国内github下载文件推荐](https://toolwa.com/github/)

### 感谢以下开源软件的支持！

[reillywatson](https://github.com/reillywatson/qtplist)&nbsp;&nbsp;&nbsp;&nbsp;[bluer007](https://github.com/bluer007/FindESP)&nbsp;&nbsp;&nbsp;&nbsp;[microsoft](https://github.com/microsoft/winfile)

## 讨论区：

[insanelymac](https://www.insanelymac.com/forum/topic/344752-open-source-cross-platform-opencore-configuration-file-editor/)


### 备注：此项目开始于2020年8月

