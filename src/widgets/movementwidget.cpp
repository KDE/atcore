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
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

#include "axiscontrol.h"
#include "movementwidget.h"

class MovementWidget::MovementWidgetPrivate {
    public:
    QComboBox *comboMoveAxis = nullptr;
    QDoubleSpinBox *sbMoveAxis = nullptr;
    AxisControl *axisControl = nullptr;

    QPushButton *homeAll;
    QPushButton *homeX;
    QPushButton *homeY;
    QPushButton *homeZ;
    QPushButton *disableMotors;

    QPushButton *move;
};

MovementWidget::MovementWidget(QWidget *parent)
: QWidget(parent)
, d(new MovementWidgetPrivate())
{
    initialize();
}

MovementWidget::MovementWidget(bool showHomeAndDisableWidgets, QWidget *parent)
    : QWidget(parent)
    , d(new MovementWidgetPrivate())
{
    initialize();
    toggleHomeButtons(showHomeAndDisableWidgets);
    toggleDisableMotorsButton(showHomeAndDisableWidgets);
}

MovementWidget::~MovementWidget() {
    delete d;
}

void MovementWidget::initialize()
{
    auto *mainLayout = new QVBoxLayout;
    auto *hBoxLayout = new QHBoxLayout;

    d->homeAll = new QPushButton(tr("Home All"), this);
    hBoxLayout->addWidget(d->homeAll);
    connect(d->homeAll, &QPushButton::clicked, this, [this] { emit homeAllPressed(); });

    d->homeX = new QPushButton(tr("Home X"), this);
    hBoxLayout->addWidget(d->homeX);
    connect(d->homeX, &QPushButton::clicked, this, [this] { emit homeXPressed(); });

    d->homeY = new QPushButton(tr("Home Y"), this);
    hBoxLayout->addWidget(d->homeY);
    connect(d->homeY, &QPushButton::clicked, this, [this] { emit homeYPressed(); });

    d->homeZ = new QPushButton(tr("Home Z"), this);
    hBoxLayout->addWidget(d->homeZ);
    connect(d->homeZ, &QPushButton::clicked, this, [this] { emit homeZPressed(); });

    mainLayout->addLayout(hBoxLayout);

    d->disableMotors = new QPushButton(tr("Disable Motors"), this);
    mainLayout->addWidget(d->disableMotors);
    connect(d->disableMotors, &QPushButton::clicked, this, [this] { emit disableMotorsPressed(); });

    d->comboMoveAxis = new QComboBox(this);
    d->comboMoveAxis->addItem(tr("Move X Axis to"));
    d->comboMoveAxis->addItem(tr("Move Y Axis to"));
    d->comboMoveAxis->addItem(tr("Move Z Axis to"));

    d->sbMoveAxis = new QDoubleSpinBox(this);
    d->sbMoveAxis->setRange(0, 200);

    d->move = new QPushButton(tr("Go"), this);
    connect(d->move, &QPushButton::clicked, this, [this] {
        if (d->comboMoveAxis->currentIndex() == 0) {
            emit absoluteMove(QLatin1Char('X'), d->sbMoveAxis->value());
        } else if (d->comboMoveAxis->currentIndex() == 1) {
            emit absoluteMove(QLatin1Char('Y'), d->sbMoveAxis->value());
        } else if (d->comboMoveAxis->currentIndex() == 2) {
            emit absoluteMove(QLatin1Char('Z'), d->sbMoveAxis->value());
        }
    });

    hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(d->comboMoveAxis);
    hBoxLayout->addWidget(d->sbMoveAxis);
    hBoxLayout->addWidget(d->move);
    mainLayout->addLayout(hBoxLayout);

    d->axisControl = new AxisControl(this);
    mainLayout->addWidget(d->axisControl);
    connect(d->axisControl, &AxisControl::clicked, this, &MovementWidget::relativeMove);
    connect(d->axisControl, &AxisControl::unitsChanged, this, &MovementWidget::unitsChanged);

    setLayout(mainLayout);
}

void MovementWidget::toggleHomeButtons(bool toggle)
{
    d->homeAll->setVisible(toggle);
    d->homeX->setVisible(toggle);
    d->homeY->setVisible(toggle);
    d->homeZ->setVisible(toggle);
}

void MovementWidget::toggleDisableMotorsButton(bool toggle)
{
    d->disableMotors->setVisible(toggle);
}
