/* AtCore
    Copyright (C) <2016>

    Authors:
        Tomaz Canabrava <tcanabrava@kde.org>
        Patrick José Pereira <patrickjp@kde.org>

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

/**
 * @brief The Temperature class
 *
 * Read and hold the Temperature info for the printer
 */
class ATCORE_EXPORT Temperature : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float bedTemperature READ bedTemperature NOTIFY bedTemperatureChanged)
    Q_PROPERTY(float bedTargetTemperature READ bedTargetTemperature NOTIFY bedTargetTemperatureChanged)
    Q_PROPERTY(float extruderTemperature READ extruderTemperature NOTIFY extruderTemperatureChanged)
    Q_PROPERTY(float extruderTargetTemperature READ extruderTargetTemperature NOTIFY extruderTargetTemperatureChanged)

    friend class TemperatureTests;

public:
    /**
     * @brief Create a new Temperature object
     * @param parent
     */
    explicit Temperature(QObject *parent = nullptr);

    /**
     * @brief Get bed current temperature
     */
    float bedTemperature() const;

    /**
     * @brief Get bed target temperature
     */
    float bedTargetTemperature() const;

    /**
     * @brief decode Temp values from string \p msg
     * @param msg: string to read vaules from
     */
    void decodeTemp(const QByteArray &msg);

    /**
     * @brief Get extruder temperature
     */
    float extruderTemperature() const;

    /**
     * @brief Get extruder target temperature
     */
    float extruderTargetTemperature() const;

signals:
    /**
     * @brief bed temperature has changed
     */
    void bedTemperatureChanged();

    /**
     * @brief bed target temperature has changed
     */
    void bedTargetTemperatureChanged();

    /**
     * @brief extruder temperature has changed
     */
    void extruderTemperatureChanged();

    /**
     * @brief extruder target temperature has changed
     */
    void extruderTargetTemperatureChanged();

protected:
    /**
     * @brief Reset internal temperature data, For Tests-Only
     */
    void resetData();

private:
    struct TemperaturePrivate;
    TemperaturePrivate *d;
};
