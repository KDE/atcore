/* AtCore Test Client
    Copyright (C) <2016>

    Authors:
        Patrick José Pereira <patrickelectric@gmail.com>
        Lays Rodrigues <laysrodrigues@gmail.com>
        Chris Rizzitello <rizzitello@kde.org>
        Tomaz Canabrava <tcanabrava@kde.org>

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

#include <QTemporaryFile>
#include <QMainWindow>
#include <QSerialPort>

#include "atcore.h"
#include "widgets/plotwidget.h"

class SerialLayer;

class  MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

public slots:
    /**
     * @brief Check received messages
     *
     */
    void checkReceivedCommand(const QByteArray &message);

    /**
     * @brief Check pushed message
     *
     * @param  bmsg : Message
     */
    void checkPushedCommands(QByteArray);

    /**
     * @brief Check temperature
     *
     * @param  sensorType : type of sensor
     * @param  number     : index of sensor
     * @param  temp       : temperature
     */
    void checkTemperature(uint sensorType, uint number, uint temp);

private slots:
    //ButtonEvents
    /**
     * @brief axisControlClicked Used to catch the axis control.
     * @param axis The Axis clicked on (X |Y |Z)
     * @param value Distance Value
     */
    void axisControlClicked(QLatin1Char axis, int value);
    /**
     * @brief the printing progress has changed
     * @param progress: the new progress
     */
    void printProgressChanged(int progress);

    /**
     * @brief Connect Button Clicked will connect or disconnect based on printer state
     */
    void connectPBClicked();

    /**
     * @brief Send Command Clicked
     */
    void sendPBClicked();

    /**
     * @brief Home All Clicked
     */
    void homeAllPBClicked();

    /**
     * @brief Home X Axis Clicked
     */
    void homeXPBClicked();

    /**
     * @brief Home Y Axis Clicked
     */
    void homeYPBClicked();

    /**
     * @brief Home Z Axis Clicked
     */
    void homeZPBClicked();

    /**
     * @brief Set Bed Temp Clicked
     */
    void bedTempPBClicked();

    /**
     * @brief Set Extruder Temp Clicked
     */
    void extTempPBClicked();

    /**
     * @brief Move Axis Clicked
     */
    void mvAxisPBClicked();

    /**
     * @brief Set Fan Speed Clicked
     */
    void fanSpeedPBClicked();

    /**
     * @brief Print Button Clicked, can also pause /resue print based on printer state
     */
    void printPBClicked();

    /**
     * @brief Print Button for Sd Prints clicked.
     */
    void sdPrintPBClicked();
    /**
     * @brief Save the log file.
     */
    void saveLogPBClicked();

    /**
     * @brief  set printer speed clicked
     */
    void printerSpeedPBClicked();

    /**
     * @brief lowRatePB has been clicked
     */
    void flowRatePBClicked();

    /**
     * @brief disableMotorsPB has been clicked
     */
    void disableMotorsPBClicked();

    /**
     * @brief printerStateChanged Catch and proccess printer state commands
     * @param state: new printer state
     */
    void printerStateChanged(AtCore::STATES state);

    /**
     * @brief showMessage show a message on the printers LCD
     */
    void showMessage();

    /**
     * @brief Update the print Time
     */
    void updatePrintTime();

    /**
     * @brief show/hide dock titlebars
     * @param checked: True if shown
     */
    void toggleDockTitles(bool checked);

    /**
     * @brief Toggle disable state of dangerous docks
     * Command, Sd Card, Movement, Temperature Controls and Printing docks
     * @param disabled: True if items are disabled.
     */
    void setDangeriousDocksDisabled(bool disabled);

    /**
     * @brief List Files on the sd card.
     */
    void getSdList();

    /**
     * @brief Sd Card Delete file clicked
     */
    void sdDelPBClicked();

private:
    AtCore *core;
    QTemporaryFile *logFile;
    QTime *printTime;
    QTimer *printTimer;
    // Define max number of fans
    static int fanCount;

    void closeEvent(QCloseEvent *event) override;
    /**
     * @brief Locate serial port
     *
     */
    void locateSerialPort(const QStringList &ports);

    /**
     * @brief Return string with actual time
     *
     * @return QString
     */
    QString getTime();

    /**
     * @brief Append text in temporary file
     *
     * @param text
     */
    void writeTempFile(QString text);

    /**
     * @brief Normal header
     *
     * @return QString
     */
    QString logHeader();

    /**
     * @brief Header of type received
     *
     * @return QString
     */
    QString rLogHeader();

    /**
     * @brief Header of type send
     *
     * @return QString
     */
    QString sLogHeader();

    /**
     * @brief Add in logger normal type message
     *
     * @param  msg: Message
     */
    void addLog(QString msg);

    /**
     * @brief Add in logger received type message
     *
     * @param  msg: Message
     */
    void addRLog(QString msg);

    /**
     * @brief Add in logger send type message
     *
     * @param  msg: Message
     */
    void addSLog(QString msg);

    /**
     * @brief pluginCB index changed
     */
    void pluginCBChanged(QString currentText);

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

    //UI Functions

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

//Private GUI Items
    //menuView is global to allow for docks to be added / removed.
    QMenu *menuView = nullptr;
    //Status Bar Items
    QLabel *lblState = nullptr;
    QLabel *lblSd = nullptr;
    QWidget *printProgressWidget = nullptr;
    QProgressBar *printingProgress = nullptr;
    QLabel *lblTime = nullptr;
    QLabel *lblTimeLeft = nullptr;
    //Docks
    void makeLogDock();
    QDockWidget *logDock = nullptr;
    QPlainTextEdit *textLog = nullptr;

    void makeTempTimelineDock();
    QDockWidget *tempTimelineDock = nullptr;
    PlotWidget *plotWidget = nullptr;

    void makeCommandDock();
    QDockWidget *printDock = nullptr;
    QDockWidget *commandDock = nullptr;
    QLineEdit *lineCommand = nullptr;
    QLineEdit *lineMessage = nullptr;

    void makePrintDock();
    QPushButton *buttonPrint = nullptr;
    QLineEdit *linePostPause = nullptr;
    QSpinBox *sbFlowRate = nullptr;
    QSpinBox *sbPrintSpeed = nullptr;

    void makeConnectDock();
    QDockWidget *connectDock = nullptr;
    QComboBox *comboPort = nullptr;
    QComboBox *comboBAUD = nullptr;
    QComboBox *comboPlugin = nullptr;
    QPushButton *buttonConnect = nullptr;

    void makeMoveDock();
    QDockWidget *moveDock = nullptr;
    QComboBox *comboMoveAxis = nullptr;
    QDoubleSpinBox *sbMoveAxis = nullptr;

    void makeTempControlsDock();
    QDockWidget *tempControlsDock = nullptr;
    QCheckBox *checkAndWait = nullptr;
    QSpinBox *sbBedTemp = nullptr;
    QComboBox *comboExtruderSelect;
    QSpinBox *sbExtruderTemp;
    QComboBox *comboFanSelect;
    QSpinBox *sbFanSpeed;

    void makeSdDock();
    QDockWidget *sdDock = nullptr;
    QListWidget *listSdFiles = nullptr;
};
