![Logo](testclient/AtCoreTest.png) AtCore - KDE Print Service for 3DPrinters
----
#### Build Status
Linux [![Build Status](https://build.kde.org/job/Extragear%20atcore%20kf5-qt5%20SUSEQt5.9/badge/icon)](https://build.kde.org/job/Extragear%20atcore%20kf5-qt5%20SUSEQt5.9/) Windows [![Build Status](https://binary-factory.kde.org/job/atcore-master-win32/badge/icon)](https://binary-factory.kde.org/job/atcore-master-win32/) MacOs [![Build Status](https://binary-factory.kde.org/job/atcore-master-macos/badge/icon)](https://binary-factory.kde.org/job/atcore-master-macos/) FreeBSD [![Build Status](https://build.kde.org/job/Extragear%20atcore%20kf5-qt5%20FreeBSDQt5.9/badge/icon)](https://build.kde.org/job/Extragear%20atcore%20kf5-qt5%20FreeBSDQt5.9/)

----

AtCore is a API to manage the serial connection between the computer and 3D Printers.<br/>
This project is under LGPL V2+, for more information read COPYING.TXT.

----
### AtCore is under development! Please have that in mind while testing!
----
#### Supported Firmwares
 - Repetier
 - Marlin
 - Teacup
 - APrinter
 - SPrinter
 - Smoothie
 - Grbl
 ---
#### Getting AtCore

Install a Premade package for ArchLinux:
 - AUR [atcore-git](https://aur.archlinux.org/packages/atcore-git/)
---
#### Building AtCore For Development
##### Minimal Qt Version: 5.7

Dependencies for AtCore:
 - qt5-base
 - qt5-serialport
 - extra-cmake-modules

Extra Dependencies for Test GUI
 - qt5-charts

Optional Dependencies
 - doxygen

Build Switches
 - -DBUILD_TEST_GUI = ( ON | OFF )  Build the test client (Default is ON)

----
#### Building on Linux
```bash
$ git clone https://github.com/KDE/atcore.git
$ cmake -DCMAKE_INSTALL_PREFIX=$(qtpaths --install-prefix) -DCMAKE_INSTALL_LIBDIR=lib CMakeLists.txt
$ make
$ make install
```
----
#### Building on Windows

For Windows build you need to set up [Craft](https://community.kde.org/Guidelines_and_HOWTOs/Build_from_source/Windows)
It's tricky, but with all the last changes in half an hour you can have the setup done.
Remember, it's easier if you have Python3.6 or higher installed.
After you have the Craft environment setup do:
```powershell
craft atcore
```
This will dowload all Qt and KDE libraries needed and compile Atcore.
For running, inside the prompt call:
```powershell
AtCoreTest
```
For debugging run:
```powershell
craft vsd
vsd AtCoreTest
```
This will give you feedback on the prompt to check what is happening.

For packing:
```powershell
craft --package atcore
```
At C:/KDE/craft/tmp you will find the installer.

#### Building on Mac OS
Install:
 - Git
 - QtCreator
 - XCode
 - CMake
 - Extra-Cmake-Modules via [Craft](https://community.kde.org/Guidelines_and_HOWTOs/Build_from_source/Mac)
Open a terminal and run:
```bash
$ git clone https://github.com/KDE/atcore.git
```
Now you open QtCreator and open AtCore like project selecting the CMakeLists.txt on the root of AtCore folder.
Setup your development environment:
 - Set your CMAKE_C_COMPILER to /usr/bin/cc
 - Set your ECM_DIR to your ECM install directory.

Now you can build AtCore.
After build do:
 - Copy the plugins files from <buildpath>/src/plugins/*.dylib to AtCoreTest.app/Contents/MacOS/plugins
 - Run macdeployqt on the app

### Docker
We have a docker container within this project. To use it on your Docker environment just pull it:
```bash
$ docker pull lays147/atcore
```
To run it on Linux:
```bash
$ xhost: local:root
$ docker run --rm -it -v /tmp/.X11-unix:/tmp/.X11-unix -e DISPLAY --device DEVICE lays147/atcore:latest
```

You need to use the --device option to give the container access to your printer. It must point to the printers serial device.
Overriding DEVICE value to the port of your printer.
<br/>
Example: ```--device /dev/ttyUSB0```

### Testing AtCore

Testing is simple and will help us a lot.
  - Build / Install the testclient
  - Connect to your printer
  - Test the controls in the testclient
  - Test Print Command
  - Save your log and send it with errors
  
---
 > **Note:**
 > For your test results to be helpful they must include the full log output.

---
#### Getting in Touch
You can reach us via: <br/>
[IRC] - freenode #kde-atelier <br/>
[Telegram] - KDE Atelier group <br/>
[Web] - Our web page
#### Bugs
For any bug that you find it you can go on [KDE Bugs] system and report it. Please try to give all the information about the issue, and backtrace if you have one.

[IRC]: https://webchat.freenode.net/
[Telegram]: telegram.me/KDEAtelier
[KDE Bugs]: https://bugs.kde.org/enter_bug.cgi?product=Atelier
[Web]: https://atelier.kde.org
