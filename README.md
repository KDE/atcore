![LOGO](https://invent.kde.org/kde/atcore/raw/master/testclient/atcore-gui.png)  AtCore - KDE Print Service for 3DPrinters
----
OS   | Build Status
:---:|:----:
**Linux** | [![Build Status](https://build.kde.org/job/Extragear/job/atcore/job/kf5-qt5%20SUSEQt5.14/badge/icon)](https://build.kde.org/job/Extragear/job/atcore/job/kf5-qt5%20SUSEQt5.14/)
**Windows** | [![Build Status](https://binary-factory.kde.org/job/AtCore_Nightly_win64/badge/icon)](https://binary-factory.kde.org/job/AtCore_Nightly_win64/)
**FreeBSD** | [![Build Status](https://build.kde.org/job/Extragear/job/atcore/job/kf5-qt5%20FreeBSDQt5.14/badge/icon)](https://build.kde.org/job/Extragear/job/atcore/job/kf5-qt5%20FreeBSDQt5.14/)
**MacOs** | [![Build Status](https://binary-factory.kde.org/job/AtCore_Nightly_macos/badge/icon)](https://binary-factory.kde.org/job/AtCore_Nightly_macos/)


AtCore is a API to manage the serial connection between the computer and 3D Printers.<br/>
This project is under [LGPL-2.0]+.

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
CMake should find AtCore and link it to MYAPP using the following in your CMakeLists
```CMake
include (AtCore REQUIRED COMPONATES AtCore )
include (AtCore REQUIRED COMPONATES AtCoreWidgets )
...
target_link_libraries(MYAPP AtCore::AtCore AtCore::AtCoreWidgets ... )
```

## Getting Involved
 - [IRC] - freenode \#kde-atelier
 - [Telegram] - Atelier group
 - [Web] - Our web page
 - Report Bugs to our [Bug Tracker].
 - Support us by making a [Donation]
 - [Contribute] to AtCore.

[IRC]: https://webchat.freenode.net/
[Telegram]: https://telegram.me/KDEAtelier
[Bug Tracker]: https://bugs.kde.org/enter_bug.cgi?product=Atelier&component=AtCore
[KDE]: https://www.kde.org
[Web]: https://atelier.kde.org
[LGPL-2.0]:https://www.gnu.org/licenses/old-licenses/lgpl-2.0.html
[Building and Deploying]: doc/build.md
[Contribute]: CONTRIBUTING.md
[Donation]:https://kde.org/donate/?app=atcore
