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

void AtCoreTests::testStateChange()
{
    QList<QVariant> args;
    QSignalSpy sSpy(core, SIGNAL(stateChanged(AtCore::STATES)));
    QVERIFY(sSpy.isValid());

    core->setState(AtCore::CONNECTING);
    args = sSpy.takeFirst();
    QVERIFY(args.at(0).toInt() == AtCore::CONNECTING);

    core->setState(AtCore::IDLE);
    args = sSpy.takeFirst();
    QVERIFY(args.at(0).toInt() == AtCore::IDLE);

    core->setState(AtCore::BUSY);
    args = sSpy.takeFirst();
    QVERIFY(args.at(0).toInt() == AtCore::BUSY);

    core->setState(AtCore::PAUSE);
    args = sSpy.takeFirst();
    QVERIFY(args.at(0).toInt() == AtCore::PAUSE);

    core->setState(AtCore::STOP);
    args = sSpy.takeFirst();
    QVERIFY(args.at(0).toInt() == AtCore::STOP);

    core->setState(AtCore::ERRORSTATE);
    args = sSpy.takeFirst();
    QVERIFY(args.at(0).toInt() == AtCore::ERRORSTATE);

    core->setState(AtCore::STARTPRINT);
    args = sSpy.takeFirst();
    QVERIFY(args.at(0).toInt() == AtCore::STARTPRINT);

    core->setState(AtCore::FINISHEDPRINT);
    args = sSpy.takeFirst();
    QVERIFY(args.at(0).toInt() == AtCore::FINISHEDPRINT);

    core->setState(AtCore::DISCONNECTED);
    args = sSpy.takeFirst();
    QVERIFY(args.at(0).toInt() == AtCore::DISCONNECTED);
}

void AtCoreTests::testSdMountChanged()
{
    QList<QVariant> args;
    QSignalSpy sSpy(core, SIGNAL(sdMountChanged(bool)));
    QVERIFY(sSpy.isValid());

    core->setSdMounted(true);
    args = sSpy.takeFirst();
    QVERIFY(args.at(0).toBool() == true);

    core->setSdMounted(false);
    args = sSpy.takeFirst();
    QVERIFY(args.at(0).toBool() == false);
}

void AtCoreTests::testSdFileList()
{
    QSignalSpy sSpy(core, SIGNAL(sdCardFileListChanged(QStringList)));
    QVERIFY(sSpy.isValid());
    core->appendSdCardFileList(QStringLiteral("FILE1"));
    core->appendSdCardFileList(QStringLiteral("FILE2"));
    core->appendSdCardFileList(QStringLiteral("FILE3"));
    core->appendSdCardFileList(QStringLiteral("FILE4"));
    core->appendSdCardFileList(QStringLiteral("FILE5"));

    QList<QVariant> args = sSpy.takeLast();
    QStringList fileList = {
        QStringLiteral("FILE1"),
        QStringLiteral("FILE2"),
        QStringLiteral("FILE3"),
        QStringLiteral("FILE4"),
        QStringLiteral("FILE5")
    };
    QVERIFY(args.at(0).toStringList() == fileList);

    core->clearSdCardFileList();
    args = sSpy.takeLast();
    QVERIFY(args.at(0).toStringList().isEmpty());
}

void AtCoreTests::testSerialTimerIntervalChanged()
{
    QSignalSpy sSpy(core, SIGNAL(serialTimerIntervalChanged(int)));
    QVERIFY(sSpy.isValid());
    core->setSerialTimerInterval(1000);
    core->setSerialTimerInterval(1000);
    core->setSerialTimerInterval(2000);
    core->setSerialTimerInterval(0);
    QVERIFY(sSpy.count() == 3);
}

void AtCoreTests::testExtruderCountChanged()
{
    QSignalSpy sSpy(core, SIGNAL(extruderCountChanged(int)));
    QVERIFY(sSpy.isValid());
    core->setExtruderCount(1);
    core->setExtruderCount(2);
    core->setExtruderCount(1);
    QVERIFY(sSpy.count() == 2);
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
    QVERIFY(sSpy.count() == 1);
}

void AtCoreTests::testPluginGrbl_translate()
{
    QVERIFY(core->firmwarePlugin()->translate(QStringLiteral("G28")) == "G28");
    QVERIFY(core->firmwarePlugin()->translate(QStringLiteral("M104 S50 G28 X")) == "M104 S50\r\nG28 X");
    QVERIFY(core->firmwarePlugin()->translate(QStringLiteral("G00 G43 H0  Z0.1")) == "G00\r\nG43 H0 Z0.1");
    QVERIFY(core->firmwarePlugin()->translate(QStringLiteral("M6 T0")) == "M6 T0");
    QVERIFY(core->firmwarePlugin()->translate(QStringLiteral("G0 G49 G40 G17 G80 G50 G90")) == "G0\r\nG49\r\nG40\r\nG17\r\nG80\r\nG50\r\nG90");
    QVERIFY(core->firmwarePlugin()->translate(QStringLiteral("G0 S49 XY G40")) == "G0 S49 XY\r\nG40");
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
