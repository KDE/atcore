/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2016 Tomaz Canabrava <tcanabrava@kde.org>
    SPDX-FileCopyrightText: 2016-2018 Chris Rizzitello <rizzitello@kde.org>
    SPDX-FileCopyrightText: 2016-2018 Patrick José Pereira <patrickjp@kde.org>
    SPDX-FileCopyrightText: 2017 Lays Rodrigues <lays.rodrigues@kde.org>
*/

#include <QLoggingCategory>
#include <QString>

#include "atcore.h"
#include "repetierplugin.h"

Q_LOGGING_CATEGORY(REPETIER_PLUGIN, "org.kde.atelier.core.firmware.repetier")

QString RepetierPlugin::name() const
{
    return QStringLiteral("Repetier");
}

bool RepetierPlugin::isSdSupported() const
{
    return true;
}

RepetierPlugin::RepetierPlugin()
{
    qCDebug(REPETIER_PLUGIN) << name() << " plugin loaded!";
}

void RepetierPlugin::validateCommand(const QString &lastMessage)
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
        if (lastMessage.contains(QStringLiteral("SD card"))) {
            if (lastMessage.contains(QStringLiteral("inserted"))) {
                core()->setSdMounted(true);
            } else if (lastMessage.contains(QStringLiteral("removed"))) {
                core()->setSdMounted(false);
            }
        } else if (lastMessage.contains(QStringLiteral("Begin file list"))) {
            core()->setSdMounted(true);
            core()->setReadingSdCardList(true);
            core()->clearSdCardFileList();
        } else if (lastMessage.contains(QStringLiteral("SD printing byte"))) {
            if (lastMessage.contains(QStringLiteral("SD printing byte 0/0"))) {
                // not printing a file
                return;
            }
            if (core()->state() != AtCore::BUSY) {
                // This should only happen if Attached to an Sd printing machine.
                // Just tell the client were starting a job like normal.
                // For this to work the client should check if sdCardPrintStatus()
                // Upon the Connection to a known firmware with sdSupport
                core()->setState(AtCore::STARTPRINT);
                core()->setState(AtCore::BUSY);
            }
            QString temp = lastMessage;
            temp.replace(QStringLiteral("SD printing byte"), QString());
            qlonglong total = temp.mid(temp.lastIndexOf(QChar::fromLatin1('/')) + 1, temp.length() - temp.lastIndexOf(QChar::fromLatin1('/'))).toLongLong();
            if (total) {
                temp.chop(temp.length() - temp.lastIndexOf(QChar::fromLatin1('/')));
                qlonglong remaining = total - temp.toLongLong();
                float progress = float(total - remaining) * 100 / float(total);
                emit core()->printProgressChanged(progress);
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
