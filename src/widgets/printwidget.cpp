/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2018-2020 Chris Rizzitello <rizzitello@kde.org>
    SPDX-FileCopyrightText: 2018 Patrick José Pereira <patrickjp@kde.org>
*/

#include "printwidget.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

PrintWidget::PrintWidget(bool showAllControls, QWidget *parent)
    : QWidget(parent)
{
    auto mainLayout = new QVBoxLayout;
    QPushButton *newButton = nullptr;
    QLabel *newLabel = nullptr;
    QHBoxLayout *hBoxLayout = nullptr;
    if (showAllControls) {
        buttonPrint = new QPushButton(tr("Print File"), this);
        connect(buttonPrint, &QPushButton::clicked, this, &PrintWidget::printPressed);

        newButton = new QPushButton(tr("Emergency Stop"), this);
        connect(newButton, &QPushButton::clicked, this, &PrintWidget::emergencyStopPressed);

        hBoxLayout = new QHBoxLayout;
        hBoxLayout->addWidget(buttonPrint);
        hBoxLayout->addWidget(newButton);
        mainLayout->addLayout(hBoxLayout);
    }

    newLabel = new QLabel(tr("Printer Speed"), this);
    sbPrintSpeed = new QSpinBox(this);
    sbPrintSpeed->setRange(1, 300);
    sbPrintSpeed->setValue(100);
    sbPrintSpeed->setSuffix(QStringLiteral("%"));

    newButton = new QPushButton(tr("Set"), this);
    connect(newButton, &QPushButton::clicked, this, [this] { Q_EMIT printSpeedChanged(sbPrintSpeed->value()); });

    hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(newLabel, 60);
    hBoxLayout->addWidget(sbPrintSpeed, 20);
    hBoxLayout->addWidget(newButton, 20);
    mainLayout->addLayout(hBoxLayout);

    newLabel = new QLabel(tr("Flow Rate"), this);
    sbFlowRate = new QSpinBox(this);
    sbFlowRate->setRange(1, 300);
    sbFlowRate->setValue(100);
    sbFlowRate->setSuffix(QStringLiteral("%"));

    newButton = new QPushButton(tr("Set"), this);
    connect(newButton, &QPushButton::clicked, this, [this] { Q_EMIT flowRateChanged(sbFlowRate->value()); });
    hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(newLabel, 60);
    hBoxLayout->addWidget(sbFlowRate, 20);
    hBoxLayout->addWidget(newButton, 20);
    mainLayout->addLayout(hBoxLayout);

    comboFanSelect = new QComboBox(this);
    sbFanSpeed = new QSpinBox(this);
    sbFanSpeed->setRange(0, 100);
    sbFanSpeed->setSuffix(QStringLiteral("%"));

    newButton = new QPushButton(tr("Set"), this);
    connect(newButton, &QPushButton::clicked, this, [this] {
        // Fan speed has a range of 0-255.
        int speed = sbFanSpeed->value() * 255 / 100;
        Q_EMIT fanSpeedChanged(speed, comboFanSelect->currentIndex());
    });

    hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(comboFanSelect, 60);
    hBoxLayout->addWidget(sbFanSpeed, 20);
    hBoxLayout->addWidget(newButton, 20);
    mainLayout->addLayout(hBoxLayout);

    setLayout(mainLayout);
}

void PrintWidget::setPrintText(const QString &text)
{
    buttonPrint->setText(text);
}

void PrintWidget::updateFanCount(const int count)
{
    comboFanSelect->clear();
    for (int i = 0; i < count; i++) {
        comboFanSelect->insertItem(i, tr("Fan %1 speed").arg(i));
    }
}
