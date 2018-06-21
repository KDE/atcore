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
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

#include "sdwidget.h"

SdWidget::SdWidget(QWidget *parent) :
    QWidget(parent)
{
    auto hBoxLayout = new QHBoxLayout;

    auto newButton = new QPushButton(tr("Get List"));
    hBoxLayout->addWidget(newButton);
    connect(newButton, &QPushButton::clicked, this, [this] {
        emit(requestSdList());
    });

    newButton = new QPushButton(tr("Print Selected"));
    hBoxLayout->addWidget(newButton);
    connect(newButton, &QPushButton::clicked, this, [this] {
        if (listSdFiles->currentRow() != -1)
        {
            emit(printSdFile(listSdFiles->currentItem()->text()));
        }
    });

    newButton = new QPushButton(tr("Delete Selected"));
    hBoxLayout->addWidget(newButton);
    connect(newButton, &QPushButton::clicked, this, [this] {
        if (listSdFiles->currentRow() != -1)
        {
            emit(deleteSdFile(listSdFiles->currentItem()->text()));
            listSdFiles->setCurrentRow(-1);
        }
    });

    auto groupFiles =  new QGroupBox(tr("Files On Sd Card"));
    listSdFiles = new QListWidget;
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
