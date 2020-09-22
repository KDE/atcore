<!--
    SPDX-FileCopyrightText: AtCore Authors
    SPDX-License-Identifier: CC-BY-SA-4.0
-->
Atcore scripts
===================
---

> **Note:**

> - The scripts here is for development use.
> - Please, make sure that you know what are you doing !

#### <i class="icon-file"></i> fakeprinter.py

Run the script to fake a 3D printer, before using it, run **socat.sh**.

#### <i class="icon-file"></i> socat.sh

Create two fake serial devices in */dev/ttyVirtual1* and */dev/ttyVirtual2*, the first will be used by the interface and the second by the **fakeprinter.py** script.

> **Note:**

> - Make sure that socat is installed in your system.
> - Only tested in ArchLinux.
