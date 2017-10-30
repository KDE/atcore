/*
    This file is part of the KDE project

    Copyright (C) 2017 Chris Rizzitello <rizzitello@kde.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <algorithm>

#include "atcoretests.h"

void AtCoreTests::initTestCase()
{
    core = new AtCore();
}

void AtCoreTests::cleanupTestCase()
{

}

void AtCoreTests::testInitState()
{
    QVERIFY(core->state() == AtCore::DISCONNECTED);
}

void AtCoreTests::testPluginDetect()
{
    QStringList fwPluginsFound = core->availableFirmwarePlugins();
    QStringList fwPluginsActual = {
        QStringLiteral("aprinter"),
        QStringLiteral("grbl"),
        QStringLiteral("marlin"),
        QStringLiteral("redeem"),
        QStringLiteral("repetier"),
        QStringLiteral("smoothie"),
        QStringLiteral("sprinter"),
        QStringLiteral("teacup")
    };

    std::sort(fwPluginsFound.begin(), fwPluginsFound.end());
    std::sort(fwPluginsActual.begin(), fwPluginsActual.end());
    QVERIFY(fwPluginsFound == fwPluginsActual);
}

void AtCoreTests::testConnectInvalidDevice()
{
    QEXPECT_FAIL("", "Invalid Device Attempt", Continue);
    QVERIFY(core->initSerial(QStringLiteral("/dev/ptyp5"), 9600));
}

void AtCoreTests::testPluginAprinter_load()
{
    core->loadFirmwarePlugin(QStringLiteral("aprinter"));
    QVERIFY(core->firmwarePlugin()->name() == QStringLiteral("Aprinter"));
}

void AtCoreTests::testPluginAprinter_validate()
{
    QSignalSpy sSpy(core->firmwarePlugin(), SIGNAL(readyForCommand()));
    QVERIFY(sSpy.isValid() == true);
    core->firmwarePlugin()->validateCommand(QStringLiteral("ok"));
    core->firmwarePlugin()->validateCommand(QStringLiteral("other text"));
    QVERIFY(sSpy.count() == 1);
}

void AtCoreTests::testPluginGrbl_load()
{
    core->loadFirmwarePlugin(QStringLiteral("grbl"));
    QVERIFY(core->firmwarePlugin()->name() == QStringLiteral("Grbl"));
}

void AtCoreTests::testPluginGrbl_validate()
{
    QSignalSpy sSpy(core->firmwarePlugin(), SIGNAL(readyForCommand()));
    QVERIFY(sSpy.isValid() == true);
    core->firmwarePlugin()->validateCommand(QStringLiteral("ok"));
    core->firmwarePlugin()->validateCommand(QStringLiteral("other text"));
    QVERIFY(sSpy.count() == 2);
}

void AtCoreTests::testPluginMarlin_load()
{
    core->loadFirmwarePlugin(QStringLiteral("marlin"));
    QVERIFY(core->firmwarePlugin()->name() == QStringLiteral("Marlin"));
}

void AtCoreTests::testPluginMarlin_validate()
{
    QSignalSpy sSpy(core->firmwarePlugin(), SIGNAL(readyForCommand()));
    QVERIFY(sSpy.isValid() == true);
    core->firmwarePlugin()->validateCommand(QStringLiteral("ok"));
    core->firmwarePlugin()->validateCommand(QStringLiteral("other text"));
    QVERIFY(sSpy.count() == 1);
}

void AtCoreTests::testPluginRedeem_load()
{
    core->loadFirmwarePlugin(QStringLiteral("redeem"));
    QVERIFY(core->firmwarePlugin()->name() == QStringLiteral("Redeem"));
}

void AtCoreTests::testPluginRedeem_validate()
{
    QSignalSpy sSpy(core->firmwarePlugin(), SIGNAL(readyForCommand()));
    QVERIFY(sSpy.isValid() == true);
    core->firmwarePlugin()->validateCommand(QStringLiteral("ok"));
    core->firmwarePlugin()->validateCommand(QStringLiteral("other text"));
    QVERIFY(sSpy.count() == 1);
}

void AtCoreTests::testPluginRepetier_load()
{
    core->loadFirmwarePlugin(QStringLiteral("repetier"));
    QVERIFY(core->firmwarePlugin()->name() == QStringLiteral("Repetier"));
}

void AtCoreTests::testPluginRepetier_validate()
{
    QSignalSpy sSpy(core->firmwarePlugin(), SIGNAL(readyForCommand()));
    QVERIFY(sSpy.isValid() == true);
    core->firmwarePlugin()->validateCommand(QStringLiteral("ok"));
    core->firmwarePlugin()->validateCommand(QStringLiteral("other text"));
    QVERIFY(sSpy.count() == 1);
}

void AtCoreTests::testPluginSmoothie_load()
{
    core->loadFirmwarePlugin(QStringLiteral("smoothie"));
    QVERIFY(core->firmwarePlugin()->name() == QStringLiteral("Smoothie"));
}

void AtCoreTests::testPluginSmoothie_validate()
{
    QSignalSpy sSpy(core->firmwarePlugin(), SIGNAL(readyForCommand()));
    QVERIFY(sSpy.isValid() == true);
    core->firmwarePlugin()->validateCommand(QStringLiteral("ok"));
    core->firmwarePlugin()->validateCommand(QStringLiteral("other text"));
    QVERIFY(sSpy.count() == 1);
}

void AtCoreTests::testPluginSprinter_load()
{
    core->loadFirmwarePlugin(QStringLiteral("sprinter"));
    QVERIFY(core->firmwarePlugin()->name() == QStringLiteral("Sprinter"));
}

void AtCoreTests::testPluginSprinter_validate()
{
    QSignalSpy sSpy(core->firmwarePlugin(), SIGNAL(readyForCommand()));
    QVERIFY(sSpy.isValid() == true);
    core->firmwarePlugin()->validateCommand(QStringLiteral("ok"));
    core->firmwarePlugin()->validateCommand(QStringLiteral("other text"));
    QVERIFY(sSpy.count() == 1);
}

void AtCoreTests::testPluginTeacup_load()
{
    core->loadFirmwarePlugin(QStringLiteral("teacup"));
    QVERIFY(core->firmwarePlugin()->name() == QStringLiteral("Teacup"));
}

void AtCoreTests::testPluginTeacup_validate()
{
    QSignalSpy sSpy(core->firmwarePlugin(), SIGNAL(readyForCommand()));
    QVERIFY(sSpy.isValid() == true);
    core->firmwarePlugin()->validateCommand(QStringLiteral("ok"));
    core->firmwarePlugin()->validateCommand(QStringLiteral("other text"));
    QVERIFY(sSpy.count() == 1);
}

void AtCoreTests::testPluginTeacup_translate()
{
    QVERIFY(core->firmwarePlugin()->translate(QStringLiteral("G28")) == "G28");
    QVERIFY(core->firmwarePlugin()->translate(QStringLiteral("M109 S50")) == "M104 S50\r\nM116");
    QVERIFY(core->firmwarePlugin()->translate(QStringLiteral("M190 S50")) == "M140 S50\r\nM116");
}

QTEST_MAIN(AtCoreTests)
