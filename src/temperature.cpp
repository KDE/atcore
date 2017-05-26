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

#include <QRegularExpressionMatch>

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
    d->bedTargetTemp = temp;
    emit bedTargetTemperatureChanged(temp);
}

void Temperature::setBedTemperature(float temp)
{
    d->bedTemp = temp;
    emit bedTemperatureChanged(temp);
}

void Temperature::setExtruderTargetTemperature(float temp)
{
    d->extruderTargetTemp = temp;
    emit extruderTargetTemperatureChanged(temp);
}

void Temperature::setExtruderTemperature(float temp)
{
    d->extruderTemp = temp;
    emit extruderTemperatureChanged(temp);
}

void Temperature::decodeTemp(const QByteArray &msg)
{
    QRegularExpression tempRegEx(QStringLiteral("(T:(?<extruder>\\d+\\.?\\d*))"));
    QRegularExpression targetTempRegEx(QStringLiteral("(\\/)(?<extruderTarget>\\d*)(.+)"));
    QRegularExpressionMatch tempCheck = tempRegEx.match(QString::fromLatin1(msg));
    QRegularExpressionMatch targetTempCheck = targetTempRegEx.match(QString::fromLatin1(msg));

    if (tempCheck.hasMatch()) {
        setExtruderTemperature(tempCheck.captured(QStringLiteral("extruder")).toFloat());
    }
    if (targetTempCheck.hasMatch()) {
        setExtruderTargetTemperature(targetTempCheck.captured(QStringLiteral("extruderTarget")).toFloat());
    }

    if (msg.contains(QString::fromLatin1("B:").toLocal8Bit())) {
        QRegularExpression bedRegEx(QStringLiteral("(B:(?<bed>\\d+\\.?\\d*))"));
        QRegularExpressionMatch bedCheck = bedRegEx.match(QString::fromLatin1(msg));
        QRegularExpression targetBedRegEx(QStringLiteral("B:(.+)(\\/)(?<bedTarget>\\d+)"));
        QRegularExpressionMatch targetBedCheck = targetBedRegEx.match(QString::fromLatin1(msg));

        if (bedCheck.hasMatch()) {
            setBedTemperature(bedCheck.captured(QStringLiteral("bed")).toFloat());
        }

        if (targetBedCheck.hasMatch()) {
            setBedTargetTemperature(targetBedCheck.captured(QStringLiteral("bedTarget")).toFloat());
        }

    }
}