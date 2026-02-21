<!--
    SPDX-FileCopyrightText: AtCore Authors
    SPDX-License-Identifier: CC-BY-SA-4.0
-->
# Building And Deploying AtCore
Building AtCore is broken up into two main steps, configuration and building. Deploying AtCore is also covered here.

## Configuration 
In order to configure your build you will need [cmake] and [extra-cmake-modules].

Build Options Are: 
 - -DBUILD_GUI = ( ON | OFF )  Build the test client (Default is OFF)
 - -DBUILD_DOCS = (ON | OFF ) Build the Documentation (Default is OFF)
 - -DBUILD_TESTING = ( ON | OFF ) Build Unittests (Default is ON)
 - -DDEPLOY_PLUGINS_WITH_BINARY ( ON | OFF ) Deploy the plugins to bin/plugins (Default is OFF, Turn on for win/mac)
 
Recommended CMake Command

Linux
```
mkdir build
cmake -S . -B build
```

Mac OS/ Windows
```
cmake -DDEPLOY_PLUGINS_WITH_BINARY=ON CMakeLists.txt
```
## Building
After configuring you should be able to run make in the build directory to build all targets.

```
make
```

## Dependencies
In addition to a working development environment for your system you will need the following to build AtCore
 - qt6-base
 - qt6-serialport
 - qt6-widgets
 - qt6-charts
 
Building The Documentation adds the following dependency:
 - [doxygen]

### Installing Dependencies on Windows and Mac OS
Warning, may be outdated!

#### MacOS
MacOS users could use [homebrew] to install both cmake and cmake-extra-modules using.
```
brew update
brew install cmake
brew tap KDE-mac/kde;
brew install kf5-extra-cmake-modules
```
Then can download and install [Qt]

#### Windows
Windows users could install [chocolatey] and do something like 
```
choco install cmake
choco install ninja
git clone -q git://anongit.kde.org/extra-cmake-modules.git
cd extra-cmake-modules
cmake -G "Ninja" . -DCMAKE_INSTALL_PREFIX= <CMAKE_INSTALL_ROOT>
ninja install
```
Then Download and install [Qt]

## Deploying AtCore
After you build you may wish to deploy AtCore on your system for use
### Linux
From the build dir the command below to install atcore with its plugins to the system (assuming cmake used above)
```
sudo make install
```
### Finding Plugins
AtCore will check each directory below for plugins.

    1. QApplication::applicationDirPath/plugins
    2. QApplication::applicationDirPath/AtCore
    3. QApplication::applicationDirPath/../PlugIns/AtCore
    4. INSTALL_PREFIX/KDE_PLUGIN_DIR/AtCore
    5. QT_Plugin_Path/AtCore
    6. KDE_PLUGIN_DIR/AtCore
    7. BUILDDIR for plugins.

For Locations starting with 'QApplication::applicationDirPath'  atcore will look next to the binary using atcore.
Usually this is in a directory next to the program 
```
atcore_test_GUI\atcore-gui.exe
atcore_test_GUI\AtCore.dll
atcore_test_GUI\plugins\repetier.dll
```

But on Mac OS this is in the app bundle 
```
atcore-gui.app/Contents/MacOS/atcore-gui
atcore-gui.app/Contents/MacOS/AtCore.dylib
atcore-gui.app/Contents/MacOS/plugins/repetier.dylib
```
[Qt]:https://www.qt.io
[doxygen]:http://www.doxygen.nl/
[cmake]:https://cmake.org/
[extra-cmake-modules]: https://anongit.kde.org/extra-cmake-modules
[homebrew]:https://brew.sh/
[chocolatey]:https://chocolatey.org/
