/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2018, 2020 Chris Rizzitello <rizzitello@kde.org>
*/

#pragma once

#include <QWidget>

#include "atcorewidgets_export.h"

class QCheckBox;
class QComboBox;
class QSpinBox;

/**
 * @brief The TemperatureWidget Control the bed and extruder(s) temperatures
 */
class ATCOREWIDGETS_EXPORT TemperatureWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief TemperatureWidget
     * @param parent
     */
    explicit TemperatureWidget(QWidget *parent = nullptr);
    ~TemperatureWidget() = default;

    /**
     * @brief Update widget with count extruder controls
     * @param count: number of extruder controls needed.
     */
    void updateExtruderCount(const int count);

Q_SIGNALS:
    /**
     * @brief User has changed the bed temperature.
     * @param temperature: new temperature
     * @param andWait: true if heat and wait mode
     */
    void bedTempChanged(const int temperature, bool andWait);

    /**
     * @brief User has changed the extruder temperature.
     * @param temperature: new temperature
     * @param extNum: the extruder to change temperature of
     * @param andWait: true if head and wait mode.
     */
    void extTempChanged(const int temperature, const int extNum, bool andWait);

private:
    QCheckBox *checkAndWait = nullptr;
    QComboBox *comboExtruderSelect;
    QSpinBox *sbBedTemp = nullptr;
    QSpinBox *sbExtruderTemp;
};
