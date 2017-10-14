/* AtCore
    Copyright (C) <2016>

    Authors:
        Tomaz Canabrava <tcanabrava@kde.org>
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

#include "atcore_export.h"

class TemperaturePrivate;
/**
 * @brief The Temperature class
 *
 * Read and hold the Temperature info for the printer
 */
class ATCORE_EXPORT Temperature : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float bedTemperature READ bedTemperature WRITE setBedTemperature NOTIFY bedTemperatureChanged)
    Q_PROPERTY(float bedTargetTemperature READ bedTargetTemperature WRITE setBedTargetTemperature NOTIFY bedTargetTemperatureChanged)
    Q_PROPERTY(float extruderTemperature READ extruderTemperature WRITE setExtruderTemperature NOTIFY extruderTemperatureChanged)
    Q_PROPERTY(float extruderTargetTemperature READ extruderTargetTemperature WRITE setExtruderTargetTemperature NOTIFY extruderTargetTemperatureChanged)

public:
    /**
     * @brief Create a new Temperature object
     * @param parent
     */
    Temperature(QObject *parent = nullptr);

    /**
     * @brief Get bed current temperature
     */
    float bedTemperature() const;

    /**
     * @brief Get bed target temperature
     */
    float bedTargetTemperature() const;

    /**
     * @brief Get extruder temperature
     */
    float extruderTemperature() const;

    /**
     * @brief Get extruder target temperature
     */
    float extruderTargetTemperature() const;

    /**
     * @brief decode Temp values from string \p msg
     * @param msg: string to read vaules from
     */
    void decodeTemp(const QByteArray &msg);

public slots:
    /**
     * @brief Set bed temperature
     * @param temp: bed temperature
     */
    void setBedTemperature(float temp);

    /**
     * @brief Set bed target temperature
     * @param temp: bed target temperature
     */
    void setBedTargetTemperature(float temp);

    /**
     * @brief Set exturder temperature
     * @param temp: bed temperature
     */
    void setExtruderTemperature(float temp);

    /**
    * @brief Set extruder target temperature
    * @param temp: extruder target temperature
    */
    void setExtruderTargetTemperature(float temp);

signals:
    /**
     * @brief bed temperature has changed
     * @param temp : new bed temperature
     */
    void bedTemperatureChanged(float temp);

    /**
     * @brief bed target temperature has changed
     * @param temp : new bed target temperature
     */
    void bedTargetTemperatureChanged(float temp);

    /**
     * @brief extruder temperature has changed
     * @param temp : new extruder temperature
     */
    void extruderTemperatureChanged(float temp);

    /**
     * @brief extruder target temperature has changed
     * @param temp : new extruder target temperature
     */
    void extruderTargetTemperatureChanged(float temp);

private:
    TemperaturePrivate *d;
};
