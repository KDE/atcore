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
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QWidget>

#include "atcorewidgets_export.h"
/* Usage:
 *
 * Create a instance of the print widget.
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
     * @brief Get post pause string
     * @return The Post Pause string.
     */
    QString postPauseCommand() const;

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
    QLineEdit *linePostPause = nullptr;
    QSpinBox *sbFlowRate = nullptr;
    QSpinBox *sbPrintSpeed = nullptr;
    QComboBox *comboFanSelect = nullptr;
    QSpinBox *sbFanSpeed = nullptr;
};
