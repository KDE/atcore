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
QString MarlinPlugin::_extruderTemp = QStringLiteral("T:");
QString MarlinPlugin::_bedTemp = QStringLiteral("B:");

Q_LOGGING_CATEGORY(MARLIN_PLUGIN, "org.kde.atelier.core.firmware.marlin");

QString MarlinPlugin::name() const
{
    return QStringLiteral("Marlin");
}

float _lastBedTemp;
float _lastExtruderTemp;
float _lastTargetBedTemp;
float _lastTargetExtruderTemp;

MarlinPlugin::MarlinPlugin() :
    _lastBedTemp(0.0f),
    _lastExtruderTemp(0.0f),
    _lastTargetBedTemp(0.0f),
    _lastTargetExtruderTemp(0.0f)
{
    qCDebug(MARLIN_PLUGIN) << name() << " plugin loaded!";
}

void MarlinPlugin::extractTemp(const QString &lastMessage)
{
    QStringList list = lastMessage.split(QChar::fromLatin1(' '));
    bool hasBed = lastMessage.contains(_bedTemp);
    if (list.length() < 5) {
        // T:151.10 E:0 B:63.2
        // T:142.6 E:0 W:?

        if (list[0].indexOf(_extruderTemp) == 0) {
            _lastExtruderTemp = list[0].mid(2).toFloat();
        }
        if (hasBed && list[2].indexOf(_bedTemp) == 0) {
            _lastBedTemp = list[2].mid(2).toFloat();
        }
    } else {
        // ok T:185.4 /185.0 B:60.5 /60.0

        // T:185.4 - current temperature
        _lastExtruderTemp = list[1].mid(2).toFloat();
        // /185.0 - target temperature
        _lastTargetExtruderTemp = list[2].mid(1).toFloat();
        if (hasBed) {
            // B:185.4 - current temperature
            _lastBedTemp = list[3].mid(2).toFloat();
            // /60.0 - target temperature
            _lastTargetBedTemp = list[4].mid(1).toFloat();
        }
    }
    core()->temperature().setExtruderTemperature(_lastExtruderTemp);
    core()->temperature().setExtruderTargetTemperature(_lastTargetExtruderTemp);
    if (hasBed) {
        core()->temperature().setBedTemperature(_lastBedTemp);
        core()->temperature().setBedTargetTemperature(_lastTargetBedTemp);
    }
}

void MarlinPlugin::validateCommand(const QString &lastMessage)
{
    if (lastMessage.contains(_extruderTemp) || lastMessage.contains(_bedTemp)) {
        extractTemp(lastMessage);
    }
    if (lastMessage.contains(_ok)) {
        emit readyForCommand();
    }
}

QByteArray MarlinPlugin::translate(const QString &command)
{
    return command.toLocal8Bit();
}
