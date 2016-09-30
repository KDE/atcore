#pragma once

#include "ui_mainwindow.h"
#include "atcore.h"

#include <QMainWindow>
#include <QSerialPort>
#include <QTemporaryFile>

#include <KF5/Solid/Solid/DeviceNotifier>

class SerialLayer;

class  MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
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

private slots:
    //ButtonEvents
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

private:
    Ui::MainWindow *ui;
    AtCore *core;
    QStringList serialPortList;
    Solid::DeviceNotifier *deviceNotifier;
    QTemporaryFile *logFile;

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
};
