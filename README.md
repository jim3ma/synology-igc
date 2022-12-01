# Intel igc driver for Synology Kernel 4.4.180

> Backport from Linux Kernel v5.12, commit: https://github.com/torvalds/linux/commit/9f4ad9e425a1d3b6a34617b8ea226d56a119a717

## Known issues

* None

## Compatible devices&NICs manifest

I have updated the most pciids of I225/I226 NICs. All I225/I226 NICs should work normal.

If this igc driver is compatible with your devices, you can reply in [this issue](https://github.com/jim3ma/synology-igc/issues/3).

| Motherboard | NIC Version | Device ID | Ports | Firmware Version |
| --- | --- | --- | --- | --- |
| UGREEN DX4600 | I225-V B3 | 8086:15F3| 2 | 1057:8754 |
| ROG MAXIMUS XIII HERO | I225-V B3 | 8086:15F3 | 2 | 1057:8754 |
| CW N5105-N6005-6SATA | I226-V | 8086:125C | 4 | 2014:8877 |

## Prebuild module

Download from https://github.com/jim3ma/synology-igc/tree/main/igc/releases

Eg: https://github.com/jim3ma/synology-igc/raw/main/igc/releases/igc-geminilake.tgz

Follow load module action: https://github.com/jim3ma/synology-igc#3-load-module

## Build in docker

```
mkdir -p output

# avaliable platform:
#   apollolake
#   broadwell
#   broadwellnk
#   denverton
#   geminilake
#   v1000
# I think the platform is not important for building `igc`, change it as your own
PLATFORM=geminilake

docker run -u 1000 --rm -t -v "${PWD}":/input -v "${PWD}/output":/output fbelavenuto/syno-compiler compile-module ${PLATFORM}
```

The `output/igc.ko` is the module.

And then Follow load module actions: https://github.com/jim3ma/synology-igc#3-load-module

## Build in Synology develop environment

### 1. Setup develop environment

#### Prepare Environment

Refer: https://help.synology.com/developer-guide/getting_started/prepare_environment.html

Assume `platform=geminilake`, `version=7.1`, working directory is `/synology-toolkit`

Now the build env is `/synology-toolkit/build_env/ds.geminilake-7.1`

#### Enter chrooot

```
sudo chroot /synology-toolkit/build_env/ds.geminilake-7.1
```

#### Clone source code

```
cd /usr/src/
git clone https://github.com/jim3ma/synology-igc.git
```

### 2. Build module

```shell
cd /usr/src/synology-igc/
make -C /usr/local/x86_64-pc-linux-gnu/x86_64-pc-linux-gnu/sys-root/usr/lib/modules/DSM-7.1/build M=$PWD modules
```

### 3. Load module

Copy `igc.ko` to target machine

#### Find i225/i226 pci device

```
03:00.0 Ethernet controller: Intel Corporation Ethernet Controller I225-V (rev 03)
```

```
insmod igc.ko debug=16 # enable debug log
echo igc > /sys/bus/pci/devices/0000:03:00.0/driver_override
echo 0000:03:00.0 > /sys/bus/pci/drivers_probe
ip link set up eth1 # in my machine, the nic name is eth1
```

## TODO

1. tc support
2. igc: Enable internal i225 PPS - https://github.com/torvalds/linux/commit/64433e5bf40abf893c7edbc60899bdcdd7c70b76

## History

### 1.3.1

_Date 2022.11.26_
* update ethtool version

### 1.3.0

_Date 2022.11.23_

* fix page refcount not decrease when free page

### 1.2.8

_Date 2022.11.17_

* fix pages leak for IGC_RING_FLAG_RX_3K_BUFFER devices

### 1.2.4 - 1.2.7

Only test for automating build, no code changes

### 1.2.3

_Date 2022.11.16_

This version support i226 with device id 8086:125C, firmware version: 2014:8877

* igc: Remove phy->type checking
* igc: Remove _I_PHY_ID checking

### 1.2.2

_Date 2022.11.15_

* Update defines for compile error

### 1.2.1

_Date 2022.11.03_

* igc: Fix BUG: scheduling while atomic
* igc: Add new device ID I226_LMVP
* igc: Fix suspending when PTM is active

### 1.2.0

_Date 2022.10.27_

Backport fixes from upstream

* igc: Fix TX timestamp support for non-MSI-X platforms
* igc: Fix typo in i225 LTR functions
* igc: Update I226_K device ID
* igc: Reinstate IGC_REMOVED logic and implement it properly
* igc: Fix infinite loop in release_swfw_sync
* igc: Increase timeout value for Speed 100/1000/2500
* igc: Fix use-after-free error during reset
* igc: change default return of igc_read_phy_reg()
* igc: Fix an error handling path in 'igc_probe()'

### 1.1.0

_Date 2022.10.27_

* Support ethtool operations

### 1.0.0

_Date 2022.10.26_

* Backport from Linux Kernel v5.12, commit: 9f4ad9e425a1d3b6a34617b8ea226d56a119a717.