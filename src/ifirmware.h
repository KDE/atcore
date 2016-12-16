/* AtCore
    Copyright (C) <2016>

    Authors:
        Tomaz Canabrava <tcanabrava@kde.org>
        Chris Rizzitello <sithlord48@gmail.com>
        Patrick Pereira <patrickelectrick@gmail.com>

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
#pragma once

#include <QObject>
#include <QString>

#include "katcore_export.h"

class Temperature;
class IFirmwarePrivate;

class KATCORE_EXPORT  IFirmware : public QObject
{
    Q_OBJECT
public:
    IFirmware(QObject *parent = nullptr);
    virtual ~IFirmware();

    /* virtuals, needs to reimplement */
    virtual QString name() const = 0;
    virtual bool readyForNextCommand(const QString &lastMessage) = 0;
    virtual QByteArray translate(const QString &command) = 0;

    /* no need to reimplement */
    Temperature *temperature();

private:
    IFirmwarePrivate *d;
};

Q_DECLARE_INTERFACE(IFirmware, "org.kde.atelier.core.firmware")
