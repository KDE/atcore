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
#include "commandwidget.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

CommandWidget::CommandWidget(QWidget *parent) :
    QWidget(parent)
{
    //Expose the least amount of object outside the creation function.

    // First we make our mainLayout
    auto mainLayout = new QVBoxLayout;

    //Begin making content from top to bottom or left to right.
    //Making child layouts in the order you want to put them
    // onto the mainLayout
    lineCommand = new QLineEdit;
    lineCommand->setPlaceholderText(tr("Send Command"));

    //we have a few buttons to make here. Lets name this newButton so its easier to reuse
    auto newButton = new QPushButton(tr("Send"));
    connect(newButton, &QPushButton::clicked, this, [this] {
        emit(commandPressed(lineCommand->text()));
        lineCommand->clear();
    });
    //When you have created a Row put the items into layout.
    auto hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(lineCommand);
    hBoxLayout->addWidget(newButton);
    //Put the Layout or Widget on the mainLayout when its finished.
    //This will free your pointers for reuse.
    mainLayout->addLayout(hBoxLayout);

    //Start making items for the next layout to place onto the mainLayout.
    lineMessage = new QLineEdit;
    lineMessage->setPlaceholderText(tr("Show Message"));

    //Reuse our button pointer.
    newButton = new QPushButton(tr("Send"));

    connect(newButton, &QPushButton::clicked, this, [this] {
        emit(messagePressed(lineMessage->text()));
        lineMessage->clear();
    });

    //We reuse the hBoxLayout pointer in the same way as the button pointer.
    hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(lineMessage);
    hBoxLayout->addWidget(newButton);
    mainLayout->addLayout(hBoxLayout);

    setLayout(mainLayout);
}
