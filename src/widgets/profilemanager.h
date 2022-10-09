/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2019-2020 Chris Rizzitello <rizzitello@kde.org>
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
    explicit ProfileManager(QWidget *parent = nullptr);

private:
    void onCbProfileEditingFinished();
    void onRadioCartesianToggled(bool checked);
    QStringList detectFWPlugins();
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
    const QStringList BAUDS = {QStringLiteral("9600"),
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
                               QStringLiteral("1000000")
                              };
};
