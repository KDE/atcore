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

#include "../src/atcore.h"

class AtCoreTests: public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void testInitState();
    void testPluginDetect();
    void testConnectInvalidDevice();
    void cleanupTestCase();
    void testPluginAprinter_load();
    void testPluginAprinter_validate();
    void testPluginGrbl_load();
    void testPluginGrbl_validate();
    void testPluginMarlin_load();
    void testPluginMarlin_validate();
    void testPluginRepetier_load();
    void testPluginRepetier_validate();
    void testPluginSmoothie_load();
    void testPluginSmoothie_validate();
    void testPluginSprinter_load();
    void testPluginSprinter_validate();
    void testPluginTeacup_load();
    void testPluginTeacup_validate();
    void testPluginTeacup_translate();
private:
    AtCore *core = nullptr;
};
