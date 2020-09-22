/* AtCore Unit Tests
 * SPDX-License-Identifier: GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 * SPDX-FileCopyrightText: 2017 Tomaz Canabrava <tcanabrava@kde.org>
 * SPDX-FileCopyrightText: 2017, 2018 Chris Rizzitello <rizzitello@kde.org>
 * SPDX-FileCopyrightText: 2018 Patrick José Pereira <patrickjp@kde.org>
 */
#include <QObject>

#include "../src/core/atcore.h"
#include "../src/core/seriallayer.h"

class AtCoreTests : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void testInitState();
    void testPluginDetect();
    void testConnectInvalidDevice();
    void cleanupTestCase();
    void testStateChange();
    void testSdMountChanged();
    void testSdFileList();
    void testSerialTimerIntervalChanged();
    void testExtruderCountChanged();
    void testPluginAprinter_load();
    void testPluginAprinter_validate();
    void testPluginGrbl_load();
    void testPluginGrbl_validate();
    void testPluginGrbl_translate();
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
