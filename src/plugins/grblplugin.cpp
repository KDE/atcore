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
#include <QLoggingCategory>
#include <QRegularExpressionMatch>
#include <QString>
#include "grblplugin.h"

Q_LOGGING_CATEGORY(GRBL_PLUGIN, "org.kde.atelier.core.firmware.grbl")

QString GrblPlugin::name() const
{
    return QStringLiteral("Grbl");
}

GrblPlugin::GrblPlugin()
{
    qCDebug(GRBL_PLUGIN) << name() << " plugin loaded!";
}

void GrblPlugin::validateCommand(const QString &lastMessage)
{
    if (lastMessage.contains(QStringLiteral("ok"))) {
        emit readyForCommand();
    }
}

QByteArray GrblPlugin::translate(const QString &command)
{
    QString temp = command;
    //Match all G and M Commands up until the start of the next G/M command or the end of the string.
    //ex: G28 X Y M1 would capture "G28 X Y" and "M1"
    static const auto regEx = QRegularExpression(QStringLiteral("[GM]\\d+.[^GM]+"));
    QRegularExpressionMatch secondCommand = regEx.match(temp, 1);

    if (secondCommand.hasMatch()) {
        QRegularExpressionMatchIterator commandMatch = regEx.globalMatch(temp);
        temp.clear();
        while (commandMatch.hasNext()) {
            QRegularExpressionMatch t = commandMatch.next();
            temp.append(t.captured());
            if (commandMatch.hasNext()) {
                temp.append(QStringLiteral("\r\n"));
            }
        }
    }
    return temp.toLocal8Bit();
}
