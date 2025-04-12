/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2018, 2020 Chris Rizzitello <rizzitello@kde.org>
    SPDX-FileCopyrightText: 2020 Tomaz Canabrava <tcanabrava@kde.org>
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
    explicit MovementWidget(QWidget *parent = nullptr);
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
Q_SIGNALS:
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
