/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2016-2018 Chris Rizzitello <rizzitello@kde.org>
    SPDX-FileCopyrightText: 2016 Tomaz Canabrava <tcanabrava@kde.org>
    SPDX-FileCopyrightText: 2016-2018 Patrick José Pereira <patrickjp@kde.org>
    SPDX-FileCopyrightText: 2017 Lays Rodrigues <lays.rodrigues@kde.org>
*/

#include <QLoggingCategory>
#include <QString>

#include "atcore.h"
#include "smoothieplugin.h"

Q_LOGGING_CATEGORY(SMOOTHIE_PLUGIN, "org.kde.atelier.core.firmware.smoothie")

QString SmoothiePlugin::name() const
{
    return QStringLiteral("Smoothie");
}

bool SmoothiePlugin::isSdSupported() const
{
    return false;
}

SmoothiePlugin::SmoothiePlugin()
{
    qCDebug(SMOOTHIE_PLUGIN) << SmoothiePlugin::name() << " plugin loaded!";
}
