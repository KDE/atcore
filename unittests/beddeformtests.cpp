/*
    This file is part of the KDE project

    Copyright (C) 2019 Chris Rizzitello <rizzitello@kde.org>

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
#include "beddeformtests.h"

void BedDeformTests::initTestCase()
{
    bedDeform = new BedDeform(this);
}

void BedDeformTests::testDecode()
{
    QStringList temp = {
        QStringLiteral("echo:busy: processing")
        , QStringLiteral("echo:busy: processing")
        , QStringLiteral("echo:busy: processing")
        , QStringLiteral("echo:busy: processing")
        , QStringLiteral("echo:busy: processing")
        , QStringLiteral("echo:busy: processing")
        , QStringLiteral("echo:busy: processing")
        , QStringLiteral("echo:busy: processing")
        , QStringLiteral("echo:busy: processing")
        , QStringLiteral("echo:busy: processing")
        , QStringLiteral("echo:busy: processing")
        , QStringLiteral("Bilinear Leveling Grid:")
        , QStringLiteral("0      1      2")
        , QStringLiteral("0 +0.255 +0.225 -0.038")
        , QStringLiteral("1 +0.060 +0.008 -0.255")
        , QStringLiteral("2 -0.153 -0.245 -0.528")
        , QString()
        , QStringLiteral("X:155.00 Y:185.00 Z:12.23 E:0.00 Count X:12400 Y:14800 Z:4720")
        , QStringLiteral("ok")
    };
    bedDeform->decodeDeform(temp);
    QVERIFY(bedDeform->bedDeformationGrid().at(0).toList().at(0).toDouble() == 0.255);
    QVERIFY(bedDeform->bedDeformationGrid().at(0).toList().at(1).toDouble() == 0.225);
    QVERIFY(bedDeform->bedDeformationGrid().at(0).toList().at(2).toDouble() == -0.038);
    QVERIFY(bedDeform->bedDeformationGrid().at(1).toList().at(0).toDouble() == 0.060);
    QVERIFY(bedDeform->bedDeformationGrid().at(1).toList().at(1).toDouble() == 0.008);
    QVERIFY(bedDeform->bedDeformationGrid().at(1).toList().at(2).toDouble() == -0.255);
    QVERIFY(bedDeform->bedDeformationGrid().at(2).toList().at(0).toDouble() == -0.153);
    QVERIFY(bedDeform->bedDeformationGrid().at(2).toList().at(1).toDouble() == -0.245);
    QVERIFY(bedDeform->bedDeformationGrid().at(2).toList().at(2).toDouble() == -0.528);
}

QTEST_MAIN(BedDeformTests)
