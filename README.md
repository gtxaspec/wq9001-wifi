# WQ9001 Linux Driver

## Overview
The WQ9001 is a high-end Wi-Fi 4 SoC chip that integrates 2.4 GHz RF, PHY & MAC, RISC-V CPU, OTP, USB interface, and power management module. The chip provides IEEE802.11 b/g/n compatibility.  

For more information, visit the [WuQi WQ9001 product page](https://www.wuqi-micro.com/en/product-center/digital-wifi-chip/5).

## Features
- IEEE 802.11a/b/g/n support
- 1x1 2.4 GHz RF
- Maximum PHY rate of 72 Mbps 
- Integrated PA, LNA, and TR switch
- High-performance RISC-V CPU
- USB 2.0 interface

## Building the Driver

### Compilation

To compile the driver, use:

```bash
make KSRC=/path/to/your/kernel/source \
     CROSS_COMPILE=mipsel-linux- \
     ARCH=mips \
     WIFI_DRIVER_DIR=$PWD
```

Example:
```bash
make KSRC=/home/user/kernel/linux-3.10 \
     CROSS_COMPILE=mipsel-linux- \
     ARCH=mips \
     WIFI_DRIVER_DIR=$PWD
```

### Installation
After successful compilation, the driver module `hawk_usb.ko` will be created. Copy this to your target device and load it.
