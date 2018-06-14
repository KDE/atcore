/* AtCore KDE Libary for 3D Printers
    Copyright (C) <2016>

    Authors:
        Tomaz Canabrava <tcanabrava@kde.org>
        Chris Rizzitello <rizzitello@kde.org>
        Patrick José Pereira <patrickjp@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <QString>
#include <QLoggingCategory>

#include "teacupplugin.h"
#include "atcore.h"

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
    qCDebug(TEACUP_PLUGIN) << name() << " plugin loaded!";
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
