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
#include <QPushButton>
#include <QSpinBox>
#include <QWidget>

/* Usage:
 *
 * Create a instance of the print widget.
 */

class PrintWidget : public QWidget
{
    Q_OBJECT
public:
    PrintWidget(QWidget *parent = nullptr);
    QString postPauseCommand() const;
    void setPrintText(const QString &text);

signals:
    void emergencyStopPressed();
    void flowRateChanged(const int rate);
    void printPressed();
    void printSpeedChanged(const int speed);

private:
    QPushButton *buttonPrint = nullptr;
    QLineEdit *linePostPause = nullptr;
    QSpinBox *sbFlowRate = nullptr;
    QSpinBox *sbPrintSpeed = nullptr;
};
