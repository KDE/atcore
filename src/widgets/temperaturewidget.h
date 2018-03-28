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
/* Usage:
 *
 * Create a instance of the temperature widget.
 */

class ATCOREWIDGETS_EXPORT TemperatureWidget : public QWidget
{
    Q_OBJECT
public:
    TemperatureWidget(QWidget *parent = nullptr);
    void updateExtruderCount(const int count);
    void updateFanCount(const int count);

signals:
    void bedTempChanged(const int temperature, bool andWait);
    void extTempChanged(const int temperature, const int extNum, bool andWait);
    void fanSpeedChanged(const int speed, const int fanNum);

private:
    QCheckBox *checkAndWait = nullptr;
    QComboBox *comboExtruderSelect;
    QComboBox *comboFanSelect;
    QSpinBox *sbBedTemp = nullptr;
    QSpinBox *sbExtruderTemp;
    QSpinBox *sbFanSpeed;
};
