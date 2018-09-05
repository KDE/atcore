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

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QWidget>

#include "atcorewidgets_export.h"
/**
 * @brief The MovementWidget class
 * This widget will provide Basic Movement Controls. Create it with "showHomeAndDisableWidgets" false if your client provides its own actions for homing and disabling the motors.
 */
class ATCOREWIDGETS_EXPORT MovementWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Create a Movement Widget
     * @param showHomeAndDisableWidgets: set False to hide the Home and Disable Motors buttons [default = true]
     * @param parent: Parent of this widget.
     */
    MovementWidget(bool showHomeAndDisableWidgets = true, QWidget *parent = nullptr);

signals:
    /**
     * @brief The Home All button was clicked.
     * This should be connected to AtCore::home()
     */
    void homeAllPressed();

    /**
     * @brief The Home X button was clicked.
     * This should be connected to AtCore::home(AtCore::X)
     */
    void homeXPressed();

    /**
     * @brief The Home Y button was clicked.
     * This should be connected to AtCore::home(AtCore::Y)
     */
    void homeYPressed();

    /**
     * @brief The Home Z button was clicked.
     * This should be connected to AtCore::home(AtCore::Z)
     */
    void homeZPressed();

    /**
     * @brief The Disable Motors button was clicked.
     * This should be connected to AtCore::disableMotors(0)
     */
    void disableMotorsPressed();

    /**
     * @brief An absoluteMove was requested
     * This should be connected to AtCore::move(axis,value)
     * @param axis: the axis to move
     * @param value: where to move
     */
    void absoluteMove(const QLatin1Char &axis, const double value);

    /**
     * @brief A relativeMove was requested from the AxisControl
     * This should connect to a function that does the following
     *  AtCore::setRelativePosition()
     *  AtCore::move(axis, value)
     *  AtCore::setAbsolutePosition()
     * @param axis: the axis to move.
     * @param value: the value to move it by.
     */
    void relativeMove(const QLatin1Char &axis, const double value);

private:
    QComboBox *comboMoveAxis = nullptr;
    QDoubleSpinBox *sbMoveAxis = nullptr;
};
