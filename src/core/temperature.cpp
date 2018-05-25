/* AtCore
    Copyright (C) <2016>

    Authors:
        Tomaz Canabrava <tcanabrava@kde.org>
        Patrick José Pereira <patrickjp@kde.org>
        Chris Rizzitello <rizzitello@kde.org>

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
#include <QRegularExpressionMatch>
#include <cmath>

#include "temperature.h"
/**
 * @brief The TemperaturePrivate class
 *
 * Private Data of Temperature
 */
class TemperaturePrivate
{
public:
    float extruderTemp;         //!< @param extruderTemp: Extruder current temperature
    float extruderTargetTemp;   //!< @param extruderTargetTemp: Extruder target temperature
    float bedTemp;              //!< @param bedTemp: Bed current temperature
    float bedTargetTemp;        //!< @param bedTargetTemp: Bed target temperature
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
    int bloc = msg.indexOf(QStringLiteral("B:"));

    float firstTargetTemperature = 0;
    float secondTargetTemperature = 0;

    QRegularExpression tempRegEx(QStringLiteral("(T:(?<extruder>\\d+\\.?\\d*))"));
    QRegularExpression targetTempRegEx(QStringLiteral("(\\/)(?<extruderTarget>\\d*)(.+)"));
    QRegularExpressionMatch tempCheck = tempRegEx.match(QString::fromLatin1(msg));
    QRegularExpressionMatch targetTempCheck = targetTempRegEx.match(QString::fromLatin1(msg));

    if (tempCheck.hasMatch()) {
        setExtruderTemperature(tempCheck.captured(QStringLiteral("extruder")).toFloat());
    }

    if (targetTempCheck.hasMatch()) {
        firstTargetTemperature = targetTempCheck.captured(QStringLiteral("extruderTarget")).toFloat();
    }

    if (bloc != -1) {
        QRegularExpression bedRegEx(QStringLiteral("(B:(?<bed>\\d+\\.?\\d*))"));
        QRegularExpressionMatch bedCheck = bedRegEx.match(QString::fromLatin1(msg));
        QRegularExpression targetBedRegEx(QStringLiteral("B:(.+)(\\/)(?<bedTarget>\\d+)"));
        QRegularExpressionMatch targetBedCheck = targetBedRegEx.match(QString::fromLatin1(msg));

        if (bedCheck.hasMatch()) {
            setBedTemperature(bedCheck.captured(QStringLiteral("bed")).toFloat());
        }

        if (targetBedCheck.hasMatch()) {
            secondTargetTemperature = targetBedCheck.captured(QStringLiteral("bedTarget")).toFloat();
        }
    }
    //Currently the first value after / is stored in firstTargetTemperature and the second / in secondTargetTemperature
    //Because of this we need to check what came first in the return and place the values
    //The regex for temperature needs to look at the whole T: or B: block to correctly decode targets
    if (bloc < msg.indexOf(QStringLiteral("T:")) && bloc != -1) {
        setExtruderTargetTemperature(secondTargetTemperature);
        setBedTargetTemperature(firstTargetTemperature);
    } else {
        setExtruderTargetTemperature(firstTargetTemperature);
        setBedTargetTemperature(secondTargetTemperature);
    }
}
