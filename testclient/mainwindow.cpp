#include <QTime>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>

#include "mainwindow.h"
#include "seriallayer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    core(new AtCore(this)),
    deviceNotifier(Solid::DeviceNotifier::instance()),
    logFile(new QTemporaryFile(QDir::tempPath() + QStringLiteral("/AtCore_")))
{
    ui->setupUi(this);
    QValidator *validator = new QIntValidator();
    ui->serialPortCB->setEditable(true);
    ui->baudRateLE->setValidator(validator);
    ui->baudRateLE->setText(QStringLiteral("115200"));

    ui->pluginCB->addItem(tr("Autodetect"));
    ui->pluginCB->addItems(core->availablePlugins());

    addLog(tr("Attempting to locate Serial Ports"));

    //hide the printing progress bar.
    ui->printingProgress->setVisible(false);

    locateSerialPort();

    connect(ui->connectPB, &QPushButton::clicked, this, &MainWindow::connectPBClicked);
    connect(ui->saveLogPB, &QPushButton::clicked, this, &MainWindow::saveLogPBClicked);
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
    connect(deviceNotifier, &Solid::DeviceNotifier::deviceAdded, this, &MainWindow::locateSerialPort);
    connect(deviceNotifier, &Solid::DeviceNotifier::deviceRemoved, this, &MainWindow::locateSerialPort);
    connect(core, &AtCore::printProgressChanged, ui->printingProgress, &QProgressBar::setValue);
    connect(ui->pluginCB, &QComboBox::currentTextChanged, this , &MainWindow::pluginCBChanged);
    connect(core, &AtCore::stateChanged, this, &MainWindow::printerStateChanged);
    connect(this, &MainWindow::printFile, core, &AtCore::print);
    connect(ui->stopPB, &QPushButton::clicked, core, &AtCore::stop);
}

MainWindow::~MainWindow()
{
    delete logFile;
    delete ui;
}

QString MainWindow::getTime()
{
    return QTime::currentTime().toString(QStringLiteral("hh:mm:ss:zzz"));
}

QString MainWindow::logHeader()
{
    return QStringLiteral("[%1]  ").arg(getTime());
}

QString MainWindow::rLogHeader()
{
    return QStringLiteral("[%1]< ").arg(getTime());
}

QString MainWindow::sLogHeader()
{
    return QStringLiteral("[%1]> ").arg(getTime());
}

void MainWindow::writeTempFile(QString text)
{
    /*
    A QTemporaryFile will always be opened in QIODevice::ReadWrite mode,
    this allows easy access to the data in the file. This function will
    return true upon success and will set the fileName() to the unique
    filename used.
    */
    logFile->open();
    logFile->seek(logFile->size());
    logFile->write(text.toLatin1());
    logFile->putChar('\n');
    logFile->close();
}

void MainWindow::addLog(QString msg)
{
    QString message(logHeader() + msg);
    ui->logTE->appendPlainText(message);
    writeTempFile(message);
}

void MainWindow::addRLog(QString msg)
{
    QString message(rLogHeader() + msg);
    ui->logTE->appendPlainText(message);
    writeTempFile(message);
}

void MainWindow::addSLog(QString msg)
{
    QString message(sLogHeader() + msg);
    ui->logTE->appendPlainText(message);
    writeTempFile(message);
}

void MainWindow::checkReceivedCommand()
{
    addRLog(QString::fromUtf8(core->popCommand()));
}

void MainWindow::checkPushedCommands(QByteArray bmsg)
{
    QString msg = QString::fromUtf8(bmsg);
    QRegExp _newLine(QChar::fromLatin1('\n'));
    QRegExp _return(QChar::fromLatin1('\r'));
    msg.replace(_newLine, QStringLiteral("\\n"));
    msg.replace(_return, QStringLiteral("\\r"));
    addSLog(msg);
}
/**
 * @brief MainWindow::locateSerialPort
 * Locate all active serial ports on the computer and add to the list
 * of serial ports
 */
void MainWindow::locateSerialPort()
{
    QStringList ports;
    QList<QSerialPortInfo> serialPortInfoList = QSerialPortInfo::availablePorts();
    if (!serialPortInfoList.isEmpty()) {
        foreach (const QSerialPortInfo &serialPortInfo, serialPortInfoList) {
            ports.append(QStringLiteral("/dev/") + serialPortInfo.portName());
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
        serialPortList.clear();
        ui->serialPortCB->clear();
        addLog(tr("Not available ports! Please connect a serial device to continue!"));
    }
}

void MainWindow::connectPBClicked()
{
    if (core->state() == DISCONNECTED) {
        core->initFirmware(ui->serialPortCB->currentText(), ui->baudRateLE->text().toInt());
        connect(core->serial(), &SerialLayer::receivedCommand, this, &MainWindow::checkReceivedCommand);
        connect(core->serial(), &SerialLayer::pushedCommand, this, &MainWindow::checkPushedCommands);
        addLog(tr("Serial connected"));
        ui->connectPB->setText(tr("Disconnect"));
        if (!core->pluginLoaded()) {
            if (ui->pluginCB->currentText().contains(tr("Autodetect"))) {
                addLog(tr("No plugin loaded !"));
                addLog(tr("Requesting Firmware..."));
                core->detectFirmware();
            } else {
                core->loadFirmware(ui->pluginCB->currentText());
            }
        }
    } else {
        core->serial()->closeConnection();
        core->setState(DISCONNECTED);
        addLog(tr("Disconnected"));
        ui->connectPB->setText(tr("Connect"));
    }
}

void MainWindow::sendPBClicked()
{
    QString comm = ui->commandLE->text().toUpper();
    core->pushCommand(comm);
    ui->commandLE->clear();
}

void MainWindow::homeAllPBClicked()
{
    addSLog(tr("Home All"));
    core->pushCommand(QStringLiteral("G28"));
}

void MainWindow::homeXPBClicked()
{
    addSLog(tr("Home X"));
    core->pushCommand(QStringLiteral("G28 X0"));
}

void MainWindow::homeYPBClicked()
{
    addSLog(tr("Home Y"));
    core->pushCommand(QStringLiteral("G28 Y0"));
}

void MainWindow::homeZPBClicked()
{
    addSLog(tr("Home Z"));
    core->pushCommand(QStringLiteral("G28 Z0"));
}

void MainWindow::bedTempPBClicked()
{
    addSLog(tr("Set Bed Temp: %1")
            .arg(QString::number(ui->bedTempSB->value())));
    core->pushCommand(QStringLiteral("M140 S%1")
                                .arg(ui->bedTempSB->value()));
}

void MainWindow::extTempPBClicked()
{
    addSLog(tr("Set Extruder(%1) Temp: %2")
            .arg(ui->extTempSelCB->currentText().at(9), QString::number(ui->extTempSB->value())));
    core->pushCommand(QStringLiteral("M104 P%1 S%2")
                                .arg(ui->extTempSelCB->currentText().at(9), QString::number(ui->extTempSB->value()))
                                );
}

void MainWindow::mvAxisPBClicked()
{
    addSLog(tr("Move %1 to %2")
            .arg(ui->mvAxisCB->currentText().at(5), QString::number(ui->mvAxisSB->value())));
    core->pushCommand(QStringLiteral("G1 %1%2")
                                .arg(ui->mvAxisCB->currentText().at(5), QString::number(ui->mvAxisSB->value()))
                                );
}

void MainWindow::fanSpeedPBClicked()
{
    addSLog(tr("Set Fan(%1) Speed: %2\%")
            .arg(ui->fanSpeedSelCB->currentText().at(4), QString::number(ui->fanSpeedSB->value())));
    core->pushCommand(QStringLiteral("M106 P%1 S%2")
                                .arg(ui->fanSpeedSelCB->currentText().at(4), QString::number(ui->fanSpeedSB->value()))
                                );
}

void MainWindow::printPBClicked()
{
    QString fileName;
    switch (core->state()) {

    case DISCONNECTED:
        QMessageBox::information(this, tr("Error"), tr("Not Connected To a Printer"));
        break;

    case CONNECTING:
        QMessageBox::information(this, tr("Error"), tr("Firmware Plugin not loaded"));
        break;

    case IDLE:
        fileName = QFileDialog::getOpenFileName(this, tr("Select a file to print"), QDir::homePath(), QStringLiteral("*.gcode"));
        if (fileName.isNull()) {
            addLog(tr("No File Selected"));
        } else {
            addLog(tr("Print: %1").arg(fileName));
            emit(printFile(fileName));
        }
        break;

    case BUSY:
        core->setState(PAUSE);
        break;

    case PAUSE:
        core->setState(BUSY);
        break;
    default:
        qDebug() << "ERROR / STOP unhandled.";
    }
}

void MainWindow::saveLogPBClicked()
{
    // Note that if a file with the name newName already exists, copy() returns false (i.e. QFile will not overwrite it).
    QString fileName = QDir::homePath() + QChar::fromLatin1('/') + QFileInfo(logFile->fileName()).fileName();
    QString saveFileName = QFileDialog::getSaveFileName(this, tr("Save Log to file"), fileName);
    QFile::copy(logFile->fileName(), saveFileName);
    logFile->close();
}
void MainWindow::pluginCBChanged(QString currentText)
{
    if (core->state() != DISCONNECTED) {
        if (!currentText.contains(tr("Autodetect"))) {
            core->loadFirmware(currentText);
        } else {
            core->detectFirmware();
        }
    }
}

void MainWindow::printerStateChanged(PrinterState state)
{
    switch (state) {
    case IDLE:
        ui->printPB->setText(tr("Print File"));
        break;

    case STARTPRINT:
        ui->printPB->setText(tr("Pause Print"));
        ui->printingProgress->setVisible(true);
        break;

    case FINISHEDPRINT:
        ui->printPB->setText(tr("Print File"));
        ui->printingProgress->setVisible(false);
        break;

    case PAUSE:
        ui->printPB->setText(tr("Resume Print"));
        break;

    case BUSY:
        ui->printPB->setText(tr("Pause Print"));
        break;

    default:
        break;
    }
}
