# Intel igc driver for Synology Kernel 4.4.180

> Backport from Linux Kernel v5.12, commit: 9f4ad9e425a1d3b6a34617b8ea226d56a119a717

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