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

    void checkCommand();

private:
    SerialLayer *ser;
    Ui::MainWindow *ui;
    void locateSerialPort();
    QStringList serialPortList;
};
