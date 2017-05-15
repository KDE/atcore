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
#include "aprinterplugin.h"
#include "temperature.h"
#include "atcore.h"

#include <QLoggingCategory>
#include <QString>

QString AprinterPlugin::_ok = QStringLiteral("ok");
QString AprinterPlugin::_extruderTemp = QStringLiteral("T:");
QString AprinterPlugin::_bedTemp = QStringLiteral("B:");

Q_LOGGING_CATEGORY(APRINTER_PLUGIN, "org.kde.atelier.core.firmware.aprinter");

QString AprinterPlugin::name() const
{
    return QStringLiteral("Aprinter");
}

AprinterPlugin::AprinterPlugin()
{
    qCDebug(APRINTER_PLUGIN) << name() << " plugin loaded!";
}

void AprinterPlugin::extractTemp(const QString &lastMessage)
{
    //ok B:60.4 /60 T:185.4 /185.0
    //ok B:58.7 /40,err T:... <- on Thermistor Error
    QString err = QStringLiteral(",err");
    QString newMessage = lastMessage;

    if (newMessage.contains(err)) {
        newMessage.replace(err, QString());
    }

    QStringList list = newMessage.split(QChar::fromLatin1(' '));

    if (newMessage.contains(_bedTemp)) {
        // T:185.4 - current temperature
        core()->temperature().setExtruderTemperature(list[3].mid(2).toFloat());
        // /185 - target temperature
        core()->temperature().setExtruderTargetTemperature(list[4].mid(1).toFloat());
        // B:60.4 - current temperature
        core()->temperature().setBedTemperature(list[1].mid(2).toFloat());
        // /60 - target temperature
        core()->temperature().setBedTargetTemperature(list[2].mid(1).toFloat());
    } else {
        // T:185.4 - current temperature
        core()->temperature().setExtruderTemperature(list[1].mid(2).toFloat());
        // /185.0 - target temperature
        core()->temperature().setExtruderTargetTemperature(list[2].mid(1).toFloat());
    }
}

void AprinterPlugin::validateCommand(const QString &lastMessage)
{
    if (lastMessage.contains(_extruderTemp) || lastMessage.contains(_bedTemp)) {
        extractTemp(lastMessage);
    }
    if (lastMessage.contains(_ok)) {
        emit readyForCommand();
    }
}

QByteArray AprinterPlugin::translate(const QString &command)
{
    return command.toLocal8Bit();
}
