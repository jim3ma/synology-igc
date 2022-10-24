# Intel igc driver for Synology

> Backport from Linux Kernel v5.12, commit: 9f4ad9e425a1d3b6a34617b8ea226d56a119a717

## Build

### 1. Setup develop environment

```
sudo chroot /synology-toolkit/build_env/ds.geminilake-7.1
```

### 3. Build module

```shell
cd /usr/src/synology-igc/
make -C /usr/local/x86_64-pc-linux-gnu/x86_64-pc-linux-gnu/sys-root/usr/lib/modules/DSM-7.1/build M=$PWD modules
```