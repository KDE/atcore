/* AtCore KDE Libary for 3D Printers
    Copyright (C) <2016>

    Authors:
        Tomaz Canabrava <tcanabrava@kde.org>
        Chris Rizzitello <rizzitello@kde.org>
        Patrick José Pereira <patrickelectric@gmail.com>

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
#include <QLoggingCategory>
#include <QString>

#include "redeemplugin.h"
#include "atcore.h"

QString RedeemPlugin::_ok = QStringLiteral("ok");

Q_LOGGING_CATEGORY(REDEEM_PLUGIN, "org.kde.atelier.core.firmware.redeem")

QString RedeemPlugin::name() const
{
    return QStringLiteral("Redeem");
}

RedeemPlugin::RedeemPlugin()
{
    qCDebug(REDEEM_PLUGIN) << name() << " plugin loaded!";
}

void RedeemPlugin::validateCommand(const QString &lastMessage)
{
    if (lastMessage.contains(_ok)) {
        emit readyForCommand();
    }
}

QByteArray RedeemPlugin::translate(const QString &command)
{
    return command.toLocal8Bit();
}
