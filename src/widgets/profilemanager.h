/* AtCore - Widgets
    Copyright (C) <2019>
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

#include <QWidget>

#include "atcorewidgets_export.h"

class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QRadioButton;
class QSpinBox;

class ATCOREWIDGETS_EXPORT ProfileManager : public QWidget
{
    Q_OBJECT
public:
    ProfileManager(QWidget *parent = nullptr);

private:
    void onCbProfileEditingFinished();
    void onRadioCartesianToggled(bool checked);
    QStringList detectFWPlugins();
    QStringList firmwaresInPath(const QString &path);
    void loadProfile(const QString &profileName);

    QCheckBox *checkAutoTempReport = nullptr;
    QComboBox *cbBaud = nullptr;
    QComboBox *cbFirmware = nullptr;
    QComboBox *cbProfile = nullptr;
    QRadioButton *radioCartesian = nullptr;
    QRadioButton *radioDelta = nullptr;
    QLabel *lblX = nullptr;
    QLabel *lblZ = nullptr;
    QLineEdit *lineName = nullptr;
    QLineEdit *linePostPause = nullptr;
    QSpinBox *sbMaxBedTemp = nullptr;
    QSpinBox *sbMaxExtTemp = nullptr;
    QSpinBox *sbMaxX = nullptr;
    QSpinBox *sbMaxY = nullptr;
    QSpinBox *sbMaxZ = nullptr;
    QWidget *axisY = nullptr;
};

namespace SERIAL
{
static const QStringList BAUDS = {QStringLiteral("9600"),
                                  QStringLiteral("14400"),
                                  QStringLiteral("19200"),
                                  QStringLiteral("28800"),
                                  QStringLiteral("38400"),
                                  QStringLiteral("57600"),
                                  QStringLiteral("76800"),
                                  QStringLiteral("115200"),
                                  QStringLiteral("230400"),
                                  QStringLiteral("250000"),
                                  QStringLiteral("500000"),
                                  QStringLiteral("1000000")};
}
