#pragma once

#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QSerialPort>


class SerialLayer;

class  MainWindow: public QMainWindow
{
Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    /**
     * @brief Check received message
     *
     */
    void checkReceivedCommand();

    /**
     * @brief Check pushed message
     *
     * @param  bmsg : Message
     */
    void checkPushedCommands(QByteArray);

private:
    SerialLayer *ser;
    Ui::MainWindow *ui;
    QStringList serialPortList;

    /**
     * @brief Locate serial port
     *
     */
    void locateSerialPort();

    /**
     * @brief Check click event
     *
     */
    void click();

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
