# Instructions

## Generate EFI Folders using OpenCore Auxiliary Tools

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
	- **ACPI > Add**: add additional ACPI Tabels if your hardware configuration requires them. 2nd to 3rd Gen Intel Core CPUs require `SSDT-PM` (create in Post-Install)
	- **DeviceProperties**:
		- Check if the correct Framebuffer Patch is enabled in `PciRoot(0x0)/Pci(0x2,0x0)` (Configs for Intel Core CPUs usually contain two, one enabled)
		- Add additional PCI paths (if required for your hardware)
	- **Kernel > Add**: Add additional kexts rquired for your hardware and features (a base-set required for the selected system is already included)
	-  **Kernel > Patch**: AMD-only. See chapter "AMD: adjusting CPU Core Count" 
	- **PlatformInfo > Generic**: Generate `SMBIOS` Data for the selected Mac model
	- **NVRAM > Add > 7C436110-AB2A-4BBB-A880-FE41995C9F82**: add additional boot-args if your hardware requires them (see next section)
- Save and deploy the EFI folder (put it on a FAT32 formatted USB flash drive and try booting from it)

**NOTE**: Open the config.plist with a plist editor to find additional information

### Additional `boot-args`
Depending on the combination of CPU, GPU (iGPU and/or dGPU) and SMBIOS, additional `boot-args` may be required. These are not included in the configs and need to be added manually before deploying the EFI folder!

#### GPU-Specific `boot-args`
Parameter|Description
:----|:----
**agdpmod=pikera**|Disables Board-ID checks on Navi GPUs (**RX 5000 Series**). Without it, you'll get a black screen. **Don't use if you don't have a Navi GPU** (ie. Polaris or Vega).
**nvda_drv_vrl=1**|For enabling Nvidia Web Drivers on Maxwell and Pascal cards in Sierra and High Sierra.

#### General purpose `boot-args`
Parameter|Description
:----|:----
**npci=0x2000** or **npci=0x3000**| Disables PCI debugging related to `kIOPCIConfiguratorPFM64`. Alternatively, use `npci=0x3000` which also disables debugging of `gIOPCITunnelledKey`.<br>Required when getting stuck at `PCI Start Configuration` as there are IRQ conflicts related to your PCI lanes. **Not needed if `Above4GDecoding` can be enabled in BIOS**

#### AMD-specific settings: adjusting CPU Core Count 
- In config.plist, search for `algrey - Force cpuid_cores_per_package` 
- There should be 3 Patches with the same name (for various versions of macOS)
- In the `Replace` field, find:
	- B8 **00** 0000 0000 (for macOS 10.13, 10.14)
	- B8 **00** 0000 0000 (for macOS 10.15, 11)
	- BA **00** 0000 0090 (for macOS 12)
	- Replace the **3rd** and **4th** digit with the correct Hex value from the table below:

		|Core Count |Hex Value|
		|:--------:|:-------:|
		| 4 Cores  | `04` |
		| 6 Cores  | `06` |
		| 8 Cores  | `08` |
		| 12 Cores | `0C` |
		| 16 Cores | `10` |
		| 24 Cores | `18` |
		| 32 Cores | `20` |
	- Example: for a 6-Core AMD Ryzen 5600X, the resulting `Replace` value for the 3 patches would be:
		- B8 **06** 0000 0000 (for macOS 10.13, 10.14)
		- BA **06** 0000 0000 (for macOS 10.15, 11)
		- BA **06** 0000 0090 (for macOS 12)

### Intel-specific: Fixing CPU Power Management on Sandy and Ivy Bridge CPUs
2nd and 3rd Gen Intel CPUs use a different method for CPU Power Management. Use [**ssdtPRGen**](https://github.com/Piker-Alpha/ssdtPRGen.sh) to generate a `SSDT-PM.aml` in Post-Install, add it to your `EFI\OC\ACPI` folder and config to get proper CPU Power Management.

You can follow this [**guide**]( https://dortania.github.io/OpenCore-Post-Install/universal/pm.html#sandy-and-ivy-bridge-power-management) to do so.

**NOTES**:

- Open the config.plist in a Plist Editor to find additional info
- View Device Properties to check the included Framebuffer-Patches. Usually, 2 versions are included: one for using the iGPU for driving a Display and a 2nd one for using the iGPU for computational tasks only.
- Depending on your hardware configuration (CPU, Mainboard, Peripherals) you may have to add additional SSDT Hotpatches, boot-args, DeviceProperties and/or Kexts – check before deployment!
- Reference Dortania's OpenCore Install Guide for your CPU family if you are uncertain about certain settings

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
- **Latest versions of the comfigs:**
