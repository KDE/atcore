#include <QDebug>
#include <QTimer>
#include <QSerialPortInfo>
#include <QMessageBox>

#include "mainwindow.h"
#include "seriallayer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QValidator *validator = new QIntValidator();
    ui->baudRateLE->setValidator(validator);
    locateSerialPort();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::checkCommand()
{
    while (ser->commandAvailable())
        qDebug() << ser->popCommand();
}

/**
 * @brief MainWindow::locateSerialPort
 * Locate all active serial ports on the computer and add to the list
 * of serial ports
 */
void MainWindow::locateSerialPort()
{   QStringList ports;
    QList<QSerialPortInfo> serialPortInfoList = QSerialPortInfo::availablePorts();
    if(!serialPortInfoList.isEmpty())
    {   foreach (const QSerialPortInfo &serialPortInfo, serialPortInfoList) {
            ports.append("/dev/"+serialPortInfo.portName());
        }
        if(ports == serialPortList)
            return;
        else{
            serialPortList.clear();
            serialPortList = ports;
            ui->serialPortCB->clear();
            ui->serialPortCB->addItems(serialPortList);
        }
    }else{
        QMessageBox msg;
        msg.setText(tr("Not available ports! Please connect a serial device to continue!"));
        msg.setIcon(QMessageBox::Critical);
        msg.exec();
    }

}
/**
 * @brief MainWindow::on_connectPB_clicked
 * Start the communication between the serial device
 * and the computer
 */
void MainWindow::on_connectPB_clicked()
{
    QString port = ui->serialPortCB->currentText();
    uint baud = ui->baudRateLE->text().toUInt();
    ser = new SerialLayer(port, baud);
    qDebug() << "Connected!";

}
/**
 * @brief MainWindow::on_sendPB_clicked
 * Send command to serial port
 */
void MainWindow::on_sendPB_clicked()
{
    QByteArray comm = ui->commandLE->text().toLocal8Bit();
    ser->pushCommand(comm);
    qDebug() << "Command send it!";
}
/**
 * @brief MainWindow::on_disconnectPB_clicked
 * Close Serial Port
 */
void MainWindow::on_disconnectPB_clicked()
{
    //Close serial port
}
