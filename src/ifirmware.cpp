/* AtCore
    Copyright (C) <2016>

    Authors:
        Tomaz Canabrava <tcanabrava@kde.org>
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
#include "ifirmware.h"
#include "temperature.h"

class IFirmwarePrivate
{
public:
    IFirmwarePrivate() {}
    Temperature temperature;
};

Temperature *IFirmware::temperature()
{
    return &d->temperature;
}

IFirmware::IFirmware(QObject *parent) : QObject(parent)
    , d(new IFirmwarePrivate)
{
}

IFirmware::~IFirmware()
{
}
