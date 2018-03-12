![Logo](testclient/atcore-gui.png) AtCore - KDE Print Service for 3DPrinters
----
**Linux** [![Build Status](https://build.kde.org/job/Extragear%20atcore%20kf5-qt5%20SUSEQt5.9/badge/icon)](https://build.kde.org/job/Extragear%20atcore%20kf5-qt5%20SUSEQt5.9/) **Windows** [![Build Status](https://binary-factory.kde.org/job/atcore-master-win32/badge/icon)](https://binary-factory.kde.org/job/atcore-master-win32/) **MacOs** [![Build Status](https://binary-factory.kde.org/job/atcore-master-macos/badge/icon)](https://binary-factory.kde.org/job/atcore-master-macos/) **FreeBSD** [![Build Status](https://build.kde.org/job/Extragear%20atcore%20kf5-qt5%20FreeBSDQt5.9/badge/icon)](https://build.kde.org/job/Extragear%20atcore%20kf5-qt5%20FreeBSDQt5.9/)

AtCore is a API to manage the serial connection between the computer and 3D Printers.<br/>
This project is under LGPL V2+, for more information read ![COPYING.TXT](COPYING.TXT).

----
## Supported Firmwares
Currenty the following firmwares are supported.

 Firmware Name |Basic Use| Control SD
 :------------:|:-------:|:----------:
 Repetier      | YES     | YES
 Marlin        | YES     | YES
 Teacup        | YES     | NO
 APrinter      | YES     | NO
 SPrinter      | YES     | NO
 Smoothie      | YES     | NO
 GRBL          | YES     | NO

## Building  Deploying  AtCore
 See: [Building and Deploying]

## Importing AtCore in your CMakeList
CMake should find AtCore using the following in your CMakeLists
```CMake
include (AtCore REQUIRED COMPONATES AtCore )
```

## Getting Involved
 - [IRC] - freenode \#kde-atelier
 - [Telegram] - Atelier group
 - [Web] - Our web page
 - Report Bugs to our [Bug Tracker].
 - Support us by making a [Donation]
 - [Contribute] to AtCore.

[IRC]: https://webchat.freenode.net/
[Telegram]: telegram.me/KDEAtelier
[Bug Tracker]: https://bugs.kde.org/enter_bug.cgi?product=Atelier&component=AtCore
[KDE]:https://www.kde.org
[Web]: https://atelier.kde.org
[LGPL-2.0]:https://www.gnu.org/licenses/old-licenses/lgpl-2.0.html
[Building and Deploying]:doc/build.md
[Contribute]:doc/contrib.md
[Donation]:https://kde.org/donate/?app=atcore

