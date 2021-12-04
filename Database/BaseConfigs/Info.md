# Preconfigured OpenCore Desktop EFI Folders

## Generate EFI Folders using OpenCore Auxiliary Tools

- Run OpenCore Auxiliary Tools
- Open the Database
- Double-click on a config of your choise
- The generated EFI Folder is placed on your desktop
- Open the config.plist and generate SMBIOS data for the selected model
- Save

Enjoy your base OpenCore EFI Folder

**NOTES**:

- Open the config.plist in a Plist Editor to find additional info.
- View Device Properties to check the included Framebuffer-Patches and select another one if it fits your setup better. Usually, 2 versions are included: one für using the iGPU for driving a Display and a 2nd one for using the iGPU for computational tasks only.
- Depending in your hardware configuration (CPU, Mainboard, Peripherals) you may have to add more SSDT Hotpatches and/or Kexts.

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
	- AMD_Threadripper_iMac14,2_Kepler+_A520+B550plist
	- AMD_Threadripper_iMac14,2_Kepler+
	- AMD_Threadripper_iMacPro1,1_RX_Polaris_A520+B550
	- AMD_Threadripper_iMacPro1,1_RX_Polaris
	- AMD_Threadripper_MacPro6,1_R5/R7/R9_A520+B550
	- AMD_Threadripper_MacPro6,1_R5/R7/R9
- **AMD Bulldozer and A-Series**
	- AMD_Bulldoze+Jaguar_iMacPro1,1_Polaris
	- AMD_Bulldoze+Jaguar_MacPro6,1_R5R7R9
	- AMD_Bulldoze+Jaguar_MacPro7,1_Polaris
	- AMD_Bulldoze+Jaguar_MacPro14,2_Kepler+ 

## References
- **OpenCore Install Guide:** https://dortania.github.io/OpenCore-Install-Guide
- **Base Configs**: https://github.com/luchina-gabriel?tab=repositories
- **Additional Files/Settings**: https://github.com/5T33Z0/OC-Little-Translated
