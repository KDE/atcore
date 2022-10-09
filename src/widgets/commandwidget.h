/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2018, 2020 Chris Rizzitello <rizzitello@kde.org>
*/

#pragma once

#include <QWidget>

#include "atcorewidgets_export.h"

class QLineEdit;

/**
 * @brief CommandWidget provides a widget to send commands or messages to the printer.
 *
 * Usage:
 * Connect to commandPressed or messagePressed to capture the events.
 */
class ATCOREWIDGETS_EXPORT CommandWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CommandWidget(QWidget *parent = nullptr);
    ~CommandWidget() = default;

signals:
    /**
     * @brief User has requested to send a command.
     * @param command: The command to send.
     */
    void commandPressed(const QString &command);

    /**
     * @brief User has requested to send a message
     * @param message: The message to send.
     */
    void messagePressed(const QString &message);

private:
    QLineEdit *lineCommand = nullptr;
    QLineEdit *lineMessage = nullptr;
};
