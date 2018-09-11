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
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

#include "axiscontrol.h"
#include "movementwidget.h"

MovementWidget::MovementWidget(bool showHomeAndDisableWidgets, QWidget *parent) :
    QWidget(parent)
{
    auto mainLayout = new QVBoxLayout;
    auto hBoxLayout = new QHBoxLayout;
    auto newButton = new QPushButton;

    if (showHomeAndDisableWidgets) {
        newButton = new QPushButton(tr("Home All"));
        hBoxLayout->addWidget(newButton);
        connect(newButton, &QPushButton::clicked, this, [this] {
            emit homeAllPressed();
        });

        newButton = new QPushButton(tr("Home X"));
        hBoxLayout->addWidget(newButton);
        connect(newButton, &QPushButton::clicked, this, [this] {
            emit homeXPressed();
        });

        newButton = new QPushButton(tr("Home Y"));
        hBoxLayout->addWidget(newButton);
        connect(newButton, &QPushButton::clicked, this, [this] {
            emit homeYPressed();
        });

        newButton = new QPushButton(tr("Home Z"));
        hBoxLayout->addWidget(newButton);
        connect(newButton, &QPushButton::clicked, this, [this] {
            emit homeZPressed();
        });
        mainLayout->addLayout(hBoxLayout);

        newButton = new QPushButton(tr("Disable Motors"));
        mainLayout->addWidget(newButton);
        connect(newButton, &QPushButton::clicked, this, [this] {
            emit disableMotorsPressed();
        });

    }
    comboMoveAxis = new QComboBox;
    comboMoveAxis->addItem(tr("Move X Axis to"));
    comboMoveAxis->addItem(tr("Move Y Axis to"));
    comboMoveAxis->addItem(tr("Move Z Axis to"));

    sbMoveAxis = new QDoubleSpinBox;
    sbMoveAxis->setRange(0, 200);

    newButton = new QPushButton(tr("Go"));
    connect(newButton, &QPushButton::clicked, this, [this] {
        if (comboMoveAxis->currentIndex() == 0)
        {
            emit absoluteMove(QLatin1Char('X'), sbMoveAxis->value());
        } else if (comboMoveAxis->currentIndex() == 1)
        {
            emit absoluteMove(QLatin1Char('Y'), sbMoveAxis->value());
        } else if (comboMoveAxis->currentIndex() == 2)
        {
            emit absoluteMove(QLatin1Char('Z'), sbMoveAxis->value());
        }
    });

    hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(comboMoveAxis);
    hBoxLayout->addWidget(sbMoveAxis);
    hBoxLayout->addWidget(newButton);
    mainLayout->addLayout(hBoxLayout);

    auto axisControl = new AxisControl;
    mainLayout->addWidget(axisControl);
    connect(axisControl, &AxisControl::clicked, this, &MovementWidget::relativeMove);
    connect(axisControl, &AxisControl::unitsChanged, this, &MovementWidget::unitsChanged);

    setLayout(mainLayout);
}
