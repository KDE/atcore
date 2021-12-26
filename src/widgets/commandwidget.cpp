/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2018-2020 Chris Rizzitello <rizzitello@kde.org>
    SPDX-FileCopyrightText: 2018 Patrick José Pereira <patrickjp@kde.org>
*/

#include "commandwidget.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

CommandWidget::CommandWidget(QWidget *parent)
    : QWidget(parent)
{
    // Expose the least amount of object outside the creation function.

    // First we make our mainLayout
    auto mainLayout = new QVBoxLayout;

    // Begin making content from top to bottom or left to right.
    // Making child layouts in the order you want to put them
    // onto the mainLayout
    lineCommand = new QLineEdit(this);
    lineCommand->setPlaceholderText(tr("Send Command"));

    // we have a few buttons to make here. Lets name this newButton so its easier to reuse
    auto newButton = new QPushButton(tr("Send"), this);
    connect(newButton, &QPushButton::clicked, this, [this] {
        if (!lineCommand->text().isEmpty()) {
            if (lineCommand->text().startsWith(QStringLiteral("g"))) {
                lineCommand->setText(lineCommand->text().replace(0, 1, QStringLiteral("G")));
            } else if (lineCommand->text().startsWith(QStringLiteral("m"))) {
                lineCommand->setText(lineCommand->text().replace(0, 1, QStringLiteral("M")));
            }
            Q_EMIT commandPressed(lineCommand->text());
            lineCommand->clear();
        }
    });
    // When you have created a Row put the items into layout.
    auto hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(lineCommand);
    hBoxLayout->addWidget(newButton);
    // Put the Layout or Widget on the mainLayout when its finished.
    // This will free your pointers for reuse.
    mainLayout->addLayout(hBoxLayout);

    // Start making items for the next layout to place onto the mainLayout.
    lineMessage = new QLineEdit(this);
    lineMessage->setPlaceholderText(tr("Show Message"));

    // Reuse our button pointer.
    newButton = new QPushButton(tr("Send"), this);

    connect(newButton, &QPushButton::clicked, this, [this] {
        if (!lineMessage->text().isEmpty()) {
            Q_EMIT messagePressed(lineMessage->text());
            lineMessage->clear();
        }
    });

    // We reuse the hBoxLayout pointer in the same way as the button pointer.
    hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(lineMessage);
    hBoxLayout->addWidget(newButton);
    mainLayout->addLayout(hBoxLayout);

    setLayout(mainLayout);
}
