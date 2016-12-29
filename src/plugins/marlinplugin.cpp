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
#include "marlinplugin.h"
#include <QLoggingCategory>
#include <QString>
#include "temperature.h"
#include "atcore.h"

QString MarlinPlugin::_ok = QStringLiteral("ok");
QString MarlinPlugin::_wait = QStringLiteral("wait");
QString MarlinPlugin::_extruderTemp = QStringLiteral("T:");
QString MarlinPlugin::_bedTemp = QStringLiteral("B:");

Q_LOGGING_CATEGORY(MARLIN_PLUGIN, "org.kde.atelier.core.firmware.marlin");

QString MarlinPlugin::name() const
{
    return QStringLiteral("Marlin");
}

MarlinPlugin::MarlinPlugin()
{
    qCDebug(MARLIN_PLUGIN) << name() << " plugin loaded!";
}

void MarlinPlugin::extractTemp(const QString &lastMessage)
{
    // ok T:185.4 /185.0 B:60.5 /60.0
    QStringList list = lastMessage.split(QChar::fromLatin1(' '));

    // T:185.4 - current temperature
    core()->temperature().setExtruderTemperature(list[0].mid(2).toFloat());

    // /185.0 - target temperature
    core()->temperature().setExtruderTargetTemperature(list[1].mid(1).toFloat());

    // B:185.4 - current temperature
    core()->temperature().setBedTemperature(list[2].mid(2).toFloat());

    // /60.0 - target temperature
    core()->temperature().setBedTargetTemperature(list[3].mid(1).toFloat());
}

bool MarlinPlugin::validateCommand(const QString &lastMessage)
{
    if (lastMessage.contains(_extruderTemp) || lastMessage.contains(_bedTemp)) {
        extractTemp(lastMessage);
    } else if (lastMessage.contains(_ok) || lastMessage.contains(_wait)) {
        return true;
    }
    return false;
}

QByteArray MarlinPlugin::translate(const QString &command)
{
    return command.toLocal8Bit();
}

bool MarlinPlugin::readyForNextCommand(const QString &lastMessage)
{
    return validateCommand(lastMessage);
}
