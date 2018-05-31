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
#include <QtTest>
#include <QObject>

#include "../src/core/temperature.h"

class TemperatureTests: public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanup();
    void setExtruderTemperature();
    void setExtruderTargetTemperature();
    void setBedTemperature();
    void setBedTargetTemperature();
    void testDecodeAprinter();
    void testDecodeMarlin();
    void testDecodeRepetier();
    void testDecodeSmoothie();
    void testDecodeSprinter();
    void testDecodeTeacup();
private:
    Temperature *temperature;
};
