# Pre-configured OpenCore Desktop EFI Folders for Intel CPUs
## About
This section includes OpenCore configs for Intel CPUs based on the work of **Gabriel Luchina** who took a lot of time and effort to create EFI folders with configs for each CPU Family listed in Dortania's OpenCore install Guide. I took his base configs and modified them so they work out of the box (hopefully).

## New approach: generating EFIs from `config.plist`
Instead of downloading pre-configured and possibly outdated OpenCore EFI folders from the net or github, you can use OpenCore Auxiliary Tools (OCAT) to generate the whole EFI folder based on the config included in the App's Database. This way, you always have the latest version of OpenCore, the config, kexts and drivers.

### Included Files and Settings
- Base configs for Intel Desktop and High End Desktop CPUs with variations for Dell, Sony, HP and other Board/Chipsets.
- Required SSDT Hotpatches for each CPU family (some are disabled – check before deployment!)
- Necessary Quirks for each CPU Family (also available as Presets inside of OCAT)
- Necessary Device Properties for each platform (mostly Framebuffer Patches for: iGPU only, iGPU+dGPU and GPU only)
- Base-set of Kexts (see chart below)
- `MinDate` and `MinVersion` for the APFS Driver set to `-1`, so all macOS versions are supported.

### Included Kexts (universal, config-based)
Kext|Description
:----|:----
[Lilu.kext](https://github.com/acidanthera/Lilu/releases)|Patch engine required for AppleALC, WhateverGreen, VirtualSMC and many other kexts.
[VirtualSMC.kext](https://github.com/acidanthera/VirtualSMC/releases)|Emulates the System Management Controller (SMC) found on real Macs. Without it macOS won't boot boot.
[WhateverGreen.kext](https://github.com/acidanthera/WhateverGreen/releases)|Used for graphics patching, DRM fixes, board ID checks, framebuffer fixes, etc; all GPUs benefit from this kext.
|[AppleALC](https://github.com/acidanthera/AppleALC/releases)|Kext for enabling native macOS HD audio for unsupported Audio CODECs without filesystem modifications.
[CpuTscSync.kext](https://github.com/acidanthera/CpuTscSync/releases)|For syncing TSC on Intel HEDT and Server mainboards. Without it, macOS may run extremely slow or won't boot at all.

<details>
<summary><strong>Optional Kexts (click to reveal)</strong></summary>

### Optional Kexts
Listed below, you'll find optional kexts for various features and hardware. Add as needed.

#### CPU
Kext|Description
:----|:----
|SMCProcessor.kext|For temperature monitoring for Intel CPUs (Pennryn and newer). Included in [VirtualSMC](https://github.com/acidanthera/VirtualSMC)
|SMCSuperIO.kext|For Fan Speed Monitoring. Included in [VirtualSMC](https://github.com/acidanthera/VirtualSMC)

#### Audio
Kext|Description
:----|:----
[AppleALC.kext](https://github.com/acidanthera/AppleALC/releases)|Used for AppleHDA patching, allowing support for the majority of on-board sound controllers.<br>AMD 15h/16h may have issues with this and Ryzen/Threadripper systems rarely have mic support.
[VoodooHDA.kext](https://sourceforge.net/projects/voodoohda/)|Audio for FX systems and front panel Mic+Audio support for Ryzen system, do not mix with AppleALC.<br>Audio quality is noticeably worse than AppleALC on Zen CPUs.

#### Ethernet
Kext|Description
:----|:----
[IntelMausi.kext](https://github.com/acidanthera/IntelMausi/releases)|Intel's 82578, 82579, I217, I218 and I219 NICs are officially supported.
[AtherosE2200Ethernet.kext](https://github.com/Mieze/AtherosE2200Ethernet/releases)|Required for Atheros and Killer NICs.<br>**Note**: Atheros Killer E2500 models are actually Realtek based, for these systems please use RealtekRTL8111 instead.
[RealtekRTL8111.kext](https://github.com/Mieze/RTL8111_driver_for_OS_X/releases)|For Realtek's Gigabit Ethernet.<br>Sometimes the latest version of the kext might not work properly with your Ethernet. If you see this issue, try older versions.
[LucyRTL8125Ethernet.kext](https://www.insanelymac.com/forum/files/file/1004-lucyrtl8125ethernet/)|For Realtek's 2.5Gb Ethernet.
[SmallTreeIntel82576.kext](https://github.com/khronokernel/SmallTree-I211-AT-patch/releases)| Required for I211 NICs, based off of the SmallTree kext but patched to support I211.<br>Required for most AMD boards running Intel NICs.

#### WiFi and Bluetooth
Kext|Description
:----|:----
[AirportItlwm](https://github.com/OpenIntelWireless/itlwm/releases)|Adds support for a large variety of Intel wireless cards and works natively in recovery thanks to IO80211Family integration.
[IntelBluetoothFirmware](https://github.com/OpenIntelWireless/IntelBluetoothFirmware/releases)|Adds Bluetooth support to macOS when paired with an Intel wireless card.
[AirportBrcmFixup](https://github.com/acidanthera/AirportBrcmFixup/releases)|Used for patching non-Apple/non-Fenvi Broadcom cards, will not work on Intel, Killer, Realtek, etc.<br>For Big Sur see [Big Sur Known Issues](https://dortania.github.io/OpenCore-Install-Guide/extras/big-sur#known-issues) for extra steps regarding AirPortBrcm4360 drivers.
[BrcmPatchRAM](https://github.com/acidanthera/BrcmPatchRAM/releases)|Used for uploading firmware on Broadcom Bluetooth chipset, required for all non-Apple/non-Fenvi Airport cards.

#### Other Kexts
Kext|Description
:----|:----
[NVMeFix](https://github.com/acidanthera/NVMeFix/releases)|Used for fixing power management and initialization on non-Apple NVMe.
[SATA-Unsupported](https://github.com/khronokernel/Legacy-Kexts/blob/master/Injectors/Zip/SATA-unsupported.kext.zip)|Adds support for a large variety of SATA controllers, mainly relevant for laptops which have issues seeing the SATA drive in macOS.<br>We recommend testing without this first.
[AppleMCEReporterDisabler](https://github.com/acidanthera/bugtracker/files/3703498/AppleMCEReporterDisabler.kext.zip)|Useful starting with Catalina to disable the AppleMCEReporter kext which will cause kernel panics on AMD CPUs.<br>Recommended for dual-socket systems (ie. Intel Xeon).
[RestrictEvents](https://github.com/acidanthera/RestrictEvents/releases)|Better experience with unsupported processors like AMD, Disable MacPro7,1 memory warnings and provide upgrade to macOS Monterey via Software Updates when available.
</details>

## Instructions

### 1. Generate a base EFI Folder for the CPU of your choice
- Run OCAuxiliaryTools (OCAT)
- Open the **Database**
- Double-click on a config of your choice
- An EFI Folder will be generated and placed on your Desktop including SSDTs, Kexts, Drivers, Themes and the `config.plist`.

### 2. Modifying the `config.plist` 
After the base EFI has been generated, the `config.plist` *maybe* has to be modified based on the used CPU, GPU, additional hardware, peripherals and SMBIOS.

- Go to the Desktop
- Open the `config.plist` included in `\EFI\OC\` with **OCAT**
- Check the following Settings:
	- **ACPI > Add**: add additional ACPI Tables if your hardware configuration requires them. 2nd to 3rd Gen Intel Core CPUs require `SSDT-PM` (create in Post-Install)
	- **DeviceProperties**:
		- Check if the correct Framebuffer Patch is enabled in `PciRoot(0x0)/Pci(0x2,0x0)` (Configs for Intel Core CPUs usually contain two, one enabled)
		- Add additional PCI paths (if required for your hardware)
	- **Kernel > Add**: Add additional kexts required for your hardware and features (a base-set required for the selected system is already included)
	- **PlatformInfo > Generic**: Generate `SMBIOS` Data for the selected Mac model
	- **NVRAM > Add > 7C436110-AB2A-4BBB-A880-FE41995C9F82**: add additional boot-args if your hardware requires them (see next section)
- Save and deploy the EFI folder (put it on a FAT32 formatted USB flash drive and try booting from it)

### 3. Post-Install: fixing CPU Power Management on Sandy and Ivy Bridge CPUs
2nd and 3rd Gen Intel CPUs use a different method for CPU Power Management. Use [**ssdtPRGen**](https://github.com/Piker-Alpha/ssdtPRGen.sh) to generate a `SSDT-PM.aml` in Post-Install, add it to your `EFI\OC\ACPI` folder and config to get proper CPU Power Management.

You can follow this [**guide**]( https://dortania.github.io/OpenCore-Post-Install/universal/pm.html#sandy-and-ivy-bridge-power-management) to do so.

## Additional `boot-args`
Depending on the combination of CPU, GPU (iGPU and/or dGPU) and SMBIOS, additional `boot-args` may be required. These are not included in the configs and need to be added manually before deploying the EFI folder!

### GPU-specific boot-args
|Boot-arg|Description|
|:------:|-----------|
**`agdpmod=pikera`**|Disables Board-ID checks on AMD Navi GPUs (RX 5000 & 6000 series). Without this you'll get a black screen. Don't use on Navi Cards (i.e. Polaris and Vega).
**`-igfxvesa`**|Disables graphics acceleration in favor of software rendering. Useful if iGPU and dGPU are incompatible or if you are using an NVIDIA GeForce Card and the WebDrivers are outdated after updating macOS, so the display won't turn on during boot.
**`-wegnoegpu`**|Disables all GPUs but the integrated graphics on Intel CPU. Use if GPU is incompatible with macOS. Doesn't work all the time.
**`nvda_drv=1`**|Enable Web Drivers for NVIDIA Graphics Cards (supported up to macOS High Sierra only).
**`nv_disable=1`**|Disables NVIDIA GPUs (***don't*** combine this with `nvda_drv=1`)

### Debugging
|Boot-arg|Description|
|:------:|-----------|
**`-v`**|_V_erbose Mode. Replaces the progress bar with a terminal output with a bootlog which helps resolving issues. Combine with `debug=0x100` and `keepsyms=1`
**`debug=0x100`**|Disables macOS'es watchdog. Prevents the machine from restarting on a kernel panic. That way you can hopefully glean some useful info and follow the breadcrumbs to get past the issues.
**`keepsyms=1`**|Companion setting to `debug=0x100` that tells the OS to also print the symbols on a kernel panic. That can give some more helpful insight as to what's causing the panic itself.
**`dart=0`**|Disables VT-x/VT-d. Nowadays, `DisableIOMapper` Quirk is used instead.
**`cpus=1`**|Limits the number of CPU cores to 1. Helpful in cases where macOS won't boot or install otherwise.
**`npci=0x2000`**/**`npci=0x3000`**|Disables PCI debugging related to `kIOPCIConfiguratorPFM64`. Alternatively, use `npci=0x3000` which also disables debugging of `gIOPCITunnelledKey`. Required when stuck at `PCI Start Configuration` as there are IRQ conflicts related to your PCI lanes. **Not needed if `Above4GDecoding` can be enabled in BIOS**
**`-no_compat_check`**|Disables macOS compatibility check. For example, macOS 11.0 BigSur no longer supports iMac models introduced before 2014. Enabling this allows installing and booting macOS on otherwise unsupported SMBIOS. Downside: you can't install system updates if this is enabled.

## Manual update (optional)
Although these configs are included in OCAT now, they are maintained and updated by me, so the latest versions will always be present in my [**GitHub Repo**](https://github.com/5T33Z0/OC-Little-Translated/tree/main/F_Desktop_EFIs).

To manually update the config plists, do the following:

- Download [**_BaseConfigs.zip**](https://github.com/5T33Z0/OC-Little-Translated/blob/main/F_Desktop_EFIs/_BaseConfigs.zip?raw=true) and extract it
- Copy the Files to the Database Folder inside of the **OCAuxiliaryTools** App:
	- right-click the app and select "Show package contents"
	- browse to `/Contents/MacOS/Database/BaseConfigs/`
	- paste the files
	- restart OCAT

## Notes

- Open the config.plist in a Plist Editor to find additional info
- View `DeviceProperties` to check the included Framebuffer-Patches. Usually, 2 versions are included: one for using the iGPU for driving a Display and a 2nd one for using the iGPU for computational tasks only.
- Depending on your hardware configuration (CPU, Mainboard, Peripherals) you may have to add additional SSDT Hotpatches, boot-args, DeviceProperties and/or Kexts – check before deployment!
- Reference Dortania's OpenCore Install Guide for your CPU family if you are uncertain about certain settings
- For enabling Linux support, you can follow this [guide](https://github.com/5T33Z0/OC-Little-Translated/tree/main/G_Linux).

## References
- **OpenCore Auxiliary Tools**: https://github.com/ic005k/QtOpenCoreConfig
- **OpenCore Bootloader**: https://github.com/acidanthera/OpenCorePkg
- **Base Configs Source**: https://github.com/luchina-gabriel?tab=repositories
- **OpenCore Install Guide**: https://dortania.github.io/OpenCore-Install-Guide
