/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2016 Tomaz Canabrava <tcanabrava@kde.org>
    SPDX-FileCopyrightText: 2016-2018 Chris Rizzitello <rizzitello@kde.org>
    SPDX-FileCopyrightText: 2016-2018 Patrick José Pereira <patrickjp@kde.org>
    SPDX-FileCopyrightText: 2017 Lays Rodrigues <lays.rodrigues@kde.org>
*/

#include <QLoggingCategory>
#include <QString>

#include "aprinterplugin.h"
#include "atcore.h"

Q_LOGGING_CATEGORY(APRINTER_PLUGIN, "org.kde.atelier.core.firmware.aprinter")

QString AprinterPlugin::name() const
{
    return QStringLiteral("Aprinter");
}

bool AprinterPlugin::isSdSupported() const
{
    return false;
}

AprinterPlugin::AprinterPlugin()
{
    qCDebug(APRINTER_PLUGIN) << AprinterPlugin::name() << " plugin loaded!";
}
