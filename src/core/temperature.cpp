/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2016, 2018 Tomaz Canabrava <tcanabrava@kde.org>
    SPDX-FileCopyrightText: 2016-2017 Patrick José Pereira <patrickjp@kde.org>
    SPDX-FileCopyrightText: 2016-2019 Chris Rizzitello <rizzitello@kde.org>
    SPDX-FileCopyrightText: 2018 Lays Rodrigues <lays.rodrigues@kde.org>
    SPDX-FileCopyrightText: 2018 Leandro Santiago <leandrosansilva@gmail.com>
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

Temperature::~Temperature()
{
    delete d;
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

void Temperature::decodeTemp(const QByteArray &msg)
{
    QString msgString = QString::fromLatin1(msg);
    QRegularExpressionMatch tempCheck = d->tempRegEx.match(msgString);
    QRegularExpressionMatch targetTempCheck = d->targetTempRegEx.match(msgString);

    if (tempCheck.hasMatch()) {
        d->extruderTemp = tempCheck.captured(QStringLiteral("extruder")).toFloat();
        emit extruderTemperatureChanged();
    }

    if (targetTempCheck.hasMatch()) {
        d->extruderTargetTemp = targetTempCheck.captured(QStringLiteral("extruderTarget")).toFloat();
        emit extruderTargetTemperatureChanged();
    }

    if (msgString.contains(QStringLiteral("B:"), Qt::CaseInsensitive))  {
        QRegularExpressionMatch bedCheck = d->bedRegEx.match(msgString);
        QRegularExpressionMatch targetBedCheck = d->targetBedRegEx.match(msgString);

        if (bedCheck.hasMatch()) {
            d->bedTemp = bedCheck.captured(QStringLiteral("bed")).toFloat();
            emit bedTemperatureChanged();
        }

        if (targetBedCheck.hasMatch()) {
            d->bedTargetTemp = targetBedCheck.captured(QStringLiteral("bedTarget")).toFloat();
            emit bedTargetTemperatureChanged();
        }
    }
}

void Temperature::resetData()
{
    d->extruderTemp = 0.0;
    d->extruderTargetTemp = 0.0;
    d->bedTemp = 0.0;
    d->bedTargetTemp = 0.0;
}
