/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2016>
    Author: Lays Rodrigues - lays.rodrigues@kde.org
            Chris Rizzitello - rizzitello@kde.org

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

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>

#include "atcorewidgets_export.h"

class ATCOREWIDGETS_EXPORT PieButton : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    PieButton(QLatin1Char &axis, int value, int size, int angle);
    void setPalette(QPalette palette);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *);
signals:
    void clicked(QLatin1Char axis, int value);
private:
    QLatin1Char _axis;
    int _value;
    QPalette _palette;
};

class ATCOREWIDGETS_EXPORT RectButton : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    RectButton(QLatin1Char &axis, int value, int size);
    void setPalette(QPalette palette);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *);
signals:
    void clicked(QLatin1Char axis, int value);
private:
    QLatin1Char _axis;
    int _value;
    QPalette _palette;
};

/**
 * @brief AxisControl is a Widget to generate axis relative movements.
 *
 * Usage:
 * Create a instance of AxisControl and connect the clicked signal, it will give you the axis and value that was clicked.
 */
class ATCOREWIDGETS_EXPORT AxisControl : public QGraphicsView
{
    Q_OBJECT

public:
    explicit AxisControl(const QList<int> &movementValues = {1, 10, 25}, QWidget *parent = nullptr);

private:
    void setLabels(QGraphicsItem *item, QLatin1Char &axis, int value);

protected:
    void resizeEvent(QResizeEvent *);

signals:
    /**
     * @brief User has clicked to move an axis.
     * @param axis: Axis to move
     * @param value: Amount to move
     */
    void clicked(QLatin1Char axis, int value);

};
