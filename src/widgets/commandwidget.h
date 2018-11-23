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
#pragma once

#include <QLineEdit>
#include <QWidget>

#include "atcorewidgets_export.h"
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
    CommandWidget(QWidget *parent = nullptr);
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
