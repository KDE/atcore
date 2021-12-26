/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2016-2018 Chris Rizzitello <rizzitello@kde.org>
    SPDX-FileCopyrightText: 2016 Tomaz Canabrava <tcanabrava@kde.org>
    SPDX-FileCopyrightText: 2016-2018 Patrick José Pereira <patrickjp@kde.org>
    SPDX-FileCopyrightText: 2017 Lays Rodrigues <lays.rodrigues@kde.org>
*/

#include "grblplugin.h"
#include <QLoggingCategory>
#include <QRegularExpressionMatch>
#include <QString>

Q_LOGGING_CATEGORY(GRBL_PLUGIN, "org.kde.atelier.core.firmware.grbl")

QString GrblPlugin::name() const
{
    return QStringLiteral("Grbl");
}

GrblPlugin::GrblPlugin()
{
    qCDebug(GRBL_PLUGIN) << GrblPlugin::name() << " plugin loaded!";
}

bool GrblPlugin::isSdSupported() const
{
    return false;
}

void GrblPlugin::validateCommand(const QString &lastMessage)
{
    if (lastMessage.contains(QStringLiteral("ok"))) {
        Q_EMIT readyForCommand();
    }
}

QByteArray GrblPlugin::translate(const QString &command)
{
    QString temp = command;
    // Match all G and M commands followed by one or more digits up to and include the space,
    // if thats followed by a letter capture any non G or M starting text
    // else just grab the digits that follow.
    // ex: G28 X Y M1 would capture "G28 X Y" and "M1"
    static const auto regEx = QRegularExpression(QStringLiteral("[GM]\\d+.(?(?=\\D)[^GM]+|\\d+)?"));

    QRegularExpressionMatch secondCommand = regEx.match(temp, 1);

    if (secondCommand.hasMatch()) {
        QRegularExpressionMatchIterator commandMatch = regEx.globalMatch(temp);
        temp.clear();
        while (commandMatch.hasNext()) {
            QRegularExpressionMatch t = commandMatch.next();
            temp.append(t.captured().simplified());
            if (commandMatch.hasNext()) {
                temp.append(QStringLiteral("\r\n"));
            }
        }
    }
    return temp.toLocal8Bit();
}
