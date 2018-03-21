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

/* Usage:
 *
 * Create a instance of the command widget.
 */

class CommandWidget : public QWidget
{
    Q_OBJECT
public:
    CommandWidget(QWidget *parent = nullptr);

signals:
    void commandPressed(const QString &command);
    void messagePressed(const QString &message);

private:
    QLineEdit *lineCommand = nullptr;
    QLineEdit *lineMessage = nullptr;
};
