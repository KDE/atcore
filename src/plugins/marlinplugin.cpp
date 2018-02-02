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

#include "marlinplugin.h"
#include "atcore.h"

Q_LOGGING_CATEGORY(MARLIN_PLUGIN, "org.kde.atelier.core.firmware.marlin")

QString MarlinPlugin::name() const
{
    return QStringLiteral("Marlin");
}

MarlinPlugin::MarlinPlugin()
{
    qCDebug(MARLIN_PLUGIN) << name() << " plugin loaded!";
}

void MarlinPlugin::validateCommand(const QString &lastMessage)
{
    if (lastMessage.contains(QStringLiteral("End file list"))) {
        core()->setReadingSdCardList(false);
    } else if (core()->isReadingSdCardList()) {
        // Below is to not add directories
        if (!lastMessage.endsWith(QChar::fromLatin1('/'))) {
            QString fileName = lastMessage;
            fileName.chop(fileName.length() - fileName.lastIndexOf(QChar::fromLatin1(' ')));
            core()->appendSdCardFileList(fileName);
        }
    } else {
        if (lastMessage.contains(QStringLiteral("SD card ok"))) {
            core()->setSdMounted(true);
        } else if (lastMessage.contains(QStringLiteral("SD init fail"))) {
            core()->setSdMounted(false);
        } else if (lastMessage.contains(QStringLiteral("Begin file list"))) {
            core()->setSdMounted(true);
            core()->clearSdCardFileList();
            core()->setReadingSdCardList(true);
        } else if (lastMessage.contains(QStringLiteral("SD printing byte"))) {
            QString temp = lastMessage;
            temp.replace(QStringLiteral("SD printing byte"), QString());
            qlonglong total = temp.mid(temp.lastIndexOf(QChar::fromLatin1('/')) + 1, temp.length() - temp.lastIndexOf(QChar::fromLatin1('/'))).toLongLong();
            if (total) {
                temp.chop(temp.length() - temp.lastIndexOf(QChar::fromLatin1('/')));
                qlonglong remaining = total - temp.toLongLong();
                float progress = float(total - remaining) * 100 / float(total);
                core()->printProgressChanged(progress);
                if (progress >= 100) {
                    core()->setState(AtCore::FINISHEDPRINT);
                    core()->setState(AtCore::IDLE);
                }
            } else {
                core()->setState(AtCore::FINISHEDPRINT);
                core()->setState(AtCore::IDLE);
            }
        }
        if (lastMessage.contains(QStringLiteral("ok"))) {
            emit readyForCommand();
        }
    }
}
