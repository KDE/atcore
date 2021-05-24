<!--
    SPDX-FileCopyrightText: AtCore Authors
    SPDX-License-Identifier: CC-BY-SA-4.0
-->
# AtCore - KDE Print Service for 3DPrinters

AtCore is a API to manage the serial connection between the computer and 3D Printers.<br/>
This project is licensed under [LGPL-2.1] or [LGPL-3.0] or [KDE Accepted LGPL].

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
 - [IRC] - Libera Chat \#kde-atelier
 - [Telegram] - Atelier group
 - [Web] - Our web page
 - Report Bugs to our [Bug Tracker].
 - Support us by making a [Donation]
 - [Contribute] to AtCore.

[IRC]: https://web.libera.chat/
[Telegram]: https://telegram.me/KDEAtelier
[Bug Tracker]: https://bugs.kde.org/enter_bug.cgi?product=Atelier&component=AtCore
[KDE]: https://www.kde.org
[Web]: https://atelier.kde.org
[LGPL-2.1]:https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
[LGPL-3.0]:https://www.gnu.org/licenses/lgpl-3.0.html
[KDE Accepted LGPL]:https://community.kde.org/Policies/Licensing_Policy#LicenseRef-KDE-Accepted-LGPL
[Building and Deploying]: build.md
[Contribute]: ../CONTRIBUTING.md
[Donation]:https://kde.org/donate/?app=atcore
