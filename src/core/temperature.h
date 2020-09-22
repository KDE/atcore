/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2016, 2018 Tomaz Canabrava <tcanabrava@kde.org>
    SPDX-FileCopyrightText: 2016-2019 Chris Rizzitello <rizzitello@kde.org>
    SPDX-FileCopyrightText: 2016-2019 Patrick José Pereira <patrickjp@kde.org>
    SPDX-FileCopyrightText: 2018 Leandro Santiago <leandrosansilva@gmail.com>
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
