/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2016>
    Author: Lays Rodrigues - laysrodriguessilva@gmail.com

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
#include <QList>

/* Usage:
 *
 * Create a instance of PrinterHotendPositionVisualController and
 * connect the clicked signal, it will give you the axis and value
 * that was clicked.
 */

class PieButton : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    PieButton(QLatin1Char axis, int value, int size, int angle);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
signals:
    void clicked(QLatin1Char axis, int value);
private:
    QLatin1Char _axis;
    int _value;
};

class RectButton : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    RectButton(QLatin1Char axis, int value, int size);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
signals:
    void clicked(QLatin1Char axis, int value);
private:
    QLatin1Char _axis;
    int _value;
};

class PrinterHotendPositionVisualController : public QGraphicsView
{
    Q_OBJECT

public:
    explicit PrinterHotendPositionVisualController(QWidget *parent = 0);

private:
    void setLabels(QGraphicsItem *item, QLatin1Char axis, int value);

protected:
    void resizeEvent(QResizeEvent *event);

signals:
    void clicked(QLatin1Char axis, int value);

};
