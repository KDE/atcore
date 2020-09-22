/* AtCore Test Client
 * SPDX-License-Identifier: GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 * SPDX-FileCopyrightText: 2016-2018 Patrick José Pereira <patrickjp@kde.org>
 * SPDX-FileCopyrightText: 2016-2017 Lays Rodrigues <lays.rodrigues@kde.org>
 * SPDX-FileCopyrightText: 2016-2019 Chris Rizzitello <rizzitello@kde.org>
 * SPDX-FileCopyrightText: 2016 Tomaz Canabrava <tcanabrava@kde.org>
 */

#pragma once

#include <QMainWindow>

#include "atcore.h"
#include "commandwidget.h"
#include "logwidget.h"
#include "movementwidget.h"
#include "plotwidget.h"
#include "printwidget.h"
#include "profilemanager.h"
#include "sdwidget.h"
#include "statuswidget.h"
#include "temperaturewidget.h"

class SerialLayer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

public slots:
    /**
     * @brief Check temperature
     *
     * @param  sensorType : type of sensor
     * @param  number     : index of sensor
     * @param  temp       : temperature
     */
    void checkTemperature(uint sensorType, uint number, float temp);

private slots:
    // ButtonEvents

    /**
     * @brief Connect Button Clicked will connect or disconnect based on printer state
     */
    void connectPBClicked();

    /**
     * @brief Print Button Clicked, can also pause /resue print based on printer state
     */
    void printPBClicked();

    /**
     * @brief printerStateChanged Catch and proccess printer state commands
     * @param state: new printer state
     */
    void printerStateChanged(AtCore::STATES state);

    /**
     * @brief show/hide dock titlebars
     * @param checked: True if shown
     */
    void toggleDockTitles(bool checked);

    /**
     * @brief set enabled for connection option widgets (port, baud and fw selector)
     * @param enabled: true if enabled.
     */
    void setConnectionWidgetsEnabled(bool enabled);
    /**
     * @brief Toggle disable state of dangerous docks
     * Command, Sd Card, Movement, Temperature Controls and Printing docks
     * @param disabled: True if items are disabled.
     */
    void setDangeriousDocksDisabled(bool disabled);
    /**
     * @brief Called when atcore changes it temperature reporting mode
     * @param autoReport: True if using temperature auto reporting
     */
    void updateAutoTemperatureReport(bool autoReport);

private:
    AtCore *core;
    // Define max number of fans
    static int fanCount;

    void closeEvent(QCloseEvent *event) override;
    /**
     * @brief Locate serial port
     *
     */
    void locateSerialPort(const QStringList &ports);

    /**
     * @brief setupActions for KXMLGui
     */
    void setupActions();

    /**
     * @brief Populate comboboxes
     */
    void populateCBs();

    /**
     * @brief Gui Changes for when sd card mount status has changed.
     */
    void sdChanged(bool mounted);

    // UI Functions

    /**
     * @brief Create The Menubar
     */
    void initMenu();

    /**
     * @brief Create StatusBar
     */
    void initStatusBar();

    /**
     * @brief Create Main Widgets.
     */
    void initWidgets();

    // Private GUI Items
    // menuView is global to allow for docks to be added / removed.
    QMenu *menuView = nullptr;
    // Status Bar Items
    StatusWidget *statusWidget = nullptr;
    // Docks
    void makeLogDock();
    QDockWidget *logDock = nullptr;
    LogWidget *logWidget = nullptr;

    void makeTempTimelineDock();
    QDockWidget *tempTimelineDock = nullptr;
    PlotWidget *plotWidget = nullptr;

    void makeCommandDock();
    QDockWidget *commandDock = nullptr;
    CommandWidget *commandWidget = nullptr;

    void makePrintDock();
    QDockWidget *printDock = nullptr;
    PrintWidget *printWidget = nullptr;

    void makeConnectDock();
    QDockWidget *connectDock = nullptr;
    QComboBox *comboPort = nullptr;
    QComboBox *comboProfile = nullptr;
    QPushButton *buttonConnect = nullptr;
    QCheckBox *cbReset = nullptr;
    QTimer *connectionTimer = nullptr;
    QSpinBox *sbTemperatureTimer = nullptr;
    void makeMoveDock();
    QDockWidget *moveDock = nullptr;
    MovementWidget *movementWidget = nullptr;

    void makeTempControlsDock();
    QDockWidget *tempControlsDock = nullptr;
    TemperatureWidget *temperatureWidget = nullptr;

    void makeSdDock();
    QDockWidget *sdDock = nullptr;
    SdWidget *sdWidget = nullptr;

    void makeProfileDock();
    QDockWidget *profileDock = nullptr;
    ProfileManager *profileManager = nullptr;
};
