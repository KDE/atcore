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
#include <QLabel>
#include <QProgressBar>
#include <QTimer>
#include <QWidget>

/* Usage:
 *
 * Create a instance of the status widget.
 */

class StatusWidget : public QWidget
{
    Q_OBJECT
public:
    StatusWidget(QWidget *parent = nullptr);

    void setSD(bool hasSd);
    void setState(const QString &state);
    void showPrintArea(bool visible);
    void updatePrintProgress(const float &progress);

signals:
    void stopPressed();

private slots:
    void updatePrintTime();

private:
    QLabel *lblState = nullptr;
    QLabel *lblSd = nullptr;
    QLabel *lblTime = nullptr;
    QLabel *lblTimeLeft = nullptr;
    QTime *printTime = nullptr;
    QTimer *printTimer = nullptr;
    QProgressBar *printingProgress = nullptr;
    QWidget *printProgressWidget = nullptr;
};
