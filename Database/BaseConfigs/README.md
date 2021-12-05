# Preconfigured OpenCore Desktop EFI Folders
This section includes configs based on the work of Gabriel Luchina who took a lot of time and effort to create EFI folders with configs for each CPU Family listed in Dortania's OpenCore install Guide. I took his base configs but modified them so they work out of the box (hopefully).

I changed the following:

- Removed Linux support since most users don't use Linux
- Removed Bootchime support
- Added mandatory Kernel Quirks required to boot macOS
- Added required SSDT Hotpatches for each platform (some are disabled – check before deployment)
- Added a base-set of Kexts (Lilu, VirtualSMC, Whatevergreen and AppleALC)
- Added Device Properties containing additional Framebuffer Patches
- Changed `MinDate` and `MinVersion` for the APFS Driver to `-1`, so all macOS versions are supported. This is the #1 reason why users request help because they can't see their drives in the boot picker if they don't run macOS Big Sur or newer
- Created variations of configs for Dell, Sony, HP and other Board/Chipset variants

## Generate EFI Folders using OpenCore Auxiliary Tools
- Start OCAT
- Open the Database
- Double-click on a config of your choice
- An EFI Folder will be generated and placed on your Desktop including SSDTs, Kexts, Drivers, Themes
- Open the included config.plist and generate SMBIOS data for the selected model
- Add addtional SSDTs, Kexts or Device Properties for your hardware-setup (if neccessary)
- Save

Enjoy your base OpenCore EFI Folder

**NOTES**:

- Open the config.plist in a Plist Editor to find additional info
- View Device Properties to check the included Framebuffer-Patches. Usually, 2 versions are included: one for using the iGPU for driving a Display and a 2nd one for using the iGPU for computational tasks only.
- Depending on your hardware configuration (CPU, Mainboard, Peripherals) you may have to add additional SSDT Hotpatches, DeviceProperties and/or Kexts – check before deployment!
- Reference Dortania's OpenCore Install Guide for your CPU family if you are uncertain about certain settings

## Included Configs

### INTEL

#### High End Desktop
- **X299 Cascade Lake X/W**
	- HEDT_X299_Skylake-X/W_Cascade_Lake-X/W_Dell
	- HEDT_X299_Skylake-X/W_Cascade_Lake-X/W_HP
	- HEDT_X299_Skylake-X/W_Cascade_Lake-X/W
- **X99 Haswell-E and Broadwell-E**
	- HEDT_X99_Haswell-E_iMacPro1,1
	- HEDT_X99_Broadwell-E_iMacPro1,1
- **X79 Sandy Bridge-E and Ivy Bridge-E**
	- HEDT_X79_SandyBridge+IvyBridge-E_iMac6,1_Dell
	- HEDT_X79_SandyBridge+IvyBridge-E_iMac6,1_HP
	- HEDT_X79_SandyBridge+IvyBridge-E_iMac6,1
- **X59 Nehalem and Westmere**
	- HEDT_X59_Nehalem+Westmere_MacPro5,1_Dell
	- HEDT_X59_Nehalem+Westmere_MacPro5,1_HP
	- HEDT_X59_Nehalem+Westmere_MacPro5,1
	- HEDT_X59_Nehalem+Westmere_MacPro6,1_Dell
	- HEDT_X59_Nehalem+Westmere_MacPro6,1_HP
	- HEDT_X59_Nehalem+Westmere_MacPro6,1

#### Intel Core i5/i7/i9
- **11th Gen Rocket Lake**
 	- Desktop_11thGen_Rocket_Lake_iMacPro1,1
 	- Desktop_11thGen_Rocket_Lake_MacPro7,1
- **10th Gen Comet Lake**
	- Desktop_10thGen_Comet_Lake_iMac20,1
	- Desktop_10thGen_Comet_Lake_iMac20,2
- **8th and 9th Gen Coffee Lake**
	- Desktop_8th-9thGen_Coffee_Lake_iMac18,1
	- Desktop_8th-9thGen_Coffee_Lake_iMac18,3
	- Desktop_8th-9thGen_Coffee_Lake_iMac19,1
	- Desktop_8th-9thGen_Coffee_Lake_iMac19,1_Z390
	- Desktop_8th-9thGen_Coffee_Lake_iMac19,2
	- Desktop_8th-9thGen_Coffee_Lake_iMac19,2_Z390
- **7th Gen Kaby Lake**
	- Desktop_7thGen_Kaby_Lake_iMac18,1
	- Desktop_7thGen_Kaby_Lake_iMac18,3 
- **6th Gen Skylake**
	- Desktop_6thGen_Skylake_iMac17,1 
- **5th Gen Broadwell**
	- Desktop_5thGen_Broadwell_iMac16,2
	- Desktop_5thGen_Broadwell_iMac17,1
- **4th Gen Broadwell**
	- Desktop_4thGen_Haswell_iMac14,4
	- Desktop_4thGen_Haswell_iMac15,1 
- **3rd Gen Ivy Bridge**
	- Desktop_3rdGen_Ivy_Bridge_iMac13,1
	- Desktop_3rdGen_Ivy_Bridge_iMac13,2
	- Desktop_3rdGen_Ivy_Bridge_iMac14,4
	- Desktop_3rdGen_Ivy_Bridge_iMac15,1
	- Desktop_3rdGen_Ivy_Bridge_MacPro6,1 
- **2nd Gen Sandy Bridge**
	- Desktop_2ndGen_Sandy_Bridge_iMac12,2
	- Desktop_2ndGen_Sandy_Bridge_MacPro6,1
- **1st Gen Lynnfield and Clarkdale**:
	- Desktop_1stGen_Clarkdale_iMac11,2
	- Desktop_1stGen_Lynnfield_iMac11,1
	- Desktop_1stGen_Lynnfield_Clarkdale_MacPro6,1

### AMD

- **AMD Ryzen and Threadripper**
	- AMD_Ryzen_iMac14,2_Kepler+
	- AMD_Ryzen_iMacPro1,1_RX_Polaris
	- AMD_Ryzen_MacPro6,1_R5/R7R9
	- AMD_Ryzen_MacPro7,1_RX_Polaris
	- AMD_Threadripper_iMac14,2_Kepler+_A520+B550
	- AMD_Threadripper_iMac14,2_Kepler+
	- AMD_Threadripper_iMacPro1,1_RX_Polaris_A520+B550
	- AMD_Threadripper_iMacPro1,1_RX_Polaris
	- AMD_Threadripper_MacPro6,1_R5/R7/R9_A520+B550
	- AMD_Threadripper_MacPro6,1_R5/R7/R9
	- AMD_Threadripper_MacPro7,1_RX_Polaris
 	- AMD_Threadripper_MacPro7,1_RX_Polaris_A520+B550
- **AMD Bulldozer and A-Series**
	- AMD_Bulldozer+Jaguar_iMacPro1,1_Polaris
	- AMD_Bulldozer+Jaguar_MacPro6,1_R5R7R9
	- AMD_Bulldozer+Jaguar_MacPro7,1_Polaris
	- AMD_Bulldozer+Jaguar_MacPro14,2_Kepler+

## Manual Update
Althogh these configs are included in OCAT now, they are maintained and updated by me, so the latest versions will always be present in my [**repo**](https://github.com/5T33Z0/OC-Little-Translated/tree/main/F_Desktop_EFIs).

To manually update the config plists, do the following:

- Download [**_BaseConfigs.zip**](https://github.com/5T33Z0/OC-Little-Translated/blob/main/F_Desktop_EFIs/_BaseConfigs.zip?raw=true) and extract it
- Copy the Files to the Database Folder inside of the **OCAuxiliaryTools** App:
	- right-click the app and select "Show package contents"
	- browse to `/Contents/MacOS/Database/BaseConfigs/`
	- paste the files
	- restart OCAT

## References
- **OpenCore Auxiliary Tools**: https://github.com/ic005k/QtOpenCoreConfig
- **Base Configs**: https://github.com/luchina-gabriel?tab=repositories
- **OpenCore Install Guide**: https://dortania.github.io/OpenCore-Install-Guide
- **OpenCore Bootloader**: https://github.com/acidanthera/OpenCorePkg
