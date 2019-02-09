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

#include <QtTest>

#include "temperaturetests.h"

void TemperatureTests::initTestCase()
{
    temperature = new Temperature(this);
}

void TemperatureTests::cleanup()
{
    temperature->resetData();
}

void TemperatureTests::testDecodeAprinter()
{
    temperature->decodeTemp(QByteArray("ok B:49.06 /55 T:64.78 /215"));
    QVERIFY(temperature->extruderTemperature() == float(64.78));
    QVERIFY(temperature->extruderTargetTemperature() == 215);
    QVERIFY(temperature->bedTemperature() == float(49.06));
    QVERIFY(temperature->bedTargetTemperature() == 55);
}

void TemperatureTests::testDecodeMarlin()
{
    temperature->decodeTemp(QByteArray("ok T:49.74 /60.00 B:36.23 /50.00 @:0 B@:0"));
    QVERIFY(temperature->extruderTemperature() == float(49.74));
    QVERIFY(temperature->extruderTargetTemperature() == 60);
    QVERIFY(temperature->bedTemperature() == float(36.23));
    QVERIFY(temperature->bedTargetTemperature() == 50);
}

void TemperatureTests::testDecodeMarlinCreality()
{
    temperature->decodeTemp(QByteArray("ok T:48.8 /215.0 B:57.5 /70.0 T0:48.8 /0.0 @:0 B@:0"));
    QVERIFY(temperature->extruderTemperature() == float(48.8));
    QVERIFY(temperature->extruderTargetTemperature() == 215);
    QVERIFY(temperature->bedTemperature() == float(57.5));
    QVERIFY(temperature->bedTargetTemperature() == 70);
}

void TemperatureTests::testDecodeRepetier()
{
    temperature->decodeTemp(QByteArray("T:25.47 /230 B:69.42 /80 B@:255 @:0"));
    QVERIFY(temperature->extruderTemperature() == float(25.47));
    QVERIFY(temperature->extruderTargetTemperature() == 230);
    QVERIFY(temperature->bedTemperature() == float(69.42));
    QVERIFY(temperature->bedTargetTemperature() == 80);
}
void TemperatureTests::testDecodeSmoothie()
{
    temperature->decodeTemp(QByteArray("ok T:76.36 /220.0 @0 B:24.1 /60.0 @"));
    QVERIFY(temperature->extruderTemperature() == float(76.36));
    QVERIFY(temperature->extruderTargetTemperature() == 220);
    QVERIFY(temperature->bedTemperature() == float(24.1));
    QVERIFY(temperature->bedTargetTemperature() == 60);

}
void TemperatureTests::testDecodeSprinter()
{
    temperature->decodeTemp(QByteArray("ok T:154 @:0 B:150"));
    QVERIFY(temperature->extruderTemperature() == 154);
    QVERIFY(temperature->bedTemperature() == 150);
}

void TemperatureTests::testDecodeTeacup()
{
    temperature->decodeTemp(QByteArray("T:15.50/210.0 B:46.80/82.0"));
    QVERIFY(temperature->extruderTemperature() == float(15.50));
    QVERIFY(temperature->extruderTargetTemperature() == 210);
    QVERIFY(temperature->bedTemperature() == float(46.80));
    QVERIFY(temperature->bedTargetTemperature() == 82);
}

QTEST_MAIN(TemperatureTests)
