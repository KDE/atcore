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

#include <QWidget>

#include "atcorewidgets_export.h"

class QComboBox;
class QDoubleSpinBox;

/**
 * @brief The MovementWidget class
 * This widget will provide Basic Movement Controls.
 */
class ATCOREWIDGETS_EXPORT MovementWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Create a Movement Widget
     * @param parent: Parent of this widget.
     */
    MovementWidget(QWidget *parent = nullptr);
    ~MovementWidget();

    /**
     * @brief Sets the visibility of Row of home buttons
     * @param visible: set False to hide the Home buttons [default = true]
     */
    void setHomeButtonsVisible(bool visible);

    /**
     * @brief Sets the visibility of button used to disable the motors
     * @param visible: set False to hide the Home button [default = true]
     */
    void setDisableMotorsButtonVisible(bool visible);

    /**
     * @brief Sets the absolute movement value for x,y,z axis.
     * @param xMax: Max distance the X Axis can travel in mm [default = 200]
     * @param yMax: Max distance the Y Axis can travel in mm [default = 200]
     * @param zMax: Max distance the Z Axis can travel in mm [default = 200]
     */
    void setAxisMax(int xMax, int yMax, int zMax);
    
    //TODO: This widget should have a AtCore *core; member and connect
    // things internally.
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

    /**
     * @brief A Change of units was requested from the AxisControl
     * This should connect to a function that calls AtCore::setUnits
     * @param units: 0=Metric 1=Imperial
     */
    void unitsChanged(int units);

private:
    // common code for constructors.
    void initialize();

    class MovementWidgetPrivate;
    MovementWidgetPrivate *d;
};
