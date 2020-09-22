/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2016, 2018 Tomaz Canabrava <tcanabrava@kde.org>
    SPDX-FileCopyrightText: 2016-2018 Chris Rizzitello <rizzitello@kde.org>
    SPDX-FileCopyrightText: 2018 Patrick José Pereira <patrickjp@kde.org>
    SPDX-FileCopyrightText: 2018 Leandro Santiago <leandrosansilva@gmail.com>
*/

#include "ifirmware.h"
#include "atcore.h"

/**
 * @brief The IFirmwarePrivate struct
 * @param parent: parent of this object
 */
struct IFirmware::IFirmwarePrivate {
    AtCore *parent;
    /**
     * @brief command finished string
     */
    static const QString _ok;
};

const QString IFirmware::IFirmwarePrivate::_ok = QStringLiteral("ok");

IFirmware::IFirmware()
    : d(new IFirmwarePrivate)
{
}

void IFirmware::init(AtCore *parent)
{
    setParent(parent);
    d->parent = parent;
    connect(d->parent, &AtCore::receivedMessage, this, &IFirmware::checkCommand);
}

AtCore *IFirmware::core() const
{
    return d->parent;
}

void IFirmware::checkCommand(const QByteArray &lastMessage)
{
    validateCommand(QString::fromLatin1(lastMessage));
}

void IFirmware::validateCommand(const QString &lastMessage)
{
    if (lastMessage.contains(IFirmwarePrivate::_ok)) {
        emit readyForCommand();
    }
}

QByteArray IFirmware::translate(const QString &command)
{
    return command.toLocal8Bit();
}
