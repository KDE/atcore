/* AtCore
    Copyright (C) <2016>

    Authors:
        Tomaz Canabrava <tcanabrava@kde.org>
        Patrick José Pereira <patrickelectric@gmail.com>
        Chris Rizzitello <sithlord48@gmail.com>

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
#include "temperature.h"
#include <cmath>

class TemperaturePrivate
{
public:
    float extruderTemp;
    float extruderTargetTemp;
    float bedTemp;
    float bedTargetTemp;
};

namespace
{
static const float delta = 1e-3;
}

Temperature::Temperature(QObject *parent)
    : QObject(parent)
    , d(new TemperaturePrivate)
{
}

float Temperature::bedTargetTemperature() const
{
    return d->bedTargetTemp;
}

float Temperature::bedTemperature() const
{
    return d->bedTemp;
}

float Temperature::extruderTargetTemperature() const
{
    return d->extruderTargetTemp;
}

float Temperature::extruderTemperature() const
{
    return d->extruderTemp;
}

void Temperature::setBedTargetTemperature(float temp)
{
    if (std::abs(temp - d->bedTargetTemp) > delta) {
        d->bedTargetTemp = temp;
        emit bedTargetTemperatureChanged(temp);
    }
}

void Temperature::setBedTemperature(float temp)
{
    if (std::abs(temp - d->bedTemp) > delta) {
        d->bedTemp = temp;
        emit bedTemperatureChanged(temp);
    }
}

void Temperature::setExtruderTargetTemperature(float temp)
{
    if (std::abs(temp - d->extruderTargetTemp) > delta) {
        d->extruderTargetTemp = temp;
        emit extruderTargetTemperatureChanged(temp);
    }
}

void Temperature::setExtruderTemperature(float temp)
{
    if (std::abs(temp - d->extruderTemp) > delta) {
        d->extruderTemp = temp;
        emit extruderTemperatureChanged(temp);
    }
}
