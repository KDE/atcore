/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2018, 2020 Chris Rizzitello <rizzitello@kde.org>
    SPDX-FileCopyrightText: 2018 Patrick José Pereira <patrickjp@kde.org>
*/

#include "temperaturewidget.h"

#include <QCheckBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

TemperatureWidget::TemperatureWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *mainLayout = new QVBoxLayout;
    checkAndWait = new QCheckBox(tr("Wait Until Temperature Stabilizes"), this);
    mainLayout->addWidget(checkAndWait);

    auto label = new QLabel(tr("Bed Temp"), this);

    sbBedTemp = new QSpinBox(this);
    sbBedTemp->setRange(0, 120);
    sbBedTemp->setSuffix(QStringLiteral("°C"));

    auto *newButton = new QPushButton(tr("Set"), this);
    connect(newButton, &QPushButton::clicked, this, [this] { Q_EMIT bedTempChanged(sbBedTemp->value(), checkAndWait->isChecked()); });

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
    connect(newButton, &QPushButton::clicked, this, [this] { Q_EMIT extTempChanged(sbExtruderTemp->value(), comboExtruderSelect->currentIndex(), checkAndWait->isChecked()); });

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
