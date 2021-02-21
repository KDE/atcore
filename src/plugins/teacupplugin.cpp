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
#include "teacupplugin.h"

Q_LOGGING_CATEGORY(TEACUP_PLUGIN, "org.kde.atelier.core.firmware.teacup")

QString TeacupPlugin::name() const
{
    return QStringLiteral("Teacup");
}

bool TeacupPlugin::isSdSupported() const
{
    return false;
}

TeacupPlugin::TeacupPlugin()
{
    qCDebug(TEACUP_PLUGIN) << TeacupPlugin::name() << " plugin loaded!";
}

QByteArray TeacupPlugin::translate(const QString &command)
{
    QString temp = command;
    if (command.contains(QStringLiteral("M109"))) {
        temp.replace(QStringLiteral("M109"), QStringLiteral("M104"));
        temp.append(QStringLiteral("\r\nM116"));
    } else if (command.contains(QStringLiteral("M190"))) {
        temp.replace(QStringLiteral("M190"), QStringLiteral("M140"));
        temp.append(QStringLiteral("\r\nM116"));
    }
    return temp.toLocal8Bit();
}
