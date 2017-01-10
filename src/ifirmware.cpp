/* AtCore
    Copyright (C) <2016>

    Authors:
        Tomaz Canabrava <tcanabrava@kde.org>
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
#include "ifirmware.h"
#include "temperature.h"
#include "atcore.h"

struct IFirmwarePrivate {
    AtCore *parent;
};

IFirmware::IFirmware()
    : d(new IFirmwarePrivate)
{
}

void IFirmware::init(AtCore *parent)
{
    d->parent = parent;
    connect(d->parent, &AtCore::receivedMessage, this, &IFirmware::checkCommand);
}

AtCore *IFirmware::core() const
{
    return d->parent;
}

IFirmware::~IFirmware()
{
}

void IFirmware::checkCommand(const QByteArray &lastMessage)
{
    validateCommand(QString::fromLatin1(lastMessage));
}

