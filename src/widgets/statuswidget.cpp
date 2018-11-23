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
#include <QStyle>
#include <QTime>

#include "statuswidget.h"

StatusWidget::StatusWidget(bool showStop, QWidget *parent) :
    QWidget(parent)
{
    //first create the item for the print Progress.
    auto hBoxLayout = new QHBoxLayout;

    printingProgress = new QProgressBar(this);
    printingProgress->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    hBoxLayout->addWidget(printingProgress);

    if (showStop) {
        auto newButton = new QPushButton(style()->standardIcon(QStyle::SP_BrowserStop), QString(), this);
        connect(newButton, &QPushButton::clicked, this, [this] {
            emit stopPressed();
        });
        hBoxLayout->addWidget(newButton);
    }

    lblTime = new QLabel(QStringLiteral("00:00:00"), this);
    lblTime->setAlignment(Qt::AlignHCenter);
    auto newLabel = new QLabel(QStringLiteral(" / "), this);
    lblTimeLeft = new QLabel(QStringLiteral("??:??:??"), this);
    lblTimeLeft->setAlignment(Qt::AlignHCenter);

    hBoxLayout->addWidget(lblTime);
    hBoxLayout->addWidget(newLabel);
    hBoxLayout->addWidget(lblTimeLeft);
    printProgressWidget = new QWidget(this);
    printProgressWidget->setLayout(hBoxLayout);

    //Then Create the full bar.
    newLabel = new QLabel(tr("AtCore State:"), this);
    lblState = new QLabel(tr("Not Connected"), this);
    lblSd = new QLabel(this);

    spacer = new QSpacerItem(10, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(newLabel);
    hBoxLayout->addWidget(lblState);
    hBoxLayout->addSpacerItem(new QSpacerItem(5, 20, QSizePolicy::Fixed));
    hBoxLayout->addWidget(lblSd);
    hBoxLayout->addSpacerItem(spacer);
    hBoxLayout->addWidget(printProgressWidget);

    setLayout(hBoxLayout);

    printTime = new QTime();
    printTimer = new QTimer(this);
    printTimer->setInterval(1000);
    printTimer->setSingleShot(false);
    connect(printTimer, &QTimer::timeout, this, &StatusWidget::updatePrintTime);
}

void StatusWidget::setSD(bool hasSd)
{
    QString labelText = hasSd ? tr("SD") : QString();
    lblSd->setText(labelText);
}
void StatusWidget::setState(const QString &state)
{
    lblState->setText(state);
}

void StatusWidget::showPrintArea(bool visible)
{
    printProgressWidget->setVisible(visible);
    if (visible) {
        spacer->changeSize(10, 20, QSizePolicy::Fixed, QSizePolicy::Fixed);
        printTime->start();
        printTimer->start();
    } else {
        printTimer->stop();
        spacer->changeSize(10, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    }
}

void StatusWidget::updatePrintTime()
{
    QTime temp(0, 0, 0);
    lblTime->setText(temp.addMSecs(printTime->elapsed()).toString(QStringLiteral("hh:mm:ss")));
}

void StatusWidget::updatePrintProgress(const int progress)
{
    printingProgress->setValue(progress);
    if (progress >= 1) {
        QTime temp(0, 0, 0);
        lblTimeLeft->setText(temp.addMSecs((100 - progress) * (printTime->elapsed() / progress)).toString(QStringLiteral("hh:mm:ss")));
    } else {
        lblTimeLeft->setText(QStringLiteral("??:??:??"));
    }
}
