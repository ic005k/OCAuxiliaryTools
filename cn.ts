<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_CN">
<context>
    <name>MainWindow</name>
    <message>
        <location filename="mainwindow.ui" line="6333"/>
        <location filename="mainwindow.ui" line="6377"/>
        <source>Open</source>
        <oldsource>打开-Open</oldsource>
        <translation>打开</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6305"/>
        <location filename="mainwindow.ui" line="6382"/>
        <source>Save</source>
        <oldsource>保存-Save</oldsource>
        <translation>保存</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6281"/>
        <location filename="mainwindow.ui" line="6387"/>
        <source>Save As...</source>
        <oldsource>另存-Save As...</oldsource>
        <translation>另存...</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6295"/>
        <source>MountESP</source>
        <oldsource>挂载ESP-MountESP</oldsource>
        <translation>挂载ESP分区</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="229"/>
        <source>Provide reset register and flag in FADT table to enable reboot and shutdown.</source>
        <translation>在FADT表中提供复位寄存器和标志，以启用重启和关机功能。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="232"/>
        <source>FadtEnableReset</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="244"/>
        <source> Cleanup ACPI header fields to workaround macOS ACPI implementation bug causing boot crashes.</source>
        <translation>清理ACPI标头字段以解决导致引导崩溃的macOS ACPI实现错误。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="247"/>
        <source>NormalizeHeaders</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="269"/>
        <source>Attempt to heuristically relocate ACPI memory regions. Not recommended.</source>
        <translation>尝试试探性地重定位ACPI内存区域。 不建议。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="272"/>
        <source>RebaseRegions</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="283"/>
        <source>Reset FACS table HardwareSignature value to 0.</source>
        <translation>将FACS表的HardwareSignature值重置为0。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="286"/>
        <source>ResetHwSig</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="296"/>
        <source>Reset BGRT table Displayed status field to false.</source>
        <translation>重置BGRT表的显示状态字段为false。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="299"/>
        <source>ResetLogoStatus</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="110"/>
        <location filename="mainwindow.ui" line="980"/>
        <location filename="mainwindow.ui" line="3477"/>
        <source>Delete</source>
        <oldsource>删除条目-Delete</oldsource>
        <translation>Delete（删除）</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="60"/>
        <location filename="mainwindow.ui" line="901"/>
        <location filename="mainwindow.ui" line="1077"/>
        <location filename="mainwindow.ui" line="3389"/>
        <source>Add</source>
        <translation>Add（增加）</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="160"/>
        <location filename="mainwindow.ui" line="1243"/>
        <source>Patch</source>
        <oldsource>补丁-Patch</oldsource>
        <translation>Patch（补丁）</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="310"/>
        <location filename="mainwindow.cpp" line="40"/>
        <source>Booter</source>
        <oldsource>启动设置-Booter</oldsource>
        <translation>Booter
引导</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="44"/>
        <location filename="mainwindow.cpp" line="39"/>
        <source>ACPI</source>
        <translation>ACPI</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6312"/>
        <source>Help</source>
        <translation>帮助文档</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="225"/>
        <source>Mainly required on legacy hardware and few laptops. Can also fix power-button shortcuts. Not recommended
unless required.</source>
        <translation>主要在旧硬件和少量笔记本电脑上需要。 也可以修复电源按钮的快捷方式。 除非需要，否则不建议使用。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="239"/>
        <source> Cleanup ACPI header fields to workaround macOS ACPI implementation bug causing boot crashes.
Reference: Debugging AppleACPIPlatform on 10.13 by Alex James aka theracermaster. The issue is fixed in
macOS Mojave (10.14).</source>
        <translation>清理ACPI标头字段以解决导致引导崩溃的macOS ACPI实现错误。 参考：Alex James aka theracermaster在10.13上调试AppleACPIPlatform。 问题已在macOS Mojave（10.14）中修复。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="254"/>
        <source>ACPI tables are often generated dynamically by underlying firmware implementation. 
Among the position-independent code, 
ACPI tables may contain physical addresses of MMIO areas used for device configuration,
usually grouped in regions (e.g. OperationRegion). Changing firmware settings or hardware configuration,
upgrading or patching the firmware inevitably leads to changes in dynamically generated ACPI code, which
sometimes lead to the shift of the addresses in aforementioned OperationRegion constructions.
For this reason it is very dangerous to apply any kind of modifications to ACPI tables. The most reasonable
approach is to make as few as possible changes to ACPI and try to not replace any tables, especially DSDT.
When this is not possible, then at least attempt to ensure that custom DSDT is based on the most recent DSDT
or remove writes and reads for the affected areas.
When nothing else helps this option could be tried to avoid stalls at PCI Configuration Begin phase of macOS
booting by attempting to fix the ACPI addresses. It does not do magic, and only works with most common cases.
Do not use unless absolutely required.</source>
        <translation>ACPI表通常由底层固件实现动态生成。在与位置无关的代码中，ACPI表可能包含用于设备配置的MMIO区域的物理地址，通常按区域（例如OperationRegion）分组。更改固件设置或硬件配置，升级或修补固件不可避免地会导致动态生成的ACPI代码发生变化，有时会导致上述OperationRegion结构中的地址发生变化。因此，对ACPI表进行任何形式的修改都是非常危险的。最合理的方法是对ACPI进行尽可能少的更改，并尝试不替换任何表，尤其是DSDT。如果无法做到这一点，则至少尝试确保自定义DSDT基于最新的DSDT或删除受影响区域的写入和读取。如果没有其他帮助，可以尝试通过尝试修复ACPI地址来避免在macOS引导的PCI Configuration Begin阶段出现停顿的情况。它不做魔术，仅适用于大多数常见情况。除非绝对必要，否则请勿使用。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="279"/>
        <source>This works around firmwares that fail to maintain hardware signature across the reboots and cause issues with
waking from hibernation.</source>
        <translation>这可以解决固件无法在重新启动过程中保持硬件签名并导致从休眠状态唤醒的问题。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="293"/>
        <source>This works around firmwares that provide BGRT table but fail to handle screen updates afterwards.</source>
        <translation>这适用于提供BGRT表但随后无法处理屏幕更新的固件。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="82"/>
        <location filename="mainwindow.ui" line="132"/>
        <location filename="mainwindow.ui" line="188"/>
        <location filename="mainwindow.ui" line="355"/>
        <location filename="mainwindow.ui" line="925"/>
        <location filename="mainwindow.ui" line="965"/>
        <location filename="mainwindow.ui" line="1002"/>
        <location filename="mainwindow.ui" line="1042"/>
        <location filename="mainwindow.ui" line="1100"/>
        <location filename="mainwindow.ui" line="1163"/>
        <location filename="mainwindow.ui" line="1215"/>
        <location filename="mainwindow.ui" line="1272"/>
        <location filename="mainwindow.ui" line="3247"/>
        <location filename="mainwindow.ui" line="3294"/>
        <location filename="mainwindow.ui" line="3341"/>
        <location filename="mainwindow.ui" line="3408"/>
        <location filename="mainwindow.ui" line="3448"/>
        <location filename="mainwindow.ui" line="3499"/>
        <location filename="mainwindow.ui" line="3513"/>
        <location filename="mainwindow.ui" line="3563"/>
        <location filename="mainwindow.ui" line="3590"/>
        <location filename="mainwindow.ui" line="4272"/>
        <location filename="mainwindow.ui" line="5289"/>
        <location filename="mainwindow.ui" line="6012"/>
        <source>AddItem</source>
        <translation>增加条目</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="89"/>
        <location filename="mainwindow.ui" line="139"/>
        <location filename="mainwindow.ui" line="195"/>
        <location filename="mainwindow.ui" line="362"/>
        <location filename="mainwindow.ui" line="945"/>
        <location filename="mainwindow.ui" line="972"/>
        <location filename="mainwindow.ui" line="1022"/>
        <location filename="mainwindow.ui" line="1049"/>
        <location filename="mainwindow.ui" line="1107"/>
        <location filename="mainwindow.ui" line="1170"/>
        <location filename="mainwindow.ui" line="1222"/>
        <location filename="mainwindow.ui" line="1279"/>
        <location filename="mainwindow.ui" line="3254"/>
        <location filename="mainwindow.ui" line="3301"/>
        <location filename="mainwindow.ui" line="3348"/>
        <location filename="mainwindow.ui" line="3428"/>
        <location filename="mainwindow.ui" line="3455"/>
        <location filename="mainwindow.ui" line="3506"/>
        <location filename="mainwindow.ui" line="3520"/>
        <location filename="mainwindow.ui" line="3570"/>
        <location filename="mainwindow.ui" line="3597"/>
        <location filename="mainwindow.ui" line="4279"/>
        <location filename="mainwindow.ui" line="5296"/>
        <location filename="mainwindow.ui" line="6019"/>
        <source>DeleteItem</source>
        <translation>删除条目</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="326"/>
        <source>MmioWhitelist</source>
        <oldsource>内存映射白名单-MmioWhitelist</oldsource>
        <translation>MmioWhitelist（Mmio白名单）</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="379"/>
        <source>This option fixes UEFI runtime services (date, time, NVRAM, power control, etc.) support on many firmwares
using SMM backing for select services like variable storage. SMM may try to access physical addresses, but they
get moved by boot.efi.
Note: Most but Apple and VMware firmwares need this quirk.</source>
        <translation>此选项修复了许多固件的UEFI运行时服务（日期，时间，NVRAM，电源控制等）支持，这些固件使用SMM支持某些服务（如变量存储）。 SMM可能会尝试访问物理地址，但是它们会被boot.efi移动。
注意：除Apple和VMware固件外，大多数都需要此功能。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="385"/>
        <source>Protect from boot.efi runtime memory defragmentation.</source>
        <translation>防止boot.efi运行时内存碎片整理。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="388"/>
        <source>AvoidRuntimeDefrag</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="408"/>
        <source>This option reduces stolen memory footprint from the memory map by removing runtime bit for known memory
regions. This quirk may result in the increase of KASLR slides available, but is not necessarily compatible with
the target board without additional measures. In general this frees from 64 to 256 megabytes of memory (present
in the debug log), and on some platforms it is the only way to boot macOS, which otherwise fails with allocation
error at bootloader stage.
This option is generally useful on all firmwares except some very old ones, like Sandy Bridge. On select firmwares
it may require a list of exceptional addresses that still need to get their virtual addresses for proper NVRAM and
hibernation functioning. Use MmioWhitelist section to do this.</source>
        <translation>通过删除已知内存区域的运行时位，此选项可减少内存映射中被盗的内存占用。 这种奇怪现象可能会增加可用的KASLR滑轨，但如果没有其他措施，则不一定与目标板兼容。 通常，这会释放64到256 MB的内存（出现在调试日志中），并且在某些平台上，这是引导macOS的唯一方法，否则将导致macOS失败，并在引导加载程序阶段出现分配错误。 该选项通常对所有固件都有用，除了一些非常古老的固件（例如Sandy Bridge）。 在某些固件上，它可能需要一个例外地址列表，为了使NVRAM和休眠功能正常工作，仍然需要获取其虚拟地址。 使用MmioWhitelist部分来执行此操作。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="418"/>
        <source>Remove runtime attribute from select MMIO regions.</source>
        <translation>从选定的MMIO区域中删除运行时属性。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="421"/>
        <source>DevirtualiseMmio</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="441"/>
        <source>This is a security option allowing one to restrict single user mode usage by ignoring CMD+S hotkey and -s boot
argument. The behaviour with this quirk enabled is supposed to match T2-based model behaviour. Read this
archived article to understand how to use single user mode with this quirk enabled.</source>
        <translation>这是一个安全选项，允许通过忽略CMD + S热键和-s boot参数来限制单用户模式的使用。 启用此选项的行为应该与基于T2的模型行为相匹配。 阅读此存档文章，以了解如何在启用此选项的情况下使用单用户模式。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="446"/>
        <source>Disable single user mode.</source>
        <translation>禁用单用户模式。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="449"/>
        <source>DisableSingleUser</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="469"/>
        <source>This is a security option allowing one to restrict NVRAM access in macOS. This quirk requires OC_FIRMWARE_RUNTIME
protocol implemented in OpenRuntime.efi.
Note: This quirk can also be used as an ugly workaround to buggy UEFI runtime services implementations that
fail to write variables to NVRAM and break the rest of the operating system.</source>
        <translation>这是一种安全选项，允许您限制macOS中的NVRAM访问。 此选项需要在OpenRuntime.efi中实现OC_FIRMWARE_RUNTIME协议。
注意：此选项也可用作解决UEFI运行时服务实现错误的不完善的解决方法，这些实现未能将变量写入NVRAM并破坏了操作系统的其余部分。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="475"/>
        <source>Protect from macOS NVRAM write access.</source>
        <translation>保护免受macOS NVRAM写入访问。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="478"/>
        <source>DisableVariableWrite</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="498"/>
        <source>This option forces XNU kernel to ignore newly supplied memory map and assume that it did not change after
waking from hibernation. This behaviour is required to work by Windows, which mandates to preserve runtime
memory size and location after S4 wake.
Note: This may be used to workaround buggy memory maps on older hardware, and is now considered rare legacy.
Examples of such hardware are Ivy Bridge laptops with Insyde firmware, like Acer V3-571G. Do not use this
unless you fully understand the consequences.</source>
        <translation>此选项强制XNU内核忽略新提供的内存映射，并假定它从休眠状态唤醒后未更改。 Windows必须执行此行为，Windows要求在S4唤醒后保留运行时内存的大小和位置。 注意：这可能用于解决较旧硬件上的错误内存映射，现在被认为是稀有遗产。 这种硬件的示例是具有Insyde固件的Ivy Bridge笔记本电脑，例如Acer V3-571G。 除非您完全了解后果，否则请勿使用此功能。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="506"/>
        <source>Reuse original hibernate memory map.</source>
        <translation>重用原始的休眠内存映射。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="509"/>
        <source>DiscardHibernateMap</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="803"/>
        <source>This option is relevant to the users that have issues booting to safe mode (e.g. by holding shift or using -x boot
argument). By default safe mode forces 0 slide as if the system was launched with slide=0 boot argument. This
quirk tries to patch boot.efi to lift that limitation and let some other value (from 1 to 255) be used. This quirk
requires ProvideCustomSlide to be enabled.
Note: The necessity of this quirk is determined by safe mode availability. If booting to safe mode fails, this option
can be tried to be enabled.</source>
        <translation>此选项与在启动到安全模式时遇到问题的用户有关（例如，通过按住shift或使用-x boot参数）。 默认情况下，安全模式会强制滑动0，就好像系统是使用slide = 0引导参数启动的。 这个选项尝试修补boot.efi以解除该限制，并使用其他值（从1到255）。 此选项要求启用ProvideCustomSlide。 
注意：此选项的必要性取决于安全模式的可用性。 如果启动到安全模式失败，则可以尝试启用此选项。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="811"/>
        <source> Patch bootloader to have KASLR enabled in safe mode.</source>
        <translation>修补引导加载程序以在安全模式下启用KASLR。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="814"/>
        <source>EnableSafeModeSlide</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="516"/>
        <source>This option bypasses RˆX permissions in code pages of UEFI runtime services by removing write protection (WP)
bit from CR0 register during their execution. This quirk requires OC_FIRMWARE_RUNTIME protocol implemented in
OpenRuntime.efi.
Note: This quirk may potentially weaken firmware security, please use RebuildAppleMemoryMap if your firmware
supports memory attributes table (MAT). Refer to OCABC: MAT support is 1/0 log entry to determine whether
MAT is supported.</source>
        <translation>此选项通过在执行期间从CR0寄存器中删除写保护（WP）位来绕过UEFI运行时服务代码页中的RˆX权限。 此选项需要在OpenRuntime.efi中实现OC_FIRMWARE_RUNTIME协议。 
注意：此选项可能会削弱固件的安全性，如果您的固件支持内存属性表（MAT），请使用RebuildAppleMemoryMap。 请参阅OCABC：MAT支持是1/0日志条目，以确定是否支持MAT。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="524"/>
        <source> Permit write access to UEFI runtime services code.</source>
        <translation>允许对UEFI运行时服务代码的写访问。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="527"/>
        <source>EnableWriteUnprotector</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="547"/>
        <source>Try to ensure that ExitBootServices call succeeds even with outdated MemoryMap key argument by obtaining
current memory map and retrying ExitBootServices call.
Note: The necessity of this quirk is determined by early boot crashes of the firmware. Do not use this unless you
fully understand the consequences.</source>
        <translation>尝试通过获取当前内存映射并重试ExitBootServices调用来确保即使使用了过期的MemoryMap键参数也可以成功退出ExitBootServices调用。 
注意：此选项的必要性取决于固件的早期启动崩溃。 除非您完全了解后果，否则请勿使用此功能。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="553"/>
        <source>Retry ExitBootServices with new memory map on failure.</source>
        <translation>失败时，使用新的内存映射重试ExitBootServices。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="556"/>
        <source>ForceExitBootServices</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="576"/>
        <source>Some firmwares incorrectly map select memory regions:
• CSM region can be marked as boot services code or data, which leaves it as free memory for XNU kernel.
• MMIO regions can be marked as reserved memory and stay unmapped, but may be required to be accessible
at runtime for NVRAM support.
This quirk attempts to fix types of these regions, e.g. ACPI NVS for CSM or MMIO for MMIO.
Note: The necessity of this quirk is determined by artifacts, sleep wake issues, and boot failures. In general only
very old firmwares need this quirk.</source>
        <translation>一些固件错误地映射了选定的内存区域：•CSM区域可以标记为引导服务代码或数据，从而将其留作XNU内核的可用内存。 •MMIO区域可以标记为保留内存并保持未映射状态，但是可能需要在运行时对其进行访问以支持NVRAM。 这个选项尝试修复这些区域的类型，例如 CSM的ACPI NVS或MMIO的MMIO。 
注意：此选项的必要性由工件，睡眠唤醒问题和引导失败确定。 通常，只有非常老的固件才需要此选项。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="585"/>
        <source>Protect memory regions from incorrect access.</source>
        <translation>保护内存区域免受错误访问。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="588"/>
        <source>ProtectMemoryRegions</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="608"/>
        <source>Reports security violation during attempts to write to db, dbx, PK, and KEK variables from the operating system.
Note: This quirk mainly attempts to avoid issues with NVRAM implementations with problematic defragmentation,
such as select Insyde or MacPro5,1.</source>
        <translation>在尝试从操作系统写入db，dbx，PK和KEK变量期间报告安全冲突。 
注意：此选项主要试图避免NVRAM实施中出现问题的碎片整理问题，例如select Insyde或MacPro5,1。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="613"/>
        <source>Protect UEFI Secure Boot variables from being written.</source>
        <translation>保护UEFI安全启动变量不被写入。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="616"/>
        <source>ProtectSecureBoot</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="636"/>
        <source>Some modern firmwares including both hardware and virtual machines, like VMware, may update pointers to
UEFI services during driver loading and related actions. Consequentially this directly breaks other quirks that
affect memory management, like DevirtualiseMmio, ProtectMemoryRegions, or RebuildAppleMemoryMap, and
may also break other quirks depending on the effects of these.
Note: On VMware the need for this quirk may be diagnosed by “Your Mac OS guest might run unreliably with
more than one virtual core.” message.</source>
        <translation>包括硬件和虚拟机（例如VMware）在内的某些现代固件可能会在驱动程序加载和相关操作期间更新指向UEFI服务的指针。 因此，这直接破坏了影响内存管理的其他选项，例如DevirtualiseMmio，ProtectMemoryRegions或RebuildAppleMemoryMap，并且还可能破坏其他选项，具体取决于这些选项。
注意：在VMware上，可以通过“您的Mac OS来宾系统可能不可靠地在多个虚拟内核上运行”来诊断此问题。 信息。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="644"/>
        <source>Protect UEFI services from being overridden by the firmware.</source>
        <translation>保护UEFI服务不被固件覆盖。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="647"/>
        <source>ProtectUefiServices</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="784"/>
        <source>This option performs memory map analysis of your firmware and checks whether all slides (from 1 to 255) can be
used. As boot.efi generates this value randomly with rdrand or pseudo randomly rdtsc, there is a chance of
boot failure when it chooses a conflicting slide. In case potential conflicts exist, this option forces macOS to use a
pseudo random value among the available ones. This also ensures that slide= argument is never passed to the
operating system for security reasons.
Note: The necessity of this quirk is determined by OCABC: Only N/256 slide values are usable! message
in the debug log. If the message is present, this option is to be enabled.</source>
        <translation>此选项对固件执行内存映射分析，并检查是否可以使用所有幻灯片（从1到255）。 由于boot.efi使用rdrand或伪随机rdtsc随机生成此值，因此当它选择冲突的幻灯片时可能会导致启动失败。 如果存在潜在冲突，此选项将强制macOS使用可用随机值中的伪随机值。 这也可以确保出于安全原因，slide =参数不会传递给操作系统。
注意：此选项的必要性由OCABC决定：仅N / 256个滑块值可用！ 消息在调试日志中。 如果显示该消息，则将启用此选项。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="793"/>
        <source>Provide custom KASLR slide on low memory.</source>
        <translation>在内存不足时提供自定义的KASLR幻灯片。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="796"/>
        <source>ProvideCustomSlide</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="654"/>
        <source>Apple kernel has several limitations in parsing UEFI memory map:
• Memory map size must not exceed 4096 bytes as Apple kernel maps it as a single 4K page. Since some
firmwares have very large memory maps (approximately over 100 entries) Apple kernel will crash at boot.
• Memory attributes table is ignored. EfiRuntimeServicesCode memory statically gets RX permissions, and
all other memory types get RW permissions. Since some firmware drivers may write to global variables
at runtime, Apple kernel will crash at calling UEFI runtime services, unless driver .data section has
EfiRuntimeServicesData type.
To workaround these limitations this quirk applies memory attributes table permissions to memory map passed
to Apple kernel and optionally attempts to unify contiguous slots of similar types if the resulting memory map
exceeds 4 KB.
Note 1 : Since many firmwares come with incorrect memory protection table this quirk often comes in pair with
SyncRuntimePermissions.
Note 2 : The necessity of this quirk is determined by early boot failures. This quirk replaces EnableWriteUnprotector
on firmwares supporting memory attributes table (MAT). This quirk is generally unnecessary when using
OpenDuetPkg, but may be required to boot macOS 10.6 and earlier for unclear reasons.</source>
        <translation>Apple内核在解析UEFI内存映射时有一些限制：
•内存映射大小不得超过4096字节，因为Apple内核将其映射为单个4K页面。由于某些固件具有非常大的内存映射（大约超过100个条目），Apple内核将在启动时崩溃。
•内存属性表被忽略。 EfiRuntimeServicesCode内存静态获取RX权限，所有其他内存类型获取RW权限。由于某些固件驱动程序可能会在运行时写入全局变量，因此，除非驱动程序.data节具有EfiRuntimeServicesData类型，否则Apple内核将在调用UEFI运行时服务时崩溃。要解决这些限制，此选项将内存属性表权限应用于传递给Apple内核的内存映射，并且如果生成的内存映射超过4 KB，则可以选择尝试统一相似类型的连续插槽。
注1：由于许多固件附带不正确的内存保护表，因此该问题通常与SyncRuntimePermissions一起使用。
注2：此选项的必要性取决于早期启动故障。此选项取代了支持内存属性表（MAT）的固件上的EnableWriteUnprotector。使用OpenDuetPkg时，通常不需要此选项，但出于不清楚的原因，可能需要引导macOS 10.6和更早版本。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="671"/>
        <source>Generate Memory Map compatible with macOS.</source>
        <translation>生成与macOS兼容的内存映射。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="674"/>
        <source>RebuildAppleMemoryMap</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="694"/>
        <source>Select firmwares access memory by virtual addresses after SetVirtualAddresses call, which results in early boot
crashes. This quirk workarounds the problem by performing early boot identity mapping of assigned virtual
addresses to physical memory.
Note: The necessity of this quirk is determined by early boot failures. Currently new firmwares with memory
protection support (like OVMF) do not support this quirk due to acidanthera/bugtracker#719.</source>
        <translation>在SetVirtualAddresses调用之后，某些固件通过虚拟地址访问内存，这会导致早期启动崩溃。 该怪异解决方法通过执行分配的虚拟地址到物理内存的早期启动身份映射来解决该问题。
注意：此选项的必要性取决于早期启动故障。 目前，由于acidanthera / bugtracker＃719，支持内存保护的新固件（例如OVMF）不支持此功能。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="701"/>
        <source> Setup virtual memory at SetVirtualAddresses.</source>
        <translation>在SetVirtualAddresses上设置虚拟内存。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="704"/>
        <source>SetupVirtualMap</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="724"/>
        <source>This quirk is useful on Mac firmwares, which behave differently in different OS. For example, it is supposed to
enable Intel GPU in Windows and Linux in some dual-GPU MacBook models.</source>
        <translation>Mac OS固件在不同的OS上具有不同的行为，因此该操作非常有用。 例如，应该在某些双GPU MacBook型号中在Windows和Linux中启用Intel GPU。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="728"/>
        <source> Report macOS being loaded through OS Info for any OS.</source>
        <translation>报告通过任何操作系统的OS Info加载的macOS。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="731"/>
        <source>SignalAppleOS</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="751"/>
        <source>Some firmwares either fail to properly handle runtime permissions:
• They incorrectly mark OpenRuntime as not executable in the memory map.
• They incorrectly mark OpenRuntime as not executable in the memory attributes table.
• They lose entries from the memory attributes table after OpenRuntime is loaded.
• They mark items in the memory attributes table as read-write-execute.
This quirk tries to update memory map and memory attributes table to correct this.
Note: The necessity of this quirk is determined by early boot failures either in macOS or in Linux/Windows. In
general only firmwares released in 2018 or later are affected.</source>
        <translation>某些固件无法正确处理运行时权限：
•它们错误地将OpenRuntime标记为在内存映射中不可执行。
•它们在内存属性表中错误地将OpenRuntime标记为不可执行。
•加载OpenRuntime之后，它们将从内存属性表中丢失条目。
•它们将内存属性表中的项目标记为读写执行。 此选项尝试更新内存映射和内存属性表以更正此问题。
注意：此选项的必要性取决于macOS或Linux / Windows中的早期启动故障。 通常，仅2018年或之后发布的固件会受到影响。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="761"/>
        <source>Update memory permissions for runtime environment.</source>
        <translation>更新运行时环境的内存权限。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="764"/>
        <source>SyncRuntimePermissions</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="858"/>
        <source>This option overrides the maximum slide of 255 by a user specified value between 1 and 254 inclusive when
ProvideCustomSlide is enabled. It is believed that modern firmwares allocate pool memory from top to bottom,
effectively resulting in free memory at the time of slide scanning being later used as temporary memory during
kernel loading. In case those memory are unavailable, this option can stop evaluating higher slides.
Note: The necessity of this quirk is determined by random boot failure when ProvideCustomSlide is enabled
and the randomized slide fall into the unavailable range. When AppleDebug is enabled, usually the debug log may
contain messages like AAPL: [EB|‘LD:LKC] } Err(0x9). To find the optimal value, manually append slide=X
to boot-args and log the largest one that won’t cause boot failure.</source>
        <translation>启用ProvideCustomSlide时，此选项将用用户指定的值（介于1和254之间，包括255）覆盖最大的255幻灯片。 可以相信，现代固件从上到下分配池内存，有效地导致了幻灯片扫描时的可用内存，后来在内核加载期间用作临时内存。 如果这些内存不可用，此选项可以停止评估更高的幻灯片。
注意：启用ProvideCustomSlide且随机化的幻灯片落在不可用范围内时，此奇怪的必要性取决于随机引导失败。 启用AppleDebug时，通常，调试日志中可能包含诸如AAPL的消息：[EB |’LD：LKC]} Err（0x9）。 要找到最佳值，请手动将slide = X附加到boot-args并记录不会导致启动失败的最大参数。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="868"/>
        <source>Provide maximum KASLR slide when higher ones are unavailable.</source>
        <translation>当没有更高的KASLR幻灯片时，请提供最大的数量。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="871"/>
        <source>ProvideMaxSlide</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5543"/>
        <source>Description: Forces Resolution to be set in cases where the desired resolution is not available by default, such
as on legacy Intel GMA and first generation Intel HD Graphics (Ironlake/Arrandale). Setting Resolution to Max
will try to pull the largest available resolution from the connected display’s EDID.
Note: This option depends on the OC_FORCE_RESOLUTION_PROTOCOL protocol being present. This protocol is
currently only supported by OpenDuetPkg. The OpenDuetPkg implementation currently only supports Intel
iGPUs.</source>
        <translation>说明：在默认情况下（例如在旧版Intel GMA和第一代Intel HD Graphics（Ironlake / Arrandale）上）无法使用所需分辨率的情况下，强制设置分辨率。 将“分辨率”设置为“最大”将尝试从连接的显示器的EDID中获取最大的可用分辨率。 注意：此选项取决于存在的OC_FORCE_RESOLUTION_PROTOCOL协议。 目前仅OpenDuetPkg支持此协议。 OpenDuetPkg实现当前仅支持Intel iGPU。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5551"/>
        <source>ForceResolution</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5945"/>
        <location filename="mainwindow.cpp" line="42"/>
        <source>DeviceProperties</source>
        <oldsource>设备属性配置-DeviceProperties</oldsource>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1061"/>
        <location filename="mainwindow.cpp" line="44"/>
        <source>Kernel</source>
        <oldsource>内核设置-Kernel</oldsource>
        <translation>Kernel
内核</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1114"/>
        <source>Up</source>
        <translation>条目上移</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1121"/>
        <source>Down</source>
        <translation>条目下移</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1142"/>
        <source>Block</source>
        <oldsource>阻止-Block</oldsource>
        <translation>Block（阻止）</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1191"/>
        <source>Force</source>
        <oldsource>强制-Force</oldsource>
        <translation>Force（强制）</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1300"/>
        <source>Emulate</source>
        <oldsource>Quirks + Emulate</oldsource>
        <translation>Emulate（仿冒CPU）</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1306"/>
        <source>This property serves for two needs:
• Enabling support of an unsupported CPU model.
• Enabling XCPM support for an unsupported CPU variant.
Normally it is only the value of EAX that needs to be taken care of, since it represents the full CPUID. The
remaining bytes are to be left as zeroes. Byte order is Little Endian, so for example, C3 06 03 00 stands for
CPUID 0x0306C3 (Haswell).
For XCPM support it is recommended to use the following combinations.
• Haswell-E (0x0306F2) to Haswell (0x0306C3):
Cpuid1Data: C3 06 03 00 00 00 00 00 00 00 00 00 00 00 00 00
Cpuid1Mask: FF FF FF FF 00 00 00 00 00 00 00 00 00 00 00 00
• Broadwell-E (0x0406F1) to Broadwell (0x0306D4):
Cpuid1Data: D4 06 03 00 00 00 00 00 00 00 00 00 00 00 00 00
Cpuid1Mask: FF FF FF FF 00 00 00 00 00 00 00 00 00 00 00 00
Keep in mind, that the following configurations are unsupported (at least out of the box):
• Consumer Ivy Bridge (0x0306A9) as Apple disabled XCPM for Ivy Bridge and recommends legacy power
management for these CPUs. You will need to manually patch _xcpm_bootstrap to force XCPM on these
CPUs instead of using this option.
• Low-end CPUs (e.g. Haswell+ Pentium) as they are not supported properly by macOS. Legacy hacks for
older models can be found in the Special NOTES section of acidanthera/bugtracker#365.</source>
        <translation>此属性满足两个需求：
•启用对不受支持的CPU型号的支持。
•为不受支持的CPU变体启用XCPM支持。通常，只需要注意EAX的值，因为它表示完整的CPUID。其余字节将保留为零。字节顺序为Little Endian，因此，例如，C3 06 03 00代表CPUID 0x0306C3（Haswell）。对于XCPM支持，建议使用以下组合。
•Haswell-E（0x0306F2）到Haswell（0x0306C3）：Cpuid1Data：C3 06 03 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 Cpuid1Mask：FF FF FF FF 00 00 00 00 00 00 00 00 00 00 00 00 00
•Broadwell-E（0x0406F1）到Broadwell（0x0306D4）：Cpuid1Data：D4 06 03 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 Cpuid1Mask：FF FF FF FF 00 00 00 00 00 00 00 00 00 00 00 00 00 Keep请记住，以下配置不受支持（至少开箱即用）：
•Consumer Ivy Bridge（0x0306A9），因为Apple禁用了Ivy Bridge的XCPM，并建议为这些CPU使用旧式电源管理。您将需要手动修补_xcpm_bootstrap以在这些CPU上强制执行XCPM，而不是使用此选项。
•低端CPU（例如Haswell + Pentium），因为macOS不正确地支持它们。旧版本的旧版骇客可在acidanthera / bugtracker＃365的“特殊说明”部分中找到。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1327"/>
        <source> Sequence of EAX, EBX, ECX, EDX values to replace CPUID (1) call in XNU kernel.</source>
        <translation>EAX，EBX，ECX，EDX值的序列，以替换XNU内核中的CPUID（1）调用。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1330"/>
        <source>Cpuid1Data</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1340"/>
        <source>When each Cpuid1Mask bit is set to 0, the original CPU bit is used, otherwise set bits take the value of Cpuid1Data.</source>
        <translation>当每个Cpuid1Mask位设置为0时，将使用原始CPU位，否则，设置位采用Cpuid1Data的值。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1343"/>
        <source> Bit mask of active bits in Cpuid1Data.</source>
        <translation>Cpuid1Data中活动位的位掩码。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1346"/>
        <source>Cpuid1Mask</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1356"/>
        <source>Note 1 : This option is a preferred alternative to NullCpuPowerManagement.kext for CPUs without native power
management driver in macOS.
Note 2 : While this option is usually needed to disable AppleIntelCpuPowerManagement on unsupported platforms,
it can also be used to disable this kext in other situations (e.g. with Cpuid1Data left blank).</source>
        <translation>注1：对于在macOS中没有本机电源管理驱动程序的CPU，此选项是NullCpuPowerManagement.kext的首选替代方案。
注意2：虽然通常需要此选项来在不受支持的平台上禁用AppleIntelCpuPowerManagement，但在其他情况下（例如Cpuid1Data留为空白），也可以用于禁用此kext。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1372"/>
        <location filename="mainwindow.ui" line="1388"/>
        <source>Note: Refer to Add MaxKernel description for matching logic.</source>
        <translation>注意：有关匹配逻辑，请参阅添加MaxKernel描述。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1375"/>
        <source> Emulates CPUID and applies DummyPowerManagement on specified macOS version or older.</source>
        <translation>模拟CPUID并在指定的macOS版本或更早版本上应用DummyPowerManagement。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1391"/>
        <source>Emulates CPUID and applies DummyPowerManagement on specified macOS version or newer.</source>
        <translation>模拟CPUID，并在指定的macOS版本或更高版本上应用DummyPowerManagement。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1405"/>
        <source>Quirks</source>
        <translation>Quirks（内核杂项）</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1411"/>
        <source>Certain firmwares lock PKG_CST_CONFIG_CONTROL MSR register. To check its state one can use bundled
VerifyMsrE2 tool. Select firmwares have this register locked on some cores only.
As modern firmwares provide CFG Lock setting, which allows configuring PKG_CST_CONFIG_CONTROL MSR register
lock, this option should be avoided whenever possible. For several APTIO firmwares not displaying CFG Lock
setting in the GUI it is possible to access the option directly:
(a) Download UEFITool and IFR-Extractor.
(b) Open your firmware image in UEFITool and find CFG Lock unicode string. If it is not present, your firmware
may not have this option and you should stop.
(c) Extract the Setup.bin PE32 Image Section (the one UEFITool found) through Extract Body menu option.
(d) Run IFR-Extractor on the extracted file (e.g. ./ifrextract Setup.bin Setup.txt).
(e) Find CFG Lock, VarStoreInfo (VarOffset/VarName): in Setup.txt and remember the offset right after
it (e.g. 0x123).
(f) Download and run Modified GRUB Shell compiled by brainsucker or use a newer version by datasone.
(g) Enter setup_var 0x123 0x00 command, where 0x123 should be replaced by your actual offset, and reboot.
Warning: Variable offsets are unique not only to each motherboard but even to its firmware version. Never ever
try to use an offset without checking.</source>
        <translation>某些固件会锁定PKG_CST_CONFIG_CONTROL MSR寄存器。要检查其状态，可以使用捆绑的VerifyMsrE2工具。某些固件仅将此寄存器锁定在某些内核上。由于现代固件提供CFG锁定设置，该设置允许配置PKG_CST_CONFIG_CONTROL MSR寄存器锁定，因此应尽可能避免使用此选项。对于一些APTIO固件未在GUI中显示CFG锁定设置的情况，可以直接访问该选项：
（a）下载UEFITool和IFR-Extractor。
（b）在UEFITool中打开固件映像，然后找到CFG Lock Unicode字符串。如果不存在，则您的固件可能没有此选项，应该停止。
（c）通过“提取主体”菜单选项提取Setup.bin PE32图像部分（找到一个UEFITool）。
（d）在提取的文件上运行IFR-Extractor（例如./ifrextract Setup.bin Setup.txt）。
（e）在Setup.txt中找到CFG锁，VarStoreInfo（VarOffset / VarName）：并记住紧随其后的偏移量（例如0x123）。
（f）下载并运行由Brainsucker编译的Modified GRUB Shell，或使用datasone的较新版本。
（g）输入setup_var 0x123 0x00命令，其中0x123应替换为您的实际偏移量，然后重新启动。警告：可变偏移量不仅对于每个主板都是唯一的，甚至对于其固件版本也是唯一的。绝对不要在未经检查的情况下使用偏移量。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1429"/>
        <source>Disables PKG_CST_CONFIG_CONTROL (0xE2) MSR modification in AppleIntelCPUPowerManagement.kext, commonly causing early kernel panic, when it is locked from writing.</source>
        <translation>禁用AppleIntelCPUPowerManagement.kext中的PKG_CST_CONFIG_CONTROL（0xE2）MSR修改，当它被禁止写入时，通常会导致早期内核崩溃。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1432"/>
        <source>AppleCpuPmCfgLock</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1439"/>
        <source>This is normally used in conjunction with Emulate section on Haswell-E, Broadwell-E, Skylake-SP, and similar
CPUs. More details on the XCPM patches are outlined in acidanthera/bugtracker#365.
Note: Additional not provided patches will be required for Ivy Bridge or Pentium CPUs. It is recommended to
use AppleIntelCpuPowerManagement.kext for the former.</source>
        <translation>这通常与Haswell-E，Broadwell-E，Skylake-SP和类似CPU的“仿真”部分结合使用。 有关XCPM修补程序的更多详细信息，请参见acidanthera / bugtracker＃365。
注意：Ivy Bridge或Pentium CPU将需要其他未提供的补丁。 建议对前者使用AppleIntelCpuPowerManagement.kext。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1445"/>
        <source>Disables multiple MSR access critical for select CPUs, which have no native XCPM support.</source>
        <translation>禁用对不支持本机XCPM的某些CPU至关重要的多个MSR访问。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1448"/>
        <source>AppleXcpmExtraMsrs</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1455"/>
        <source>Note: This option should be avoided whenever possible. In general the necessity of this option means misconfigured
or broken firmware.</source>
        <translation>注意：应尽可能避免使用此选项。 通常，此选项的必要性意味着配置错误或损坏的固件。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1459"/>
        <source> Increases 32-bit PCI bar size in IOPCIFamily from 1 to 4 GBs.</source>
        <translation>将IOPCIFamily中的32位PCI条尺寸从1 GB增加到4 GB。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1462"/>
        <source>IncreasePciBarSize</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1469"/>
        <source>Note: This option should be avoided whenever possible. See AppleCpuPmCfgLock description for more details.</source>
        <translation>注意：应尽可能避免使用此选项。 有关更多详细信息，请参见AppleCpuPmCfgLock描述。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1472"/>
        <source>Disables PKG_CST_CONFIG_CONTROL (0xE2) MSR modification in XNU kernel, commonly causing
 early kernel panic, when it is locked from writing (XCPM power management).</source>
        <translation>在XNU内核中禁用PKG_CST_CONFIG_CONTROL（0xE2）MSR修改，通常是早期内核恐慌，当它无法写入时（XCPM电源管理）。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1476"/>
        <source>AppleXcpmCfgLock</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1483"/>
        <source>Note: This option is a preferred alternative to deleting DMAR ACPI table and disabling VT-d in firmware preferences,
which does not break VT-d support in other systems in case they need it.</source>
        <translation>注意：此选项是删除DMAR ACPI表并在固件首选项中禁用VT-d的首选替代方法，这不会破坏其他系统中的VT-d支持（如果需要）。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1487"/>
        <source>Disables IOMapper support in XNU (VT-d), which may conflict with the firmware implementation.</source>
        <translation>禁用XNU（VT-d）中的IOMapper支持，这可能与固件实现冲突。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1490"/>
        <source>DisableIoMapper</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1497"/>
        <source>This patch writes 0xFF00 to MSR_IA32_PERF_CONTROL (0x199), effectively setting maximum multiplier for all the
time.
Note: While this may increase the performance, this patch is strongly discouraged on all systems but those
explicitly dedicated to scientific or media calculations. In general only certain Xeon models benefit from the
patch.</source>
        <translation>此补丁将0xFF00写入MSR_IA32_PERF_CONTROL（0x199），从而始终有效地设置最大乘数。
注意：虽然这可能会提高性能，但强烈建议不要在所有系统上使用此修补程序，但必须专门用于科学或媒体计算。 通常，只有某些Xeon型号才能从该补丁程序中受益。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1504"/>
        <source> Forces maximum performance in XCPM mode.</source>
        <translation>在XCPM模式下强制发挥最佳性能。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1507"/>
        <source>AppleXcpmForceBoost</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1514"/>
        <source> Performs GUID patching for UpdateSMBIOSMode Custom mode. Usually relevant for Dell laptops.</source>
        <translation>对UpdateSMBIOSMode自定义模式执行GUID修补。 通常与戴尔笔记本电脑有关。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1517"/>
        <source>Performs GUID patching for UpdateSMBIOSMode Custom mode. Usually relevant for Dell laptops.</source>
        <translation>对UpdateSMBIOSMode自定义模式执行GUID修补。 通常与戴尔笔记本电脑有关。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1520"/>
        <source>CustomSMBIOSGuid</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1527"/>
        <source>Note 1 : This option will not protect other areas from being overwritten, see RTCMemoryFixup kernel extension
if this is desired.
Note 2 : This option will not protect areas from being overwritten at firmware stage (e.g. macOS bootloader), see
AppleRtcRam protocol description if this is desired.</source>
        <translation>注1：此选项不会保护其他区域不被覆盖，如果需要，请参阅RTCMemoryFixup内核扩展。 注意2：此选项将无法保护区域在固件阶段（例如macOS引导加载程序）不被覆盖，如果需要，请参阅AppleRtcRam协议说明。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1533"/>
        <source>Disables primary checksum (0x58-0x59) writing in AppleRTC.</source>
        <translation>禁用在AppleRTC中写入主校验和（0x58-0x59）。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1536"/>
        <source>DisableRtcChecksum</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1543"/>
        <location filename="mainwindow.ui" line="1546"/>
        <source>Disables kernel panic on LAPIC interrupts.</source>
        <translation>在LAPIC中断上禁用内核恐慌。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1549"/>
        <source>LapicKernelPanic</source>
        <translation></translation>
    </message>
    <message>
        <source>Note: This option is a preferred alternative to NullCpuPowerManagement.kext for CPUs without native power
management driver in macOS.</source>
        <translation type="vanished">注意：对于在macOS中没有本机电源管理驱动程序的CPU，此选项是NullCpuPowerManagement.kext的首选替代方案。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1362"/>
        <source>Disables AppleIntelCpuPowerManagement.</source>
        <translation>禁用AppleIntelCpuPowerManagement。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1365"/>
        <source>DummyPowerManagement</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1571"/>
        <source>Note: This option should be avoided whenever possible. USB port limit is imposed by the amount of used bits in
locationID format and there is no possible way to workaround this without heavy OS modification. The only valid
solution is to limit the amount of used ports to 15 (discarding some). More details can be found on AppleLife.ru.</source>
        <translation>注意：应尽可能避免使用此选项。 USB端口限制由locationID格式中使用的位数限制，如果不进行大量OS修改，则无法解决此问题。 唯一有效的解决方案是将使用的端口数限制为15（丢弃一些端口）。 可以在AppleLife.ru上找到更多详细信息。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1576"/>
        <source>Patch various kexts (AppleUSBXHCI.kext, AppleUSBXHCIPCI.kext, IOUSBHostFamily.kext) to
 remove USB port count limit of 15 ports.</source>
        <translation>修补各种kext（AppleUSBXHCI.kext，AppleUSBXHCIPCI.kext，IOUSBHostFamily.kext）以消除15个端口的USB端口数限制。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1580"/>
        <source>XhciPortLimit</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1587"/>
        <source>Requirement: 10.13 (not required for older)</source>
        <translation>要求：10.13（较早版本不需要）</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1590"/>
        <source>Prevent kernel from printing kext dump in the panic log preventing from observing panic details.
 Affects 10.13 and above.</source>
        <translation>防止内核在恐慌日志中打印kext dump，从而防止观察恐慌细节。 影响10.13及以上。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1594"/>
        <source>PanicNoKextDump</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1601"/>
        <source>Note: This option should be avoided whenever possible. Modern firmwares usually have compatible AHCI
controllers.</source>
        <translation>注意：应尽可能避免使用此选项。 现代固件通常具有兼容的AHCI控制器。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1605"/>
        <source> Apply icon type patches to AppleAHCIPort.kext to force internal disk icons for all AHCI disks.</source>
        <translation>将图标类型的修补程序应用于AppleAHCIPort.kext，以强制所有AHCI磁盘使用内部磁盘图标。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1608"/>
        <source>ExternalDiskIcons</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1615"/>
        <source>An additional security measure was added to macOS Catalina (10.15) causing kernel panic on power change
timeout for Apple drivers. Sometimes it may cause issues on misconfigured hardware, notably digital audio, which
sometimes fails to wake up. For debug kernels setpowerstate_panic=0 boot argument should be used, which is
otherwise equivalent to this quirk.</source>
        <translation>向macOS Catalina（10.15）添加了额外的安全措施，导致Apple驱动程序的电源更改超时导致内核崩溃。 有时，它可能会导致配置错误的硬件出现问题，尤其是数字音频，有时无法唤醒。 对于调试内核，应使用setpowerstate_panic = 0引导参数，否则它等同于此选项。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1621"/>
        <source> Disables kernel panic on setPowerState timeout.</source>
        <translation>在setPowerState超时时禁用内核崩溃。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1624"/>
        <source>PowerTimeoutKernelPanic</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1631"/>
        <source>Note: This option may be avoided on user preference. NVMe SSDs are compatible without the change. For AHCI
SSDs on modern macOS version there is a dedicated built-in utility called trimforce. Starting from 10.15 this
utility creates EnableTRIM variable in APPLE_BOOT_VARIABLE_GUID namespace with 01 00 00 00 value.</source>
        <translation>注意：可以根据用户喜好避免使用此选项。 NVMe SSD兼容，无需更改。 对于现代macOS版本上的AHCI SSD，有一个专用的内置实用程序，称为trimforce。 从10.15开始，此实用程序将在APPLE_BOOT_VARIABLE_GUID命名空间中创建EnableTRIM变量，其值为01 00 00 00。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1636"/>
        <source> Apply vendor patches to IOAHCIBlockStorage.kext to enable native features for third-party drives,
 such as TRIM on SSDs or hibernation support on 10.15 and newer.</source>
        <translation>将供应商补丁程序应用于IOAHCIBlockStorage.kext，以启用第三方驱动器的本机功能，例如SSD上的TRIM或10.15及更高版本上的休眠支持。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1640"/>
        <source>ThirdPartyDrives</source>
        <translation></translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;This option lets Lilu.kext and possibly some others function in macOS Big Sur with best performance without&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; color:#fc0107;&quot;&gt;keepsyms=1&lt;/span&gt; boot argument.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="obsolete">&lt;html&gt; &lt;head /&gt; &lt;body&gt; &lt;p&gt;此选项使Lilu.kext以及可能的其他一些功能在macOS Big Sur中以最佳性能运行而没有最佳效果&lt;/ p&gt; &lt;p&gt; &lt;span style =“ color：＃fc0107;” &gt; keepsyms = 1 &lt;/ span&gt;引导参数。&lt;/ p&gt; &lt;/ body&gt; &lt;/ html&gt;</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1647"/>
        <source>This option lets Lilu.kext and possibly some others function in macOS Big Sur with best performance without
keepsyms=1 boot argument.</source>
        <translation>此选项使Lilu.kext和可能的其他功能在macOS Big Sur中以最佳性能运行，而无需keepsyms = 1引导参数。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1651"/>
        <source>Disables __LINKEDIT jettison code.</source>
        <translation>禁用__LINKEDIT抛放代码。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1654"/>
        <source>DisableLinkeditJettison</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1689"/>
        <source>Scheme</source>
        <translation>Scheme（老旧系统方案）</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1701"/>
        <source>On macOS 10.6 and earlier kernelcache filename has a checksum, which essentially is adler32 from SMBIOS
product name and EfiBoot device path. On certain firmwares EfiBoot device path differs between UEFI and
macOS due to ACPI or hardware specifics, rendering kernelcache checksum as always different.
This setting allows matching the latest kernelcache with a suitable architecture when the kernelcache without
suffix is unavailable, improving macOS 10.6 boot performance on several platforms.</source>
        <translation>在macOS 10.6和更早版本上，kernelcache文件名具有校验和，本质上是SMBIOS产品名称和EfiBoot设备路径中的adler32。 在某些固件上，由于ACPI或硬件的不同，EFIBoot设备路径在UEFI和macOS之间有所不同，从而使内核缓存校验和始终不同。 当不带后缀的内核高速缓存不可用时，此设置允许将最新的内核高速缓存与合适的体系结构进行匹配，从而提高了多个平台上的macOS 10.6引导性能。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1708"/>
        <source>Use kernelcache with different checksums when available.</source>
        <translation>如果可用，将kernelcache与不同的校验和一起使用。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1711"/>
        <source>FuzzyMatch</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1724"/>
        <source>On macOS 10.7 and earlier XNU kernel can boot with architectures different from the usual x86_64. This setting
will use the specified architecture to boot macOS when it is supported by the macOS and the configuration:
• Auto — Choose the preferred architecture automatically.
• i386 — Use i386 (32-bit) kernel when available.
• i386-user32 — Use i386 (32-bit) kernel when available and force the use of 32-bit userspace on 64-bit
capable processors. On macOS 64-bit capable processors are assumed to support SSSE3. This is not the case
for older 64-bit capable Pentium processors, which cause some applications to crash on macOS 10.6. The
behaviour corresponds to -legacy kernel boot argument.
• x86_64 — Use x86_64 (64-bit) kernel when available.</source>
        <translation>在macOS 10.7和更早版本上，XNU内核可以使用不同于通常的x86_64的体系结构进行引导。 当macOS和配置支持它时，此设置将使用指定的体系结构来引导macOS：
•Auto - 自动选择最佳的架构。
•i386 —如果可用，请使用i386（32位）内核。
•i386-user32-使用i386（32位）内核（如果可用），并在支持64位的处理器上强制使用32位用户空间。 在macOS上，假定支持64位的处理器支持SSSE3。 较旧的支持64位奔腾处理器的情况并非如此，后者导致某些应用程序在macOS 10.6上崩溃。 该行为对应于-legacy kernel boot参数。
•x86_64-使用x86_64（64位）内核（如果可用）。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1735"/>
        <source> Prefer specified kernel architecture (Auto, i386, i386-user32, x86_64) when available.</source>
        <translation>如果可用，最好使用指定的内核体系结构（自动，i386，i386-user32，x86_64）。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1738"/>
        <source>KernelArch</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1761"/>
        <source>Different variants of macOS support different kernel caching variants designed to improve boot performance. This
setting allows to prevent using faster kernel caching variants if slower variants are available for debugging and
stability reasons. I.e. by specifying Mkext one will disable Prelinked for e.g. 10.6 but not 10.7.</source>
        <translation>macOS的不同变体支持旨在提高启动性能的不同内核缓存变体。 如果出于调试和稳定性的原因而可以使用较慢的变体，则此设置可以防止使用较快的内核缓存变体。 即 通过指定Mkext可以例如禁用预链接 10.6，而不是10.7。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1766"/>
        <source> Prefer specified kernel cache type (Auto, Cacheless, Mkext, Prelinked) when available.</source>
        <translation>如果可用，则首选指定的内核缓存类型（自动，无缓存，Mkext，预链接）。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1769"/>
        <source>KernelCache</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1805"/>
        <source>Boot</source>
        <oldsource>Boot-引导</oldsource>
        <translation>Boot（引导）</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:10.0622pt; color:#000000;&quot;&gt;Text renderer supports colour arguments as a sum of foreground and background colours according to UEFI &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;specifification. The value of black background and black foreground (&lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;0&lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;) is reserved. List of colour names: &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;• &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;0x00 &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;— &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;EFI_BLACK &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;• &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;0x01 &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;— &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;EFI_BLUE &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;• &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;0x02 &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;— &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;EFI_GREEN &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;• &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;0x03 &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;— &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;EFI_CYAN &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;• &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;0x04 &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;— &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;EFI_RED &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;• &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;0x05 &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;— &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;EFI_MAGENTA &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;• &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;0x06 &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;— &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;EFI_BROWN &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;• &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;0x07 &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;— &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;EFI_LIGHTGRAY &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;• &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;0x08 &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;— &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;EFI_DARKGRAY &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;• &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;0x09 &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;— &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;EFI_LIGHTBLUE &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;• &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;0x0A &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;— &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;EFI_LIGHTGREEN &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;• &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;0x0B &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;— &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;EFI_LIGHTCYAN &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;• &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;0x0C &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;— &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;EFI_LIGHTRED &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;• &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;0x0D &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;— &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;EFI_LIGHTMAGENTA &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;• &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;0x0E &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;— &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;EFI_YELLOW &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;• &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;0x0F &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;— &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;EFI_WHITE &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;• &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;0x00 &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;— &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;EFI_BACKGROUND_BLACK &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;• &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;0x10 &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;— &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;EFI_BACKGROUND_BLUE &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;• &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;0x20 &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;— &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;EFI_BACKGROUND_GREEN &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;• &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;0x30 &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;— &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;EFI_BACKGROUND_CYAN &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;• &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;0x40 &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;— &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;EFI_BACKGROUND_RED &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;• &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;0x50 &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;— &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;EFI_BACKGROUND_MAGENTA &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;• &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;0x60 &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;— &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;EFI_BACKGROUND_BROWN &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;• &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;0x70 &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;— &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;EFI_BACKGROUND_LIGHTGRAY &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Italic&apos;; font-size:9.86297pt; font-style:italic; color:#000000;&quot;&gt;Note&lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.86297pt; color:#000000;&quot;&gt;: This option may not work well with &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;System &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.86297pt; color:#000000;&quot;&gt;text renderer. Setting a background difffferent from black could &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;help testing proper GOP functioning.&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="vanished">根据UEFI规范，文本渲染器支持将颜色参数作为前景色和背景色之和。黑色背景和黑色前景（0）的值保留。颜色名称列表：
•0x00-EFI_BLACK
•0x01-EFI_BLUE
•0x02-EFI_GREEN
•0x03-EFI_CYAN
•0x04-EFI_RED
•0x05-EFI_MAGENTA
•0x06-EFI_BROWN
•0x07-EFI_LIGHTGRAY
•0x08-EFI_DARKGRAY
•0x09-EFI_LIGHTBLUE
•0x0A — EFI_LIGHTGREEN
•0x0B — EFI_LIGHTCYAN
•0x0C — EFI_LIGHTRED
•0x0D-EFI_LIGHTMAGENTA
•0x0E — EFI_YELLOW
•0x0F — EFI_WHITE
•0x00-EFI_BACKGROUND_BLACK
•0x10-EFI_BACKGROUND_BLUE
•0x20-EFI_BACKGROUND_GREEN
•0x30-EFI_BACKGROUND_CYAN
•0x40-EFI_BACKGROUND_RED
•0x50-EFI_BACKGROUND_MAGENTA
•0x60-EFI_BACKGROUND_BROWN
•0x70-EFI_BACKGROUND_LIGHTGRAY
注意：此选项可能不适用于系统文本渲染器。设置不同于黑色的背景可以帮助测试GOP的正常运行。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1890"/>
        <source>Sets specific attributes for console.</source>
        <translation>设置控制台的特定属性。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1893"/>
        <source>ConsoleAttributes</source>
        <oldsource>ConsoleAttributes-文本启动菜单颜色</oldsource>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1903"/>
        <source>Introducing a delay may give extra time to hold the right action hotkey sequence to e.g. boot to recovery mode.
On some platforms setting this option to at least 5000-10000 microseconds may be necessary to access action
hotkeys at all due to the nature of the keyboard driver.</source>
        <translation>引入延迟可能会给额外的时间来将正确的操作热键序列保持为例如 引导至恢复模式。 在某些平台上，由于键盘驱动程序的特性，可能完全需要将此选项设置为至少5000-10000微秒才能访问操作热键。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1908"/>
        <source>Delay in microseconds performed before handling picker startup and action hotkeys.</source>
        <translation>在处理选择器启动和操作热键之前执行的延迟（以微秒为单位）。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1911"/>
        <source>TakeoffDelay</source>
        <oldsource>TakeoffDelay-快捷键延迟</oldsource>
        <translation></translation>
    </message>
    <message>
        <source>Different pickers may be configured through the attribute mask containing OpenCore-reserved (BIT0~BIT15) and
OEM-specific (BIT16~BIT31) values.
Current OpenCore values include:
• 0x0001 — OC_ATTR_USE_VOLUME_ICON, provides custom icons for boot entries:
For Tools OpenCore will try to load a custom icon and fallback to the default icon:
– ResetNVRAM — ResourcesImageResetNVRAM.icns — ResetNVRAM.icns from icons directory.
– Tools&lt;TOOL_RELATIVE_PATH&gt;.icns — icon near the tool file with appended .icns extension.
For custom boot Entries OpenCore will try to load a custom icon and fallback to the volume icon or the
default icon:
– &lt;ENTRY_PATH&gt;.icns — icon near the entry file with appended .icns extension.
For all other entries OpenCore will try to load a volume icon and fallback to the default icon:
– .VolumeIcon.icns file at Preboot root for APFS.
– .VolumeIcon.icns file at volume root for other filesystems.
Volume icons can be set in Finder. Note, that enabling this may result in external and internal icons to be
indistinguishable.
• 0x0002 — OC_ATTR_USE_DISK_LABEL_FILE, provides custom rendered titles for boot entries:
– .disk_label (.disk_label_2x) file near bootloader for all filesystems.
– &lt;TOOL_NAME&gt;.lbl (&lt;TOOL_NAME&gt;.l2x) file near tool for Tools.
Prerendered labels can be generated via disklabel utility or bless command. When disabled or missing
text labels (.contentDetails or .disk_label.contentDetails) are to be rendered instead.
• 0x0004 — OC_ATTR_USE_GENERIC_LABEL_IMAGE, provides predefined label images for boot entries without
custom entries. May give less detail for the actual boot entry.
• 0x0008 — OC_ATTR_USE_ALTERNATE_ICONS, changes used icon set to an alternate one if it is supported. For
example, this could make a use of old-style icons with a custom background colour.</source>
        <translation type="vanished">可以通过包含OpenCore保留（BIT0〜BIT15）和OEM特定（BIT16〜BIT31）值的属性掩码来配置不同的选择器。当前的OpenCore值包括：
•0x0001 — OC_ATTR_USE_VOLUME_ICON，提供用于引导项的自定义图标：对于工具，OpenCore会尝试加载自定义图标并回退到默认图标：– ResetNVRAM-ResourcesImageResetNVRAM.icns-图标目录中的ResetNVRAM.icns。 – Tools &lt;TOOL_RELATIVE_PATH&gt; .icns —工具文件附近的图标，带有扩展的.icns扩展名。对于自定义启动，Entries OpenCore将尝试加载自定义图标并回退到卷图标或默认图标：– &lt;ENTRY_PATH&gt; .icns —带有扩展名.icns的条目文件附近的图标。对于所有其他条目，OpenCore会尝试加载一个卷图标并回退到默认图标：– APFS的Preboot根目录下的.VolumeIcon.icns文件。 –其他文件系统的卷根目录下的.VolumeIcon.icns文件。可以在Finder中设置音量图标。请注意，启用此选项可能会导致外部和内部图标无法区分。
•0x0002 — OC_ATTR_USE_DISK_LABEL_FILE，为引导条目提供自定义的渲染标题：–引导加载程序附近的.disk_label（.disk_label_2x）文件，用于所有文件系统。 –工具附近的&lt;TOOL_NAME&gt; .lbl（&lt;TOOL_NAME&gt; .l2x）文件。可以通过disklabel实用程序或bless命令生成预渲染的标签。禁用或丢失时，将呈现文本标签（.contentDetails或.disk_label.contentDetails）。
•0x0004 — OC_ATTR_USE_GENERIC_LABEL_IMAGE，为没有自定义条目的引导条目提供了预定义的标签图像。可能会为实际的引导条目提供较少的详细信息。
•0x0008-OC_ATTR_USE_ALTERNATE_ICONS，将已使用图标集更改为备用图标（如果支持）。例如，这可以使用具有自定义背景色的旧式图标。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1921"/>
        <source> Timeout in seconds in boot picker before automatic booting of the default boot entry. Use 0 to  disable timer.</source>
        <translation>自动启动默认启动项之前，启动选择器中的超时时间（以秒为单位）。 使用0禁用计时器。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1924"/>
        <source> Timeout in seconds in boot picker before automatic booting of the default boot entry. Use 0 to
 disable timer.</source>
        <translation>自动启动默认启动项之前，启动选择器中的超时时间（以秒为单位）。 使用0禁用计时器。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1928"/>
        <source>Timeout</source>
        <oldsource>Timeout-倒计时</oldsource>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1938"/>
        <source>Description: Hibernation detection mode. The following modes are supported:
• None — Avoid hibernation for your own good.
• Auto — Use RTC and NVRAM detection.
• RTC — Use RTC detection.
• NVRAM — Use NVRAM detection.</source>
        <translation>说明：休眠检测模式。
支持以下模式：
•None-不休眠。
•Auto-使用RTC和NVRAM检测。
•RTC-使用RTC检测。
•NVRAM-使用NVRAM检测。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1945"/>
        <source>Hibernation detection mode. The following modes are supported:</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1948"/>
        <source>HibernateMode</source>
        <oldsource>HibernateMode-休眠模式</oldsource>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1958"/>
        <source>Picker describes underlying boot management with an optional user interface responsible for handling boot
options. The following values are supported:
• Builtin — boot management is handled by OpenCore, a simple text only user interface is used.
• External — an external boot management protocol is used if available. Otherwise Builtin mode is used.
• Apple — Apple boot management is used if available. Otherwise Builtin mode is used.</source>
        <translation>Picker使用负责处理启动选项的可选用户界面描述了基础启动管理。 支持以下值：
•Builtin-引导管理由OpenCore处理，使用纯文本用户界面。 
•External-使用外部引导管理协议（如果可用）。 否则，将使用内置模式。 
•Apple-使用Apple引导管理（如果可用）。 否则，将使用内置模式。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1965"/>
        <source> Choose boot picker used for boot management.</source>
        <translation>选择用于引导管理的引导选择器。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1968"/>
        <source>PickerMode</source>
        <oldsource>PickerMode-启动菜单模式</oldsource>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2005"/>
        <source>For macOS bootloader screen reader preference is set in preferences.efires archive in isVOEnabled.int32
file and is controlled by the operating system. For OpenCore screen reader support this option is an independent
equivalent. Toggling screen reader support in both OpenCore boot picker and macOS bootloader FileVault 2
login window can also be done with Command + F5 key combination.</source>
        <translation>对于macOS引导加载程序，屏幕阅读器首选项在preferences.efires存档中的preferences.efires档案中设置，并由操作系统控制。 对于OpenCore屏幕阅读器支持，此选项是一个独立的等效项。 也可以使用Command + F5组合键在OpenCore引导选择器和macOS引导加载程序FileVault 2登录窗口中切换屏幕阅读器支持。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2011"/>
        <source>Enable screen reader by default in boot picker.</source>
        <translation>默认情况下在启动选择器中启用屏幕阅读器。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2014"/>
        <source>PickerAudioAssist</source>
        <oldsource>PickerAudioAssist-语音辅助</oldsource>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2046"/>
        <source>Show simple boot picker to allow boot entry selection.</source>
        <translation>显示简单的启动选择器，以允许选择启动项。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2049"/>
        <source> Show simple boot picker to allow boot entry selection.</source>
        <translation>显示简单的启动选择器，以允许选择启动项。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2052"/>
        <source>ShowPicker</source>
        <oldsource>ShowPicker-显示引导菜单</oldsource>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1987"/>
        <source>An entry is considered auxiliary when at least one of the following applies:
• Entry is macOS recovery.
• Entry is macOS Time Machine.
• Entry is explicitly marked as Auxiliary. • Entry is system (e.g. Reset NVRAM).
To see all entries picker menu needs to be reloaded in extended mode by pressing Spacebar key. Hiding auxiliary
entries may increase boot performance for multidisk systems.</source>
        <translation>在满足以下至少一项条件时，条目被视为辅助条目：•条目是macOS恢复。 •输入为macOS Time Machine。 •条目被明确标记为辅助。 •输入是系统信息（例如，重置NVRAM）。 要查看所有条目，请按空格键在扩展模式下重新加载选择器菜单。 隐藏辅助条目可能会提高多磁盘系统的启动性能。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1995"/>
        <source>Hides auxiliary entries from picker menu by default.</source>
        <translation>默认情况下，从选择器菜单中隐藏辅助条目。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1998"/>
        <source>HideAuxiliary</source>
        <oldsource>HideAuxiliary-隐藏辅助条目</oldsource>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2021"/>
        <source>In addition to action hotkeys, which are partially described in PickerMode section and are normally handled
by Apple BDS, there exist modifier keys, which are handled by operating system bootloader, namely boot.efi.
These keys allow to change operating system behaviour by providing different boot modes.
On some firmwares it may be problematic to use modifier keys due to driver incompatibilities. To workaround
this problem this option allows registering select hotkeys in a more permissive manner from within boot picker.
Such extensions include the support of tapping on keys in addition to holding and pressing Shift along with
other keys instead of just Shift alone, which is not detectible on many PS/2 keyboards. This list of known
modifier hotkeys includes:
• CMD+C+MINUS — disable board compatibility checking.
• CMD+K — boot release kernel, similar to kcsuffix=release. • CMD+S — single user mode.
• CMD+S+MINUS — disable KASLR slide, requires disabled SIP.
• CMD+V — verbose mode.
• Shift — safe mode.</source>
        <translation>除了PickerMode部分中部分描述的动作热键（通常由Apple BDS处理）之外，还有修饰键，这些键由操作系统的引导加载程序（boot.efi）处理。这些键允许通过提供不同的引导模式来更改操作系统行为。在某些固件上，由于驱动程序不兼容，使用修饰键可能会出现问题。要变通解决此问题，此选项允许从引导选择器中以更宽松的方式注册选择的热键。这样的扩展包括除了按住并按住Shift键以及其他键以外，还可以点击其他键，而不是仅按住Shift键，这在许多PS / 2键盘上是无法检测到的。已知的修改器热键列表包括：•CMD + C + MINUS-禁用板兼容性检查。 •CMD + K-引导发行版内核，类似于kcsuffix = release。 •CMD + S-单用户模式。 •CMD + S + MINUS-禁用KASLR幻灯片，需要禁用SIP。 •CMD + V-详细模式。 •Shift-安全模式。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2036"/>
        <source> Enable modifier hotkey handling in boot picker.</source>
        <translation>在启动选择器中启用修改器热键处理。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2039"/>
        <source>PollAppleHotKeys</source>
        <oldsource>PollAppleHotKeys-模拟苹果热键</oldsource>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2063"/>
        <source>Debug</source>
        <oldsource>Debug-调试</oldsource>
        <translation>Debug（调试）</translation>
    </message>
    <message>
        <source>The following levels are supported (discover more in
DebugLib.h):
• 0x00000002 (bit 1) — DEBUG_WARN in DEBUG, NOOPT, RELEASE. • 0x00000040 (bit 6) — DEBUG_INFO in DEBUG, NOOPT. • 0x00400000 (bit 22) — DEBUG_VERBOSE in custom builds.
• 0x80000000 (bit 31) — DEBUG_ERROR in DEBUG, NOOPT, RELEASE.</source>
        <translation type="vanished">支持以下级别（在DebugLib.h中可以找到更多内容）：
•0x00000002（位1）— DEBUG，NOOPT和RELEASE中的DEBUG_WARN。 
•0x00000040（位6）— DEBUG，NOOPT中的DEBUG_INFO。 
•0x00400000（位22）—自定义版本中的DEBUG_VERBOSE。 
•0x80000000（位31）— DEBUG，NOOPT和RELEASE中的DEBUG_ERROR。</translation>
    </message>
    <message>
        <source>EDK II debug level bitmask (sum) showed onscreen. Unless Target enables console (onscreen)
 printing, onscreen debug output will not be visible.</source>
        <translation type="vanished">屏幕上显示了EDK II调试级别位掩码（总和）。 除非Target启用控制台（屏幕上）打印，否则屏幕上的调试输出将不可见。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2072"/>
        <source>DisplayLevel</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2458"/>
        <location filename="mainwindow.ui" line="2461"/>
        <source>Delay in microseconds performed after every printed line visible onscreen (i.e. console).</source>
        <translation>屏幕上每条可见的打印行（即控制台）执行之后的微秒延迟。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2464"/>
        <source>DisplayDelay</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2474"/>
        <source>The following logging targets are supported:
• 0x01 (bit 0) — Enable logging, otherwise all log is discarded.
• 0x02 (bit 1) — Enable basic console (onscreen) logging.
• 0x04 (bit 2) — Enable logging to Data Hub.
• 0x08 (bit 3) — Enable serial port logging.
• 0x10 (bit 4) — Enable UEFI variable logging.
• 0x20 (bit 5) — Enable non-volatile UEFI variable logging.
• 0x40 (bit 6) — Enable logging to file.
Console logging prints less than all the other variants. Depending on the build type (RELEASE, DEBUG, or NOOPT)
different amount of logging may be read (from least to most).</source>
        <translation>支持以下日志记录目标：
•0x01（位0）—启用日志记录，否则将丢弃所有日志。 
•0x02（位1）—启用基本控制台（屏幕）日志记录。 
•0x04（位2）—启用记录到数据中心。 
•0x08（位3）—启用串行端口日志记录。 
•0x10（位4）—启用UEFI变量日志记录。 
•0x20（位5）—启用非易失性UEFI变量日志记录。 
•0x40（位6）—启用记录到文件。 
控制台日志记录的打印量少于所有其他变量。 根据构建类型（RELEASE，DEBUG或NOOPT），可能会读取不同数量的日志（从最小到最大）。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2486"/>
        <source>A bitmask (sum) of enabled logging targets. By default all the logging output is hidden, so this
 option is required to be set when debugging is necessary.</source>
        <translation>启用的日志记录目标的位掩码（总和）。 默认情况下，所有日志记录输出都是隐藏的，因此在需要调试时需要设置此选项。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2490"/>
        <source>Target</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2509"/>
        <source>Note: This option only applies to 10.15.4 and newer.</source>
        <translation>注意：此选项仅适用于10.15.4及更高版本。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2512"/>
        <source>Enable boot.efi debug log saving to OpenCore log.</source>
        <translation>启用boot.efi调试日志保存到OpenCore日志。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2515"/>
        <source>AppleDebug</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2539"/>
        <source>The file is saved as panic-YYYY-MM-DD-HHMMSS.txt. It is strongly recommended to have keepsyms=1 boot
argument to see debug symbols in the panic log. In case it was not present kpdescribe.sh utility (bundled with
OpenCore) may be used to partially recover the stacktrace.
Development and debug kernels produce more helpful kernel panics. Consider downloading and installing
KernelDebugKit from developer.apple.com when debugging a problem. To activate a development kernel you
will need to add a kcsuffix=development boot argument. Use uname -a command to ensure that your current
loaded kernel is a development (or a debug) kernel.</source>
        <translation>该文件另存为panic-YYYY-MM-DD-HHMMSS.txt。 强烈建议使用keepsyms = 1引导参数来查看紧急日志中的调试符号。 如果不存在，则可以使用kpdescribe.sh实用程序（与OpenCore捆绑在一起）部分恢复堆栈跟踪。 开发和调试内核会产生更多有用的内核恐慌。 调试问题时，请考虑从developer.apple.com下载并安装KernelDebugKit。 要激活开发内核，您将需要添加kcsuffix = development引导参数。 使用uname -a命令来确保当前加载的内核是开发（或调试）内核。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2548"/>
        <source> Save macOS kernel panic to OpenCore root partition.</source>
        <translation>将macOS内核紧急情况保存到OpenCore根分区。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2551"/>
        <source>ApplePanic</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2574"/>
        <source> Select firmwares may not succeed in quickly booting the operating system, especially in debug  mode, which results in watch dog timer aborting the process. This option turns off watch dog timer.</source>
        <translation>某些固件可能无法成功快速启动操作系统，尤其是在调试模式下，这会导致看门狗定时器中止该过程。 此选项关闭看门狗定时器。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2577"/>
        <source> Select firmwares may not succeed in quickly booting the operating system, especially in debug
 mode, which results in watch dog timer aborting the process. This option turns off watch dog timer.</source>
        <translation>某些固件可能无法成功快速启动操作系统，尤其是在调试模式下，这会导致看门狗定时器中止该过程。 此选项关闭看门狗定时器。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2581"/>
        <source>DisableWatchDog</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2522"/>
        <source>This option will perform serial port initialisation within OpenCore prior to enabling (any) debug logging. Serial
port configuration is defined via PCDs at compile time in gEfiMdeModulePkgTokenSpaceGuid GUID. Default
values as found in MdeModulePkg.dec are as follows:
• PcdSerialBaudRate — Baud rate: 115200. • PcdSerialLineControl — Line control: no parity, 8 data bits, 1 stop bit.
See more details in Debugging section.</source>
        <translation>在启用（任何）调试日志记录之前，此选项将在OpenCore中执行串行端口初始化。 串行端口配置是在编译时通过gEfiMdeModulePkgTokenSpaceGuid GUID中的PCD定义的。 在MdeModulePkg.dec中找到的默认值如下：•PcdSerialBaudRate-波特率：115200。•PcdSerialLineControl-线路控制：无奇偶校验，8个数据位，1个停止位。 在“调试”部分中查看更多详细信息。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2529"/>
        <source>Perform serial port initialisation.</source>
        <translation>执行串行端口初始化。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2532"/>
        <source>SerialInit</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2558"/>
        <source>This option will create a SysReport directory on ESP partition unless it is already present. The directory will
contain ACPI and SMBIOS dumps.
Note: For security reasons SysReport option is not available in RELEASE builds. Use a DEBUG build if you need
this option.</source>
        <translation>除非已经存在，否则此选项将在ESP分区上创建SysReport目录。 该目录将包含ACPI和SMBIOS转储。 注意：出于安全原因，RELEASE版本中不提供SysReport选项。 如果需要此选项，请使用DEBUG构建。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2564"/>
        <source>Produce system report on ESP folder.</source>
        <translation>在ESP文件夹上生成系统报告。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2567"/>
        <source>SysReport</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2592"/>
        <source>Security</source>
        <oldsource>Security-安全</oldsource>
        <translation>Security（安全）</translation>
    </message>
    <message>
        <source>• 0x01 — Expose printable booter path as an UEFI variable.
• 0x02 — Expose OpenCore version as an UEFI variable.
• 0x04 — Expose OpenCore version in boot picker menu title.
• 0x08 — Expose OEM information as a set of UEFI variables.</source>
        <translation type="vanished">•0x01-将可打印的引导程序路径公开为UEFI变量。
•0x02-将OpenCore版本公开为UEFI变量。
•0x04 —在引导选择器菜单标题中公开OpenCore版本。
•0x08-将OEM信息公开为一组UEFI变量。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2598"/>
        <source> Sensitive data exposure bitmask (sum) to operating system.</source>
        <translation>操作系统的敏感数据公开位掩码（总和）。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2601"/>
        <source>ExposeSensitiveData</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3048"/>
        <source> EDK II debug level bitmask (sum) causing CPU to halt (stop execution) after obtaining a message
of HaltLevel. Possible values match DisplayLevel values.</source>
        <translation>EDK II调试级别位掩码（总和）在获取HaltLevel消息后导致CPU停止（停止执行）。 可能的值与DisplayLevel值匹配。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3052"/>
        <source> EDK II debug level bitmask (sum) causing CPU to halt (stop execution) after obtaining a message
 of HaltLevel. Possible values match DisplayLevel values.</source>
        <translation>EDK II调试级别位掩码（总和）在获取HaltLevel消息后导致CPU停止（停止执行）。 可能的值与DisplayLevel值匹配。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3056"/>
        <source>HaltLevel</source>
        <translation></translation>
    </message>
    <message>
        <source>This value allows to prevent scanning (and booting) from untrusted source based on a bitmask (sum) of select
flags. As it is not possible to reliably detect every file system or device type, this feature cannot be fully relied
upon in open environments, and the additional measures are to be applied.
Third party drivers may introduce additional security (and performance) measures following the provided scan
policy. Scan policy is exposed in scan-policy variable of 4D1FDA02-38C7-4A6A-9CC6-4BCCA8B30102 GUID for
UEFI Boot Services only.</source>
        <translation type="vanished">该值可以防止基于选择标志的位掩码（总和）从不受信任的源进行扫描（和引导）。 由于无法可靠地检测每个文件系统或设备类型，因此在开放环境中不能完全依赖此功能，因此需要采取其他措施。 第三方驱动程序可能会按照提供的扫描策略引入其他安全性（和性能）措施。 仅在UEFI引导服务的4D1FDA02-38C7-4A6A-9CC6-4BCCA8B30102 GUID的scan-policy变量中公开了扫描策略。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2654"/>
        <source> Define operating system detection policy.</source>
        <translation>定义操作系统检测策略。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2657"/>
        <source>ScanPolicy</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3090"/>
        <source>Valid values:
• None — do nothing.
• Bootstrap — create or update top-priority EFIOCBootstrapBootstrap.efi boot option (Boot9696)
in UEFI variable storage at bootloader startup. For this option to work RequestBootVarRouting is required
to be enabled.
This option provides integration with third-party operating system installation and upgrade at the times they
overwrite EFIBOOTBOOTx64.efi file. By creating a custom option in Bootstrap mode this file path becomes
no longer used for bootstraping OpenCore.
Note 1 : Some firmewares may have broken NVRAM, no boot option support, or various other incompatibilities
of any kind. While unlikely, the use of this option may even cause boot failure. Use at your own risk on boards
known to be compatible.
Note 2 : Be warned that while NVRAM reset executed from OpenCore should not erase the boot option created
in Bootstrap, executing NVRAM reset prior to loading OpenCore will remove it.</source>
        <translation>有效值：
•无-不执行任何操作。
•Bootstrap —在启动引导程序时，在UEFI变量存储中创建或更新优先级最高的EFIOCBootstrapBootstrap.efi引导选项（Boot9696）。 为了使此选项起作用，需要启用RequestBootVarRouting。 此选项在覆盖EFIBOOTBOOTx64.efi文件时提供与第三方操作系统安装和升级的集成。 通过在Bootstrap模式下创建自定义选项，此文件路径将不再用于引导OpenCore。
注意1：某些固件可能损坏了NVRAM，不提供启动选项支持或各种其他不兼容性。 尽管不太可能，但使用此选项甚至可能导致引导失败。 在已知兼容的电路板上使用，后果自负。
注意2：警告：虽然从OpenCore执行的NVRAM重置不应擦除在Bootstrap中创建的引导选项，但在加载OpenCore之前执行NVRAM重置将删除它。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3105"/>
        <source> Attempt to provide bootloader persistence.</source>
        <translation>尝试提供引导加载程序的持久性。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3108"/>
        <source>BootProtect</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3134"/>
        <source>Valid values:
• Disabled — loading DMG images will fail. Disabled policy will still let macOS Recovery to load in most
cases as there usually are boot.efi files compatible with Apple Secure Boot. Manually downloaded DMG
images stored in com.apple.recovery.boot directories will not load, however.
• Signed — only Apple-signed DMG images will load. Due to Apple Secure Boot design Signed policy will
let any Apple-signed macOS Recovery to load regardless of Apple Secure Boot state, which may not always
be desired.
• Any — any DMG images will mount as normal filesystems. Any policy is strongly not recommended and will
cause a boot failure when Apple Secure Boot is activated.</source>
        <translation>有效值：
•已禁用-加载DMG图像将失败。 在大多数情况下，由于通常存在与Apple Secure Boot兼容的boot.efi文件，因此已禁用的策略仍将允许macOS Recovery加载。 但是，不会下载com.apple.recovery.boot目录中存储的手动下载的DMG映像。
•已签名-将仅加载Apple签名的DMG图像。 由于Apple安全启动设计的原因，无论Apple安全启动状态如何，“签名策略”都将允许加载任何Apple签名的macOS恢复，这并非总是需要的。
•任何-任何DMG映像都将挂载为普通文件系统。 强烈建议不要使用任何策略，并且在激活Apple Secure Boot时会导致启动失败。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3145"/>
        <source>Define Disk Image (DMG) loading policy used for macOS Recovery.</source>
        <translation>定义用于macOS恢复的磁盘映像（DMG）加载策略。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3148"/>
        <source>DmgLoading</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3174"/>
        <source>Valid values:
• Optional — require nothing, no vault is enforced, insecure.
• Basic — require vault.plist file present in OC directory. This provides basic filesystem integrity verification
and may protect from unintentional filesystem corruption.
• Secure — require vault.sig signature file for vault.plist in OC directory. This includes Basic integrity
checking but also attempts to build a trusted bootchain.</source>
        <translation>有效值：
•可选-不要求任何内容，不执行任何保管库，不安全。
•基本-要求OC目录中存在vault.plist文件。 这提供了基本的文件系统完整性验证，并可以防止意外的文件系统损坏。
•安全-OC目录中的vault.plist需要vault.sig签名文件。 这不仅包括基本完整性检查，还尝试构建可信的启动链。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3182"/>
        <source> Enables vaulting mechanism in OpenCore.</source>
        <translation>在OpenCore中启用存储机制。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3185"/>
        <source>Vault</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3066"/>
        <source>Setting this value to any non-zero 64-bit integer will allow using personalised Apple Secure Boot identifiers. If
you want to use this setting, make sure to generate a random 64-bit number with a cryptographically secure
random number generator. With this value set and SecureBootModel valid and not Disabled it is possible to
achieve Full Security of Apple Secure Boot.
To start using personalised Apple Secure Boot you will have to reinstall the operating system or personalise it.
Until your operating system is personalised you will only be able to load macOS DMG recovery. If you do not have
DMG recovery you could always download it with macrecovery utility and put to com.apple.recovery.boot
as explained in Tips and Tricks section. Keep in mind that DMG loading needs to be set to Signed to use any
DMG with Apple Secure Boot.</source>
        <translation>将此值设置为任何非零的64位整数将允许使用个性化的Apple Secure Boot标识符。 如果要使用此设置，请确保使用加密安全的随机数生成器生成随机的64位数字。 设置此值且SecureBootModel有效且未禁用，则可以实现Apple Secure Boot的完全安全性。 要开始使用个性化的Apple Secure Boot，您将必须重新安装操作系统或对其进行个性化设置。 在个性化操作系统之前，您将只能加载macOS DMG恢复。 如果没有DMG恢复，则可以始终使用macrecovery实用程序下载它，并按照“技巧和窍门”部分中的说明放入com.apple.recovery.boot。 请记住，必须将DMG加载设置为“已签名”才能将任何DMG与Apple Secure Boot一起使用。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3077"/>
        <source>Apple Enclave Identifier.</source>
        <translation>苹果飞地标识符。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3080"/>
        <source>ApECID</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3118"/>
        <location filename="mainwindow.ui" line="3121"/>
        <source>Password hash used when EnabledPassword is set.</source>
        <translation>设置EnabledPassword时使用的密码哈希。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3124"/>
        <source>PasswordHash</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3158"/>
        <location filename="mainwindow.ui" line="3161"/>
        <source> Password salt used when EnabledPassword is set.</source>
        <translation>设置EnabledPassword时使用的密码盐。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3164"/>
        <source>PasswordSalt</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3195"/>
        <source>Sets Apple Secure Boot hardware model and policy. Specifying this value defines which operating systems will be
bootable. Operating systems shipped before the specified model was released will not boot. Valid values:
• Default — Recent available model, currently set to j137. • Disabled — No model, Secure Boot will be disabled.
• j137 — iMacPro1,1 (December 2017) minimum macOS 10.13.2 (17C2111)
• j680 — MacBookPro15,1 (July 2018) minimum macOS 10.13.6 (17G2112)
• j132 — MacBookPro15,2 (July 2018) minimum macOS 10.13.6 (17G2112)
• j174 — Macmini8,1 (October 2018) minimum macOS 10.14 (18A2063)
• j140k — MacBookAir8,1 (October 2018) minimum macOS 10.14.1 (18B2084)
• j780 — MacBookPro15,3 (May 2019) minimum macOS 10.14.5 (18F132)
• j213 — MacBookPro15,4 (July 2019) minimum macOS 10.14.5 (18F2058)
• j140a — MacBookAir8,2 (July 2019) minimum macOS 10.14.5 (18F2058)
• j152f — MacBookPro16,1 (November 2019) minimum macOS 10.15.1 (19B2093)
• j160 — MacPro7,1 (December 2019) minimum macOS 10.15.1 (19B88)
• j230k — MacBookAir9,1 (March 2020) minimum macOS 10.15.3 (19D2064)
• j214k — MacBookPro16,2 (May 2020) minimum macOS 10.15.4 (19E2269)
• j223 — MacBookPro16,3 (May 2020) minimum macOS 10.15.4 (19E2265)
• j215 — MacBookPro16,4 (June 2020) minimum macOS 10.15.5 (19F96)
• j185 — iMac20,1 (August 2020) minimum macOS 10.15.6 (19G2005)
• j185f — iMac20,2 (August 2020) minimum macOS 10.15.6 (19G2005)</source>
        <translation>设置Apple安全启动硬件模型和策略。指定此值将定义可引导的操作系统。在指定型号发布之前出厂的操作系统将无法启动。有效值：
•默认-最近可用的型号，当前设置为j137。 •禁用-无型号，安全启动将被禁用。
•j137 — iMacPro1,1（2017年12月）最低macOS 10.13.2（17C2111）
•j680 — MacBookPro15,1（2018年7月）最低macOS 10.13.6（17G2112）
•j132 — MacBookPro15,2（2018年7月）最低macOS 10.13.6（17G2112）
•j174 — Macmini8,1（2018年10月）最低macOS 10.14（18A2063）
•j140k-MacBookAir8,1（2018年10月）最低macOS 10.14.1（18B2084）
•j780 — MacBookPro15,3（2019年5月）最低macOS 10.14.5（18F132）
•j213 — MacBookPro15,4（2019年7月）最低macOS 10.14.5（18F2058）
•j140a — MacBookAir8,2（2019年7月）最低macOS 10.14.5（18F2058）
•j152f — MacBookPro16,1（2019年11月）最低macOS 10.15.1（19B2093）
•j160-MacPro7.1,1（2019年12月）最低macOS 10.15.1（19B88）
•j230k-MacBookAir9,1（2020年3月）最低macOS 10.15.3（19D2064）
•j214k-MacBookPro16,2（2020年5月）最低macOS 10.15.4（19E2269）
•j223 — MacBookPro16,3（2020年5月）最低macOS 10.15.4（19E2265）
•j215 — MacBookPro16,4（2020年6月）最低macOS 10.15.5（19F96）
•j185 — iMac20,1（2020年8月）最低macOS 10.15.6（19G2005）
•j185f-iMac20,2（2020年8月）最低macOS 10.15.6（19G2005）</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3216"/>
        <source> Apple Secure Boot hardware model.</source>
        <translation>Apple安全启动硬件模型。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3219"/>
        <source>SecureBootModel</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2978"/>
        <source>Authenticated restart is a way to reboot FileVault 2 enabled macOS without entering the password. To perform
authenticated restart one can use a dedicated terminal command: sudo fdesetup authrestart. It is also used
when installing operating system updates.
VirtualSMC performs authenticated restart by saving disk encryption key split in NVRAM and RTC, which
despite being removed as soon as OpenCore starts, may be considered a security risk and thus is optional.</source>
        <translation>经过身份验证的重启是一种无需输入密码即可重启启用FileVault 2的macOS的方法。 要执行经过身份验证的重新启动，可以使用专用的终端命令：sudo fdesetup authrestart。 在安装操作系统更新时也使用它。 VirtualSMC通过将磁盘加密密钥拆分保存在NVRAM和RTC中来执行经过身份验证的重新启动，尽管OpenCore启动后立即将其删除，但可以将其视为安全隐患，因此是可选的。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2985"/>
        <source>Enable VirtualSMC-compatible authenticated restart.</source>
        <translation>启用与VirtualSMC兼容的身份验证重新启动。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2988"/>
        <source>AuthRestart</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3008"/>
        <source>Password protection ensures that sensitive operations like booting a non-default operating system (e.g. macOS
recovery or a tool), resetting NVRAM storage, trying to boot into a non-default mode (e.g. verbose mode or safe
mode) are not allowed without explicit user authentication by a custom password. Currently password and salt
are hashed with 5000000 iterations of SHA-512.
Note: This functionality is currently in development and is not ready for daily usage.</source>
        <translation>密码保护可确保在没有明确用户的情况下，不允许进行敏感操作，例如引导非默认操作系统（例如macOS恢复或工具），重置NVRAM存储，尝试引导至非默认模式（例如详细模式或安全模式）。 通过自定义密码进行身份验证。 当前，密码和盐值已通过SHA-512的5000000次迭代进行哈希处理。 注意：此功能当前正在开发中，尚不能用于日常使用。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3015"/>
        <source> Enable password protection to allow sensitive operations.</source>
        <translation>启用密码保护以允许敏感操作。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3018"/>
        <source>EnablePassword</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3233"/>
        <location filename="mainwindow.cpp" line="1260"/>
        <source>BlessOverride</source>
        <translation>BlessOverride（定制引导加载程序）</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3275"/>
        <source>Entries</source>
        <translation>Entries（添加启动项）</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3322"/>
        <source>Tools</source>
        <oldsource>Tools-工具</oldsource>
        <translation>Tools（工具）</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3373"/>
        <location filename="mainwindow.cpp" line="46"/>
        <source>NVRAM</source>
        <translation>NVRAM
变量</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3614"/>
        <source>This file must have root plist dictionary type and contain two fields:
• Version — plist integer, file version, must be set to 1.
• Add — plist dictionary, equivalent to Add from config.plist.
Variable loading happens prior to Delete (and Add) phases. Unless LegacyOverwrite is enabled, it will not
overwrite any existing variable. Variables allowed to be set must be specified in LegacySchema. Third-party
scripts may be used to create nvram.plist file. An example of such script can be found in Utilities. The use of
third-party scripts may require ExposeSensitiveData set to 0x3 to provide boot-path variable with OpenCore
EFI partition UUID.
Warning: This feature is very dangerous as it passes unprotected data to your firmware variable services. Use it
only when no hardware NVRAM implementation is provided by the firmware or it is incompatible.</source>
        <translation>该文件必须具有root plist字典类型，并包含两个字段：
•版本— plist整数（文件版本）必须设置为1。
•添加— plist词典，等效于从config.plist添加。 可变加载发生在删除（和添加）阶段之前。 除非启用LegacyOverwrite，否则它将不会覆盖任何现有变量。 必须在LegacySchema中指定允许设置的变量。 第三方脚本可用于创建nvram.plist文件。 此类脚本的示例可以在“实用工具”中找到。 使用第三方脚本可能需要将ExposeSensitiveData设置为0x3才能为OpenCore EFI分区UUID提供引导路径变量。 警告：此功能非常危险，因为它将不受保护的数据传递到固件可变服务。 仅当固件未提供硬件NVRAM实现或与固件不兼容时才使用它。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3626"/>
        <source>Enables loading of NVRAM variable file named nvram.plist from EFI volume root.</source>
        <translation>允许从EFI卷根目录加载名为nvram.plist的NVRAM变量文件。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3629"/>
        <source>LegacyEnable</source>
        <translation>LegacyEnable（传统BIOS）</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3649"/>
        <source>Note: This value is recommended to be enabled on most firmwares, but is left configurable for firmwares that
may have issues with NVRAM variable storage garbage collection or alike.
To read NVRAM variable value from macOS one could use nvram by concatenating variable GUID and name separated
by : symbol. For example, nvram 7C436110-AB2A-4BBB-A880-FE41995C9F82:boot-args.
A continuously updated variable list can be found in a corresponding document: NVRAM Variables.</source>
        <translation>注意：建议在大多数固件上启用此值，但对于可能因NVRAM变量存储垃圾收集等问题而出现问题的固件，则可对其进行配置。 要从macOS读取NVRAM变量值，可以使用nvram，方法是将变量GUID和名称分隔为：符号。 例如，nvram 7C436110-AB2A-4BBB-A880-FE41995C9F82：boot-args。 可以在相应的文档NVRAM变量中找到不断更新的变量列表。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3656"/>
        <source> Enables writing to flash memory for all added variables.</source>
        <translation>允许为所有添加的变量写入闪存。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3659"/>
        <source>WriteFlash</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3636"/>
        <source>Note: Only variables accessible from the operating system will be overwritten.</source>
        <translation>注意：仅可从操作系统访问的变量将被覆盖。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3639"/>
        <source> Permits overwriting firmware variables from nvram.plist.</source>
        <translation>允许覆盖nvram.plist中的固件变量。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3642"/>
        <source>LegacyOverwrite</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3541"/>
        <source>LegacySchema</source>
        <oldsource>LegacySchema-传统BIOS</oldsource>
        <translation>LegacySchema（传统BIOS）</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="47"/>
        <source>PlatformInfo</source>
        <oldsource>机型配置-PlatformInfo</oldsource>
        <translation>PlatformInfo
机型平台信息</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4946"/>
        <source>These fields are read from Generic or PlatformNVRAM sections depending on Automatic value. All the other
fields are to be specified with NVRAM section.
If UpdateNVRAM is set to false the aforementioned variables can be updated with NVRAM section. If UpdateNVRAM
is set to true the behaviour is undefined when any of the fields are present in NVRAM section.</source>
        <translation>这些字段是从“常规”或“平台NVRAM”部分读取的，具体取决于“自动”值。 所有其他字段都将在NVRAM部分中指定。 如果UpdateNVRAM设置为false，则可以使用NVRAM部分更新上述变量。 如果UpdateNVRAM设置为true，则当NVRAM部分中存在任何字段时，行为是不确定的。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4952"/>
        <source> Update NVRAM fields related to platform information.</source>
        <translation>更新与平台信息有关的NVRAM字段。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4955"/>
        <source>UpdateNVRAM</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4931"/>
        <source>Update Data Hub fields. These fields are read from Generic or DataHub sections depending on
Automatic value.</source>
        <translation>更新数据中心字段。 这些字段是根据“自动”值从“通用”或“ DataHub”部分读取的。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4935"/>
        <source>Update Data Hub fields. These fields are read from Generic or DataHub sections depending on
\nAutomatic value.</source>
        <translation>更新数据中心字段。 这些字段是根据\ nAutomatic值从Generic或DataHub部分读取的。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4939"/>
        <source>UpdateDataHub</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4962"/>
        <source>Update SMBIOS fields. These fields are read from Generic or SMBIOS sections depending on
Automatic value.</source>
        <translation>更新SMBIOS字段。 这些字段是从“常规”或“ SMBIOS”部分读取的，具体取决于“自动”值。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4966"/>
        <source>Update SMBIOS fields. These fields are read from Generic or SMBIOS sections depending on
 Automatic value.</source>
        <translation>更新SMBIOS字段。 这些字段是从“常规”或“ SMBIOS”部分读取的，具体取决于“自动”值。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4970"/>
        <source>UpdateSMBIOS</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4977"/>
        <source>• TryOverwrite — Overwrite if new size is &lt;= than the page-aligned original and there are no issues with
legacy region unlock. Create otherwise. Has issues with some firmwares.
• Create — Replace the tables with newly allocated EfiReservedMemoryType at AllocateMaxAddress without
any fallbacks.
• Overwrite — Overwrite existing gEfiSmbiosTableGuid and gEfiSmbiosTable3Guid data if it fits new size.
Abort with unspecified state otherwise.
• Custom — Write SMBIOS tables (gEfiSmbios(3)TableGuid) to gOcCustomSmbios(3)TableGuid to workaround
firmwares overwriting SMBIOS contents at ExitBootServices. Otherwise equivalent to Create. Requires
patching AppleSmbios.kext and AppleACPIPlatform.kext to read from another GUID: &quot;EB9D2D31&quot; -
&quot;EB9D2D35&quot; (in ASCII), done automatically by CustomSMBIOSGuid quirk.
Note: A side effect of using Custom approach is making SMBIOS updates exclusive to macOS, avoiding a collission
with existing Windows activation and custom OEM software but potentially breaking Apple-specific tools.</source>
        <translation>•TryOverwrite-如果新大小小于对齐页面的原始大小，并且旧版区域解锁没有问题，则覆盖。否则创建。某些固件有问题。
•创建-使用在AllocateMaxAddress处新分配的EfiReservedMemoryType替换表，而没有任何后备。
•覆盖-如果现有gEfiSmbiosTableGuid和gEfiSmbiosTable3Guid数据适合新大小，则将其覆盖。否则以未指定状态中止。
•自定义-将SMBIOS表（gEfiSmbios（3）TableGuid）写入gOcCustomSmbios（3）TableGuid，以解决固件在ExitBootServices覆盖SMBIOS内容的问题。否则等效于创建。需要修补AppleSmbios.kext和AppleACPIPlatform.kext以便从另一个GUID读取：“ EB9D2D31”-“ EB9D2D35”（ASCII），由CustomSMBIOSGuid选项自动完成。注意：使用自定义方法的副作用是使SMBIOS更新仅适用于macOS，从而避免了与现有Windows激活和自定义OEM软件的冲突，但有可能破坏Apple专用工具。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1675"/>
        <source>Note: This option is required on virtual machines when using SecureBootModel different from x86legacy.</source>
        <translation>注意：当使用与x86legacy不同的SecureBootModel时，在虚拟机上需要此选项。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1678"/>
        <source>Force x86 scheme for IMG4 verification.</source>
        <translation>强制使用x86方案进行IMG4验证。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1681"/>
        <source>ForceSecureBootScheme</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2069"/>
        <source>EDK II debug level bitmask (sum) showed onscreen. Unless Target enables console (onscreen)  printing, onscreen debug output will not be visible.</source>
        <translation>屏幕上显示了EDK II调试级别位掩码（总和）。 除非Target启用控制台（屏幕上）打印，否则屏幕上的调试输出将不可见。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2094"/>
        <source>Select all</source>
        <translation>全选</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2101"/>
        <source>Clear</source>
        <translation>清除</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2121"/>
        <source>DEBUG_INIT</source>
        <translation>DEBUG_INIT</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2141"/>
        <source>DEBUG_PAGE</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2161"/>
        <source>DEBUG_BLKIO</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2181"/>
        <source>DEBUG_GCD</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2188"/>
        <source>DEBUG_WARN</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2208"/>
        <source>DEBUG_INFO</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2228"/>
        <source>DEBUG_NET</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2248"/>
        <source>DEBUG_CACHE</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2255"/>
        <source>DEBUG_LOAD</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2275"/>
        <source>DEBUG_DISPATCH</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2295"/>
        <source>DEBUG_UNDI</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2315"/>
        <source>DEBUG_VERBOSE</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2322"/>
        <source>DEBUG_FS</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2342"/>
        <source>DEBUG_VARIABLE</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2362"/>
        <source>DEBUG_LOADFILE</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2382"/>
        <source>DEBUG_ERROR</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2389"/>
        <source>DEBUG_POOL</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2409"/>
        <source>DEBUG_BM</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2429"/>
        <source>DEBUG_EVENT</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2623"/>
        <source>Expose printable booter path as an UEFI variable</source>
        <translation>将可打印的引导程序路径公开为UEFI变量</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2630"/>
        <source>Expose OpenCore version as an UEFI variable</source>
        <translation>将OpenCore版本公开为UEFI变量</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2637"/>
        <source>Expose OpenCore version in boot picker menu title</source>
        <translation>在引导选择器菜单标题中公开OpenCore版本</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2644"/>
        <source>Expose OEM information as a set of UEFI variables</source>
        <translation>将OEM信息公开为一组UEFI变量</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2686"/>
        <source>OC_SCAN_FILE_SYSTEM_LOCK</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2706"/>
        <source>OC_SCAN_ALLOW_FS_ESP</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2726"/>
        <source>OC_SCAN_ALLOW_DEVICE_SASEX</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2746"/>
        <source>OC_SCAN_ALLOW_DEVICE_USB</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2753"/>
        <source>OC_SCAN_DEVICE_LOCK</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2773"/>
        <source>OC_SCAN_ALLOW_FS_NTFS</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2793"/>
        <source>OC_SCAN_ALLOW_DEVICE_SCSI</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2813"/>
        <source>OC_SCAN_ALLOW_DEVICE_FIREWIRE</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2820"/>
        <source>OC_SCAN_ALLOW_FS_APFS</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2840"/>
        <source>OC_SCAN_ALLOW_FS_EXT</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2860"/>
        <source>OC_SCAN_ALLOW_DEVICE_NVME</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2880"/>
        <source>OC_SCAN_ALLOW_DEVICE_SDCARD</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2887"/>
        <source>OC_SCAN_ALLOW_FS_HFS</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2907"/>
        <source>OC_SCAN_ALLOW_DEVICE_SATA</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2927"/>
        <source>OC_SCAN_ALLOW_DEVICE_ATAPI</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2947"/>
        <source>OC_SCAN_ALLOW_DEVICE_PCI</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3025"/>
        <source>Note: This option exists due to some operating systems, namely macOS Big Sur, being incapable of disabling
firmware updates with the NVRAM variable (run-efi-updater).</source>
        <translation>注意：由于某些操作系统，即macOS Big Sur，无法使用NVRAM变量（run-efi-updater）禁用固件更新，因此存在此选项。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3029"/>
        <source>Ignore boot options trying to update Apple peripheral firmware (e.g. MultiUpdater.efi).</source>
        <translation>忽略尝试更新Apple外围设备固件的引导选项（例如MultiUpdater.efi）。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3032"/>
        <source>BlacklistAppleUpdate</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4991"/>
        <source> Update SMBIOS fields approach.</source>
        <translation>更新SMBIOS字段的方法。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4994"/>
        <source>UpdateSMBIOSMode</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4897"/>
        <source>Enabling this option is useful when Generic section is flexible enough:
• When enabled SMBIOS, DataHub, and PlatformNVRAM data is unused.
• When disabled Generic section is unused.
Warning: It is strongly discouraged set this option to false when intending to update platform information.
The only reason to do that is when doing minor correction of the SMBIOS present and alike. In all other cases
not using Automatic may lead to hard to debug errors.</source>
        <translation>当通用部分足够灵活时，启用此选项很有用：
•启用后，将不使用SMBIOS，DataHub和PlatformNVRAM数据。
•禁用时，通用部分未使用。
警告：强烈建议在打算更新平台信息时将此选项设置为false。 这样做的唯一原因是对存在的SMBIOS等进行较小的校正时。 在所有其他情况下，不使用“自动”可能会导致难以调试的错误。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4905"/>
        <source>Generate PlatformInfo based on Generic section instead of using values from DataHub, NVRAM,
 and SMBIOS sections.</source>
        <translation>根据通用部分而不是使用DataHub，NVRAM和SMBIOS部分的值生成PlatformInfo。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4909"/>
        <source>Automatic</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4916"/>
        <source>Use custom memory configuration defined in the Memory section. This completely replaces any
existing memory configuration in SMBIOS, and is only active when UpdateSMBIOS is set to true.</source>
        <translation>使用“内存”部分中定义的自定义内存配置。 这将完全替换SMBIOS中的任何现有内存配置，并且仅在UpdateSMBIOS设置为true时才处于活动状态。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4920"/>
        <source>Use custom memory configuration defined in the Memory section. This completely replaces any
 existing memory configuration in SMBIOS, and is only active when UpdateSMBIOS is set to true.</source>
        <translation>使用“内存”部分中定义的自定义内存配置。 这将完全替换SMBIOS中的任何现有内存配置，并且仅在UpdateSMBIOS设置为true时才处于活动状态。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4924"/>
        <source>CustomMemory</source>
        <translation>CustomMemory</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3686"/>
        <source>Generic</source>
        <translation>Generic（通用）</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="834"/>
        <source>Relocation block is a scratch buffer allocated in lower 4 GB to be used for loading the kernel and related structures
by EfiBoot on firmwares where lower memory is otherwise occupied by the (assumed to be) non-runtime data.
Right before kernel startup the relocation block is copied back to lower addresses. Similarly all the other addresses
pointing to relocation block are also carefully adjusted. Relocation block can be used when:
• No better slide exists (all the memory is used)
• slide=0 is forced (by an argument or safe mode)
• KASLR (slide) is unsupported (this is macOS 10.7 or older)
This quirk requires ProvideCustomSlide to also be enabled and generally needs AvoidRuntimeDefrag to work
correctly. Hibernation is not supported when booting with a relocation block (but relocation block is not always
used when the quirk is enabled).
Note: While this quirk is required to run older macOS versions on platforms with used lower memory it is not
compatible with some hardware and macOS 11. In this case you may try to use EnableSafeModeSlide instead.</source>
        <translation>重定位块是分配给较低的4 GB的暂存缓冲区，用于由EfiBoot在固件上加载内核和相关结构，在该固件上，较低的内存被（假设是）非运行时数据占用。在内核启动之前，重定位块被复制回较低的地址。同样，所有其他指向重定位块的地址也都经过仔细调整。在以下情况下可以使用重定位块：
•没有更好的幻灯片（已使用所有内存）
•强制Slide = 0（通过自变量或安全模式）
•不支持KASLR（幻灯片）（这是macOS 10.7或更旧的版本）
此选项需要同时启用ProvideCustomSlide，并且通常需要避免RuntimeDefrag才能正常工作。使用重定位块引导时不支持休眠模式（但是启用了此功能时，并不总是使用重定位块）。

注意：虽然在具有较低内存的平台上运行旧版macOS版本需要执行此操作，但它与某些硬件和macOS 11不兼容。在这种情况下，您可以尝试使用EnableSafeModeSlide。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="848"/>
        <source>Allows booting macOS through a relocation block.</source>
        <translation>允许通过重定位块引导macOS。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="851"/>
        <source>AllowRelocationBlock</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1823"/>
        <source>OC_ATTR_USE_VOLUME_ICON</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1830"/>
        <source>OC_ATTR_USE_DISK_LABEL_FILE</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1837"/>
        <source>OC_ATTR_USE_GENERIC_LABEL_IMAGE</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1844"/>
        <source>OC_ATTR_USE_ALTERNATE_ICONS</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3692"/>
        <location filename="mainwindow.ui" line="3695"/>
        <source>Refer to SMBIOS SystemProductName.</source>
        <translation>请参考SMBIOS SystemProductName。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3698"/>
        <location filename="mainwindow.ui" line="4068"/>
        <location filename="mainwindow.ui" line="4717"/>
        <source>SystemProductName</source>
        <oldsource>SystemProductName-机型</oldsource>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3708"/>
        <location filename="mainwindow.ui" line="3763"/>
        <source>Generate</source>
        <oldsource>生成-Generate</oldsource>
        <translation>生成</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3715"/>
        <location filename="mainwindow.ui" line="3718"/>
        <source> Refer to SMBIOS SystemSerialNumber.</source>
        <translation>请参考SMBIOS SystemSerialNumber。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3721"/>
        <location filename="mainwindow.ui" line="4086"/>
        <location filename="mainwindow.ui" line="4781"/>
        <source>SystemSerialNumber</source>
        <oldsource>SystemSerialNumber-序列号</oldsource>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3731"/>
        <location filename="mainwindow.ui" line="3734"/>
        <source>Refer to SMBIOS BoardSerialNumber.</source>
        <translation>请参考SMBIOS BoardSerialNumber。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3737"/>
        <location filename="mainwindow.ui" line="4366"/>
        <source>MLB</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3747"/>
        <location filename="mainwindow.ui" line="3750"/>
        <source>Refer to SMBIOS SystemUUID.</source>
        <translation>请参阅SMBIOS SystemUUID。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3753"/>
        <location filename="mainwindow.ui" line="4102"/>
        <location filename="mainwindow.ui" line="4398"/>
        <location filename="mainwindow.ui" line="4815"/>
        <source>SystemUUID</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3770"/>
        <source>Valid values:
• Auto — use the original PlatformFeature value.
• Upgradable — explicitly unset PT_FEATURE_HAS_SOLDERED_SYSTEM_MEMORY (0x2) in PlatformFeature. 
• Soldered — explicitly set PT_FEATURE_HAS_SOLDERED_SYSTEM_MEMORY (0x2) in PlatformFeature.

Note: On certain Mac models (namely MacBookPro10,x and any MacBookAir), SPMemoryReporter.spreporter
will ignore PT_FEATURE_HAS_SOLDERED_SYSTEM_MEMORY and assume that system memory is non-upgradable.</source>
        <translation>有效值：
•Auto-使用原始的PlatformFeature值。
•Upgradable—在PlatformFeature中明确未设置PT_FEATURE_HAS_SOLDERED_SYSTEM_MEMORY（0x2）。
•Soldered-在PlatformFeature中显式设置PT_FEATURE_HAS_SOLDERED_SYSTEM_MEMORY（0x2）。

注意：在某些Mac型号（即MacBookPro10，x和任何MacBookAir）上，SPMemoryReporter.spreporter
将忽略PT_FEATURE_HAS_SOLDERED_SYSTEM_MEMORY，并假定系统内存不可升级。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3779"/>
        <source> Indicates whether system memory is upgradable in PlatformFeature. This controls the visibility
 of the Memory tab in About This Mac.</source>
        <translation>指示系统内存是否可以在PlatformFeature中升级。 这可以控制“关于本机”中“内存”选项卡的可见性。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3783"/>
        <source>SystemMemoryStatus</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3809"/>
        <location filename="mainwindow.ui" line="3812"/>
        <source>Refer to 4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14:ROM.</source>
        <translation>请参阅4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14：ROM。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3815"/>
        <location filename="mainwindow.ui" line="4384"/>
        <source>ROM</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3825"/>
        <source>Added bits to FirmwareFeatures: • FW_FEATURE_SUPPORTS_CSM_LEGACY_MODE (0x1) - Without this bit it is not possible to reboot to Windows
installed on a drive with EFI partition being not the first partition on the disk.
• FW_FEATURE_SUPPORTS_UEFI_WINDOWS_BOOT (0x20000000) - Without this bit it is not possible to reboot to
Windows installed on a drive with EFI partition being the first partition on the disk.</source>
        <translation>向固件功能添加了一些功能：
•FW_FEATURE_SUPPORTS_CSM_LEGACY_MODE（0x1）-如果没有此位，则无法重新启动到安装在驱动器上的Windows，而EFI分区不是磁盘上的第一个分区。
•FW_FEATURE_SUPPORTS_UEFI_WINDOWS_BOOT（0x20000000）-如果没有此位，则无法重新启动到以EFI分区为磁盘第一个分区的驱动器上安装的Windows。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3831"/>
        <source>Forces Windows support in FirmwareFeatures.</source>
        <translation>在FirmwareFeatures中强制Windows支持。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3834"/>
        <source>AdviseWindows</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3841"/>
        <source>It is dangerous to use Apple in SMBIOS vendor fields for reasons given in SystemManufacturer description.
However, certain firmwares may not provide valid values otherwise, which could break some software.</source>
        <translation>由于SystemManufacturer说明中给出的原因，在SMBIOS供应商字段中使用Apple是危险的。 但是，某些固件可能无法提供有效值，否则可能会破坏某些软件。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3845"/>
        <source>Sets SMBIOS vendor fields to Acidanthera.</source>
        <translation>将SMBIOS供应商字段设置为Acidanthera。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3848"/>
        <source>SpoofVendor</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3856"/>
        <location filename="mainwindow.ui" line="5887"/>
        <source>DataHub</source>
        <translation>DataHub（数据Hub）</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3862"/>
        <source>This value contains CPU ART frequency, also known as crystal clock frequency. Its existence is exclusive to
Skylake generation and newer. The value is specified in Hz, and is normally 24 MHz for client Intel segment, 25
MHz for server Intel segment, and 19.2 MHz for Intel Atom CPUs. macOS till 10.15 inclusive assumes 24 MHz
by default.
Note: On Intel Skylake X ART frequency may be a little less (approx. 0.25%) than 24 or 25 MHz due to special
EMI-reduction circuit as described in Acidanthera Bugtracker.</source>
        <translation>该值包含CPU ART频率，也称为晶体时钟频率。 它的存在是Skylake或更高版本独有的。 该值以Hz为单位指定，对于客户端Intel网段通常为24 MHz，对于服务器Intel网段通常为25 MHz，对于Intel Atom CPU为19.2 MHz。 直到10.15（含10.15）的macOS默认情况下均假定为24 MHz。
注意：在英特尔Skylake X ART频率上，由于使用Acidanthera Bugtracker中所述的特殊EMI降低电路，其频率可能比24或25 MHz小（约0.25％）。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3870"/>
        <source>Sets ARTFrequency in gEfiProcessorSubClassGuid.</source>
        <translation>在gEfiProcessorSubClassGuid中设置ARTFrequency。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3873"/>
        <source>ARTFrequency</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3883"/>
        <source> Sets board-id in gEfiMiscSubClassGuid. Value found on Macs is equal to SMBIOS BoardProduct
in ASCII.</source>
        <translation>在gEfiMiscSubClassGuid中设置板号。 在Mac上找到的值等于ASCII中的SMBIOS BoardProduct。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3887"/>
        <source> Sets board-id in gEfiMiscSubClassGuid. Value found on Macs is equal to SMBIOS BoardProduct
 in ASCII.</source>
        <translation>在gEfiMiscSubClassGuid中设置板号。 在Mac上找到的值等于ASCII中的SMBIOS BoardProduct。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3891"/>
        <location filename="mainwindow.ui" line="4627"/>
        <source>BoardProduct</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3901"/>
        <source> Sets board-rev in gEfiMiscSubClassGuid. Value found on Macs seems to correspond to internal
board revision (e.g. 01).</source>
        <translation>在gEfiMiscSubClassGuid中设置board-rev。 在Mac上发现的值似乎与内部电路板的版本相对应（例如01）。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3905"/>
        <source> Sets board-rev in gEfiMiscSubClassGuid. Value found on Macs seems to correspond to internal
 board revision (e.g. 01).</source>
        <translation>在gEfiMiscSubClassGuid中设置board-rev。 在Mac上发现的值似乎与内部电路板的版本相对应（例如01）。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3909"/>
        <source>BoardRevision</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3919"/>
        <source> Sets DevicePathsSupported in gEfiMiscSubClassGuid. Must be set to 1 for AppleACPIPlatform.kext to append SATA device paths to Boot#### and efi-boot-device-data variables. Set to 1 on all
modern Macs.</source>
        <translation>设置gEfiMiscSubClassGuid中的DevicePathsSupported。 对于AppleACPIPlatform.kext，必须将其设置为1，以将SATA设备路径附加到Boot ####和efi-boot-device-data变量。 在所有现代Mac上设置为1。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3923"/>
        <source>Type: plist integer, 32-bit</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3926"/>
        <source>DevicePathsSupported</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3936"/>
        <location filename="mainwindow.ui" line="3939"/>
        <source>Sets FSBFrequency in gEfiProcessorSubClassGuid.</source>
        <translation>在gEfiProcessorSubClassGuid中设置FSBFrequency。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3942"/>
        <source>FSBFrequency</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3952"/>
        <location filename="mainwindow.ui" line="3955"/>
        <source> Sets InitialTSC in gEfiProcessorSubClassGuid. Sets initial TSC value, normally 0.</source>
        <translation>在gEfiProcessorSubClassGuid中设置InitialTSC。 设置TSC初始值，通常为0。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3958"/>
        <source>InitialTSC</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3968"/>
        <location filename="mainwindow.ui" line="3971"/>
        <source>Sets name in gEfiMiscSubClassGuid. Value found on Macs is platform in ASCII.</source>
        <translation>在gEfiMiscSubClassGuid中设置名称。 在Mac上找到的值是ASCII平台。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3974"/>
        <source>PlatformName</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3984"/>
        <source> Sets RBr in gEfiMiscSubClassGuid. Custom property read by VirtualSMC or FakeSMC to generate
SMC RBr key.</source>
        <translation>在gEfiMiscSubClassGuid中设置RBr。 由VirtualSMC或FakeSMC读取的自定义属性，以生成SMC RBr密钥。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3988"/>
        <source> Sets RBr in gEfiMiscSubClassGuid. Custom property read by VirtualSMC or FakeSMC to generate
 SMC RBr key.</source>
        <translation>在gEfiMiscSubClassGuid中设置RBr。 由VirtualSMC或FakeSMC读取的自定义属性，以生成SMC RBr密钥。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3992"/>
        <source>SmcBranch</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4002"/>
        <source> Sets RPlt in gEfiMiscSubClassGuid. Custom property read by VirtualSMC or FakeSMC to
generate SMC RPlt key.</source>
        <translation>在gEfiMiscSubClassGuid中设置RPlt。 由VirtualSMC或FakeSMC读取的自定义属性，以生成SMC RPlt密钥。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4006"/>
        <source>Type: plist data, 8 bytes</source>
        <translation>类型：plist数据，8字节</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4009"/>
        <source>SmcPlatform</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4019"/>
        <source> Sets REV in gEfiMiscSubClassGuid. Custom property read by VirtualSMC or FakeSMC to generate
SMC REV key.</source>
        <translation>在gEfiMiscSubClassGuid中设置REV。 由VirtualSMC或FakeSMC读取的自定义属性，以生成SMC REV密钥。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4023"/>
        <source> Sets REV in gEfiMiscSubClassGuid. Custom property read by VirtualSMC or FakeSMC to generate
 SMC REV key.</source>
        <translation>在gEfiMiscSubClassGuid中设置REV。 由VirtualSMC或FakeSMC读取的自定义属性，以生成SMC REV密钥。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4027"/>
        <source>SmcRevision</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4037"/>
        <source>Known bits read by X86PlatformPlugin.kext: • 0x00000001 — Shutdown cause was a PWROK event (Same as GEN_PMCON_2 bit 0)
• 0x00000002 — Shutdown cause was a SYS_PWROK event (Same as GEN_PMCON_2 bit 1)
• 0x00000004 — Shutdown cause was a THRMTRIP# event (Same as GEN_PMCON_2 bit 3)
• 0x00000008 — Rebooted due to a SYS_RESET# event (Same as GEN_PMCON_2 bit 4)
• 0x00000010 — Power Failure (Same as GEN_PMCON_3 bit 1 PWR_FLR) • 0x00000020 — Loss of RTC Well Power (Same as GEN_PMCON_3 bit 2 RTC_PWR_STS) • 0x00000040 — General Reset Status (Same as GEN_PMCON_3 bit 9 GEN_RST_STS) • 0xffffff80 — SUS Well Power Loss (Same as GEN_PMCON_3 bit 14)
• 0x00010000 — Wake cause was a ME Wake event (Same as PRSTS bit 0, ME_WAKE_STS) • 0x00020000 — Cold Reboot was ME Induced event (Same as PRSTS bit 1 ME_HRST_COLD_STS) • 0x00040000 — Warm Reboot was ME Induced event (Same as PRSTS bit 2 ME_HRST_WARM_STS) • 0x00080000 — Shutdown was ME Induced event (Same as PRSTS bit 3 ME_HOST_PWRDN) • 0x00100000 — Global reset ME Wachdog Timer event (Same as PRSTS bit 6)
• 0x00200000 — Global reset PowerManagment Wachdog Timer event (Same as PRSTS bit 15)</source>
        <translation>X86PlatformPlugin.kext读取的已知位：
•0x00000001-关机原因是PWROK事件（与GEN_PMCON_2位0相同）
•0x00000002-关机原因是SYS_PWROK事件（与GEN_PMCON_2位1相同）
•0x00000004-关机原因是THRMTRIP＃事件（与GEN_PMCON_2位3相同）
•0x00000008-由于SYS_RESET＃事件而重新启动（与GEN_PMCON_2位4相同）
•0x00000010-电源故障（与GEN_PMCON_3位1 PWR_FLR相同）
•0x00000020-RTC阱电源丢失（与GEN_PMCON_3的第2位RTC_PWR_STS相同）
•0x00000040-常规复位状态（与GEN_PMCON_3的第9位GEN_RST_STS相同）
•0xffffff80-SUS阱功率损耗（与GEN_PMCON_3位14相同）
•0x00010000 —唤醒原因是ME唤醒事件（与PRSTS位0，ME_WAKE_STS相同）
•0x00020000-冷重启是ME引发的事件（与PRSTS位1 ME_HRST_COLD_STS相同）
•0x00040000-热重启是ME引发的事件（与PRSTS位2 ME_HRST_WARM_STS相同）
•0x00080000-关机是由ME引发的事件（与PRSTS位3 ME_HOST_PWRDN相同）
•0x00100000 —全局重置ME Wachdog Timer事件（与PRSTS的位6相同）
•0x00200000 —全局复位PowerManagment Wachdog Timer事件（与PRSTS位15相同）</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4046"/>
        <source> Sets StartupPowerEvents in gEfiMiscSubClassGuid. Value found on Macs is power management
 state bitmask, normally 0.</source>
        <translation>在gEfiMiscSubClassGuid中设置StartupPowerEvents。 在Mac上找到的值为电源管理状态位掩码，通常为0。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4050"/>
        <source>StartupPowerEvents</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4060"/>
        <source> Sets Model in gEfiMiscSubClassGuid. Value found on Macs is equal to SMBIOS SystemProductName
in Unicode.</source>
        <translation>在gEfiMiscSubClassGuid中设置模型。 在Mac上找到的值等于Unicode中的SMBIOS SystemProductName。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4064"/>
        <source> Sets Model in gEfiMiscSubClassGuid. Value found on Macs is equal to SMBIOS SystemProductName
 in Unicode.</source>
        <translation>在gEfiMiscSubClassGuid中设置模型。 在Mac上找到的值等于Unicode中的SMBIOS SystemProductName。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4078"/>
        <source>Sets SystemSerialNumber in gEfiMiscSubClassGuid. Value found on Macs is equal to SMBIOS
SystemSerialNumber in Unicode.</source>
        <translation>在gEfiMiscSubClassGuid中设置SystemSerialNumber。 在Mac上找到的值等于Unicode中的SMBIOS SystemSerialNumber。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4082"/>
        <source>Sets SystemSerialNumber in gEfiMiscSubClassGuid. Value found on Macs is equal to SMBIOS
 SystemSerialNumber in Unicode.</source>
        <translation>在gEfiMiscSubClassGuid中设置SystemSerialNumber。 在Mac上找到的值等于Unicode中的SMBIOS SystemSerialNumber。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4096"/>
        <location filename="mainwindow.ui" line="4099"/>
        <source>Sets system-id in gEfiMiscSubClassGuid. Value found on Macs is equal to SMBIOS SystemUUID.</source>
        <translation>在gEfiMiscSubClassGuid中设置系统ID。 在Mac上找到的值等于SMBIOS SystemUUID。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4113"/>
        <source>Memory</source>
        <translation>Memory（自定义内存）</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4119"/>
        <source>Specifies the data width, in bits, of the memory. A DataWidth of 0 and a TotalWidth of 8
indicates that the device is being used solely to provide 8 error-correction bits.</source>
        <translation>指定内存的数据宽度（以位为单位）。 DataWidth为0，TotalWidth为8表示该设备仅用于提供8个纠错位。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4123"/>
        <source>Specifies the data width, in bits, of the memory. A DataWidth of 0 and a TotalWidth of 8
 indicates that the device is being used solely to provide 8 error-correction bits.</source>
        <translation>指定内存的数据宽度（以位为单位）。 DataWidth为0，TotalWidth为8表示该设备仅用于提供8个纠错位。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4127"/>
        <source>DataWidth</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4137"/>
        <source> Specifies the primary hardware error correction or detection method supported by the memory.
• 0x01 — Other
• 0x02 — Unknown
• 0x03 — None
• 0x04 — Parity
• 0x05 — Single-bit ECC
• 0x06 — Multi-bit ECC
• 0x07 — CRC</source>
        <translation>指定内存支持的主要硬件错误纠正或检测方法。
•0x01-其他
•0x02-未知
•0x03-无
•0x04 —奇偶校验
•0x05-单位ECC
•0x06-多位ECC
•0x07-CRC</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4147"/>
        <source>Physical Memory Array (Type 16) — Memory Error Correction</source>
        <translation>物理内存阵列（类型16）—内存错误纠正。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4150"/>
        <source>ErrorCorrection</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4160"/>
        <source> Specifies the form factor of the memory. On Macs this should usually be DIMM or SODIMM.
Commonly used form factors are listed below.
When CustomMemory is false, this value is automatically set based on Mac product name.
• 0x01 — Other
• 0x02 — Unknown
• 0x09 — DIMM
• 0x0D — SODIMM
• 0x0F — FB-DIMM</source>
        <translation>指定内存的尺寸。 在Mac上，通常应为DIMM或SODIMM。 下面列出了常用的外形尺寸。 如果CustomMemory为false，则根据Mac产品名称自动设置此值。
•0x01-其他
•0x02-未知
•0x09 — DIMM
•0x0D-SODIMM
•0x0F — FB-DIMM</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4170"/>
        <source>Memory Device (Type 17) — Form Factor</source>
        <translation>存储设备（类型17）—尺寸</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4173"/>
        <source>FormFactor</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4183"/>
        <source>Specifies the maximum amount of memory, in bytes, supported by the system.</source>
        <translation>指定系统支持的最大内存量（以字节为单位）。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4186"/>
        <source> Physical Memory Array (Type 16) — Maximum Capacity</source>
        <translation>物理内存阵列（类型16）—最大容量</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4189"/>
        <source>MaxCapacity</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4199"/>
        <source> Specifies the total width, in bits, of the memory, including any check or error-correction bits. If
there are no error-correction bits, this value should be equal to DataWidth.</source>
        <translation>指定存储器的总宽度（以位为单位），包括所有检查或纠错位。 如果没有纠错位，则此值应等于DataWidth。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4203"/>
        <source>TotalWidth</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4213"/>
        <source>Description: Specifies the memory type. Commonly used types are listed below.
• 0x01 — Other
• 0x02 — Unknown
• 0x0F — SDRAM
• 0x12 — DDR
• 0x13 — DDR2
• 0x14 — DDR2 FB-DIMM
• 0x18 — DDR3
• 0x1A — DDR4
• 0x1B — LPDDR
• 0x1C — LPDDR2
• 0x1D — LPDDR3
• 0x1E — LPDDR4</source>
        <translation>说明：指定内存类型。 下面列出了常用的类型。
•0x01-其他
•0x02-未知
•0x0F — SDRAM
•0x12 — DDR
•0x13 — DDR2
•0x14 — DDR2 FB-DIMM
•0x18 — DDR3
•0x1A — DDR4
•0x1B — LPDDR
•0x1C — LPDDR2
•0x1D — LPDDR3
•0x1E — LPDDR4</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4238"/>
        <source> Specifies additional memory type information.
• Bit 0 — Reserved, set to 0
• Bit 1 — Other
• Bit 2 — Unknown
• Bit 7 — Synchronous
• Bit 13 — Registered (buffered)
• Bit 14 — Unbuffered (unregistered)</source>
        <translation>指定其他内存类型信息。
•位0-保留，设置为0
•位1-其他
•位2-未知
•位7-同步
•位13-已注册（缓冲）
•位14-未缓冲（未注册）</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4247"/>
        <source>TypeDetail</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4300"/>
        <source>PlatformNVRAM</source>
        <translation>PlatformNVRAM（平台NVRAM）</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4306"/>
        <location filename="mainwindow.ui" line="4309"/>
        <source>Specifies the value of NVRAM variable 4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14:HW_BID.</source>
        <translation>指定NVRAM变量4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14：HW_BID的值。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4312"/>
        <source>BID</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4322"/>
        <source>Specifies the values of NVRAM variables:
• 4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14:FirmwareFeatures
• 4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14:ExtendedFirmwareFeatures</source>
        <translation>指定NVRAM变量的值：•4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14：FirmwareFeatures•4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14：ExtendedFirmwareFeatures</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4327"/>
        <source> This variable comes in pair with FirmwareFeaturesMask.</source>
        <translation>该变量与FirmwareFeaturesMask配对使用。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4330"/>
        <location filename="mainwindow.ui" line="4471"/>
        <source>FirmwareFeatures</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4340"/>
        <source> Specifies the values of NVRAM variables:
• 4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14:FirmwareFeaturesMask
• 4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14:ExtendedFirmwareFeaturesMask</source>
        <translation>指定NVRAM变量的值：
•4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14：固件功能蒙版
•4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14：扩展的固件功能蒙版</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4345"/>
        <source> This variable comes in pair with FirmwareFeatures.</source>
        <translation>该变量与FirmwareFeatures一起使用。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4348"/>
        <location filename="mainwindow.ui" line="4509"/>
        <source>FirmwareFeaturesMask</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4358"/>
        <source> Specifies the values of NVRAM variables 4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14:HW_MLB and
4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14:MLB.</source>
        <translation>指定NVRAM变量4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14：HW_MLB和4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14：MLB的值。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4362"/>
        <source> Specifies the values of NVRAM variables 4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14:HW_MLB and
 4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14:MLB.</source>
        <translation>指定NVRAM变量4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14：HW_MLB和4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14：MLB的值。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4376"/>
        <source>Specifies the values of NVRAM variables 4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14:HW_ROM and
4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14:ROM.</source>
        <translation>指定NVRAM变量4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14：HW_ROM和4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14：ROM的值。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4380"/>
        <source>Specifies the values of NVRAM variables 4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14:HW_ROM and
 4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14:ROM.</source>
        <translation>指定NVRAM变量4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14：HW_ROM和4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14：ROM的值。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4394"/>
        <source>Specifies the value of NVRAM variable 4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14:system-id
for boot services only. Value found on Macs is equal to SMBIOS SystemUUID.</source>
        <translation>指定仅用于引导服务的NVRAM变量4D1EDE05-38C7-4A6A-9CC6-4BCCA8B38C14：system-id的值。 在Mac上找到的值等于SMBIOS SystemUUID。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4409"/>
        <source>SMBIOS</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4415"/>
        <location filename="mainwindow.ui" line="4418"/>
        <source> Firmware release date. Similar to BIOSVersion. May look like 12/08/2017.</source>
        <translation>固件发布日期。 类似于BIOSVersion。 可能看起来像12/08/2017。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4421"/>
        <source>BIOSReleaseDate</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4431"/>
        <location filename="mainwindow.ui" line="4434"/>
        <source> Should match BoardProduct.</source>
        <translation>应该匹配BoardProduct。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4437"/>
        <source>ChassisVersion</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4447"/>
        <location filename="mainwindow.ui" line="4450"/>
        <source> BIOS Vendor. All rules of SystemManufacturer do apply.</source>
        <translation>BIOS供应商。 SystemManufacturer的所有规则均适用。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4453"/>
        <source>BIOSVendor</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4463"/>
        <source> 64-bit firmware features bitmask. Refer to AppleFeatures.h for more details. Lower 32 bits match
FirmwareFeatures. Upper 64 bits match ExtendedFirmwareFeatures.</source>
        <translation>64位固件具有位掩码。 有关更多详细信息，请参考AppleFeatures.h。 低32位与FirmwareFeatures相匹配。 高64位与ExtendedFirmwareFeatures相匹配。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4467"/>
        <source> 64-bit firmware features bitmask. Refer to AppleFeatures.h for more details. Lower 32 bits match
 FirmwareFeatures. Upper 64 bits match ExtendedFirmwareFeatures.</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4481"/>
        <source> Firmware version. This value gets updated and takes part in update delivery configuration and
macOS version compatibility. This value could look like MM71.88Z.0234.B00.1809171422 in older firmwares,
and is described in BiosId.h. In newer firmwares it should look like 236.0.0.0.0 or 220.230.16.0.0 (iBridge:
16.16.2542.0.0,0). iBridge version is read from BridgeOSVersion variable, and is only present on macs with
T2.</source>
        <translation>固件版本。 此值将更新，并参与更新交付配置和macOS版本兼容性。 在旧固件中，该值看起来像MM71.88Z.0234.B00.1809171422，并在BiosId.h中进行了描述。 在较新的固件中，它看起来应该像236.0.0.0.0或220.230.16.0.0（iBridge：16.16.2542.0.0,0）。 iBridge版本是从BridgeOSVersion变量读取的，并且仅在具有T2的macs上存在。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4488"/>
        <source>BIOS Information (Type 0) — BIOS Version</source>
        <translation>BIOS信息（类型0）— BIOS版本</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4491"/>
        <source>BIOSVersion</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4501"/>
        <source> Supported bits of extended firmware features bitmask. Refer to AppleFeatures.h for more details.
Lower 32 bits match FirmwareFeaturesMask. Upper 64 bits match ExtendedFirmwareFeaturesMask.</source>
        <translation>扩展固件支持的位具有位掩码。 有关更多详细信息，请参考AppleFeatures.h。 低32位与FirmwareFeaturesMask相匹配。 高64位与ExtendedFirmwareFeaturesMask相匹配。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4505"/>
        <source> Supported bits of extended firmware features bitmask. Refer to AppleFeatures.h for more details.
 Lower 32 bits match FirmwareFeaturesMask. Upper 64 bits match ExtendedFirmwareFeaturesMask.</source>
        <translation>扩展固件支持的位具有位掩码。 有关更多详细信息，请参考AppleFeatures.h。 低32位与FirmwareFeaturesMask相匹配。 高64位与ExtendedFirmwareFeaturesMask相匹配。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4519"/>
        <location filename="mainwindow.ui" line="4522"/>
        <source>Asset tag number. Varies, may be empty or Type2 - Board Asset Tag.</source>
        <translation>资产标签号。 有所不同，可能为空，也可能为Type2-Board资产标签。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4525"/>
        <source>BoardAssetTag</source>
        <translation>BoardAssetTag</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6288"/>
        <source>OC Validate</source>
        <translation type="unfinished">OC验证</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6412"/>
        <source>OpenCore Forum</source>
        <translation>OpenCore讨论区（国际）</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6417"/>
        <source>OpenCore Factory</source>
        <translation>OpenCore工厂</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6422"/>
        <source>OpenCore</source>
        <translation>OpenCore官方</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6427"/>
        <source>Simplified Chinese Manual</source>
        <translation>简体中文参考手册（非官方）</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6432"/>
        <source>Plist editor</source>
        <translation>Plist文件编辑器</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6437"/>
        <source>DSDT&amp;&amp;SSDT editor</source>
        <translation>DSDT&amp;&amp;SSDT编辑器</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6442"/>
        <source>OpenCanopyIcons</source>
        <translation>OC图标合集</translation>
    </message>
    <message>
        <source>Memory form factor. On Macs it should be DIMM or SODIMM.</source>
        <translation type="vanished">内存尺寸。 在Mac上，它应该是DIMM或SODIMM。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4553"/>
        <location filename="mainwindow.ui" line="4556"/>
        <source>Varies, may be empty or Part Component.</source>
        <translation>变量，可能为空或零件零件。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4559"/>
        <source>BoardLocationInChassis</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4535"/>
        <source> Platform features bitmask. Refer to AppleFeatures.h for more details. Use 0xFFFFFFFF value to
not provide this table.</source>
        <translation>平台具有位掩码。 有关更多详细信息，请参考AppleFeatures.h。 使用0xFFFFFFFF值不提供此表。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4539"/>
        <source> Platform features bitmask. Refer to AppleFeatures.h for more details. Use 0xFFFFFFFF value to
 not provide this table.</source>
        <translation>平台具有位掩码。 有关更多详细信息，请参考AppleFeatures.h。 使用0xFFFFFFFF值不提供此表。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4543"/>
        <source>PlatformFeature</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4585"/>
        <location filename="mainwindow.ui" line="4588"/>
        <source> Board manufacturer. All rules of SystemManufacturer do apply.</source>
        <translation>电路板制造商。 SystemManufacturer的所有规则均适用。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4591"/>
        <source>BoardManufacturer</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3793"/>
        <location filename="mainwindow.ui" line="3796"/>
        <location filename="mainwindow.ui" line="4569"/>
        <location filename="mainwindow.ui" line="4572"/>
        <source> Combined of Processor Major and Minor types.</source>
        <translation>处理器主要和次要类型的组合。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6319"/>
        <source>ExportMaster</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6326"/>
        <source>ImportMaster</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="885"/>
        <source>DP</source>
        <translation>DP
设备属性</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1556"/>
        <source>Replaces the default 64-bit commpage bcopy implementation with one that does not require
SSSE3, useful for legacy platforms. This prevents a commpage no match for last panic due to no available
64-bit bcopy functions that do not require SSSE3.</source>
        <translation>用不需要SSSE3的默认64位combpage bcopy实现替换，该实现对旧平台有用。 这可以防止由于没有可用的不需要SSSE3的64位bcopy函数而导致最后一次紧急情况无法匹配的commpage。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1561"/>
        <source>Requirement: 10.4 - 10.6</source>
        <translation>要求：10.4-10.6</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1564"/>
        <source>LegacyCommpage</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1661"/>
        <source>Note: This option is a substitution for BT4LEContinuityFixup.kext, which does not function properly due to late
patching progress.</source>
        <translation>注意：此选项代替了BT4LEContinuityFixup.kext，由于后期修补进度而无法正常运行。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1665"/>
        <source>Set FeatureFlags to 0x0F for full functionality of Bluetooth, including Continuity.</source>
        <translation>将FeatureFlags设置为0x0F，以获取蓝牙的全部功能，包括Continuity。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1668"/>
        <source>ExtendBTFeatureFlags</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1860"/>
        <source>Text renderer supports colour arguments as a sum of foreground and background colours according to UEFI
specification. The value of black background and black foreground (0) is reserved. List of colour names:
• 0x00 — EFI_BLACK
• 0x01 — EFI_BLUE
• 0x02 — EFI_GREEN
• 0x03 — EFI_CYAN
• 0x04 — EFI_RED
• 0x05 — EFI_MAGENTA
• 0x06 — EFI_BROWN
• 0x07 — EFI_LIGHTGRAY
• 0x08 — EFI_DARKGRAY
• 0x09 — EFI_LIGHTBLUE
• 0x0A — EFI_LIGHTGREEN
• 0x0B — EFI_LIGHTCYAN
• 0x0C — EFI_LIGHTRED
• 0x0D — EFI_LIGHTMAGENTA
• 0x0E — EFI_YELLOW
• 0x0F — EFI_WHITE
• 0x00 — EFI_BACKGROUND_BLACK
• 0x10 — EFI_BACKGROUND_BLUE
• 0x20 — EFI_BACKGROUND_GREEN
• 0x30 — EFI_BACKGROUND_CYAN
• 0x40 — EFI_BACKGROUND_RED
• 0x50 — EFI_BACKGROUND_MAGENTA
• 0x60 — EFI_BACKGROUND_BROWN
• 0x70 — EFI_BACKGROUND_LIGHTGRAY
Note: This option may not work well with System text renderer. Setting a background different from black could
help testing proper GOP functioning.</source>
        <translation>根据UEFI，文本渲染器支持颜色参数作为前景色和背景色的总和
规范。黑色背景和黑色前景（0）的值保留。颜色名称列表：
•0x00-EFI_BLACK
•0x01-EFI_BLUE
•0x02-EFI_GREEN
•0x03-EFI_CYAN
•0x04-EFI_RED
•0x05-EFI_MAGENTA
•0x06-EFI_BROWN
•0x07-EFI_LIGHTGRAY
•0x08-EFI_DARKGRAY
•0x09-EFI_LIGHTBLUE
•0x0A — EFI_LIGHTGREEN
•0x0B — EFI_LIGHTCYAN
•0x0C — EFI_LIGHTRED
•0x0D-EFI_LIGHTMAGENTA
•0x0E — EFI_YELLOW
•0x0F — EFI_WHITE
•0x00-EFI_BACKGROUND_BLACK
•0x10-EFI_BACKGROUND_BLUE
•0x20-EFI_BACKGROUND_GREEN
•0x30-EFI_BACKGROUND_CYAN
•0x40-EFI_BACKGROUND_RED
•0x50-EFI_BACKGROUND_MAGENTA
•0x60-EFI_BACKGROUND_BROWN
•0x70-EFI_BACKGROUND_LIGHTGRAY
注意：此选项可能不适用于系统文本渲染器。设置不同于黑色的背景可以帮助测试GOP的正常运行。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3670"/>
        <source>PI</source>
        <translation>PI
机型</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3799"/>
        <location filename="mainwindow.ui" line="4575"/>
        <source>ProcessorType</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4619"/>
        <source>Mac Board ID (board-id). May look like Mac-7BA5B2D9E42DDD94 or Mac-F221BEC8 in older
models.</source>
        <translation>Mac Board ID（board-id）。 在较旧的型号中，可能看起来像Mac-7BA5B2D9E42DDD94或Mac-F221BEC8。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4623"/>
        <source>Mac Board ID (board-id). May look like Mac-7BA5B2D9E42DDD94 or Mac-F221BEC8 in older
 models.</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4601"/>
        <source>ASCII string containing SMC version in upper case. Missing on T2 based Macs. Ignored when
zero.</source>
        <translation>ASCII字符串，包含SMC版本（大写）。 在基于T2的Mac上不被使用。 零时忽略。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4605"/>
        <source>ASCII string containing SMC version in upper case. Missing on T2 based Macs. Ignored when
 zero.</source>
        <translation>ASCII字符串，包含SMC版本（大写）。 在基于T2的Mac上不被使用。 零时忽略。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4609"/>
        <source>SmcVersion</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4653"/>
        <location filename="mainwindow.ui" line="4656"/>
        <source>Board serial number in defined format. Known formats are described in macserial.</source>
        <translation>电路板序列号以定义的格式。 已知格式详见macserial的描述。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4659"/>
        <source>BoardSerialNumber</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4637"/>
        <location filename="mainwindow.ui" line="4640"/>
        <source>Family name. May look like iMac Pro.</source>
        <translation>Family名称。 可能看起来像iMac Pro。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4643"/>
        <source>SystemFamily</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4688"/>
        <source> Either 0xA (Motherboard (includes processor, memory, and I/O) or 0xB (Processor/Memory
Module), refer to Table 15 – Baseboard: Board Type for more details.</source>
        <translation>0xA（主板（包括处理器，内存和I / O））或0xB（处理器/内存模块），请参见表15 –基板：板类型以获取更多详细信息。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4692"/>
        <source> Either 0xA (Motherboard (includes processor, memory, and I/O) or 0xB (Processor/Memory
 Module), refer to Table 15 – Baseboard: Board Type for more details.</source>
        <translation>0xA（主板（包括处理器，内存和I / O））或0xB（处理器/内存模块），请参见表15 –基板：板类型以获取更多详细信息。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4696"/>
        <source>BoardType</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4669"/>
        <source> OEM manufacturer of the particular board. Shall not be specified unless strictly required. Should
not contain Apple Inc., as this confuses numerous services present in the operating system, such as firmware
updates, eficheck, as well as kernel extensions developed in Acidanthera, such as Lilu and its plugins. In addition
it will also make some operating systems like Linux unbootable.</source>
        <translation>特定板的OEM制造商。 除非严格要求，否则不得指定。 不应包含Apple Inc.，因为这会混淆操作系统中存在的许多服务，例如固件更新，eficheck以及在Acidanthera中开发的内核扩展，例如Lilu及其插件。 此外，它还会使某些操作系统（如Linux）无法启动。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4675"/>
        <source>SMBIOS: System Information (Type 1) — Manufacturer</source>
        <translation>SMBIOS：系统信息（类型1）—制造商</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4678"/>
        <source>SystemManufacturer</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4727"/>
        <location filename="mainwindow.ui" line="4730"/>
        <source>Board version number. Varies, may match SystemProductName or SystemProductVersion.</source>
        <translation>单板版本号。 不同，可能与SystemProductName或SystemProductVersion相匹配。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4733"/>
        <source>BoardVersion</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4706"/>
        <source> Preferred Mac model used to mark the device as supported by the operating system. This value
must be specified by any configuration for later automatic generation of the related values in this and other
SMBIOS tables and related configuration parameters. If SystemProductName is not compatible with the target
operating system, -no_compat_check boot argument may be used as an override.
Note: If SystemProductName is unknown, and related fields are unspecified, default values should be assumed as
being set to MacPro6,1 data. The list of known products can be found in AppleModels.</source>
        <translation>用于将设备标记为操作系统支持的首选Mac型号。 该值必须由任何配置指定，以便以后自动生成此表和其他SMBIOS表中的相关值以及相关的配置参数。 如果SystemProductName与目标操作系统不兼容，则-no_compat_check引导参数可以用作替代。 注意：如果SystemProductName是未知的，并且未指定相关字段，则应假定默认值已设置为MacPro6,1数据。 可以在AppleModels中找到已知产品的列表。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4714"/>
        <source>SMBIOS: System Information (Type 1), Product Name</source>
        <translation>SMBIOS：系统信息（类型1），产品名称</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4759"/>
        <location filename="mainwindow.ui" line="4762"/>
        <source>Chassis type name. Varies, could be empty or MacBook-Aluminum.</source>
        <translation>机箱类型名称。 不同，可能为空或为MacBook-Aluminum。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4765"/>
        <source>ChassisAssetTag</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4743"/>
        <location filename="mainwindow.ui" line="4746"/>
        <source>Mac Board ID (board-id). May look like Mac-7BA5B2D9E42DDD94 or Mac-F221BEC8 in older models. Sometimes it can be just empty.</source>
        <translation>Mac Board ID（board-id）。 在较旧的型号中，可能看起来像Mac-7BA5B2D9E42DDD94或Mac-F221BEC8。 有时它可能是空的。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4749"/>
        <source>SystemSKUNumber</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4791"/>
        <location filename="mainwindow.ui" line="4794"/>
        <source>Board manufacturer. All rules of SystemManufacturer do apply.</source>
        <translation>电路板制造商。 SystemManufacturer的所有规则均适用。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4797"/>
        <source>ChassisManufacturer</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4775"/>
        <location filename="mainwindow.ui" line="4778"/>
        <source> Product serial number in defined format. Known formats are described in macserial.</source>
        <translation>定义格式的产品序列号。 已知格式以macserial描述。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4825"/>
        <location filename="mainwindow.ui" line="4828"/>
        <source> Should match SystemSerialNumber.</source>
        <translation>应该匹配SystemSerialNumber。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4831"/>
        <source>ChassisSerialNumber</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4807"/>
        <source> A UUID is an identifier that is designed to be unique across both time and space. It requires no
central registration process.</source>
        <translation>UUID是被设计为在时间和空间上都是唯一的标识符。 它不需要申请或注册过程。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4811"/>
        <source> A UUID is an identifier that is designed to be unique across both time and space. It requires no
 central registration process.</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4857"/>
        <location filename="mainwindow.ui" line="4860"/>
        <source> Chassis type, refer to Table 17 — System Enclosure or Chassis Types for more details.</source>
        <translation>机箱类型，请参阅表17 —系统机箱或机箱类型以了解更多详细信息。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4863"/>
        <source>ChassisType</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4841"/>
        <location filename="mainwindow.ui" line="4844"/>
        <source> Product iteration version number. May look like 1.1.</source>
        <translation>产品迭代版本号。 可能看起来像1.1。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4847"/>
        <source>SystemVersion</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4874"/>
        <source>SystemInfo</source>
        <oldsource>系统信息-SystemInfo</oldsource>
        <translation>SystemInfo（系统信息）</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5030"/>
        <source>APFS EFI driver is bundled in all bootable APFS containers. This option performs loading of signed APFS
drivers with respect to ScanPolicy. See more details in “EFI Jumpstart” section of Apple File System Reference.</source>
        <translation>APFS EFI驱动程序捆绑在所有可启动的APFS容器中。 此选项执行与ScanPolicy相关的已签名APFS驱动程序的加载。 在Apple文件系统参考的“ EFI Jumpstart”部分中查看更多详细信息。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5034"/>
        <source> Load embedded APFS drivers from APFS containers.</source>
        <translation>从APFS容器加载嵌入式APFS驱动程序。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5037"/>
        <source>EnableJumpstart</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5044"/>
        <source>APFS verbose output can be useful for debugging.</source>
        <translation>APFS详细输出对调试很有用。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5047"/>
        <source>Hide verbose output from APFS driver.</source>
        <translation>隐藏APFS驱动程序的详细输出。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5050"/>
        <source>HideVerbose</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5057"/>
        <source>Performs APFS driver loading not only at OpenCore startup but also during boot picker. This permits APFS
USB hot plug. Disable if not required.</source>
        <translation>不仅在OpenCore启动时而且在启动选择器期间执行APFS驱动程序加载。 这允许APFS USB热插拔。 如果不需要，请禁用。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5061"/>
        <source> Load APFS drivers for newly connected devices.</source>
        <translation>为新连接的设备加载APFS驱动程序。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5064"/>
        <source>JumpstartHotPlug</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5071"/>
        <source>Instead of partition handle connection normally used for APFS driver loading every handle is connected recursively.
This may take more time than usual but can be the only way to access APFS partitions on some firmwares like
those found on older HP laptops.</source>
        <translation>代替通常用于APFS驱动程序加载的分区句柄连接，每个句柄都是递归连接的。 这可能比平时花费更多的时间，但可能是访问某些固件（例如旧式HP笔记本电脑上的固件）上的APFS分区的唯一方法。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5076"/>
        <source> Perform full device connection during APFS loading.</source>
        <translation>在APFS加载期间执行完整的设备连接。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5079"/>
        <source>GlobalConnect</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5086"/>
        <source>APFS driver date connects APFS driver with the calendar release date. Older versions of APFS drivers may
contain unpatched vulnerabilities, which can be used to inflict harm on your computer. This option permits
restricting APFS drivers to only recent releases.
• 0 — require the default supported release date of APFS in OpenCore. The default release date will increase
with time and thus this setting is recommended. Currently set to 2018/06/21.
• -1 — permit any release date to load (strongly discouraged).
• Other — use custom minimal APFS release date, e.g. 20200401 for 2020/04/01. APFS release dates can be
found in OpenCore boot log and OcApfsLib.</source>
        <translation>APFS驱动程序日期将APFS驱动程序与日历发布日期连接起来。 较旧版本的APFS驱动程序可能包含未修补的漏洞，可用来对您的计算机造成伤害。 此选项允许将APFS驱动程序限制为仅最新版本。
•0-需要OpenCore中默认支持的APFS发行日期。 默认发行日期将随着时间增加，因此建议使用此设置。 当前设置为2018/06/21
•-1-允许加载任何发行日期（强烈建议）。
•其他-使用自定义的最低APFS发行日期，例如 2020/04/01为20200401。 APFS的发布日期可以在OpenCore引导日志和OcApfsLib中找到。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5096"/>
        <source>Minimal allowed APFS driver date.</source>
        <translation>允许的最小APFS驱动程序日期。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5099"/>
        <source>MinDate</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5109"/>
        <source>APFS driver version connects APFS driver with the macOS release. APFS drivers from older macOS releases will
become unsupported and thus may contain unpatched vulnerabilities, which can be used to inflict harm on your
computer. This option permits restricting APFS drivers to only modern macOS versions.
• 0 — require the default supported version of APFS in OpenCore. The default version will increase with
time and thus this setting is recommended. Currently set to the latest point release from High Sierra from
App Store (748077008000000).
• -1 — permit any version to load (strongly discouraged).
• Other — use custom minimal APFS version, e.g. 1412101001000000 from macOS Catalina 10.15.4. APFS
versions can be found in OpenCore boot log and OcApfsLib.</source>
        <translation>APFS驱动程序版本将APFS驱动程序与macOS版本连接。 较早的macOS版本的APFS驱动程序将不再受支持，因此可能包含未修补的漏洞，这些漏洞可用于对计算机造成伤害。 此选项允许将APFS驱动程序限制为仅适用于现代macOS版本。
•0-需要OpenCore中默认支持的APFS版本。 默认版本会随着时间增加，因此建议使用此设置。 当前设置为App Store中High Sierra的最新版本（748077008000000）。
•-1-允许加载任何版本（强烈建议不要这样做）。 
•其他-使用自定义的最低APFS版本，例如 来自macOS Catalina 10.15.4的1412101001000000。 可以在OpenCore引导日志和OcApfsLib中找到APFS版本。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5120"/>
        <source>Minimal allowed APFS driver version.</source>
        <translation>允许的最低APFS驱动程序版本。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5123"/>
        <source>MinVersion</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5134"/>
        <source>Audio</source>
        <oldsource>Audio设置-Audio</oldsource>
        <translation>Audio（音频）</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5140"/>
        <source>Normally this contains first audio codec address on the builtin analog audio controller (HDEF). Audio codec
addresses, e.g. 2, can be found in the debug log (marked in bold-italic):
OCAU: 1/3 PciRoot(0x0)/Pci(0x1,0x0)/Pci(0x0,0x1)/VenMsg(&lt;redacted&gt;,00000000) (4 outputs)
OCAU: 2/3 PciRoot(0x0)/Pci(0x3,0x0)/VenMsg(&lt;redacted&gt;,00000000) (1 outputs)
OCAU: 3/3 PciRoot(0x0)/Pci(0x1B,0x0)/VenMsg(&lt;redacted&gt;,02000000) (7 outputs)
As an alternative this value can be obtained from IOHDACodecDevice class in I/O Registry containing it in
IOHDACodecAddress field.</source>
        <translation>通常，它包含内置模拟音频控制器（HDEF）上的第一个音频编解码器地址。 音频编解码器地址，例如 2，可以在调试日志中找到（以粗斜体标出）：
OCAU：1/3 PciRoot（0x0）/ Pci（0x1,0x0）/ Pci（0x0,0x1）/ VenMsg（&lt;已编辑&gt;，00000000）（4个输出）
OCAU：2/3 PciRoot（0x0）/ Pci（0x3,0x0）/ VenMsg（&lt;已编辑&gt;，00000000）（1个输出）
OCAU：3/3 PciRoot（0x0）/ Pci（0x1B，0x0）/ VenMsg（&lt;已编辑&gt; ， 02000000）（7个输出）作为替代，可以从I / O注册表中的IOHDACodecDevice类获取此值，该值包含在IOHDACodecAddress字段中 。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5149"/>
        <source>Codec address on the specified audio controller for audio support.</source>
        <translation>用于音频支持的指定音频控制器上的编解码器地址。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5152"/>
        <source>AudioCodec</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5162"/>
        <source>Normally this contains builtin analog audio controller (HDEF) device path, e.g. PciRoot(0x0)/Pci(0x1b,0x0).
The list of recognised audio controllers can be found in the debug log (marked in bold-italic):
OCAU: 1/3 PciRoot(0x0)/Pci(0x1,0x0)/Pci(0x0,0x1)/VenMsg(&lt;redacted&gt;,00000000) (4 outputs)
OCAU: 2/3 PciRoot(0x0)/Pci(0x3,0x0)/VenMsg(&lt;redacted&gt;,00000000) (1 outputs)
OCAU: 3/3 PciRoot(0x0)/Pci(0x1B,0x0)/VenMsg(&lt;redacted&gt;,02000000) (7 outputs)
As an alternative gfxutil -f HDEF command can be used in macOS. Specifying empty device path will result in
the first available audio controller to be used.</source>
        <translation>通常，它包含内置的模拟音频控制器（HDEF）设备路径，例如 PciRoot（0x0）/ Pci（0x1b，0x0）。 可以在调试日志中找到已识别的音频控制器的列表（以粗斜体标记）：
OCAU：1/3 PciRoot（0x0）/ Pci（0x1,0x0）/ Pci（0x0,0x1）/ VenMsg（&lt;已编辑&gt;，00000000）（4个输出）
OCAU：2/3 PciRoot（0x0）/ Pci（0x3,0x0）/ VenMsg（&lt;已编辑&gt;，00000000）（1个输出）
OCAU：3/3 PciRoot（0x0）/ Pci（0x1B，0x0）/ VenMsg（&lt;已编辑&gt;，02000000）（7个输出）作为替代，gfxutil -f HDEF命令可在macOS中使用。 指定空的设备路径将导致使用第一个可用的音频控制器。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5171"/>
        <source>Device path of the specified audio controller for audio support.</source>
        <translation>用于音频支持的指定音频控制器的设备路径。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5174"/>
        <source>AudioDevice</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5184"/>
        <source>Normally this contains the index of the green out of the builtin analog audio controller (HDEF). The number of
output nodes (N) in the debug log (marked in bold-italic):
OCAU: 1/3 PciRoot(0x0)/Pci(0x1,0x0)/Pci(0x0,0x1)/VenMsg(&lt;redacted&gt;,00000000) (4 outputs)
OCAU: 2/3 PciRoot(0x0)/Pci(0x3,0x0)/VenMsg(&lt;redacted&gt;,00000000) (1 outputs)
OCAU: 3/3 PciRoot(0x0)/Pci(0x1B,0x0)/VenMsg(&lt;redacted&gt;,02000000) (7 outputs)
The quickest way to find the right port is to bruteforce the values from 0 to N - 1.</source>
        <translation>通常，它包含内置模拟音频控制器（HDEF）中绿色的索引。 调试日志中的输出节点数（N）（以粗斜体表示）：
OCAU：1/3 PciRoot（0x0）/ Pci（0x1,0x0）/ Pci（0x0,0x1）/ VenMsg（&lt;已编辑&gt;，00000000）（4个输出）
OCAU：2/3 PciRoot（0x0）/ Pci（0x3,0x0）/ VenMsg（&lt;已编辑&gt;，00000000）（1个输出）
OCAU：3/3 PciRoot（0x0）/ Pci（0x1B，0x0）/ VenMsg（&lt;已编辑&gt;，02000000）（7个输出）查找正确端口的最快方法是将值从0暴力破解到N-1。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5192"/>
        <source> Index of the output port of the specified codec starting from 0.</source>
        <translation>指定编解码器输出端口的索引，从0开始。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5195"/>
        <source>AudioOut</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5205"/>
        <source>Screen reader will use this volume level, when the calculated volume level is less than MinimumVolume. Boot
chime sound will not play if the calculated volume level is less than MinimumVolume.</source>
        <translation>当计算出的音量级别小于MinimumVolume时，屏幕阅读器将使用此音量级别。 如果计算出的音量小于MinimumVolume，则不会发出开机提示音。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5209"/>
        <source>Minimal heard volume level from 0 to 100.</source>
        <translation>最小音量从0到100。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5212"/>
        <source>MinimumVolume</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5222"/>
        <source>Volume level range read from SystemAudioVolume varies depending on the codec. To transform read value in
[0, 127] range into raw volume range [0, 100] the read value is scaled to VolumeAmplifier percents:
RawV olume = MIN(
SystemAudioV olume ∗ V olumeAmplif ier
100
, 100)
Note: the transformation used in macOS is not linear, but it is very close and this nuance is thus ignored.</source>
        <translation>从SystemAudioVolume读取的音量级别范围取决于编解码器。 要将[0，127]范围内的读取值转换为原始音量范围[0，100]，应将读取值缩放为VolumeAmplifier百分比：RawV olume = MIN（SystemAudioV olume * V olumeAmplifier 100，100）注意： macOS不是线性的，但是非常接近，因此细微差别被忽略了。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5231"/>
        <source>Multiplication coefficient for system volume to raw volume linear translation from 0 to 1000.</source>
        <translation>系统体积到原始体积线性转换的乘数范围是0到1000。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5234"/>
        <source>VolumeAmplifier</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5244"/>
        <source>Enabling this setting routes audio playback from builtin protocols to a dedicated audio port (AudioOut) of the
specified codec (AudioCodec) located on the audio controller (AudioDevice).</source>
        <translation>启用此设置会将音频播放从内置协议路由到位于音频控制器（AudioDevice）上的指定编解码器（AudioCodec）的专用音频端口（AudioOut）。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5248"/>
        <source>Activate audio support by connecting to a backend driver.</source>
        <translation>通过连接到后端驱动程序来激活音频支持。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5251"/>
        <source>AudioSupport</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5258"/>
        <source>Enabling this setting plays boot chime through builtin audio support. Volume level is determined by MinimumVolume
and VolumeAmplifier settings and SystemAudioVolume NVRAM variable.
Note: this setting is separate from StartupMute NVRAM variable to avoid conflicts when the firmware is able to
play boot chime.</source>
        <translation>启用此设置可通过内置音频支持播放启动铃声。 音量级别由MinimumVolume和VolumeAmplifier设置以及SystemAudioVolume NVRAM变量确定。 注意：此设置与StartupMute NVRAM变量分开，以避免在固件能够播放启动铃声时发生冲突。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5264"/>
        <source> Play chime sound at startup.</source>
        <translation>在启动时播放提示音。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5267"/>
        <source>PlayChime</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5275"/>
        <location filename="mainwindow.cpp" line="2404"/>
        <source>Drivers</source>
        <oldsource>UEFI驱动-Drivers</oldsource>
        <translation>Drivers（驱动）</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5303"/>
        <source>This option is useful for loading drivers following UEFI driver model as they may not start by themselves.
Examples of such drivers are filesystem or audio drivers. While effective, this option may not be necessary for
drivers performing automatic connection, and may slightly slowdown the boot.
Note: Some firmwares, made by Apple in particular, only connect the boot drive to speedup the boot process.
Enable this option to be able to see all the boot options when having multiple drives.</source>
        <translation>此选项对于加载遵循UEFI驱动程序模型的驱动程序很有用，因为它们可能无法自行启动。 此类驱动程序的示例是文件系统或音频驱动程序。 虽然有效，但此选项对于执行自动连接的驱动程序可能不是必需的，并且可能会稍微减慢启动速度。
注意：某些固件（尤其是Apple制造的固件）仅连接引导驱动器以加快引导过程。 启用此选项可以在拥有多个驱动器时查看所有引导选项。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5310"/>
        <source> Perform UEFI controller connection after driver loading.</source>
        <translation>驱动程序加载后执行UEFI控制器连接。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5313"/>
        <source>ConnectDrivers</source>
        <oldsource>ConnectDrivers-连接驱动</oldsource>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5334"/>
        <source>Input</source>
        <oldsource>键盘设置-Input</oldsource>
        <translation>Input（键盘输入）</translation>
    </message>
    <message>
        <source>AppleKeyMapAggregator protocol is supposed to contain a fixed length buffer of currently pressed keys. However,
the majority of the drivers only report key presses as interrupts and pressing and holding the key on the keyboard
results in subsequent submissions of this key with some defined time interval. As a result we use a timeout to
remove once pressed keys from the buffer once the timeout expires and no new submission of this key happened.
This option allows to set this timeout based on your platform. The recommended value that works on the majority
of the platforms is 5 milliseconds. For reference, holding one key on VMware will repeat it roughly every 2
milliseconds and the same value for APTIO V is 3-4 milliseconds. Thus it is possible to set a slightly lower value
on faster platforms and slightly higher value on slower platforms for more responsive input.
Note: Some platforms may require different values, higher or lower. For example, when detecting key misses in
OpenCanopy try increasing this value (e.g. to 10), and when detecting key stall, try decreasing this value. Since
every platform is different it may be reasonable to check every value from 1 to 25.
3.</source>
        <translation type="obsolete">AppleKeyMapAggregator协议应该包含当前按下的键的固定长度缓冲区。但是，大多数驱动程序仅将按键按下报告为中断，并且按住键盘上的按键会导致在一定的时间间隔内随后提交该按键。因此，一旦超时到期，并且没有新提交的键发生，我们将使用超时从缓冲区中删除一次按下的键。此选项允许根据您的平台设置此超时。在大多数平台上均可使用的建议值为5毫秒。作为参考，在VMware上按住一个键大约每2毫秒重复一次，而APTIO V的相同值是3-4毫秒。因此，可以在较快的平台上设置稍低的值，而在较慢的平台上设置稍高的值，以提高响应速度。
注意：某些平台可能要求更高或更低的不同值。例如，当在OpenCanopy中检测到按键遗失时，请尝试将此值增加（例如，增加到10），而在检测到按键停滞时，请尝试减小此值。由于每个平台都不相同，因此检查从1到25的每个值可能是合理的。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5354"/>
        <source>Remove key unless it was submitted during this timeout in milliseconds.</source>
        <translation>除非在此超时期间（以毫秒为单位）提交了密钥，否则请除去密钥。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5357"/>
        <source>KeyForgetThreshold</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5367"/>
        <source>Similarly to KeyForgetThreshold, this option works around the sequential nature of key submission. To be able
to recognise simultaneously pressed keys in the situation when all keys arrive sequentially, we are required to set
a timeout within which we assume the keys were pressed together.
Holding multiple keys results in reports every 2 and 1 milliseconds for VMware and APTIO V respectively.
Pressing keys one after the other results in delays of at least 6 and 10 milliseconds for the same platforms. The
recommended value for this option is 2 milliseconds, but it may be decreased for faster platforms and increased
for slower.</source>
        <translation>与KeyForgetThreshold相似，此选项可解决密钥提交的顺序性质。 为了能够在所有按键顺序到达的情况下识别同时按下的按键，我们需要设置一个超时时间，在此时间内我们假定按键被同时按下。 按住多个键将分别每2毫秒和1毫秒报告一次VMware和APTIOV。 对于同一平台，一个接一个地按下键会导致至少6到10毫秒的延迟。 此选项的建议值为2毫秒，但对于较快的平台，可能会减小，而对于较慢的平台，可能会增加。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5376"/>
        <source>Assume simultaneous combination for keys submitted within this timeout in milliseconds.</source>
        <translation>假设在此超时时间内提交的密钥同时进行组合（以毫秒为单位）。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5379"/>
        <source>KeyMergeThreshold</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5389"/>
        <source>• Auto — Performs automatic choice as available with the following preference: AMI, V2, V1. • V1 — Uses UEFI standard legacy input protocol EFI_SIMPLE_TEXT_INPUT_PROTOCOL. • V2 — Uses UEFI standard modern input protocol EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL. • AMI — Uses APTIO input protocol AMI_EFIKEYCODE_PROTOCOL.
Note: Currently V1, V2, and AMI unlike Auto only do filtering of the particular specified protocol. This may
change in the future versions.</source>
        <translation>•Auto（自动）—进行自动选择。
•V1-使用UEFI标准传统输入协议EFI_SIMPLE_TEXT_INPUT_PROTOCOL。
•V2-使用UEFI标准的现代输入协议EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL。
•AMI-使用APTIO输入协议AMI_EFIKEYCODE_PROTOCOL。
注意：当前，V1，V2和AMI与“自动”不同，仅对特定的指定协议进行过滤。 在将来的版本中，这可能会更改。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5394"/>
        <source> Set internal keyboard input translation to AppleKeyMapAggregator protocol mode.</source>
        <translation>将内部键盘输入转换设置为AppleKeyMapAggregator协议模式。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5397"/>
        <source>KeySupportMode</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5407"/>
        <source>Currently the only supported variant is ASUS, using specialised protocol available on select Z87 and Z97 ASUS
boards. More details can be found in LongSoft/UefiTool#116.</source>
        <translation>当前，唯一受支持的变量是ASUS，它使用了某些Z87和Z97 ASUS板上可用的专用协议。 可以在LongSoft / UefiTool＃116中找到更多详细信息。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5411"/>
        <source> Set OEM protocol used for internal pointer driver.</source>
        <translation>设置用于内部指针驱动程序的OEM协议。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5414"/>
        <source>PointerSupportMode</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5424"/>
        <source>This option allows to update firmware architecture timer period with the specified value in 100 nanosecond units.
Setting a lower value generally improves performance and responsiveness of the interface and input handling.
The recommended value is 50000 (5 milliseconds) or slightly higher. Select ASUS Z87 boards use 60000 for the
interface. Apple boards use 100000. You may leave it as 0 in case there are issues.</source>
        <translation>此选项允许以100纳秒为单位的指定值更新固件体系结构计时器周期。 设置较低的值通常可以提高界面和输入处理的性能和响应能力。 推荐值是50000（5毫秒）或更高。 选择ASUS Z87主板时，请使用60000作为接口。 苹果主板使用100000。如果有问题，可以将其保留为0。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5430"/>
        <source>Set architecture timer resolution.</source>
        <translation>设置架构计时器分辨率。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5433"/>
        <source>TimerResolution</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5443"/>
        <source>Apparently some boards like GA Z77P-D3 may return uninitialised data in EFI_INPUT_KEY with all input protocols.
This option discards keys that are neither ASCII, nor are defined in the UEFI specification (see tables 107 and
108 in version 2.8).</source>
        <translation>显然，某些GA Z77P-D3等主板可能会通过所有输入协议在EFI_INPUT_KEY中返回未初始化的数据。 此选项丢弃既不是ASCII也不在UEFI规范中定义的键（请参见2.8版中的表107和108）。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5448"/>
        <source> Enable keyboard input sanity checking.</source>
        <translation>启用键盘输入的健全性检查。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5451"/>
        <source>KeyFiltering</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5458"/>
        <source>This option may be useful for keyboard layouts with Option key situated to the right of Command key.</source>
        <translation>对于在Command键右侧具有Option键的键盘布局，此选项可能很有用。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5461"/>
        <source>Swap Command and Option keys during submission.</source>
        <translation>提交期间交换Command和Option键。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5464"/>
        <source>KeySwap</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5471"/>
        <source>This option activates the internal keyboard interceptor driver, based on AppleGenericInput aka (AptioInputFix),
to fill AppleKeyMapAggregator database for input functioning. In case a separate driver is used, such as
OpenUsbKbDxe, this option should never be enabled.</source>
        <translation>此选项根据AppleGenericInput aka（AptioInputFix）激活内部键盘拦截器驱动程序，以填充AppleKeyMapAggregator数据库以实现输入功能。 如果使用单独的驱动程序，例如OpenUsbKbDxe，则永远不要启用此选项。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5476"/>
        <source> Enable internal keyboard input translation to AppleKeyMapAggregator protocol.</source>
        <translation>启用内部键盘输入到AppleKeyMapAggregator协议的转换。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5479"/>
        <source>KeySupport</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5486"/>
        <source>This option implements standard UEFI pointer protocol (EFI_SIMPLE_POINTER_PROTOCOL) through select OEM
protocols. The option may be useful on Z87 ASUS boards, where EFI_SIMPLE_POINTER_PROTOCOL is broken.</source>
        <translation>此选项通过选择OEM协议来实现标准UEFI指针协议（EFI_SIMPLE_POINTER_PROTOCOL）。 选项可以是在Z87 ASUS板，其中EFI_SIMPLE_POINTER_PROTOCOL被破坏是有用的。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5490"/>
        <source> Enable internal pointer driver.</source>
        <translation>启用内部指针驱动程序。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5493"/>
        <source>PointerSupport</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5507"/>
        <source>Some firmwares clear only part of screen when switching from graphics to text mode, leaving a
fragment of previously drawn image visible. This option fills the entire graphics screen with black colour before
switching to text mode.
Note: This option only applies to System renderer.</source>
        <translation>从图形模式切换到文本模式时，某些固件仅清除屏幕的一部分，使以前绘制的图像片段可见。 此选项在切换到文本模式之前用黑色填充整个图形屏幕。 注意：此选项仅适用于系统渲染器。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5513"/>
        <source>Some firmwares clear only part of screen when switching from graphics to text mode, leaving a
 fragment of previously drawn image visible. This option fills the entire graphics screen with black colour before
 switching to text mode.
 Note: This option only applies to System renderer.</source>
        <translation>从图形模式切换到文本模式时，某些固件仅清除屏幕的一部分，使以前绘制的图像片段可见。 此选项在切换到文本模式之前用黑色填充整个图形屏幕。 注意：此选项仅适用于系统渲染器。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5519"/>
        <source>ClearScreenOnModeSwitch</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5526"/>
        <source>On some firmwares when screen resolution is changed via GOP, it is required to reconnect the controllers, which
produce the console protocols (simple text out). Otherwise they will not produce text based on the new resolution.
Note: On several boards this logic may result in black screen when launching OpenCore from Shell and thus it is
optional. In versions prior to 0.5.2 this option was mandatory and not configurable. Please do not use this unless
required.</source>
        <translation>在某些固件上，通过GOP更改屏幕分辨率时，需要重新连接控制器，这会产生控制台协议（简单文本输出）。 否则，它们将不会根据新的分辨率生成文本。
 注意：从Shell启动OpenCore时，此逻辑可能会导致黑屏，因此它是可选的。 在0.5.2之前的版本中，此选项为强制性且不可配置。 除非需要，否则请勿使用此功能。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5533"/>
        <source>Reconnect console controllers after changing screen resolution.</source>
        <translation>更改屏幕分辨率后，重新连接控制台控制器。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5536"/>
        <source>ReconnectOnResChange</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5558"/>
        <source> Select firmwares output text onscreen in both graphics and text mode. This is normally unexpected,
because random text may appear over graphical images and cause UI corruption. Setting this option to true will
discard all text output when console control is in mode different from Text.
Note: This option only applies to System renderer.</source>
        <translation>选择固件可在图形和文本模式下在屏幕上输出文本。 这通常是意外的，因为随机文本可能会出现在图形图像上并导致UI损坏。 当控制台控件处于不同于“文本”的模式时，将此选项设置为true将丢弃所有文本输出。 注意：此选项仅适用于系统渲染器。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5564"/>
        <source>IgnoreTextInGraphics</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5571"/>
        <source> Some firmwares reset screen resolution to a failsafe value (like 1024x768) on the attempts to clear
screen contents when large display (e.g. 2K or 4K) is used. This option attempts to apply a workaround.
Note: This option only applies to System renderer. On all known affected systems ConsoleMode had to be set to
empty string for this to work.</source>
        <translation>某些固件在尝试使用大尺寸显示（例如2K或4K）时尝试清除屏幕内容时，将屏幕分辨率重置为故障保护值（例如1024x768）。 此选项尝试应用解决方法。 注意：此选项仅适用于系统渲染器。 在所有已知的受影响系统上，必须将ConsoleMode设置为空字符串才能起作用。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5577"/>
        <source>SanitiseClearScreen</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5584"/>
        <source>On some firmwares this may provide better performance or even fix rendering issues, like on MacPro5,1. However,
it is recommended not to use this option unless there is an obvious benefit as it may even result in slower scrolling.</source>
        <translation>在某些固件上，这可能会提供更好的性能，甚至修复渲染问题，例如在MacPro5,1上。 但是，除非有明显的好处，否则建议不要使用此选项，因为它甚至可能导致滚动速度变慢。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5588"/>
        <source> Use builtin graphics output protocol renderer for console.</source>
        <translation>使用内置的图形输出协议渲染器作为控制台。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5591"/>
        <source>DirectGopRendering</source>
        <translation></translation>
    </message>
    <message>
        <source>Some firmwares do not print tab characters or even everything that follows them, causing difficulties
or inability to use the UEFI Shell builtin text editor to edit property lists and other documents. This option
makes the console output spaces instead of tabs.
Note: This option only applies to System renderer.
8.</source>
        <translation type="obsolete">某些固件不打印制表符，甚至不打印制表符，否则会导致困难或无法使用UEFI Shell内置的文本编辑器来编辑属性列表和其他文档。 此选项使控制台输出空间代替制表符。
注意：此选项仅适用于系统渲染器。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5604"/>
        <source>ReplaceTabWithSpace</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5611"/>
        <source>macOS bootloader requires GOP or UGA (for 10.4 EfiBoot) to be present on console handle, yet the exact
location of the graphics protocol is not covered by the UEFI specification. This option will ensure GOP and
UGA, if present, are available on the console handle.
Note: This option will also replace broken GOP protocol on console handle, which may be the case on MacPro5,1
with newer GPUs.</source>
        <translation>macOS引导加载程序要求在控制台手柄上存在GOP或UGA（用于10.4 EfiBoot），但是UEFI规范未涵盖图形协议的确切位置。 此选项将确保GOP和UGA（如果存在）在控制台手柄上可用。
注意：此选项还将替换控制台句柄上损坏的GOP协议，在MacPro5,1上，较新的GPU可能就是这种情况。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5618"/>
        <source>Ensure GOP (Graphics Output Protocol) on console handle.</source>
        <translation>确保控制台手柄上的GOP（图形输出协议）。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5621"/>
        <source>ProvideConsoleGop</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5628"/>
        <source>Some firmwares do not implement legacy UGA protocol, but it may be required for screen output by older EFI
applications like EfiBoot from 10.4.</source>
        <translation>某些固件未实现传统的UGA协议，但较早的EFI应用程序（如10.4版的EfiBoot）可能需要屏幕输出。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5632"/>
        <source> Provide UGA protocol instances on top of GOP protocol.</source>
        <translation>在GOP协议之上提供UGA协议实例。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5635"/>
        <source>UgaPassThrough</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5642"/>
        <source>Set to empty string not to change console mode. Set to Max to try to use largest available console mode. Currently
Builtin text renderer supports only one console mode, so this option is ignored.
Note: This field is best to be left empty on most firmwares.</source>
        <translation>设置为空字符串不更改控制台模式。 设置为最大以尝试使用最大的可用控制台模式。 当前，内置文本渲染器仅支持一种控制台模式，因此将忽略此选项。
注意：在大多数固件上，最好将此字段留空。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5647"/>
        <source>Sets console output mode as specified with the WxH (e.g. 80x24) formatted string.</source>
        <translation>按照WxH（例如80x24）格式的字符串指定的设置控制台输出模式。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5650"/>
        <source>ConsoleMode(e.g. 80x24)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5660"/>
        <source>• Set to WxH@Bpp (e.g. 1920x1080@32) or WxH (e.g. 1920x1080) formatted string to request custom resolution
from GOP if available.
• Set to empty string not to change screen resolution.
• Set to Max to try to use largest available screen resolution.
On HiDPI screens APPLE_VENDOR_VARIABLE_GUID UIScale NVRAM variable may need to be set to 02 to enable
HiDPI scaling in Builtin text renderer, FileVault 2 UEFI password interface, and boot screen logo. Refer to
Recommended Variables section for more details.
Note: This will fail when console handle has no GOP protocol. When the firmware does not provide it, it can be
added with ProvideConsoleGop set to true.</source>
        <translation>•设置为WxH @ Bpp（例如1920x1080 @ 32）或WxH（例如1920x1080）格式的字符串，以从GOP请求自定义分辨率。
•设置为空字符串不更改屏幕分辨率。
•设置为最大以尝试使用最大的可用屏幕分辨率。 在HiDPI屏幕上，可能需要将APPLE_VENDOR_VARIABLE_GUID UIScale NVRAM变量设置为02，才能在内置文本渲染器，FileVault 2 UEFI密码界面和启动屏幕徽标中启用HiDPI缩放。 有关更多详细信息，请参阅“推荐变量”部分。
注意：如果控制台句柄没有GOP协议，这将失败。 当固件不提供固件时，可以将ProvideConsoleGop设置为true进行添加。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5671"/>
        <source> Sets console output screen resolution.</source>
        <translation>设置控制台输出屏幕分辨率。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5674"/>
        <source>Resolution(e.g. 1920x1080@32) or (e.g. 1920x1080)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5684"/>
        <source>Currently two renderers are supported: Builtin and System. System renderer uses firmware services for text
rendering. Builtin bypassing firmware services and performs text rendering on its own. Different renderers
support a different set of options. It is recommended to use Builtin renderer, as it supports HiDPI mode and
uses full screen resolution.
UEFI firmwares generally support ConsoleControl with two rendering modes: Graphics and Text. Some
firmwares do not support ConsoleControl and rendering modes. OpenCore and macOS expect text to only be
shown in Graphics mode and graphics to be drawn in any mode. Since this is not required by UEFI specification,
exact behaviour varies.
Valid values are combinations of text renderer and rendering mode:
• BuiltinGraphics — Switch to Graphics mode and use Builtin renderer with custom ConsoleControl. 
• BuiltinText — Switch to Text mode and use Builtin renderer with custom ConsoleControl. 
• SystemGraphics — Switch to Graphics mode and use System renderer with custom ConsoleControl. 
• SystemText — Switch to Text mode and use System renderer with custom ConsoleControl. 
• SystemGeneric — Use System renderer with system ConsoleControl assuming it behaves correctly.
The use of BuiltinGraphics is generally straightforward. For most platforms it is necessary to enable
ProvideConsoleGop, set Resolution to Max. BuiltinText variant is an alternative BuiltinGraphics for
some very old and buggy laptop firmwares, which can only draw in Text mode.
The use of System protocols is more complicated. In general the preferred setting is SystemGraphics or
SystemText. Enabling ProvideConsoleGop, setting Resolution to Max, enabling ReplaceTabWithSpace is
useful on almost all platforms. SanitiseClearScreen, IgnoreTextInGraphics, and ClearScreenOnModeSwitch
are more specific, and their use depends on the firmware.
Note: Some Macs, namely MacPro5,1, may have broken console output with newer GPUs, and thus only
BuiltinGraphics may work for them.</source>
        <translation>当前支持两种渲染器：内置和系统。系统渲染器使用固件服务进行文本渲染。内置绕过固件服务并自行执行文本渲染。不同的渲染器支持不同的选项集。建议使用内置渲染器，因为它支持HiDPI模式并使用全屏分辨率。 UEFI固件通常以两种渲染模式支持ConsoleControl：图形和文本。某些固件不支持ConsoleControl和呈现模式。 OpenCore和macOS希望仅在“图形”模式下显示文本，而在任何模式下绘制图形。由于UEFI规范不要求这样做，因此确切的行为会有所不同。有效值是文本渲染器和渲染模式的组合：
•BuiltinGraphics —切换到“图形”模式，并使用内置渲染器和自定义ConsoleControl。
•BuiltinText-切换到“文本”模式，并使用内置渲染器和自定义ConsoleControl。
•SystemGraphics-切换到“图形”模式，并将系统渲染器与自定义ConsoleControl一起使用。
•SystemText-切换到“文本”模式，并将系统渲染器与自定义ConsoleControl一起使用。
•SystemGeneric-假设系统渲染器运行正常，则将其与系统ConsoleControl一起使用。 BuiltinGraphics的使用通常很简单。对于大多数平台，必须启用ProvideConsoleGop，将Resolution设置为Max。 BuiltinText变体是一些非常老旧且有故障的笔记本电脑固件的替代BuiltinGraphics，只能在文本模式下绘制。系统协议的使用更为复杂。通常，首选设置是SystemGraphics或SystemText。启用ProvideConsoleGop，将“分辨率”设置为“最大”，启用“ ReplaceTabWithSpace”在几乎所有平台上都很有用。 SanitiseClearScreen，IgnoreTextInGraphics和ClearScreenOnModeSwitch更具体，它们的使用取决于固件。注意：某些Mac，即MacPro5,1，可能在较新的GPU上的控制台输出已损坏，因此只有BuiltinGraphics可以为它们工作。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5709"/>
        <source> Chooses renderer for text going through standard console output.</source>
        <translation>为通过标准控制台输出的文本选择渲染器。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5712"/>
        <source>TextRenderer</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5723"/>
        <source>ProtocolOverrides</source>
        <oldsource>协议覆盖-ProtocolOverrides</oldsource>
        <translation>ProtocolOverrides（协议覆盖）</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5729"/>
        <source>Apple audio protocols allow macOS bootloader and OpenCore to play sounds and signals for screen reading or
audible error reporting. Supported protocols are beep generation and VoiceOver. VoiceOver protocol is specific to
Gibraltar machines (T2) and is not supported before macOS High Sierra (10.13). Instead older macOS versions
use AppleHDA protocol, which is currently not implemented.
Only one set of audio protocols can be available at a time, so in order to get audio playback in OpenCore user
interface on Mac system implementing some of these protocols this setting should be enabled.
Note: Backend audio driver needs to be configured in UEFI Audio section for these protocols to be able to stream
audio.</source>
        <translation>Apple音频协议允许macOS引导程序和OpenCore播放声音和信号以进行屏幕阅读或听觉错误报告。 支持的协议是蜂鸣声生成和VoiceOver。 VoiceOver协议特定于直布罗陀的机器（T2），在macOS High Sierra（10.13）之前不受支持。 相反，较早的macOS版本使用AppleHDA协议，该协议目前尚未实现。 一次只能使用一组音频协议，因此，要在实现某些协议的Mac系统上的OpenCore用户界面中获得音频播放，应启用此设置。
注意：需要在“ UEFI音频”部分中配置后端音频驱动程序，这些协议才能流音频。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5739"/>
        <source>Reinstalls Apple audio protocols with builtin versions.</source>
        <translation>重新安装具有内置版本的Apple音频协议。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5742"/>
        <source>AppleAudio</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5749"/>
        <source> Reinstalls Apple Framebuffer Info protocol with a builtin version. This may be used to override
framebuffer information on VMs or legacy Macs to improve compatibility with legacy EfiBoot like the one in
macOS 10.4.</source>
        <translation>重新安装具有内置版本的Apple Framebuffer Info协议。 这可用于覆盖VM或旧版Mac上的帧缓冲区信息，以提高与旧版EfiBoot的兼容性，例如macOS 10.4中的版本。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5754"/>
        <source> Reinstalls Apple Framebuffer Info protocol with a builtin version. This may be used to override
 framebuffer information on VMs or legacy Macs to improve compatibility with legacy EfiBoot like the one in
 macOS 10.4.</source>
        <translation>重新安装具有内置版本的Apple Framebuffer Info协议。 这可用于覆盖VM或旧版Mac上的帧缓冲区信息，以提高与旧版EfiBoot的兼容性，例如macOS 10.4中的版本。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5759"/>
        <source>AppleFramebufferInfo</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5766"/>
        <source>This protocol replaces legacy VirtualSmc UEFI driver, and is compatible with any SMC kernel extension.
However, in case FakeSMC kernel extension is used, manual NVRAM key variable addition may be needed.</source>
        <translation>该协议替代了旧版VirtualSmc UEFI驱动程序，并且与任何SMC内核扩展兼容。 但是，如果使用FakeSMC内核扩展，则可能需要手动添加NVRAM键变量。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5770"/>
        <source>Reinstalls Apple SMC I/O protocol with a builtin version.</source>
        <translation>重新安装具有内置版本的Apple SMC I / O协议。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5773"/>
        <source>AppleSmcIo</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5780"/>
        <source>Note: Several virtual machines including VMware may have corrupted cursor image in HiDPI mode and thus
may also require this setting to be enabled.</source>
        <translation>注意：包括VMware在内的多个虚拟机可能在HiDPI模式下损坏了光标图像，因此可能还需要启用此设置。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5784"/>
        <source> Forcibly wraps Firmware Volume protocols or installs new to support custom cursor images for
 File Vault 2. Should be set to true to ensure File Vault 2 compatibility on everything but VMs and legacy Macs.</source>
        <translation>强制包装固件卷协议或安装新的协议以支持File Vault 2的自定义光标图像。应设置为true以确保File Vault 2在VM和旧版Mac之外的所有兼容性。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5788"/>
        <source>FirmwareVolume</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5795"/>
        <source>Note: Some Macs, namely MacPro5,1, do have APFS compatibility, but their Apple Boot Policy protocol contains
recovery detection issues, thus using this option is advised on them as well.</source>
        <translation>注意：某些Mac（即MacPro5,1）确实具有APFS兼容性，但是其Apple Boot Policy协议包含恢复检测问题，因此建议在其上也使用此选项。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5799"/>
        <source> Reinstalls Apple Boot Policy protocol with a builtin version. This may be used to ensure APFS
 compatibility on VMs or legacy Macs.</source>
        <translation>重新安装具有内置版本的Apple Boot Policy协议。 这可用于确保VM或旧版Mac上的APFS兼容性。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5803"/>
        <source>AppleBootPolicy</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5810"/>
        <location filename="mainwindow.ui" line="5813"/>
        <source>Reinstalls Apple Image Conversion protocol with a builtin version.</source>
        <translation>重新安装具有内置版本的Apple Image Conversion协议。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5816"/>
        <source>AppleImageConversion</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5823"/>
        <location filename="mainwindow.ui" line="5826"/>
        <source>Reinstalls Apple User Interface Theme protocol with a builtin version.</source>
        <translation>重新安装具有内置版本的Apple用户界面主题协议。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5829"/>
        <source>AppleUserInterfaceTheme</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5836"/>
        <source>Forcibly reinstalls Hash Services protocols with builtin versions. Should be set to true to ensure
File Vault 2 compatibility on platforms providing broken SHA-1 hashing. Can be diagnosed by invalid cursor size
with UIScale set to 02, in general platforms prior to APTIO V (Haswell and older) are affected.</source>
        <translation>强制使用内置版本重新安装Hash Services协议。 应该设置为true以确保在提供损坏的SHA-1哈希的平台上与File Vault 2兼容。 可以通过将UIScale设置为02的无效光标大小来诊断，在APTIO V之前的一般平台（Haswell和更早版本）中会受到影响。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5841"/>
        <source>Forcibly reinstalls Hash Services protocols with builtin versions. Should be set to true to ensure
 File Vault 2 compatibility on platforms providing broken SHA-1 hashing. Can be diagnosed by invalid cursor size
 with UIScale set to 02, in general platforms prior to APTIO V (Haswell and older) are affected.</source>
        <translation>强制使用内置版本重新安装Hash Services协议。 应该设置为true以确保在提供损坏的SHA-1哈希的平台上与File Vault 2兼容。 可以通过将UIScale设置为02的无效光标大小来诊断，在APTIO V之前的一般平台（Haswell和更早版本）中会受到影响。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5846"/>
        <source>HashServices</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5853"/>
        <location filename="mainwindow.ui" line="5856"/>
        <source>Reinstalls Apple Debug Log protocol with a builtin version.</source>
        <translation>重新安装具有内置版本的Apple Debug Log协议。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5859"/>
        <source>AppleDebugLog</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5866"/>
        <location filename="mainwindow.ui" line="5869"/>
        <source> Reinstalls Apple Key Map protocols with builtin versions.</source>
        <translation>重新安装具有内置版本的Apple Key Map协议。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5872"/>
        <source>AppleKeyMap</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5879"/>
        <source> Reinstalls Data Hub protocol with a builtin version. This will delete all previous properties if the
protocol was already installed.</source>
        <translation>重新安装具有内置版本的Data Hub协议。 如果已经安装了协议，这将删除所有以前的属性。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5883"/>
        <source> Reinstalls Data Hub protocol with a builtin version. This will delete all previous properties if the
 protocol was already installed.</source>
        <translation>重新安装具有内置版本的Data Hub协议。 如果已经安装了协议，这将删除所有以前的属性。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5894"/>
        <source>Forcibly reinstalls OS Info protocol with builtin versions. This protocol is generally used to receive
notifications from macOS bootloader, by the firmware or by other applications.</source>
        <translation>强制使用内置版本重新安装OS Info协议。 该协议通常用于通过固件或其他应用程序从macOS引导加载程序接收通知。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5898"/>
        <source>Forcibly reinstalls OS Info protocol with builtin versions. This protocol is generally used to receive
 notifications from macOS bootloader, by the firmware or by other applications.</source>
        <translation>强制使用内置版本重新安装OS Info协议。 该协议通常用于通过固件或其他应用程序从macOS引导加载程序接收通知。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5902"/>
        <source>OSInfo</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5909"/>
        <source>Reinstalls Apple Event protocol with a builtin version. This may be used to ensure File Vault 2
compatibility on VMs or legacy Macs.</source>
        <translation>重新安装具有内置版本的Apple Event协议。 这可用于确保VM或旧版Mac上的File Vault 2兼容性。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5913"/>
        <source>Reinstalls Apple Event protocol with a builtin version. This may be used to ensure File Vault 2
 compatibility on VMs or legacy Macs.</source>
        <translation>重新安装具有内置版本的Apple Event协议。 这可用于确保VM或旧版Mac上的File Vault 2兼容性。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5917"/>
        <source>AppleEvent</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5924"/>
        <location filename="mainwindow.ui" line="5927"/>
        <source>Reinstalls Apple RTC RAM protocol with builtin version.</source>
        <translation>重新安装具有内置版本的Apple RTC RAM协议。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5930"/>
        <source>AppleRtcRam</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1789"/>
        <location filename="mainwindow.cpp" line="45"/>
        <source>Misc</source>
        <translation>Misc
杂项</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.86297pt; color:#000000;&quot;&gt;Difffferent pickers may be confifigured through the attribute mask containing OpenCore-reserved (&lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;BIT0&lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.86297pt; color:#000000;&quot;&gt;~&lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;BIT15&lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.86297pt; color:#000000;&quot;&gt;) and &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;OEM-specifific (&lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;BIT16&lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;~&lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;BIT31&lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;) values. &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;Current OpenCore values include: &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;• &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;0x0001 &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;— &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;OC_ATTR_USE_VOLUME_ICON&lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;, provides custom icons for boot entries: &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;For &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;Tools &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;OpenCore will try to load a custom icon and fallback to the default icon: &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Bold&apos;; font-size:9.9626pt; font-weight:600; color:#000000;&quot;&gt;– &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;ResetNVRAM &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;— &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;Resources\Image\ResetNVRAM.icns &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;— &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;ResetNVRAM.icns &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;from icons directory. &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Bold&apos;; font-size:9.9626pt; font-weight:600; color:#000000;&quot;&gt;– &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;Tools\&amp;lt;TOOL_RELATIVE_PATH&amp;gt;.icns &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;— icon near the tool fifile with appended &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;.icns &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;extension. &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.99249pt; color:#000000;&quot;&gt;For custom boot &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;Entries &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.99249pt; color:#000000;&quot;&gt;OpenCore will try to load a custom icon and fallback to the volume icon or the &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;default icon: &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Bold&apos;; font-size:9.9626pt; font-weight:600; color:#000000;&quot;&gt;– &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;&amp;lt;ENTRY_PATH&amp;gt;.icns &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;— icon near the entry fifile with appended &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;.icns &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;extension. &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;For all other entries OpenCore will try to load a volume icon and fallback to the default icon: &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Bold&apos;; font-size:9.9626pt; font-weight:600; color:#000000;&quot;&gt;– &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;.VolumeIcon.icns &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;fifile at &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;Preboot &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;root for APFS. &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Bold&apos;; font-size:9.9626pt; font-weight:600; color:#000000;&quot;&gt;– &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;.VolumeIcon.icns &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;fifile at volume root for other fifilesystems. &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.94267pt; color:#000000;&quot;&gt;Volume icons can be set in Finder. Note, that enabling this may result in external and internal icons to be &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;indistinguishable. &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;• &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;0x0002 &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;— &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;OC_ATTR_USE_DISK_LABEL_FILE&lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;, provides custom rendered titles for boot entries: &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Bold&apos;; font-size:9.9626pt; font-weight:600; color:#000000;&quot;&gt;– &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;.disk_label &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;(&lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;.disk_label_2x&lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;) fifile near bootloader for all fifilesystems. &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Bold&apos;; font-size:9.9626pt; font-weight:600; color:#000000;&quot;&gt;– &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;&amp;lt;TOOL_NAME&amp;gt;.lbl &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;(&lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;&amp;lt;TOOL_NAME&amp;gt;.l2x&lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;) fifile near tool for &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;Tools&lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;. &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:10.0074pt; color:#000000;&quot;&gt;Prerendered labels can be generated via &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;disklabel &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:10.0074pt; color:#000000;&quot;&gt;utility or &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;bless &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:10.0074pt; color:#000000;&quot;&gt;command. When disabled or missing &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;text labels (&lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;.contentDetails &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;or &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;.disk_label.contentDetails&lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;) are to be rendered instead. &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;• &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;0x0004 &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;— &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;OC_ATTR_USE_GENERIC_LABEL_IMAGE&lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;, provides predefifined label images for boot entries without &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;custom entries. May give less detail for the actual boot entry. &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;• &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;0x0008 &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.86297pt; color:#000000;&quot;&gt;— &lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMMono10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;OC_ATTR_USE_ALTERNATE_ICONS&lt;/span&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.86297pt; color:#000000;&quot;&gt;, changes used icon set to an alternate one if it is supported. For &lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-family:&apos;LMRoman10-Regular&apos;; font-size:9.9626pt; color:#000000;&quot;&gt;example, this could make a use of old-style icons with a custom background colour.&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="obsolete">可通过包含OpenCore保留（BIT0〜BIT15）和
OEM特定（BIT16〜BIT31）值。
当前的OpenCore值包括：
•0x0001 — OC_ATTR_USE_VOLUME_ICON，提供引导条目的自定义图标：
对于工具，OpenCore将尝试加载自定义图标并回退到默认图标：
– ResetNVRAM-资源\图像\ ResetNVRAM.icns-图标目录中的ResetNVRAM.icns。
– Tools \ &lt;TOOL_RELATIVE_PATH&gt; .icns —工具文件附近的图标，带有扩展的.icns扩展名。
对于自定义启动项，OpenCore会尝试加载自定义图标并回退到音量图标或
默认图标：
– &lt;ENTRY_PATH&gt; .icns —条目文件附近带有扩展名.icns的图标。
对于所有其他条目，OpenCore会尝试加载音量图标并回退到默认图标：
– APFS的预引导根目录下的.VolumeIcon.icns文件。
–其他文件系统的卷根目录下的.VolumeIcon.icns文件。
可以在Finder中设置音量图标。请注意，启用此功能可能会导致外部和内部图标
难以区分。
•0x0002 — OC_ATTR_USE_DISK_LABEL_FILE，提供引导条目的自定义渲染标题：
–对于所有文件系统，bootloader附近的.disk_label（.disk_label_2x）文件。
–工具附近的&lt;TOOL_NAME&gt; .lbl（&lt;TOOL_NAME&gt; .l2x）文件。
可以通过disklabel实用程序或bless命令生成预渲染的标签。禁用或丢失时
文本标签（.contentDetails或.disk_label.contentDetails）将被呈现。
•0x0004 — OC_ATTR_USE_GENERIC_LABEL_IMAGE，为引导项提供了预定义的标签图像，而没有
自定义条目。可能会为实际的引导条目提供较少的详细信息。
•0x0008-OC_ATTR_USE_ALTERNATE_ICONS，将已使用图标集更改为备用图标（如果支持）。对于
例如，这可以使用具有自定义背景色的旧式图标。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1811"/>
        <source>Sets specific attributes for picker.</source>
        <translation>设置选择器的特定属性。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1814"/>
        <source>PickerAttributes</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2963"/>
        <source>Note 1 : It is known that some Lenovo laptops have a firmware bug, which makes them unbootable after performing
NVRAM reset. See acidanthera/bugtracker#995 for more details.
Note 2 : Resetting NVRAM will also erase all the boot options otherwise not backed up with bless (e.g. Linux).</source>
        <translation>注意1：已知某些Lenovo笔记本电脑存在固件错误，这会使它们在执行NVRAM重置后无法启动。 有关更多详细信息，请参见acidanthera / bugtracker＃995。

注意2：重置NVRAM也会擦除所有引导选项，否则将无法通过bless进行备份（例如Linux）。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2968"/>
        <source> Allow CMD+OPT+P+R handling and enable showing NVRAM Reset entry in boot picker.</source>
        <translation>允许CMD + OPT + P + R处理，并在引导选择器中启用显示NVRAM重置条目。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2971"/>
        <source>AllowNvramReset</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="2995"/>
        <location filename="mainwindow.ui" line="2998"/>
        <source> Allow CTRL+Enter and CTRL+Index handling to set the default boot option in boot picker.</source>
        <translation>允许CTRL + Enter和CTRL + Index处理来设置启动选择器中的默认启动选项。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="3001"/>
        <source>AllowSetDefault</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5008"/>
        <location filename="mainwindow.cpp" line="48"/>
        <source>UEFI</source>
        <translation>UEFI</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5024"/>
        <source>APFS</source>
        <translation>APFS（APFS文件系统）</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5340"/>
        <source>AppleKeyMapAggregator protocol is supposed to contain a fixed length buffer of currently pressed keys. However,
the majority of the drivers only report key presses as interrupts and pressing and holding the key on the keyboard
results in subsequent submissions of this key with some defined time interval. As a result we use a timeout to
remove once pressed keys from the buffer once the timeout expires and no new submission of this key happened.
This option allows to set this timeout based on your platform. The recommended value that works on the majority
of the platforms is 5 milliseconds. For reference, holding one key on VMware will repeat it roughly every 2
milliseconds and the same value for APTIO V is 3-4 milliseconds. Thus it is possible to set a slightly lower value
on faster platforms and slightly higher value on slower platforms for more responsive input.

Note: Some platforms may require different values, higher or lower. For example, when detecting key misses in
OpenCanopy try increasing this value (e.g. to 10), and when detecting key stall, try decreasing this value. Since
every platform is different it may be reasonable to check every value from 1 to 25.</source>
        <translation>AppleKeyMapAggregator协议应该包含当前按下的键的固定长度缓冲区。但是，大多数驱动程序仅将按键按下报告为中断，并且按住键盘上的按键会导致在一定的时间间隔内随后提交该按键。因此，一旦超时到期，并且没有新提交的键发生，我们将使用超时从缓冲区中删除一次按下的键。此选项允许根据您的平台设置此超时。在大多数平台上均可使用的建议值为5毫秒。作为参考，在VMware上按住一个键大约每2毫秒重复一次，而APTIO V的相同值是3-4毫秒。因此，可以在较快的平台上设置稍低的值，而在较慢的平台上设置稍高的值，以提高响应速度。注意：某些平台可能要求更高或更低的不同值。例如，当在OpenCanopy中检测到按键遗失时，请尝试将此值增加（例如，增加到10），而在检测到按键停滞时，请尝试减小此值。由于每个平台都不同，因此检查从1到25的每个值可能是合理的。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5501"/>
        <source>Output</source>
        <translation>Output（显示输出）</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5598"/>
        <source>Some firmwares do not print tab characters or even everything that follows them, causing difficulties
or inability to use the UEFI Shell builtin text editor to edit property lists and other documents. This option
makes the console output spaces instead of tabs.
Note: This option only applies to System renderer.</source>
        <translation>某些固件不打印制表符，甚至不打印制表符，否则会导致困难或无法使用UEFI Shell内置的文本编辑器来编辑属性列表和其他文档。 此选项使控制台输出空间代替制表符。
注意：此选项仅适用于系统渲染器。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5937"/>
        <source> Reinstalls Device Property protocol with a builtin version. This will delete all previous properties
if it was already installed. This may be used to ensure full compatibility on VMs or legacy Macs.</source>
        <translation>重新安装具有内置版本的设备属性协议。 如果已经安装，它将删除所有以前的属性。 这可用于确保在VM或旧版Mac上具有完全兼容性。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5941"/>
        <source> Reinstalls Device Property protocol with a builtin version. This will delete all previous properties
 if it was already installed. This may be used to ensure full compatibility on VMs or legacy Macs.</source>
        <translation>重新安装具有内置版本的设备属性协议。 如果已经安装，它将删除所有以前的属性。 这可用于确保在VM或旧版Mac上具有完全兼容性。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5952"/>
        <location filename="mainwindow.ui" line="5955"/>
        <source> Forcibly reinstalls unicode collation services with builtin version. Should be set to true to ensure UEFI Shell compatibility on platforms providing broken unicode collation. In general legacy Insyde and APTIO platforms on Ivy Bridge and earlier are affected.</source>
        <translation>强制使用内置版本重新安装Unicode整理服务。 应该设置为true以确保UEFI Shell在提供残破的Unicode排序规则的平台上具有兼容性。 通常，Ivy Bridge和更早版本上的旧版Insyde和APTIO平台会受到影响。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5958"/>
        <source>UnicodeCollation</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5965"/>
        <source> Reinstalls Apple IMG4 Verification protocol with a builtin version. This protocol is used to verify
im4m manifest files used by Apple Secure Boot.</source>
        <translation>重新安装具有内置版本的Apple IMG4 Verification协议。 此协议用于验证Apple Secure Boot使用的im4m清单文件。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5969"/>
        <source> Reinstalls Apple IMG4 Verification protocol with a builtin version. This protocol is used to verify
 im4m manifest files used by Apple Secure Boot.</source>
        <translation>重新安装具有内置版本的Apple IMG4 Verification协议。 此协议用于验证Apple Secure Boot使用的im4m清单文件。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5973"/>
        <source>AppleImg4Verification</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5980"/>
        <location filename="mainwindow.ui" line="5983"/>
        <source> Reinstalls Apple Secure Boot protocol with a builtin version.</source>
        <translation>重新安装具有内置版本的Apple Secure Boot协议。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5986"/>
        <source>AppleSecureBoot</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="5994"/>
        <source>ReservedMemory</source>
        <oldsource>保留内存-ReservedMemory</oldsource>
        <translation>ReservedMemory（保留内存）</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6049"/>
        <source>This quirk requires RequestBootVarRouting to be enabled and therefore OC_FIRMWARE_RUNTIME protocol implemented in OpenRuntime.efi.
By redirecting Boot prefixed variables to a separate GUID namespace with the help of RequestBootVarRouting
quirk we achieve multiple goals:
• Operating systems are jailed and only controlled by OpenCore boot environment to enhance security.
• Operating systems do not mess with OpenCore boot priority, and guarantee fluent updates and hibernation
wakes for cases that require reboots with OpenCore in the middle.
• Potentially incompatible boot entries, such as macOS entries, are not deleted or anyhow corrupted.
However, some firmwares do their own boot option scanning upon startup by checking file presence on the
available disks. Quite often this scanning includes non-standard locations, such as Windows Bootloader paths.
Normally it is not an issue, but some firmwares, ASUS firmwares on APTIO V in particular, have bugs. For them
scanning is implemented improperly, and firmware preferences may get accidentally corrupted due to BootOrder
entry duplication (each option will be added twice) making it impossible to boot without resetting NVRAM.
To trigger the bug one should have some valid boot options (e.g. OpenCore) and then install Windows with
RequestBootVarRouting enabled. As Windows bootloader option will not be created by Windows installer, the
firmware will attempt to create it itself, and then corrupt its boot option list.
This quirk removes all duplicates in BootOrder variable attempting to resolve the consequences of the bugs upon
OpenCore loading. It is recommended to use this key along with BootProtect option.</source>
        <translation>此选项需要启用RequestBootVarRouting，因此需要在OpenRuntime.efi中实现OC_FIRMWARE_RUNTIME协议。通过在RequestBootVarRouting选项的帮助下将Boot前缀变量重定向到单独的GUID名称空间，我们实现了多个目标：
•操作系统被监禁，并且只能由OpenCore引导环境控制，以增强安全性。
•操作系统不会与OpenCore引导优先级混为一谈，并且对于需要在中间使用OpenCore进行重新引导的情况，可以保证流畅的更新和休眠唤醒。
•可能不兼容的引导条目（例如macOS条目）不会被删除或以任何方式损坏。但是，某些固件在启动时会通过检查可用磁盘上的文件是否存在来进行自身的启动选项扫描。此扫描经常包含非标准位置，例如Windows Bootloader路径。通常这不是问题，但是某些固件（尤其是APTIO V上的ASUS固件）存在错误。对于他们来说，扫描的执行不当，并且由于BootOrder条目重复（每个选项将被添加两次），固件首选项可能会意外损坏（无法对NVRAM进行引导）。要触发该错误，应具有一些有效的启动选项（例如OpenCore），然后在启用RequestBootVarRouting的情况下安装Windows。由于Windows安装程序不会创建Windows引导程序选项，因此固件将尝试自行创建它，然后破坏其引导选项列表。这个选项会删除BootOrder变量中的所有重复项，以尝试解决OpenCore加载时错误的后果。建议将此键与BootProtect选项一起使用。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6068"/>
        <source>Remove duplicate entries in BootOrder variable in EFI_GLOBAL_VARIABLE_GUID.</source>
        <translation>在EFI_GLOBAL_VARIABLE_GUID中的BootOrder变量中删除重复的条目。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6071"/>
        <source>DeduplicateBootOrder</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6119"/>
        <source>Attempt to detach USB controller ownership from the firmware driver. While most firmwares
manage to properly do that, or at least have an option for, select firmwares do not. As a result, operating system
may freeze upon boot. Not recommended unless required.</source>
        <translation>尝试从固件驱动程序中分离USB控制器所有权。 尽管大多数固件都设法做到这一点，或者至少有一个选择，但某些固件却没有。 结果，操作系统可能会在启动时冻结。 除非需要，否则不建议使用。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6124"/>
        <source>Attempt to detach USB controller ownership from the firmware driver. While most firmwares
 manage to properly do that, or at least have an option for, select firmwares do not. As a result, operating system
 may freeze upon boot. Not recommended unless required.</source>
        <translation>尝试从固件驱动程序中分离USB控制器所有权。 尽管大多数固件都设法做到这一点，或者至少有一个选择，但某些固件却没有。 结果，操作系统可能会在启动时冻结。 除非需要，否则不建议使用。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6129"/>
        <source>ReleaseUsbOwnership</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6179"/>
        <source>Note: The quirk is mostly relevant for select HP laptops with no drives listed.</source>
        <translation>注意：该选项与未列出驱动器的某些HP笔记本电脑最相关。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6182"/>
        <source>Some firmwares block partition handles by opening them in By Driver mode, which results in File
 System protocols being unable to install.</source>
        <translation>某些固件通过在“按驱动程序”模式下打开分区句柄来阻止分区句柄，这导致无法安装文件系统协议。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6186"/>
        <source>UnblockFsConnect</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6193"/>
        <source>This is a very ugly quirk to circumvent &quot;Still waiting for root device&quot; message on select APTIO IV firmwares,
namely ASUS Z87-Pro, when using FileVault 2 in particular. It seems that for some reason they execute code
in parallel to EXIT_BOOT_SERVICES, which results in SATA controller being inaccessible from macOS. A better
approach should be found in some future. Expect 3-5 seconds to be enough in case the quirk is needed.</source>
        <translation>特别是在使用FileVault 2时，要避免在某些APTIO IV固件（即ASUS Z87-Pro）上规避“仍然等待根设备”消息，这是一个非常丑陋的选项。 似乎由于某种原因，它们与EXIT_BOOT_SERVICES并行执行代码，这导致无法从macOS访问SATA控制器。 将来会找到更好的方法。 预计需要3-5秒，以备不时之需。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6199"/>
        <source>Adds delay in microseconds after EXIT_BOOT_SERVICES event.</source>
        <translation>在EXIT_BOOT_SERVICES事件之后增加延迟（以微秒为单位）。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6202"/>
        <source>ExitBootServicesDelay</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6225"/>
        <source>The primary purpose of this quirk is to enable early bootstrap TSC synchronisation on some server and laptop
models when running a debug XNU kernel. For the debug kernel the TSC needs to be kept in sync across the cores
before any kext could kick in rendering all other solutions problematic. The timeout is specified in microseconds
and depends on the amount of cores present on the platform, the recommended starting value is 500000.
This is an experimental quirk, which should only be used for the aforementioned problem. In all other cases the
quirk may render the operating system unstable and is not recommended. The recommended solution in the
other cases is to install a kernel driver like VoodooTSCSync, TSCAdjustReset, or CpuTscSync (a more specialised
variant of VoodooTSCSync for newer laptops).
Note: The reason this quirk cannot replace the kernel driver is because it cannot operate in ACPI S3 mode (sleep
wake) and because the UEFI firmwares provide very limited multicore support preventing the precise update of
the MSR registers.</source>
        <translation>该选项的主要目的是在运行调试XNU内核时在某些服务器和便携式计算机模型上启用早期引导TSC同步。对于调试内核，在任何kext可能导致所有其他解决方案出现问题之前，TSC需要在内核之间保持同步。超时以微秒为单位指定，并取决于平台上存在的内核数量，建议的起始值为500000。这是一个实验性的选项，仅应用于上述问题。在所有其他情况下，该选项可能会使操作系统不稳定，因此不建议这样做。在其他情况下，推荐的解决方案是安装内核驱动程序，例如VoodooTSCSync，TSCAdjustReset或CpuTscSync（VoodooTSCSync的更专业版本，用于较新的笔记本电脑）。
注意：该问题无法替换内核驱动程序的原因是，它无法在ACPI S3模式（睡眠唤醒）下运行，并且因为UEFI固件提供了非常有限的多核支持，从而无法精确更新MSR寄存器。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6238"/>
        <source>Attempts to perform TSC synchronisation with a specified timeout.</source>
        <translation>尝试执行具有指定超时的TSC同步。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6241"/>
        <source>TscSyncTimeout</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6091"/>
        <source>Note: While the option is not supposed to induce harm on unaffected firmwares, its usage is not recommended
when it is not required.</source>
        <translation>注意：虽然该选件不应该对不受影响的固件造成损害，但不建议不要使用该选件。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6095"/>
        <source>Select firmwares, namely APTIO IV, may contain invalid values in MSR_FLEX_RATIO (0x194) MSR
 register. These values may cause macOS boot failure on Intel platforms.</source>
        <translation>某些固件，即APTIO IV，可能在MSR_FLEX_RATIO（0x194）MSR寄存器中包含无效值。 这些值可能会导致Intel平台上的macOS引导失败。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6099"/>
        <source>IgnoreInvalidFlexRatio</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6149"/>
        <source>This quirk requires OC_FIRMWARE_RUNTIME protocol implemented in OpenRuntime.efi. The quirk lets default
boot entry preservation at times when firmwares delete incompatible boot entries. Simply said, you are required
to enable this quirk to be able to reliably use Startup Disk preference pane in a firmware that is not compatible
with macOS boot entries by design.</source>
        <translation>此选项需要在OpenRuntime.efi中实现OC_FIRMWARE_RUNTIME协议。 当固件删除不兼容的启动项时，该选项可以保留默认的启动项。 简而言之，您需要启用此选项才能在设计上与macOS引导项不兼容的固件中可靠地使用“启动磁盘”首选项窗格。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6155"/>
        <source> Request redirect of all Boot prefixed variables from EFI_GLOBAL_VARIABLE_GUID to
 OC_VENDOR_VARIABLE_GUID.</source>
        <translation>请求将所有Boot前缀变量从EFI_GLOBAL_VARIABLE_GUID重定向到OC_VENDOR_VARIABLE_GUID。</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6159"/>
        <source>RequestBootVarRouting</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6268"/>
        <source>DiskInfo</source>
        <oldsource>磁盘信息-DiskInfo</oldsource>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6350"/>
        <source>File</source>
        <oldsource>文件-File</oldsource>
        <translation>文件</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6360"/>
        <source>Resources</source>
        <translation>相关资源</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6392"/>
        <source>关于-About</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6397"/>
        <source>Exit</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="6402"/>
        <location filename="mainwindow.ui" line="6407"/>
        <location filename="mainwindow.cpp" line="4612"/>
        <source>About</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="121"/>
        <source>Last modified): </source>
        <oldsource>最后的编译时间(Last modified): </oldsource>
        <translation>最后修改：</translation>
    </message>
    <message>
        <source>Exceptional MMIO address, which memory descriptor should be left virtualised (unchanged) by DevirtualiseMmio.</source>
        <translation type="obsolete">异常的MMIO地址，该内存描述符应由DevirtualiseMmio进行虚拟化（不变）。</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="517"/>
        <location filename="mainwindow.cpp" line="897"/>
        <source>Mask</source>
        <translation>掩码
Mask</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="464"/>
        <location filename="mainwindow.cpp" line="488"/>
        <location filename="mainwindow.cpp" line="532"/>
        <location filename="mainwindow.cpp" line="568"/>
        <location filename="mainwindow.cpp" line="791"/>
        <location filename="mainwindow.cpp" line="822"/>
        <location filename="mainwindow.cpp" line="864"/>
        <location filename="mainwindow.cpp" line="925"/>
        <location filename="mainwindow.cpp" line="1286"/>
        <location filename="mainwindow.cpp" line="1314"/>
        <location filename="mainwindow.cpp" line="2445"/>
        <source>Enabled</source>
        <translation>是否启用
Enabled</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="506"/>
        <location filename="mainwindow.cpp" line="889"/>
        <source>Find</source>
        <translation>查找
Find</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="510"/>
        <location filename="mainwindow.cpp" line="893"/>
        <source>Replace</source>
        <translation>替换
Replace</translation>
    </message>
    <message>
        <source> Kext bundle path (e.g. Lilu.kext or MyKext.kext/Contents/PlugIns/MySubKext.kext).</source>
        <translation type="obsolete">Kext捆绑包路径（例如Lilu.kext或MyKext.kext / Contents / PlugIns / MySubKext.kext）。</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="5244"/>
        <source>Kext bundle path (e.g. Lilu.kext or MyKext.kext/Contents/PlugIns/MySubKext.kext).</source>
        <translation>Kext捆绑包路径（例如Lilu.kext或MyKext.kext / Contents / PlugIns / MySubKext.kext）。</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1520"/>
        <location filename="mainwindow.cpp" line="1548"/>
        <location filename="mainwindow.cpp" line="1565"/>
        <source>UUID</source>
        <translation>通用唯一标识符
UUID</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="761"/>
        <location filename="mainwindow.cpp" line="835"/>
        <source>BundlePath</source>
        <translation>捆绑路径
BundlePath</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="461"/>
        <location filename="mainwindow.cpp" line="482"/>
        <location filename="mainwindow.cpp" line="514"/>
        <location filename="mainwindow.cpp" line="565"/>
        <location filename="mainwindow.cpp" line="766"/>
        <location filename="mainwindow.cpp" line="810"/>
        <location filename="mainwindow.cpp" line="839"/>
        <location filename="mainwindow.cpp" line="1280"/>
        <location filename="mainwindow.cpp" line="1308"/>
        <location filename="mainwindow.cpp" line="2434"/>
        <source>Comment</source>
        <translation>注释
Comment</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="771"/>
        <location filename="mainwindow.cpp" line="843"/>
        <source>ExecutablePath</source>
        <translation>二进制文件路径
ExecutablePath</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="776"/>
        <location filename="mainwindow.cpp" line="852"/>
        <source>PlistPath</source>
        <translation>Plist路径
PlistPath</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1394"/>
        <location filename="mainwindow.cpp" line="781"/>
        <location filename="mainwindow.cpp" line="814"/>
        <location filename="mainwindow.cpp" line="856"/>
        <location filename="mainwindow.cpp" line="905"/>
        <source>MinKernel</source>
        <translation>最小内核
MinKernel</translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="1378"/>
        <location filename="mainwindow.cpp" line="786"/>
        <location filename="mainwindow.cpp" line="818"/>
        <location filename="mainwindow.cpp" line="860"/>
        <location filename="mainwindow.cpp" line="909"/>
        <source>MaxKernel</source>
        <translation>最大内核
MaxKernel</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="796"/>
        <location filename="mainwindow.cpp" line="825"/>
        <location filename="mainwindow.cpp" line="868"/>
        <location filename="mainwindow.cpp" line="928"/>
        <source>Arch</source>
        <translation>Kext架构
Arch</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="806"/>
        <location filename="mainwindow.cpp" line="847"/>
        <location filename="mainwindow.cpp" line="877"/>
        <location filename="mainwindow.cpp" line="885"/>
        <source>Identifier</source>
        <translation>标识符
Identifier</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="881"/>
        <source>Base</source>
        <translation>基础
Base</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="520"/>
        <location filename="mainwindow.cpp" line="901"/>
        <source>ReplaceMask</source>
        <translation>替换掩码
ReplaceMask</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="451"/>
        <location filename="mainwindow.cpp" line="5432"/>
        <location filename="mainwindow.cpp" line="5438"/>
        <location filename="mainwindow.cpp" line="5443"/>
        <location filename="mainwindow.cpp" line="5448"/>
        <location filename="mainwindow.cpp" line="5453"/>
        <location filename="mainwindow.cpp" line="5458"/>
        <location filename="mainwindow.cpp" line="5463"/>
        <location filename="mainwindow.cpp" line="5468"/>
        <source>Export</source>
        <translation>导出</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="452"/>
        <location filename="mainwindow.cpp" line="5433"/>
        <location filename="mainwindow.cpp" line="5439"/>
        <location filename="mainwindow.cpp" line="5444"/>
        <location filename="mainwindow.cpp" line="5449"/>
        <location filename="mainwindow.cpp" line="5454"/>
        <location filename="mainwindow.cpp" line="5459"/>
        <location filename="mainwindow.cpp" line="5464"/>
        <location filename="mainwindow.cpp" line="5469"/>
        <source>Import</source>
        <translation>导入</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="457"/>
        <location filename="mainwindow.cpp" line="1270"/>
        <location filename="mainwindow.cpp" line="1298"/>
        <source>Path</source>
        <translation>文件
Path</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="471"/>
        <location filename="mainwindow.cpp" line="496"/>
        <source>TableSignature</source>
        <translation>表签名
TableSignature</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="475"/>
        <location filename="mainwindow.cpp" line="499"/>
        <source>OemTableId</source>
        <translation>Oem表
OemTableId</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="478"/>
        <location filename="mainwindow.cpp" line="502"/>
        <source>TableLength</source>
        <translation>表长度
TableLength</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="485"/>
        <source>All</source>
        <translation>全部
All</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="523"/>
        <location filename="mainwindow.cpp" line="913"/>
        <source>Count</source>
        <translation>记数
Count</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="526"/>
        <location filename="mainwindow.cpp" line="917"/>
        <source>Limit</source>
        <translation>限制
Limit</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="529"/>
        <location filename="mainwindow.cpp" line="921"/>
        <source>Skip</source>
        <translation>跳过
Skip</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="561"/>
        <location filename="mainwindow.cpp" line="2430"/>
        <source>Address</source>
        <translation>地址
Address</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="642"/>
        <location filename="mainwindow.cpp" line="664"/>
        <source>PCILists</source>
        <translation>PCI设备列表
PCILists</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="648"/>
        <location filename="mainwindow.cpp" line="1529"/>
        <source>Key</source>
        <translation>键
Key</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="652"/>
        <location filename="mainwindow.cpp" line="669"/>
        <location filename="mainwindow.cpp" line="1533"/>
        <location filename="mainwindow.cpp" line="1555"/>
        <location filename="mainwindow.cpp" line="1571"/>
        <source>Value</source>
        <translation>值
Value</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="655"/>
        <location filename="mainwindow.cpp" line="1536"/>
        <source>Class</source>
        <translation>数据类型
Class</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1273"/>
        <location filename="mainwindow.cpp" line="1301"/>
        <source>Arguments</source>
        <translation>参数
Arguments</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1276"/>
        <location filename="mainwindow.cpp" line="1304"/>
        <source>Name</source>
        <translation>名称
Name</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1283"/>
        <location filename="mainwindow.cpp" line="1311"/>
        <source>Auxiliary</source>
        <translation>辅助
Auxiliary</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1289"/>
        <location filename="mainwindow.cpp" line="1320"/>
        <source>TextMode</source>
        <translation>文本模式
TextMode</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1317"/>
        <source>RealPath</source>
        <translation>实际路径
RealPath</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1971"/>
        <source>AssetTag</source>
        <translation>资产标签
AssetTag</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1974"/>
        <source>BankLocator</source>
        <translation>插槽位置
BankLocator</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1978"/>
        <source>DeviceLocator</source>
        <translation>设备位置
DeviceLocator</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1981"/>
        <source>Manufacturer</source>
        <translation>制造商
Manufacturer</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1984"/>
        <source>PartNumber</source>
        <translation>零件号
PartNumber</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1988"/>
        <source>SerialNumber</source>
        <translation>序号
SerialNumber</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1991"/>
        <location filename="mainwindow.cpp" line="2438"/>
        <source>Size</source>
        <translation>容量
Size</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1994"/>
        <source>Speed</source>
        <translation>频率
Speed</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="5142"/>
        <source>Application</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="5143"/>
        <source>The document has been modified.
Do you want to save your changes?

</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="7042"/>
        <source>ctrl+o</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="7045"/>
        <source>ctrl+s</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="7048"/>
        <source>ctrl+shift+s</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.ui" line="4228"/>
        <location filename="mainwindow.cpp" line="2442"/>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="3534"/>
        <source>错误：</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="5243"/>
        <source>Kext architecture (Any, i386, x86_64).</source>
        <translation>Kext架构（Any，i386，x86_64）。</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="5245"/>
        <source>Comment.</source>
        <translation>注释</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="5246"/>
        <source>This kernel driver will not be added unless set to true.</source>
        <translation>是否启用</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="5247"/>
        <source>Kext executable path relative to bundle (e.g. Contents/MacOS/Lilu).</source>
        <translation>相对于包的Kext可执行路径（例如Contents / MacOS / Lilu）。</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="5248"/>
        <source>Adds kernel driver on specified macOS version or older.</source>
        <translation>在指定的macOS版本或更早版本上添加内核驱动程序。</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="5249"/>
        <source>Adds kernel driver on specified macOS version or newer.</source>
        <translation>在指定的macOS版本或更高版本上添加内核驱动程序。</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="5250"/>
        <source> Kext Info.plist path relative to bundle (e.g. Contents/Info.plist).</source>
        <translation>相对于包的Kext Info.plist路径（例如Contents / Info.plist）。</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="5280"/>
        <source>Save File</source>
        <translation>保存文件</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="5280"/>
        <location filename="mainwindow.cpp" line="5346"/>
        <source>Config file(*.plist);;All files(*.*)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="5346"/>
        <source>Open File</source>
        <translation>打开文件</translation>
    </message>
    <message>
        <source>File paths meant to be loaded as ACPI tables. Example values include DSDT.aml, SubDir/SSDT-8.aml,SSDT-USBX.aml, etc.</source>
        <translation type="obsolete">文件路径打算作为ACPI表加载。 示例值包括DSDT.aml，SubDir / SSDT-8.aml，SSDT-USBX.aml等。</translation>
    </message>
    <message>
        <source>File paths meant to be loaded as ACPI tables. Example values include DSDT.aml, SubDir/SSDT-8.aml,SSDT-USBX.aml, etc.
%1</source>
        <translation type="obsolete">文件路径打算作为ACPI表加载。 示例值包括DSDT.aml，SubDir / SSDT-8.aml，SSDT-USBX.aml等。％1</translation>
    </message>
    <message>
        <source> Arbitrary ASCII string used to provide human readable reference for the entry. It is implementation defined whether this value is used.</source>
        <translation type="obsolete">用于为条目提供人们可读参考的任意ASCII字符串。 是否使用此值由实现定义。</translation>
    </message>
    <message>
        <source> This ACPI table will not be added unless set to true.</source>
        <translation type="obsolete">除非设置为true，否则不会添加此ACPI表。</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="5091"/>
        <source>Cannot write file %1:
%2.</source>
        <translation></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="5095"/>
        <source>Cannot open file %1 for writing:
%2.</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>RecentFiles</name>
    <message>
        <location filename="recentfiles.cpp" line="28"/>
        <source>Open Recent...</source>
        <translation>最近打开...</translation>
    </message>
</context>
</TS>
