/* AtCore Unit Tests
 * SPDX-License-Identifier: GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 * SPDX-FileCopyrightText: 2019 Chris Rizzitello <rizzitello@kde.org>
 */

#include "beddeformtests.h"
#include <QtTest>

void BedDeformTests::initTestCase()
{
    bedDeform = new BedDeform(this);
}

void BedDeformTests::testDecode()
{
    QStringList temp = {QStringLiteral("echo:busy: processing"),
                        QStringLiteral("echo:busy: processing"),
                        QStringLiteral("echo:busy: processing"),
                        QStringLiteral("echo:busy: processing"),
                        QStringLiteral("echo:busy: processing"),
                        QStringLiteral("echo:busy: processing"),
                        QStringLiteral("echo:busy: processing"),
                        QStringLiteral("echo:busy: processing"),
                        QStringLiteral("echo:busy: processing"),
                        QStringLiteral("echo:busy: processing"),
                        QStringLiteral("echo:busy: processing"),
                        QStringLiteral("Bilinear Leveling Grid:"),
                        QStringLiteral("0      1      2"),
                        QStringLiteral("0 +0.255 +0.225 -0.038"),
                        QStringLiteral("1 +0.060 +0.008 -0.255"),
                        QStringLiteral("2 -0.153 -0.245 -0.528"),
                        QString(),
                        QStringLiteral("X:155.00 Y:185.00 Z:12.23 E:0.00 Count X:12400 Y:14800 Z:4720"),
                        QStringLiteral("ok")};
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
