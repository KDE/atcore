/* AtCore Unit Tests
 * SPDX-License-Identifier: GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 * SPDX-FileCopyrightText: 2017, 2019 Chris Rizzitello <rizzitello@kde.org>
 * SPDX-FileCopyrightText: 2018 Lays Rodrigues <lays.rodrigues@kde.org>
 * SPDX-FileCopyrightText: 2018 Patrick José Pereira <patrickjp@kde.org>
 */

#include <QObject>

#include "../src/core/temperature.h"

class TemperatureTests : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanup();
    void testDecodeAprinter();
    void testDecodeMarlin();
    void testDecodeMarlinCreality();
    void testDecodeRepetier();
    void testDecodeSmoothie();
    void testDecodeSprinter();
    void testDecodeTeacup();

private:
    Temperature *temperature = nullptr;
};
