/* AtCore KDE Libary for 3D Printers
    Copyright (C) <2016>

    Authors:
        Tomaz Canabrava <tcanabrava@kde.org>
        Chris Rizzitello <sithlord48@gmail.com>
        Patrick José Pereira <patrickelectric@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "sprinterplugin.h"
#include "temperature.h"
#include "atcore.h"

#include <QLoggingCategory>
#include <QString>

QString SprinterPlugin::_ok = QStringLiteral("ok");

Q_LOGGING_CATEGORY(SPRINTER_PLUGIN, "org.kde.atelier.core.firmware.sprinter");

QString SprinterPlugin::name() const
{
    return QStringLiteral("Sprinter");
}

SprinterPlugin::SprinterPlugin()
{
    qCDebug(SPRINTER_PLUGIN) << name() << " plugin loaded!";
}

void SprinterPlugin::validateCommand(const QString &lastMessage)
{
    if (lastMessage.contains(_ok)) {
        emit readyForCommand();
    }
}

QByteArray SprinterPlugin::translate(const QString &command)
{
    return command.toLocal8Bit();
}

