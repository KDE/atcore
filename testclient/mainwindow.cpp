
#include <QTime>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>

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

    core = new AtCore(this);
    locateSerialPBClicked();

    connect(ui->locateSerialPB, &QPushButton::clicked, this, &MainWindow::locateSerialPBClicked);
    connect(ui->connectPB, &QPushButton::clicked, this, &MainWindow::connectPBClicked);
    connect(ui->sendPB, &QPushButton::clicked, this, &MainWindow::sendPBClicked);
    connect(ui->commandLE, &QLineEdit::returnPressed, this, &MainWindow::sendPBClicked);
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
    addRLog(core->serial()->popCommand());
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
 * @brief MainWindow::locateSerialPBClicked
 * Locate all active serial ports on the computer and add to the list
 * of serial ports
 */
void MainWindow::locateSerialPBClicked()
{
    QStringList ports;
    QList<QSerialPortInfo> serialPortInfoList = QSerialPortInfo::availablePorts();
    if (!serialPortInfoList.isEmpty()) {
        foreach (const QSerialPortInfo &serialPortInfo, serialPortInfoList) {
            ports.append("/dev/" + serialPortInfo.portName());
        }
        if (ports == serialPortList) {
            return;
        } else {
            serialPortList.clear();
            serialPortList = ports;
            ui->serialPortCB->clear();
            ui->serialPortCB->addItems(serialPortList);
            addLog(tr("Found %1 Ports").arg(QString::number(serialPortList.count())));
        }
    } else {
        addLog(tr("Not available ports! Please connect a serial device to continue!"));
    }
}

void MainWindow::connectPBClicked()
{
    if (core->state() == DISCONNECTED) {
        core->initFirmware(ui->serialPortCB->currentText(), ui->baudRateLE->text().toInt());
        connect(core->serial(), &SerialLayer::receivedCommand, this, &MainWindow::checkReceivedCommand);
        connect(core->serial(), &SerialLayer::pushedCommand, this, &MainWindow::checkPushedCommands);
        ui->connectPB->setText(tr("Disconnect"));
    } else {
        core->serial()->closeConnection();
        core->setState(DISCONNECTED);
        addLog(tr("Disconnected"));
        ui->connectPB->setText(tr("Connect"));
    }
}

void MainWindow::sendPBClicked()
{
    QByteArray comm = ui->commandLE->text().toUpper().toLocal8Bit();
    core->serial()->pushCommand(comm);
    ui->commandLE->clear();
}

void MainWindow::homeAllPBClicked()
{
    addSLog(tr("Home All"));
    core->serial()->pushCommand(QString("G28").toLocal8Bit());
}

void MainWindow::homeXPBClicked()
{
    addSLog(tr("Home X"));
    core->serial()->pushCommand(QString("G28 X0").toLocal8Bit());
}

void MainWindow::homeYPBClicked()
{
    addSLog(tr("Home Y"));
    core->serial()->pushCommand(QString("G28 Y0").toLocal8Bit());
}

void MainWindow::homeZPBClicked()
{
    addSLog(tr("Home Z"));
    core->serial()->pushCommand(QString("G28 Z0").toLocal8Bit());
}

void MainWindow::bedTempPBClicked()
{
    addSLog(tr("Set Bed Temp: %1")
            .arg(QString::number(ui->bedTempSB->value())));
    core->serial()->pushCommand(QString("M140 S%1")
                                .arg(ui->bedTempSB->value()).toLocal8Bit());
}

void MainWindow::extTempPBClicked()
{
    addSLog(tr("Set Extruder(%1) Temp: %2")
            .arg(ui->extTempSelCB->currentText().at(9), QString::number(ui->extTempSB->value())));
    core->serial()->pushCommand(QString("M104 P%1 S%2")
                                .arg(ui->extTempSelCB->currentText().at(9), QString::number(ui->extTempSB->value()))
                                .toLocal8Bit());
}

void MainWindow::mvAxisPBClicked()
{
    addSLog(tr("Move %1 to %2")
            .arg(ui->mvAxisCB->currentText().at(5), QString::number(ui->mvAxisSB->value())));
    core->serial()->pushCommand(QString("G1 %1%2")
                                .arg(ui->mvAxisCB->currentText().at(5), QString::number(ui->mvAxisSB->value()))
                                .toLocal8Bit());
}

void MainWindow::fanSpeedPBClicked()
{
    addSLog(tr("Set Fan(%1) Speed: %2\%")
            .arg(ui->fanSpeedSelCB->currentText().at(4), QString::number(ui->fanSpeedSB->value())));
    core->serial()->pushCommand(QString("M106 P%1 S%2")
                                .arg(ui->fanSpeedSelCB->currentText().at(4), QString::number(ui->fanSpeedSB->value()))
                                .toLocal8Bit());
}

void MainWindow::printPBClicked()
{
    QString fileName;
    switch (core->state()) {

    case DISCONNECTED:
        QMessageBox::information(this, tr("Error"), tr("Not Connected To a Printer"));
        break;

    case CONNECTING:
        QMessageBox::information(this, tr("Error"), tr("Firmware Plugin not loaded try sending M115"));
        break;

    case IDLE:
        fileName = QFileDialog::getOpenFileName(this, tr("Select a file to print"), QDir::homePath(), "*.gcode");
        if (fileName.isNull()) {
            addLog(tr("No File Selected"));
        } else {
            addLog(tr("Print: %1").arg(fileName));
            ui->printPB->setText(tr("Pause Print"));
            core->print(fileName);
        }
        break;

    case BUSY:
        core->setState(PAUSE);
        ui->printPB->setText(tr("Resume Print"));
        break;

    case PAUSE:
        core->setState(BUSY);
        ui->printPB->setText(tr("Pause Print"));
        break;
    }
}
