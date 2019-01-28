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

struct Temperature::TemperaturePrivate {
    /** Regex to capture Bed Temperature. Looks for B: then grabs ##.## */
    static const QRegularExpression bedRegEx;
    /** bedTemp: Bed current temperature */
    float bedTemp = 0.0;
    /** bedTargetTemp: Bed target temperature */
    float bedTargetTemp = 0.0;
    /** extruderTemp: Extruder current temperature */
    float extruderTemp = 0.0;
    /** extruderTargetTemp: Extruder target temperature */
    float extruderTargetTemp = 0.0;
    /** Regex to capture Bed Target Temperature: Find B:## /## and grab the second set of numbers */
    static const QRegularExpression targetBedRegEx;
    /** Regex to capture Extruder Target Temperature Finds T:## /## and grabs the second set of numbers */
    static const QRegularExpression targetTempRegEx;
    /** Regex to capture Extruder Temperature. Looks for T: then grabs ##.## */
    static const QRegularExpression tempRegEx;
};

/**
 * @brief Temperature::TemperaturePrivate::bedRegEx
 *  Look for B: capture Any int or float as "bed"
 *  Examples:
 *   B: 25.0/50.0 (Captures 25.0)
 *   B: 25/50  (Captures 25)
 */
const QRegularExpression Temperature::TemperaturePrivate::bedRegEx = QRegularExpression(QStringLiteral(R"(B:(?<bed>\d+\.?\d*))"));

/**
 * @brief Temperature::TemperaturePrivate::targetBedRegEx
 *  Look for B: [anything ] /. Capture any int or float after the '/' as "bedTarget"
 *  Examples:
 *   B: 25.0 /50.0 (Captures 50.0)
 *   B: 25/50 (Captures 50)
 */
const QRegularExpression Temperature::TemperaturePrivate::targetBedRegEx = QRegularExpression(QStringLiteral(R"(B:[^\/]*\/(?<bedTarget>\d+\.?\d*))"));

/**
 * @brief Temperature::TemperaturePrivate::targetTempRegEx
 *  Look for T: [anything ] /. Capture any int or float after the '/' as "extruderTarget"
 *  Examples:
 *   T: 25.0 /50.0 (Captures 50.0)
 *   T: 25/50 (Captures 50)
 */
const QRegularExpression Temperature::TemperaturePrivate::targetTempRegEx = QRegularExpression(QStringLiteral(R"(T:[^\/]*\/(?<extruderTarget>\d+\.?\d*))"));

/**
 * @brief Temperature::TemperaturePrivate::tempRegEx
 *  Look for T: capture Any int or float as "extruder"
 *  Examples:
 *   T: 25.0 /50 (Captures 25.0)
 *   T: 25/50 (Captures 25)
 */
const QRegularExpression Temperature::TemperaturePrivate::tempRegEx = QRegularExpression(QStringLiteral(R"(T:(?<extruder>\d+\.?\d*))"));

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
    QString msgString = QString::fromLatin1(msg);
    QRegularExpressionMatch tempCheck = d->tempRegEx.match(msgString);
    QRegularExpressionMatch targetTempCheck = d->targetTempRegEx.match(msgString);

    if (tempCheck.hasMatch()) {
        setExtruderTemperature(tempCheck.captured(QStringLiteral("extruder")).toFloat());
    }

    if (targetTempCheck.hasMatch()) {
        setExtruderTargetTemperature(targetTempCheck.captured(QStringLiteral("extruderTarget")).toFloat());
    }

    if (msg.indexOf(QStringLiteral("B:")) != -1) {
        QRegularExpressionMatch bedCheck = d->bedRegEx.match(msgString);
        QRegularExpressionMatch targetBedCheck = d->targetBedRegEx.match(msgString);

        if (bedCheck.hasMatch()) {
            setBedTemperature(bedCheck.captured(QStringLiteral("bed")).toFloat());
        }

        if (targetBedCheck.hasMatch()) {
            setBedTargetTemperature(targetBedCheck.captured(QStringLiteral("bedTarget")).toFloat());
        }
    }
}
