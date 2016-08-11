
#include <QTime>
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
    ui->baudRateLE->setText("115200");
    addLog(tr("Attempting to locate Serial Ports"));
    locateSerialPort();

    connect(ui->connectPB, &QPushButton::clicked, this, &MainWindow::click);
    connect(ui->disconnectPB, &QPushButton::clicked, this, &MainWindow::click);
    connect(ui->sendPB,  &QPushButton::clicked, this, &MainWindow::click);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::getTime()
{
    return QTime::currentTime().toString("hh:mm:ss:zzz");
}

QString MainWindow::logHeader()
{
    return QString("[%1]  ").arg(getTime());
}

QString MainWindow::rLogHeader()
{
    return QString("[%1]< ").arg(getTime());
}

QString MainWindow::sLogHeader()
{
    return QString("[%1]> ").arg(getTime());
}

void MainWindow::addLog(QString msg)
{
    ui->logTE->appendPlainText(logHeader() + msg);
}

void MainWindow::addRLog(QString msg)
{
    ui->logTE->appendPlainText(rLogHeader() + msg);
}

void MainWindow::addSLog(QString msg)
{
    ui->logTE->appendPlainText(sLogHeader() + msg);
}

void MainWindow::checkReceivedCommand()
{
    addRLog(ser->popCommand());
}

void MainWindow::checkPushedCommands(QByteArray bmsg)
{
    QString msg = QString(bmsg);
    QRegExp _newLine(QChar('\n'));
    QRegExp _return(QChar('\r'));
    msg.replace(_newLine, QString("\\n"));
    msg.replace(_return, QString("\\r"));
    addSLog(msg);
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
            addLog(tr("Found %1 Ports").arg(QString::number(serialPortList.count())));
        }
    }else{
        addLog(tr("Not available ports! Please connect a serial device to continue!"));
    }

}

void MainWindow::click()
{
     QPushButton *btn = qobject_cast<QPushButton*>(sender());

    if(btn == ui->connectPB ){

        QString port = ui->serialPortCB->currentText();
        uint baud = ui->baudRateLE->text().toUInt();
        ser = new SerialLayer(port, baud);
        connect(ser, &SerialLayer::receivedCommand, this, &MainWindow::checkReceivedCommand);
        connect(ser, &SerialLayer::pushedCommand, this, &MainWindow::checkPushedCommands);
        ui->logTE->clear();
        addLog(tr("Connected to %1").arg(port));

    } else if(btn == ui->disconnectPB){

        ser->closeConnection();
        addLog(tr("Disconnected"));

    } else if(btn == ui->sendPB){

        QByteArray comm = ui->commandLE->text().toUpper().toLocal8Bit();
        ser->pushCommand(comm);
        ui->commandLE->clear();

    }
}
