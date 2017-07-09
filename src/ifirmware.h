/* AtCore
    Copyright (C) <2016>

    Authors:
        Tomaz Canabrava <tcanabrava@kde.org>
        Chris Rizzitello <sithlord48@gmail.com>
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
#pragma once

#include <QObject>
#include <QString>

#include "atcore_export.h"

class Temperature;
class AtCore;

struct IFirmwarePrivate;

class ATCORE_EXPORT  IFirmware : public QObject
{
    Q_OBJECT
public:
    IFirmware();
    void init(AtCore *parent);
    ~IFirmware() override;

    /* virtuals, needs to reimplement */
    virtual QString name() const = 0;
    virtual QByteArray translate(const QString &command) = 0;
    AtCore *core() const;
private:
    IFirmwarePrivate *d;
    virtual void validateCommand(const QString &lastMessage) = 0;
public slots:
    void checkCommand(const QByteArray &lastMessage);
signals:
    void readyForCommand(void);
};

Q_DECLARE_INTERFACE(IFirmware, "org.kde.atelier.core.firmware")
