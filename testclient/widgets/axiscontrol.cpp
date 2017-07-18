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
#include "axiscontrol.h"

PieButton::PieButton(QLatin1Char axis, int value, int size, int angle) : _axis(axis), _value(value)
{
    const int delta = 16; // Qt Docs: angle is 16th of a degree.
    setBrush(_palette.button());
    setStartAngle(angle * delta);
    setSpanAngle(90 * delta);
    setRect(QRect(QPoint(size * -1, size * -1), QPoint(size, size)));
    setZValue(size * -1);
    setAcceptHoverEvents(true);
    setToolTip(QStringLiteral("Move the hotend to the %1 by %2 units").arg(axis).arg(value));
}

void PieButton::setPalette(QPalette palette)
{
    _palette = palette;
}

void PieButton::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    emit clicked(_axis, _value);
}

void PieButton::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
    setBrush(_palette.highlight());
}

void PieButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    setBrush(_palette.button());
}

RectButton::RectButton(QLatin1Char axis, int value, int size) : _axis(axis), _value(value)
{
    setBrush(_palette.button());
    setRect(QRect(QPoint(0, 0), QPoint(size, size)));
    setAcceptHoverEvents(true);
    setZValue(size * -1);
    setToolTip(QStringLiteral("Move the hotend to the %1 by %2 units").arg(axis).arg(value));
}

void RectButton::setPalette(QPalette palette)
{
    _palette = palette;
}

void RectButton::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    emit clicked(_axis, _value);
}

void RectButton::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
    setBrush(_palette.highlight());
}

void RectButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    setBrush(_palette.button());
}
/*  About the Magic Numbers
        I don't have experience programming with QGraphicsScene,
        Tomaz is helping me, but until we have a better solution, all the values
        that are dividing or multiplying the items is based only in tests and errors.
        Those values was choosen because it fit better on the alignment of the items
        in the scene. If you have a better solution, please share with us.
        Lays Rodrigues - Jan/2017
*/
AxisControl::AxisControl(QWidget *parent) :
    QGraphicsView(parent)
{
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    setScene(new QGraphicsScene());

    auto createPie = [ = ](QLatin1Char axis, int value, int size, int angle) {
        auto pie = new PieButton(axis, value, size, angle);
        pie->setPalette(this->palette());
        connect(pie, &PieButton::clicked, this, &AxisControl::clicked);
        if (abs(value) == 25) {
            setLabels(pie, axis, value);
        }
        scene()->addItem(pie);
    };

    auto createRect = [ = ](QLatin1Char axis, int value, int size, int xPos, int yPos) {
        auto z = new RectButton(axis, value, size);
        z->setPalette(this->palette());
        z->setPos(xPos, yPos);
        connect(z, &RectButton::clicked, this, &AxisControl::clicked);
        if (abs(value) == 25) {
            setLabels(z, axis, value);
        }
        scene()->addItem(z);
    };

    int currPieSize = 25;
    for (auto value : {
                1, 10, 25
            }) {
        createPie(QLatin1Char('X'), value, currPieSize, -45);       // Left
        createPie(QLatin1Char('X'), value * -1, currPieSize, 135);  // Right
        createPie(QLatin1Char('Y'), value, currPieSize, 45);        // Top
        createPie(QLatin1Char('Y'), value * -1, currPieSize, 225);  // Bottom
        currPieSize += 25;
    }

    int currZSize = 25;
    int xPos = sceneRect().width() - 50;
    int yPos = -75; //Align with the origin of the scene 3 * 25
    for (auto value : {
                25, 10, 1, -1, -10, -25
            }) {
        createRect(QLatin1Char('Z'), value, currZSize, xPos, yPos);
        yPos += currZSize;
    }
    setSceneRect(scene()->itemsBoundingRect());
}

void AxisControl::resizeEvent(QResizeEvent *)
{
    fitInView(sceneRect(), Qt::KeepAspectRatio);
}

void AxisControl::setLabels(QGraphicsItem *item, QLatin1Char axis, int value)
{
    auto *lb = new QGraphicsSimpleTextItem();
    lb->setBrush(palette().buttonText());

    if (this->logicalDpiX() <= 96) {
        lb->setText((value < 0) ? QStringLiteral(" -") + axis : QStringLiteral("  ") + axis);
    } else {
        lb->setText((value < 0) ? QStringLiteral("-") + axis : QStringLiteral(" ") + axis);
    }

    if (axis.toLatin1() == 'X') {
        lb->setY(item->y() - lb->boundingRect().width());
        if (value < 0) {
            lb->setX(item->x() - item->boundingRect().width() / 1.2 - lb->boundingRect().width() / 2);
        } else {
            lb->setX(item->x() + item->boundingRect().width() / 1.2 - lb->boundingRect().width() / 2);
        }
    } else if (axis.toLatin1() == 'Y') {
        lb->setX(item->x() - lb->boundingRect().width() / 2);
        if (value < 0) {
            lb->setY(item->y() + item->boundingRect().height() / 1.5);
        } else {
            lb->setY(item->y() - item->boundingRect().height());
        }
    } else {

        lb->setX(item->x() + lb->boundingRect().width() / fontMetrics().width(lb->text()));

#ifndef Q_OS_WIN
        lb->setY(item->y() - lb->boundingRect().height() / fontMetrics().xHeight());
#else
        lb->setY(item->y() - lb->boundingRect().height() / fontMetrics().height());
#endif
    }
    scene()->addItem(lb);
}
