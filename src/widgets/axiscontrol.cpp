/* AtCore Test Client
    Copyright (C) <2016 - 2018>

    Authors:
        Chris Rizzitello <rizzitello@kde.org>

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
#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>

AxisControl::AxisControl(QWidget *parent)
    : QWidget(parent)
    , sbValue(new QDoubleSpinBox)
{
    auto mainLayout = new QVBoxLayout;
    auto newLabel = new QLabel(tr("Move Axis"), this);
    sbValue->setSuffix(QStringLiteral(" mm"));
    sbValue->setDecimals(3);
    sbValue->setMaximum(100.0);
    sbValue->setValue(1);

    auto comboUnits = new QComboBox(this);
    comboUnits->addItems(QStringList {QStringLiteral("Metric"), QStringLiteral("Imperial")});

    connect(comboUnits, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int selection) {
        if (selection == 0) {
            sbValue->setSuffix(QStringLiteral(" mm"));
        } else {
            sbValue->setSuffix(QStringLiteral(" in"));
        }
        emit unitsChanged(selection);
    });

    auto layout = new QHBoxLayout();
    layout->addWidget(newLabel);
    layout->addWidget(sbValue);
    layout->addWidget(comboUnits);

    auto newWidget = new QWidget(this);
    newWidget->setLayout(layout);
    newWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    mainLayout->addWidget(newWidget);

    QSize iconSize = QSize(fontMetrics().height(), fontMetrics().height());
    auto glayout = new QGridLayout();
    newLabel = new QLabel(QStringLiteral("X/Y"), this);
    newLabel->setAlignment(Qt::AlignCenter);
    glayout->addWidget(newLabel, 2, 1);

    // Y-Axis
    glayout->addWidget(makeButton(QLatin1Char('Y'), 1, iconSize, QStringLiteral("arrow-up"), QStringLiteral("↑")), 1, 1);
    glayout->addWidget(makeButton(QLatin1Char('Y'), -1, iconSize, QStringLiteral("arrow-down"), QStringLiteral("↓")), 3, 1);

    // X-Axis
    glayout->addWidget(makeButton(QLatin1Char('X'), -1, iconSize, QStringLiteral("arrow-left"), QStringLiteral("←")), 2, 0);
    glayout->addWidget(makeButton(QLatin1Char('X'), 1, iconSize, QStringLiteral("arrow-right"), QStringLiteral("→")), 2, 3);

    auto bottomLayout = new QHBoxLayout();
    bottomLayout->addItem(glayout);

    newWidget = makeSimpleAxis(QLatin1Char('Z'), iconSize);
    bottomLayout->addWidget(newWidget);

    newWidget = makeSimpleAxis(QLatin1Char('E'), iconSize);
    bottomLayout->addWidget(newWidget);

    mainLayout->addItem(bottomLayout);
    setLayout(mainLayout);
}

QPushButton *AxisControl::makeButton(const QLatin1Char axis, int multiplier, const QSize &iconSize, const QString &themeIcon, const QString &fallbackText)
{
    auto button = new QPushButton(QIcon::fromTheme(themeIcon), QString(), this);
    button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    if (button->icon().isNull()) {
        button->setText(fallbackText);
    } else {
        button->setIconSize(iconSize);
    }

    connect(button, &QPushButton::clicked, this, [this, axis, multiplier] { emit clicked(axis, sbValue->value() * multiplier); });
    return button;
}

QWidget *AxisControl::makeSimpleAxis(const QLatin1Char axis, const QSize &iconSize)
{
    int multiplier = 1;
    if (axis == QLatin1Char('E')) {
        multiplier = -1;
    }

    auto vLayout = new QVBoxLayout;

    vLayout->addWidget(makeButton(axis, multiplier, iconSize, QStringLiteral("arrow-up"), QStringLiteral("↑")));

    auto label = new QLabel(QString(axis), this);
    label->setAlignment(Qt::AlignCenter);
    vLayout->addWidget(label);

    multiplier *= -1;
    vLayout->addWidget(makeButton(axis, multiplier, iconSize, QStringLiteral("arrow-down"), QStringLiteral("↓")));

    auto widget = new QWidget(this);
    widget->setLayout(vLayout);
    return widget;
}
