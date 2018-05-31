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
#include <QSpacerItem>
#include <QTimer>
#include <QWidget>

#include "atcorewidgets_export.h"
/**
 * @brief The StatusWidget class Status Bar information for atcore
 */
class ATCOREWIDGETS_EXPORT StatusWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Make A new Status widget
     * @param showStop: Set False if your client has the print job stop in another widget.
     * @param parent: parent of this widget.
     */
    StatusWidget(bool showStop = true, QWidget *parent = nullptr);
    /**
     * @brief Set if the status area should show SD card inserted.
     * @param hasSd
     */
    void setSD(bool hasSd);
    /**
     * @brief Set the State String
     * @param state: String to be shown
     */
    void setState(const QString &state);
    /**
     * @brief Show or hide the Print progress and time
     * @param visible : true for show
     */
    void showPrintArea(bool visible);
    /**
     * @brief Update the progres to the new progress
     * @param progress: new progress.
     */
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
    QSpacerItem *spacer = nullptr;
    QProgressBar *printingProgress = nullptr;
    QWidget *printProgressWidget = nullptr;
};
