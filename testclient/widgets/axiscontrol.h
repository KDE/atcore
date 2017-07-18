/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2016>
    Author: Lays Rodrigues - laysrodriguessilva@gmail.com
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

class RectButton : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    RectButton(QLatin1Char axis, int value, int size);
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

class AxisControl : public QGraphicsView
{
    Q_OBJECT

public:
    explicit AxisControl(QWidget *parent = nullptr);

private:
    void setLabels(QGraphicsItem *item, QLatin1Char axis, int value);

protected:
    void resizeEvent(QResizeEvent *);

signals:
    void clicked(QLatin1Char axis, int value);

};
