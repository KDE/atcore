/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2016-2018 Chris Rizzitello <rizzitello@kde.org>
    SPDX-FileCopyrightText: 2016 Tomaz Canabrava <tcanabrava@kde.org>
    SPDX-FileCopyrightText: 2016-2018 Patrick José Pereira <patrickjp@kde.org>
    SPDX-FileCopyrightText: 2017 Lays Rodrigues <lays.rodrigues@kde.org>
*/

#include <QLoggingCategory>
#include <QString>

#include "atcore.h"
#include "marlinplugin.h"

Q_LOGGING_CATEGORY(MARLIN_PLUGIN, "org.kde.atelier.core.firmware.marlin")

QString MarlinPlugin::name() const
{
    return QStringLiteral("Marlin");
}

bool MarlinPlugin::isSdSupported() const
{
    return true;
}

MarlinPlugin::MarlinPlugin()
{
    qCDebug(MARLIN_PLUGIN) << MarlinPlugin::name() << " plugin loaded!";
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
                Q_EMIT core()->printProgressChanged(progress);
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
            Q_EMIT readyForCommand();
        }
    }
}
