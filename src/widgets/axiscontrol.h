/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2017-2018, 2020 Chris Rizzitello <rizzitello@kde.org>
    SPDX-FileCopyrightText: 2018 Lays Rodrigues <lays.rodrigues@kde.org>
*/

#pragma once
#include <QWidget>

#include "atcorewidgets_export.h"

class QDoubleSpinBox;
class QPushButton;
class QString;

/**
 * @brief AxisControl is a Widget to generate axis relative movements.
 *
 * Usage:
 * Create a instance of AxisControl and connect the clicked signal, it will give you the axis and value that was clicked.
 */
class ATCOREWIDGETS_EXPORT AxisControl : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Create a new AxisControl
     * @param parent
     */
    AxisControl(QWidget *parent = nullptr);
    ~AxisControl() = default;

signals:
    /**
     * @brief User has clicked to move an axis.
     * @param axis: Axis to move
     * @param value: Amount to move
     */
    void clicked(const QLatin1Char axis, double value);

    /**
     * @brief User has changed the units.
     * @param selection: Selection of Metric (0) or Imperial(1)
     */
    void unitsChanged(int selection);

private:
    /**
     * @brief Create A push button connected to the emit event
     * @param axis: Single letter of the axis (X,Y,Z,E)
     * @param multiplier: Used to set the move direction set to 1 or -1
     * @param iconSize: size to set the icon
     * @param themeIcon: icon to use "fromTheme"
     * @param fallbackText: Fallback text if theme fails
     * @return  The Created PushButton
     */
    QPushButton *makeButton(const QLatin1Char axis, int multiplier, const QSize &iconSize, const QString &themeIcon, const QString &fallbackText);

    /**
     * @brief makeSimpleAxis
     * @param axis: Axis
     * @param iconSize: Size of the icon
     * @return Simple Axis Widget
     */
    QWidget *makeSimpleAxis(const QLatin1Char axis, const QSize &iconSize);
    QDoubleSpinBox *sbValue = nullptr;
};
