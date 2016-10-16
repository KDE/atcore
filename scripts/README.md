Atcore scripts
===================
---

> **Note:**

> - The scripts here is for development use.
> - Please, make sure that you know what are you doing !

#### <i class="icon-file"></i> style.sh

Use the script in <i class="icon-folder-open"></i> **root directory** of the project. 
button in the navigation bar. Take note that all <i class="icon-file"></i> **files** will 
be checked !

> **Note:**

> - Please, style corrections need to be done commit each file class for commit. eg: class.*
> - If you would like to use git hook, use pre-commit.style file.
> - The script use astyle, please make sure it's installed.

#### <i class="icon-file"></i> fakeprinter.py

Run the script to fake a 3D printer, before using it, run **socat.sh**.

#### <i class="icon-file"></i> socat.sh

Create two fake serial devices in */dev/ttyVirtual1* and */dev/ttyVirtual2*, the first will be used by the interface and the second by the **fakeprinter.py** script.

> **Note:**

> - Make sure that socat is installed in your system.
> - Only tested in ArchLinux.