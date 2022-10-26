## 1.2.0

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

## 1.1.0

_Date 2022.10.27_

* Support ethtool operations

## 1.0.0

_Date 2022.10.26_

* Backport from Linux Kernel v5.12, commit: 9f4ad9e425a1d3b6a34617b8ea226d56a119a717.