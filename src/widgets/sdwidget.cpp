/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2018, 2020 Chris Rizzitello <rizzitello@kde.org>
    SPDX-FileCopyrightText: 2018 Patrick José Pereira <patrickjp@kde.org>
*/

#include <QGroupBox>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>

#include "sdwidget.h"

SdWidget::SdWidget(QWidget *parent)
    : QWidget(parent)
{
    auto hBoxLayout = new QHBoxLayout;

    auto newButton = new QPushButton(tr("Get List"), this);
    hBoxLayout->addWidget(newButton);
    connect(newButton, &QPushButton::clicked, this, &SdWidget::requestSdList);

    newButton = new QPushButton(tr("Print Selected"), this);
    hBoxLayout->addWidget(newButton);
    connect(newButton, &QPushButton::clicked, this, [this] {
        if (listSdFiles->currentRow() != -1) {
            emit printSdFile(listSdFiles->currentItem()->text());
        }
    });

    newButton = new QPushButton(tr("Delete Selected"), this);
    hBoxLayout->addWidget(newButton);
    connect(newButton, &QPushButton::clicked, this, [this] {
        if (listSdFiles->currentRow() != -1) {
            emit deleteSdFile(listSdFiles->currentItem()->text());
            listSdFiles->setCurrentRow(-1);
        }
    });

    auto groupFiles = new QGroupBox(tr("Files On Sd Card"), this);
    listSdFiles = new QListWidget(this);
    auto groupLayout = new QVBoxLayout;
    groupLayout->addWidget(listSdFiles);
    groupFiles->setLayout(groupLayout);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addItem(hBoxLayout);
    mainLayout->addWidget(groupFiles);

    setLayout(mainLayout);
}

void SdWidget::updateFilelist(const QStringList &fileList)
{
    listSdFiles->clear();
    listSdFiles->addItems(fileList);
}
