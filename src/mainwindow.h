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
    void checkReceivedCommand();
    void checkPushedCommands(QByteArray);

private:
    SerialLayer *ser;
    Ui::MainWindow *ui;
    void locateSerialPort();
    QStringList serialPortList;
    void click();
    QString getTime();
    QString logHeader();
    QString rLogHeader();
    QString sLogHeader();
    void addLog(QString);

    /**
     * @brief ...
     *
     * @param  p_:...
     */
    void addRLog(QString);

    /**
     * @brief Add in logger send type message
     *
     * @param  p_: Message
     */
    void addSLog(QString);
};
