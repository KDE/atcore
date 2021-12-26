/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2018, 2020 Chris Rizzitello <rizzitello@kde.org>
    SPDX-FileCopyrightText: 2018 Patrick José Pereira <patrickjp@kde.org>
    SPDX-FileCopyrightText: 2020 Tomaz Canabrava <tcanabrava@kde.org>
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

    QPushButton *disableMotors = nullptr;
    QPushButton *homeAll = nullptr;
    QPushButton *homeX = nullptr;
    QPushButton *homeY = nullptr;
    QPushButton *homeZ = nullptr;
    QPushButton *move = nullptr;

    QList<int> _axisMaxes {200, 200, 200};
};

MovementWidget::MovementWidget(QWidget *parent)
: QWidget(parent)
, d(new MovementWidgetPrivate())
{
    initialize();
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
    connect(d->homeAll, &QPushButton::clicked, this, &MovementWidget::homeAllPressed);

    d->homeX = new QPushButton(tr("Home X"), this);
    hBoxLayout->addWidget(d->homeX);
    connect(d->homeX, &QPushButton::clicked, this, &MovementWidget::homeXPressed);

    d->homeY = new QPushButton(tr("Home Y"), this);
    hBoxLayout->addWidget(d->homeY);
    connect(d->homeY, &QPushButton::clicked, this, &MovementWidget::homeYPressed);

    d->homeZ = new QPushButton(tr("Home Z"), this);
    hBoxLayout->addWidget(d->homeZ);
    connect(d->homeZ, &QPushButton::clicked, this, &MovementWidget::homeZPressed);

    mainLayout->addLayout(hBoxLayout);

    d->disableMotors = new QPushButton(tr("Disable Motors"), this);
    mainLayout->addWidget(d->disableMotors);
    connect(d->disableMotors, &QPushButton::clicked, this, &MovementWidget::disableMotorsPressed);

    d->comboMoveAxis = new QComboBox(this);
    d->comboMoveAxis->addItem(tr("Move X Axis to"));
    d->comboMoveAxis->addItem(tr("Move Y Axis to"));
    d->comboMoveAxis->addItem(tr("Move Z Axis to"));

    d->sbMoveAxis = new QDoubleSpinBox(this);
    d->sbMoveAxis->setRange(0, d->_axisMaxes.at(d->comboMoveAxis->currentIndex()));

    connect(d->comboMoveAxis, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this] (int index) {
        d->sbMoveAxis->setMaximum(d->_axisMaxes.at(index));
    });

    d->move = new QPushButton(tr("Go"), this);
    connect(d->move, &QPushButton::clicked, this, [this] {
        if (d->comboMoveAxis->currentIndex() == 0) {
            Q_EMIT absoluteMove(QLatin1Char('X'), d->sbMoveAxis->value());
        } else if (d->comboMoveAxis->currentIndex() == 1) {
            Q_EMIT absoluteMove(QLatin1Char('Y'), d->sbMoveAxis->value());
        } else if (d->comboMoveAxis->currentIndex() == 2) {
            Q_EMIT absoluteMove(QLatin1Char('Z'), d->sbMoveAxis->value());
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

void MovementWidget::setHomeButtonsVisible(bool visible)
{
    d->homeAll->setVisible(visible);
    d->homeX->setVisible(visible);
    d->homeY->setVisible(visible);
    d->homeZ->setVisible(visible);
}

void MovementWidget::setDisableMotorsButtonVisible(bool visible)
{
    d->disableMotors->setVisible(visible);
}

void MovementWidget::setAxisMax(int xMax, int yMax, int zMax)
{
    xMax = std::max(0, xMax);
    yMax = std::max(0, yMax);
    zMax = std::max(0, zMax);
    d->_axisMaxes = QList<int> {xMax, yMax, zMax};
    d->sbMoveAxis->setMaximum(d->comboMoveAxis->currentIndex());
}
