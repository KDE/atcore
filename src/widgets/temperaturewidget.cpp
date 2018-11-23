/* AtCore Test Client
    Copyright (C) <2018>
    Author: Chris Rizzitello - rizzitello@kde.org
            Lays Rodrigues - lays.rodrigues@kde.org
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
#include "temperaturewidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

TemperatureWidget::TemperatureWidget(QWidget *parent) :
    QWidget(parent)
{
    auto *mainLayout = new QVBoxLayout;
    checkAndWait = new QCheckBox(tr("Wait Until Temperature Stabilizes"), this);
    mainLayout->addWidget(checkAndWait);

    auto label = new QLabel(tr("Bed Temp"), this);

    sbBedTemp = new QSpinBox(this);
    sbBedTemp->setRange(0, 120);
    sbBedTemp->setSuffix(QStringLiteral("°C"));

    auto *newButton = new QPushButton(tr("Set"), this);
    connect(newButton, &QPushButton::clicked, this, [this] {
        emit bedTempChanged(sbBedTemp->value(), checkAndWait->isChecked());
    });

    auto *hboxLayout = new QHBoxLayout;
    hboxLayout->addWidget(label, 80);
    hboxLayout->addWidget(sbBedTemp);
    hboxLayout->addWidget(newButton);
    mainLayout->addItem(hboxLayout);

    comboExtruderSelect = new QComboBox(this);
    sbExtruderTemp = new QSpinBox(this);
    sbExtruderTemp->setRange(0, 275);
    sbExtruderTemp->setSuffix(QStringLiteral("°C"));

    newButton = new QPushButton(tr("Set"), this);
    connect(newButton, &QPushButton::clicked, this, [this] {
        emit extTempChanged(sbExtruderTemp->value(), comboExtruderSelect->currentIndex(), checkAndWait->isChecked());
    });

    hboxLayout = new QHBoxLayout;
    hboxLayout->addWidget(comboExtruderSelect, 80);
    hboxLayout->addWidget(sbExtruderTemp);
    hboxLayout->addWidget(newButton);
    mainLayout->addItem(hboxLayout);

    setLayout(mainLayout);
}

void TemperatureWidget::updateExtruderCount(const int count)
{
    comboExtruderSelect->clear();
    for (int i = 0; i < count; i++) {
        comboExtruderSelect->insertItem(i, tr("Extruder %1").arg(i));
    }
}
