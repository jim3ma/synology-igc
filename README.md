# Intel igc driver for Synology Kernel 4.4.180

> Backport from Linux Kernel v5.12, commit: https://github.com/torvalds/linux/commit/9f4ad9e425a1d3b6a34617b8ea226d56a119a717

## Compatible devices&NICs list

Not all i225/i226 NIC is working.
My device is UGREEN DX4600 with i225v b3, the igc driver is working normally.

If this igc driver is compatible with your devices, you can reply in [this issue](https://github.com/jim3ma/synology-igc/issues/3).

| Device Name | NIC version | Ports |
| --- | --- | --- |
| UGREEN DX4600 | i225v b3 | 2 |
| ROG MAXIMUS XIII HERO | i225v b3 | 2 |

## Prebuild module

Download from https://github.com/fbelavenuto/arpl-modules

Eg: https://github.com/fbelavenuto/arpl-modules/blob/main/geminilake-4.4.180/igc.ko

Follow load module action: https://github.com/jim3ma/synology-igc#3-load-module

## Build

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