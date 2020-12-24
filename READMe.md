[English](https://github.com/ic005k/QtOpenCoreConfig/blob/master/READMe-en.md) | [简体中文](https://github.com/ic005k/QtOpenCoreConfig/blob/master/READMe.md)
# 开源跨平台的OpenCore配置文件编辑器

OpenCore逐渐流行起来，一直想做一款开源跨平台的OC配置文件编辑器，目前终于达成。这样在mac之外的平台下配置OC显得格外轻松，特别是win和linux平台。

该软件是根据OC最新的开发版提供的SampleCustom.plist为蓝本，使用该软件前，建议将OC升级到最新的开发版。

OC开发版可以在此下载：https://github.com/williambj1/OpenCore-Factory/releases  感谢@williambj1！

非官方简体中文参考手册：https://github.com/SukkaW/OpenCore-Document-zh_Hans  感谢@SukkaW！

以上链接已放入软件的“资源”菜单。

关于软件各功能的解读（翻译）: 由于时间关系，没有细致地去润色及排版，在此表示歉意，同时建议大家研读原版OC说明书。

## 特点包括但不限于：

* 稳健的plist文件底层读写代码，确保数据读写的正确性

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

......

[!截图](https://github.com/ic005k/QtOpenCoreConfig/blob/master/qtocc.png)

### [国内github下载文件推荐](https://toolwa.com/github/)

### 感谢以下第三方开源软件的支持！

[reillywatson](https://github.com/reillywatson/qtplist)&nbsp;&nbsp;&nbsp;&nbsp;[bluer007](https://github.com/bluer007/FindESP)&nbsp;&nbsp;&nbsp;&nbsp;[microsoft](https://github.com/microsoft/winfile)

## 讨论区：

[pcbeta远景](http://bbs.pcbeta.com/viewthread-1866148-1-1.html)&nbsp;&nbsp;&nbsp;&nbsp;[insanelymac](https://www.insanelymac.com/forum/topic/344752-open-source-cross-platform-opencore-configuration-file-editor/)


### 备注：此项目开始于2020年8月

