#pragma once

#include <QMainWindow>
#include <QSerialPort>

class SerialLayer;

class  MainWindow:public QMainWindow
{
Q_OBJECT

private:
    SerialLayer *ser;
public:
    MainWindow(QString port, uint baud, QWidget *parent = 0);
    ~MainWindow();

    void checkCommand();
};
