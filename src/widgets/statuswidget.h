/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2018, 2020 Chris Rizzitello <rizzitello@kde.org>
*/

#pragma once

#include <QWidget>

#include "atcorewidgets_export.h"

class QElapsedTimer;
class QLabel;
class QProgressBar;
class QSpacerItem;
class QTimer;
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
    explicit StatusWidget(bool showStop = true, QWidget *parent = nullptr);
    ~StatusWidget() = default;
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
    void updatePrintProgress(const float progress);

signals:
    void stopPressed();

private slots:
    void updatePrintTime();

private:
    QLabel *lblState = nullptr;
    QLabel *lblSd = nullptr;
    QLabel *lblTime = nullptr;
    QLabel *lblTimeLeft = nullptr;
    QElapsedTimer *printTime = nullptr;
    QTimer *printTimer = nullptr;
    QSpacerItem *spacer = nullptr;
    QProgressBar *printingProgress = nullptr;
    QWidget *printProgressWidget = nullptr;
};
