/* AtCore Test Client
    Copyright (C) <2018>
    Author: Chris Rizzitello - rizzitello@kde.org

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QWidget>

#include "atcorewidgets_export.h"
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
    TemperatureWidget(QWidget *parent = nullptr);

    /**
     * @brief Update widget with count extruder controls
     * @param count: number of extruder controls needed.
     */
    void updateExtruderCount(const int count);

signals:
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
