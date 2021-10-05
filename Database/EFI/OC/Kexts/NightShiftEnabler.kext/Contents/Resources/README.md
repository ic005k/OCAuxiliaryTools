NightShiftEnabler
=================

A [Lilu](https://github.com/acidanthera/Lilu) plugin for enabling Night Shift on unsupported Mac models. The plugin is compatible with Lilu 1.5.1 or later and macOS Sierra 10.12.4 or later.
  
#### Historical Note

NightShiftEnabler is a simple successor to a now apparently unmaintained [unlocker plugin](https://github.com/0xFireWolf/NightShiftUnlocker).
  
#### Implementation

The current version uses a [patch](https://pikeralpha.wordpress.com/2017/01/30/4398/) developed by Pike R. Alpha and is based on the "unfair" component of [WhateverGreen](https://github.com/acidanthera/WhateverGreen).
  
#### Configuration

- `-nsedbg` enables debug printing (available in DEBUG binaries)
- `-nseoff` disables the plugin
- `-nsebeta` enables the plugin on macOS versions newer than Catalina
