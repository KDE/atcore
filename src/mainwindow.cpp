
#include <QTime>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>

#include "mainwindow.h"
#include "protocollayer.h"

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

    connect(ui->connectPB, &QPushButton::clicked, this, &MainWindow::connectPBClicked);
    connect(ui->disconnectPB, &QPushButton::clicked, this, &MainWindow::disconnectPBClicked);
    connect(ui->sendPB, &QPushButton::clicked, this, &MainWindow::sendPBClicked);
    connect(ui->homeAllPB, &QPushButton::clicked, this, &MainWindow::homeAllPBClicked);
    connect(ui->homeXPB, &QPushButton::clicked, this, &MainWindow::homeXPBClicked);
    connect(ui->homeYPB, &QPushButton::clicked, this, &MainWindow::homeYPBClicked);
    connect(ui->homeZPB, &QPushButton::clicked, this, &MainWindow::homeZPBClicked);
    connect(ui->bedTempPB, &QPushButton::clicked, this, &MainWindow::bedTempPBClicked);
    connect(ui->extTempPB, &QPushButton::clicked, this, &MainWindow::extTempPBClicked);
    connect(ui->mvAxisPB, &QPushButton::clicked, this, &MainWindow::mvAxisPBClicked);
    connect(ui->fanSpeedPB, &QPushButton::clicked, this, &MainWindow::fanSpeedPBClicked);
    connect(ui->printPB, &QPushButton::clicked, this, &MainWindow::printPBClicked);
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
    rtnCommand = pro->popCommand();
    addRLog(rtnCommand);
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
void MainWindow::connectPBClicked()
{
    QString port = ui->serialPortCB->currentText();
    uint baud = ui->baudRateLE->text().toUInt();
    pro = new ProtocolLayer(port, baud);
    connect(pro, &ProtocolLayer::receivedCommand, this, &MainWindow::checkReceivedCommand);
    connect(pro, &ProtocolLayer::pushedCommand, this, &MainWindow::checkPushedCommands);
    ui->logTE->clear();
    addLog(tr("Connected to %1").arg(port));
}

void MainWindow::disconnectPBClicked()
{
    pro->closeConnection();
    delete(pro);
    addLog(tr("Disconnected"));
}

void MainWindow::sendPBClicked()
{
    QByteArray comm = ui->commandLE->text().toUpper().toLocal8Bit();
    pro->pushCommand(comm);
    ui->commandLE->clear();
}

void MainWindow::homeAllPBClicked()
{
    addSLog(tr("Home All"));
    pro->pushCommand(QString("G28").toLocal8Bit());
}

void MainWindow::homeXPBClicked()
{
    addSLog(tr("Home X"));
    pro->pushCommand(QString("G28 X0").toLocal8Bit());
}

void MainWindow::homeYPBClicked()
{
    addSLog(tr("Home Y"));
    pro->pushCommand(QString("G28 Y0").toLocal8Bit());
}

void MainWindow::homeZPBClicked()
{
    addSLog(tr("Home Z"));
    pro->pushCommand(QString("G28 Z0").toLocal8Bit());
}

void MainWindow::bedTempPBClicked()
{
    addSLog(tr("Set Bed Temp: %1")
        .arg(QString::number(ui->bedTempSB->value())));
    pro->pushCommand(QString("M140 S%1")
        .arg(ui->bedTempSB->value()).toLocal8Bit());
}

void MainWindow::extTempPBClicked()
{
    addSLog(tr("Set Extruder(%1) Temp: %2")
        .arg(ui->extTempSelCB->currentText().at(9), QString::number(ui->extTempSB->value())));
    pro->pushCommand(QString("M104 P%1 S%2")
        .arg(ui->extTempSelCB->currentText().at(9), QString::number(ui->extTempSB->value()))
        .toLocal8Bit());
}

void MainWindow::mvAxisPBClicked()
{
    addSLog(tr("Move %1 to %2")
        .arg(ui->mvAxisCB->currentText().at(5), QString::number(ui->mvAxisSB->value())));
    pro->pushCommand(QString("G1 %1%2")
        .arg(ui->mvAxisCB->currentText().at(5), QString::number(ui->mvAxisSB->value()))
        .toLocal8Bit());
}

void MainWindow::fanSpeedPBClicked()
{
    addSLog(tr("Set Fan(%1) Speed: %2\%")
        .arg(ui->fanSpeedSelCB->currentText().at(4), QString::number(ui->fanSpeedSB->value())));
    pro->pushCommand(QString("M106 P%1 S%2")
        .arg(ui->fanSpeedSelCB->currentText().at(4), QString::number(ui->fanSpeedSB->value()))
        .toLocal8Bit());
}

void MainWindow::printPBClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select A file to print"), QDir::homePath(), "*.gcode");
    if(fileName.isNull()){
        addLog(tr("No File Selected"));
    }
    else{
        addLog(tr("print: %1").arg(fileName));
        QFile file(fileName);
        file.open(QFile::ReadOnly);
        QTextStream gcodestream(&file);
        QString cline;
        QEventLoop loop;
        connect(pro, &ProtocolLayer::receivedCommand, &loop, &QEventLoop::quit);

        addLog(tr("Reading File.."));
        while(!gcodestream.atEnd()){
            cline = gcodestream.readLine();
            cline = cline.simplified();
            if(cline.contains(QChar(';'))){
                cline.resize(cline.indexOf(QChar(';')));
            }
            if(!cline.isEmpty()){
                pro->pushCommand(cline.toLocal8Bit());
                bool waiting = true;
                while(waiting){
                    if(!pro->commandAvailable()){
                        loop.exec();
                    }
                    //Handle Messages from printer.
                    if (rtnCommand == "ok 0"){
                        waiting = false;
                    }

                    else if (rtnCommand == "wait"){
                        waiting = false;
                    }
                    /* Handle resend and ok 1 soon*/
                }
            }
        }
        addLog(tr("Finished Reading File"));
    }
}

