/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2018-2020 Chris Rizzitello <rizzitello@kde.org>
*/

#pragma once

#include <QWidget>

#include "atcorewidgets_export.h"

class QComboBox;
class QLineEdit;
class QPushButton;
class QSpinBox;

/**
 * @brief PrintWidget
 * provide a basic print widget.
 */
class ATCOREWIDGETS_EXPORT PrintWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Make a new PrintWidget.
     * @param showAllControls: if true show Print , Emergency Stop and On Pause Controls
     * @param parent: Parent of this widget.
     */
    PrintWidget(bool showAllControls = true, QWidget *parent = nullptr);

    /**
     * @brief set Post Pause string text.
     * @param text: text to set to.
     */
    void setPrintText(const QString &text);

    /**
     * @brief Update Fan count.
     * @param count new fan count
     */
    void updateFanCount(const int count);

signals:
    /**
     * @brief emergencyStopPressed
     * Connect to AtCore::emergencyStop
     */
    void emergencyStopPressed();

    /**
     * @brief flowRateChanged
     * Connect to AtCore::setFlowRate
     * @param rate
     */
    void flowRateChanged(const int rate);

    /**
     * @brief printPressed
     * Users requested to print a file.
     */
    void printPressed();

    /**
     * @brief printSpeedChanged
     * Connect to AtCore::setPrinterSpeed
     * @param speed
     */
    void printSpeedChanged(const int speed);

    /**
     * @brief The Fan Speed has Changed.
     * @param speed : new Speed.
     * @param fanNum : fan to set the speed on.
     */
    void fanSpeedChanged(const int speed, const int fanNum);

private:
    QPushButton *buttonPrint = nullptr;
    QComboBox *comboFanSelect = nullptr;
    QLineEdit *linePostPause = nullptr;
    QSpinBox *sbFlowRate = nullptr;
    QSpinBox *sbPrintSpeed = nullptr;
    QSpinBox *sbFanSpeed = nullptr;
};
