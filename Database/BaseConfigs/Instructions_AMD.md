**Introduction:**

**macOS Monterey 12.3 new beta may need to install this kext AppleMCEReporterDisabler.kext**

**AMD** platform is not currently supported by Apple, there is no official support not even from the developers of the main bootloader (OpenCore), but despite this it is possible to have a hackintosh based on the **AMD** platform and OSX from **macOS High Sierra till Monterey 12.3.x** thanks to the unofficial support of the developers and to the strong commitment of the **AMD** community.

# Instructions

#### Generate EFI Folder using OpenCore Auxiliary Tools

#### 1. Generate a base EFI Folder for the CPU of your choice
- Run OCAuxiliaryTools (OCAT)
- Open the **Database**
- Double-click on an AMD plist
- An EFI Folder will be generated and placed on your Desktop including SSDTs, Kexts, Drivers, Themes and the `config.plist`.

#### 2. Modifying the `config.plist`
After the base EFI has been generated, `config.plist` must be modified according on the used CPU, GPU, additional hardware, peripherals, SMBIOS and BIOS.

Important section are **Kernel > Patch** and **Booter > MMIOWhitelist**

**AMD-X370_generic.plist** is configured by default for a 8 cores cpu, if you have an Amd Ryzen with 8 cores you can leave **Kernel > Patch** config.plist section unchanged.
If you own an Amd Ryzen with different core number you will have to modify **Kernel > Patch** config.plist section, an easy way is to delete the patches present here and get the correct ones from the presets adding correct plist for your core number in **Kernel > Patch**

**AMD-X470_generic.plist** is configured by default for a 8 cores cpu, if you have an Amd Ryzen with 8 cores you can leave **Kernel > Patch** config.plist section unchanged.
If you own an Amd Ryzen with different core number you will have to modify **Kernel > Patch** config.plist section, an easy way is to delete the patches present here and get the correct ones from the presets adding correct plist for your core number in **Kernel > Patch**

**AMD-B450M_generic.plist** is configured by default for a 8 cores cpu, if you have an Amd Ryzen with 8 cores you can leave **Kernel > Patch** config.plist section unchanged.
If you own an Amd Ryzen with different core number you will have to modify **Kernel > Patch** config.plist section, an easy way is to delete the patches present here and get the correct ones from the presets adding correct plist for your core number in **Kernel > Patch**

**AMD-B550_generic.plist** is configured by default for a 12 cores cpu, if you have an Amd Ryzen with 12 cores you can leave **Kernel > Patch** config.plist section unchanged.
If you own an Amd Ryzen with different core number you will have to modify **Kernel > Patch** config.plist section, an easy way is to delete the patches present here and get the correct ones from the presets adding correct plist for your core number in **Kernel > Patch**

**AMD-X570_generic.plist** is configured by default for a 16 cores cpu, if you have an Amd Ryzen with 16 cores you can leave **Kernel > Patch** config.plist section unchanged.
If you own an Amd Ryzen with different core number you will have to modify **Kernel > Patch** config.plist section, an easy way is to delete the patches present here and get the correct ones from the presets adding correct plist for your core number in **Kernel > Patch**

**AMD-X399_generic.plist** is configured by default for a 24 cores cpu, if you have an Amd Threadripper with 24 cores you can leave **Kernel > Patch** config.plist section unchanged.
If you own an Amd ThreadRipper with different core number you will have to modify **Kernel > Patch** config.plist section, an easy way is to delete the patches present here and get the correct ones from the presets adding correct plist for your core number in **Kernel > Patch**

**generic_trx40.plist** is configured by default for a 32 cores cpu, if you have an Amd ThreadRipper 3970x or a Amd ThreadRipper 3990x you can leave **Kernel > Patch** config.plist section unchanged.
If you own an Amd ThreadRipper 3960x you will have to modify **Kernel > Patch** config.plist section, an easy way is to delete the patches present here and get the correct ones from the preset sTRX4-AMD 3960x adding in **Kernel > Patch**

#### 3. Create a  proper `MMIO Whitelist`
The creation of a valid MMIO Whitelist is certainly the most important part for the correct functioning of an AMD ThreadRipper sTRX4 solution.
Complete information for **How to** create a proper `MMIO Whitelist` is available at the following link:
https://www.macos86.it/topic/3307-discussion-trx40-bare-metal-vanilla-patches/?do=findComment&comment=85469

#### 4. sTrx4/Ryzen `adviced SMBIOS` and basic config.plist options

- iMacPro1.1
- MacPro7.1
- Check the following Settings:
	- **ACPI > Add**: 3rd Gen Threadripper and new AMD chipset does not require any particular add in this section,  you could add in post install stage `SSDT-PLUG`,  and others SSDT to have proper USB power or GFX0 for GPU
	Some manufactured motherboard as ASUS may have the need of a fake EC device (use a proper `SSDT-EC` if you need to boot)
	Gigabyte users may have the need to disable `Wake on lan`on their BIOS.
	- **DeviceProperties**:
		 Add additional PCI paths (Usually not required for any hardware)
	- **Kernel > Add**: Add additional kexts required for your hardware and features (base-set required for the selected system is already included and it includes Lilu.kext and VirtualSMC.kext or only FakeSMC.kext)
	-  **Kernel > Patch**: AMD-only. Use included OCAT preset according to the number of cores of your CPU.
	- **PlatformInfo > Generic**: Generate `SMBIOS` Data for the selected Mac model
	- **Quirks > All Quirks config.plist sections**: Used include OCAT presets

Save the EFI folder (put it on a FAT32 formatted USB flash drive and try booting from it)

**NOTE**: Open the config.plist with a plist editor to find additional information

#### Additional

#### Mandatory AMD_Ryzen-specific settings `(NOT for sTRX4 Users)`: _mtrr_update_action fix PAT
In config.plist, search for `algrey - _mtrr_update_action fix PAT - 10.13/10.14/10.15/11.0/12.0` or for `Shaneee - _mtrr_update_action fix PAT - 10.13/10.14/10.15/11.0/12.0`

**Only one of these should be enabled.**
Usually algrey ones is safer but slower (GPU fps Performance), Shaneee ones could produce some HDMI/DP audio problems but it is faster (GPU fps performance).

#### References
- **OpenCore Auxiliary Tools**: https://github.com/ic005k/QtOpenCoreConfig
- **OpenCore Bootloader**: https://github.com/acidanthera/OpenCorePkg
- **AMD OSX Vanilla patches** https://github.com/AMD-OSX/AMD_Vanilla
- **AMD STRX4 and AMD Ryzen discussion:** https://www.macos86.it/topic/3307-discussion-trx40-bare-metal-vanilla-patches/
