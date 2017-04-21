/* AtCore Test Client
    Copyright (C) <2016>

    Authors:
        Patrick José Pereira <patrickelectric@gmail.com>
        Lays Rodrigues <laysrodrigues@gmail.com>
        Chris Rizzitello <sithlord48@gmail.com>
        Tomaz Canabrava <tcanabrava@kde.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QSerialPort>
#include <QTemporaryFile>

#include <Solid/DeviceNotifier>
#include <KXmlGui/KXmlGuiWindow>

#include "atcore.h"

class SerialLayer;

class  MainWindow: public KXmlGuiWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    /**
     * @brief Check received messages
     *
     */
    void checkReceivedCommand();

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
     * @brief Save the log file.
     */
    void saveLogPBClicked();

    /**
     * @brief  set printer speed clicked
     */
    void printerSpeedPBClicked();

    /**
     * @brief movementModeChanged The radio buttons for absoulte/relative have been toggled
     */
    void movementModeChanged(const bool &checked);

    /**
     * @brief lowRatePB has been clicked
     */
    void flowRatePBClicked();

    /**
     * @brief printerStateChanged Catch and proccess printer state commands
     * @param state: new printer state
     */
    void printerStateChanged(PrinterState state);

    /**
     * @brief showMessage show a message on the printers LCD
     */
    void showMessage();

    /**
     * @brief Update the print Time
     */
    void updatePrintTime();

signals:
    /**
     * @brief printFile emit ready to print a file to atcore
     * @param fileName : the file to print
     */
    void printFile(const QString &fileName);

private:
    Ui::MainWindow *ui;
    AtCore *core;
    Solid::DeviceNotifier *deviceNotifier;
    QTemporaryFile *logFile;
    QStringList serialPortList;
    QTime *printTime;
    QTimer *printTimer;
    void closeEvent(QCloseEvent *event);
    /**
     * @brief Locate serial port
     *
     */
    void locateSerialPort();

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

};
