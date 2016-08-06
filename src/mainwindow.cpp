
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
    _timer=new QTimer();
    QValidator *validator = new QIntValidator();
    ui->baudRateLE->setValidator(validator);
    ui->baudRateLE->setText("115200");
    ui->logTE->setPlainText(QString(tr("Attempting to locate Serial Ports")));
    locateSerialPort();

    connect(ui->connectPB, &QPushButton::clicked, this, &MainWindow::click);
    connect(ui->disconnectPB, &QPushButton::clicked, this, &MainWindow::click);
    connect(ui->sendPB,  &QPushButton::clicked, this, &MainWindow::click);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::checkCommand()
{
    while (ser->commandAvailable())
        ui->logTE->appendPlainText(QString("Recv: %1").arg(QString(ser->popCommand())));
}

void MainWindow::checkPushedCommands(QString msg)
{
    QRegExp _newLine(QChar('\n'));
    QRegExp _return(QChar('\r'));
    msg.replace(_newLine, QString("\\n"));
    msg.replace(_return, QString("\\r"));
    ui->logTE->appendPlainText(msg);
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
            ui->logTE->appendPlainText(QString(tr("Found %1 Ports").arg(QString::number(serialPortList.count()))));
        }
    }else{
        ui->logTE->appendPlainText(QString(tr("No Ports Found!")));
        QMessageBox msg;
        msg.setText(tr("Not available ports! Please connect a serial device to continue!"));
        msg.setIcon(QMessageBox::Critical);
        msg.exec();
    }

}

void MainWindow::click()
{
     QPushButton *btn = qobject_cast<QPushButton*>(sender());

    if(btn == ui->connectPB ){

        QString port = ui->serialPortCB->currentText();
        uint baud = ui->baudRateLE->text().toUInt();
        ser = new SerialLayer(port, baud);
        _timer->setInterval(1000);
        _timer->start();
        connect(_timer,&QTimer::timeout, this, &MainWindow::checkCommand);
        connect(ser, &SerialLayer::pushedCommand, this, &MainWindow::checkPushedCommands);
        ui->logTE->clear();
        ui->logTE->appendPlainText(QString(tr("Connected to %1").arg(port)));

    } else if(btn == ui->disconnectPB){

        ser->closeConnection();
        disconnect(_timer,&QTimer::timeout, this, &MainWindow::checkCommand);
        ui->logTE->appendPlainText(QString(tr("Disconnected")));

    } else if(btn == ui->sendPB){

        QByteArray comm = ui->commandLE->text().toUpper().toLocal8Bit() + "\n";
        ser->pushCommand(comm);
        ui->commandLE->clear();

    }
}
