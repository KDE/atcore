/* AtCore Unit Tests
 * SPDX-License-Identifier: GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 * SPDX-FileCopyrightText: 2019 Chris Rizzitello <rizzitello@kde.org>
 */

#include <QObject>

#include "../src/core/beddeform.h"

class BedDeformTests : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void testDecode();

private:
    BedDeform *bedDeform;
};
