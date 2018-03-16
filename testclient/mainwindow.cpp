/* AtCore Test Client
    Copyright (C) <2016>

    Authors:
        Patrick José Pereira <patrickelectric@gmail.com>
        Lays Rodrigues <laysrodrigues@gmail.com>
        Chris Rizzitello <rizzitello@kde.org>
        Tomaz Canabrava <tcanabrava@kde.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <QTime>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QLoggingCategory>

#include "mainwindow.h"
#include "seriallayer.h"
#include "gcodecommands.h"
#include "widgets/axiscontrol.h"
#include "widgets/about.h"

Q_LOGGING_CATEGORY(TESTCLIENT_MAINWINDOW, "org.kde.atelier.core")

int MainWindow::fanCount = 4;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    core(new AtCore(this)),
    logFile(new QTemporaryFile(QDir::tempPath() + QStringLiteral("/AtCore_")))
{
    setWindowTitle(tr("AtCore - Test Client"));
    setWindowIcon(QIcon(QStringLiteral(":/icon/windowIcon")));
    QCoreApplication::setApplicationVersion(core->version());
    initMenu();
    initStatusBar();
    initWidgets();

    addLog(tr("Attempting to locate Serial Ports"));
    core->setSerialTimerInterval(1000);

    printTime = new QTime();
    printTimer = new QTimer();
    printTimer->setInterval(1000);
    printTimer->setSingleShot(false);
    connect(printTimer, &QTimer::timeout, this, &MainWindow::updatePrintTime);

    connect(core, &AtCore::stateChanged, this, &MainWindow::printerStateChanged);
    connect(core, &AtCore::portsChanged, this, &MainWindow::locateSerialPort);
    connect(core, &AtCore::printProgressChanged, this, &MainWindow::printProgressChanged);
    connect(core, &AtCore::sdMountChanged, this, &MainWindow::sdChanged);

    connect(core, &AtCore::sdCardFileListChanged, [ & ](QStringList fileList) {
        listSdFiles->clear();
        listSdFiles->addItems(fileList);
    });

    connect(&core->temperature(), &Temperature::bedTemperatureChanged, [ this ](float temp) {
        checkTemperature(0x00, 0, temp);
        plotWidget->appendPoint(tr("Actual Bed"), temp);
        plotWidget->update();
    });
    connect(&core->temperature(), &Temperature::bedTargetTemperatureChanged, [ this ](float temp) {
        checkTemperature(0x01, 0, temp);
        plotWidget->appendPoint(tr("Target Bed"), temp);
        plotWidget->update();
    });
    connect(&core->temperature(), &Temperature::extruderTemperatureChanged, [ this ](float temp) {
        checkTemperature(0x02, 0, temp);
        plotWidget->appendPoint(tr("Actual Ext.1"), temp);
        plotWidget->update();
    });
    connect(&core->temperature(), &Temperature::extruderTargetTemperatureChanged, [ this ](float temp) {
        checkTemperature(0x03, 0, temp);
        plotWidget->appendPoint(tr("Target Ext.1"), temp);
        plotWidget->update();
    });
}

void MainWindow::initMenu()
{
    QMenu *menuFile = new QMenu(tr("File"));
    QAction *actionQuit = new QAction(style()->standardIcon(QStyle::SP_DialogCloseButton), tr("Quit"));
    connect(actionQuit, &QAction::triggered, this, &MainWindow::close);
    menuFile->addAction(actionQuit);

    menuView = new QMenu(tr("View"));
    QAction *actionShowDockTitles = new QAction(tr("Show Dock Titles"));
    actionShowDockTitles->setCheckable(true);
    actionShowDockTitles->setChecked(true);
    connect(actionShowDockTitles, &QAction::toggled, this, &MainWindow::toggleDockTitles);
    menuView->addAction(actionShowDockTitles);

    QMenu *menuHelp = new QMenu(tr("Help"));
    QAction *actionAbout = new QAction(tr("About"));
    actionAbout->setShortcut(QKeySequence(Qt::Key_F1));
    connect(actionAbout, &QAction::triggered, [] {
        auto *dialog = new About;
        dialog->exec();
    });
    menuHelp->addAction(actionAbout);

    menuBar()->addMenu(menuFile);
    menuBar()->addMenu(menuView);
    menuBar()->addMenu(menuHelp);
}

void MainWindow::initStatusBar()
{
    //first create the item for the print Progress.
    printingProgress = new QProgressBar;
    auto *newButton = new QPushButton(style()->standardIcon(QStyle::SP_BrowserStop), QString());
    connect(newButton, &QPushButton::clicked, core, &AtCore::stop);
    lblTime = new QLabel(QStringLiteral("00:00:00"));
    lblTime->setAlignment(Qt::AlignHCenter);
    auto *newLabel = new QLabel(QStringLiteral(" / "));
    lblTimeLeft = new QLabel(QStringLiteral("00:00:00"));
    lblTimeLeft->setAlignment(Qt::AlignHCenter);

    auto *hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(printingProgress);
    hBoxLayout->addWidget(newButton);
    hBoxLayout->addWidget(lblTime);
    hBoxLayout->addWidget(newLabel);
    hBoxLayout->addWidget(lblTimeLeft);
    printProgressWidget = new QWidget();
    printProgressWidget->setLayout(hBoxLayout);

    //Then Create the full bar.
    newLabel = new QLabel(tr("AtCore State:"));
    lblState = new QLabel(tr("Not Connected"));
    lblSd = new QLabel();

    hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(newLabel);
    hBoxLayout->addWidget(lblState);
    hBoxLayout->addSpacerItem(new QSpacerItem(10, 20, QSizePolicy::Fixed));
    hBoxLayout->addWidget(lblSd);
    hBoxLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding));
    hBoxLayout->addWidget(printProgressWidget);

    //Then put it all into a widget on the bar.
    QWidget *sBar = new QWidget();
    sBar->setLayout(hBoxLayout);
    statusBar()->addPermanentWidget(sBar, 100);
}

void MainWindow::initWidgets()
{
    //Make the Docks
    makeCommandDock();
    makePrintDock();
    makeTempTimelineDock();
    makeLogDock();
    makeConnectDock();
    makeMoveDock();
    makeTempControlsDock();
    makeSdDock();

    setDangeriousDocksDisabled(true);

    setTabPosition(Qt::LeftDockWidgetArea, QTabWidget::North);
    setTabPosition(Qt::RightDockWidgetArea, QTabWidget::North);
    tabifyDockWidget(moveDock, tempControlsDock);
    tabifyDockWidget(moveDock, sdDock);
    moveDock->raise();

    tabifyDockWidget(connectDock, printDock);
    tabifyDockWidget(connectDock, commandDock);
    connectDock->raise();
    setCentralWidget(nullptr);

    //More Gui stuff
    populateCBs();
    //hide the printing progress bar.
    printProgressWidget->setVisible(false);
}

void MainWindow::makeCommandDock()
{
    //The processing for making a dock is the same for all docks
    //Expose the least amount of object outside the creation function.

    // First we make our mainLayout for the dock.
    auto *mainLayout = new QVBoxLayout;

    //Begin making content from top to bottom or left to right.
    //Making child layouts in the order you want to put them
    // onto the mainLayout
    lineCommand = new QLineEdit;
    lineCommand->setPlaceholderText(tr("Send Command"));

    //we have a few buttons to make here. Lets name this newButton so its easier to reuse
    auto *newButton = new QPushButton(tr("Send"));
    connect(newButton, &QPushButton::clicked, this, &MainWindow::sendPBClicked);

    //When you have created a Row put the items into layout.
    auto *hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(lineCommand);
    hBoxLayout->addWidget(newButton);
    //Put the Layout or Widget on the mainLayout when its finished.
    //This will free your pointers for reuse.
    mainLayout->addLayout(hBoxLayout);

    //Start making items for the next layout to place onto the mainLayout.
    lineMessage = new QLineEdit;
    lineMessage->setPlaceholderText(tr("Show Message"));

    //Here we reuse our  button pointer my having it point to a new button.
    newButton = new QPushButton(tr("Send"));
    connect(newButton, &QPushButton::clicked, this, &MainWindow::showMessage);

    //We reuse the hBoxLayout pointer in the same way as the button pointer.
    hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(lineMessage);
    hBoxLayout->addWidget(newButton);
    mainLayout->addLayout(hBoxLayout);

    //QDockWidget::setLayout is not ment to be used by us
    //We must instead Create a widget to hold or dock contents.
    auto *dockContents = new QWidget;
    dockContents->setLayout(mainLayout);

    //Finally create the dock, and set the Widget.
    commandDock = new QDockWidget(tr("Commands"), this);
    commandDock->setWidget(dockContents);

    //Push the toggle view action into our view menu
    menuView->insertAction(nullptr, commandDock->toggleViewAction());

    //Place the Dock into a DockWidget Area.
    //Failure todo this will create some odd side effects at runtime
    addDockWidget(Qt::LeftDockWidgetArea, commandDock);
}

void MainWindow::makePrintDock()
{
    auto *mainLayout = new QVBoxLayout;

    buttonPrint = new QPushButton(tr("Print File"));
    connect(buttonPrint, &QPushButton::clicked, this, &MainWindow::printPBClicked);

    auto *newButton = new QPushButton(tr("Emergency Stop"));
    connect(newButton, &QPushButton::clicked, core, &AtCore::emergencyStop);

    auto *hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(buttonPrint);
    hBoxLayout->addWidget(newButton);
    mainLayout->addLayout(hBoxLayout);

    auto *newLabel = new QLabel(tr("On Pause:"));
    linePostPause = new QLineEdit;
    linePostPause->setPlaceholderText(QStringLiteral("G91,G0 Z1,G90,G1 X0 Y195"));

    hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(newLabel);
    hBoxLayout->addWidget(linePostPause);
    mainLayout->addLayout(hBoxLayout);

    newLabel = new QLabel(tr("Printer Speed"));
    sbPrintSpeed = new QSpinBox;
    sbPrintSpeed->setRange(1, 300);
    sbPrintSpeed->setValue(100);
    sbPrintSpeed->setSuffix(QStringLiteral("%"));

    newButton = new QPushButton(tr("Set"));
    connect(newButton, &QPushButton::clicked, this, &MainWindow::printerSpeedPBClicked);

    hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(newLabel, 35);
    hBoxLayout->addWidget(sbPrintSpeed, 10);
    hBoxLayout->addWidget(newButton, 20);
    mainLayout->addLayout(hBoxLayout);

    newLabel = new QLabel(tr("Flow Rate"));
    sbFlowRate = new QSpinBox;
    sbFlowRate->setRange(1, 300);
    sbFlowRate->setValue(100);
    sbFlowRate->setSuffix(QStringLiteral("%"));

    newButton = new QPushButton(tr("Set"));
    connect(newButton, &QPushButton::clicked, this, &MainWindow::flowRatePBClicked);

    hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(newLabel, 35);
    hBoxLayout->addWidget(sbFlowRate, 10);
    hBoxLayout->addWidget(newButton, 20);
    mainLayout->addLayout(hBoxLayout);

    auto *dockContents = new QWidget;
    dockContents->setLayout(mainLayout);

    printDock = new QDockWidget(tr("Print"), this);
    printDock->setWidget(dockContents);

    menuView->insertAction(nullptr, printDock->toggleViewAction());
    addDockWidget(Qt::LeftDockWidgetArea, printDock);
}

void MainWindow::makeTempTimelineDock()
{
    plotWidget = new PlotWidget;
    tempTimelineDock = new QDockWidget(tr("Temperature Timeline"), this);
    tempTimelineDock->setWidget(plotWidget);
    menuView->insertAction(nullptr, tempTimelineDock->toggleViewAction());
    addDockWidget(Qt::RightDockWidgetArea, tempTimelineDock);
}

void MainWindow::makeLogDock()
{
    textLog = new QPlainTextEdit;
    textLog->setReadOnly(true);
    textLog->setMaximumBlockCount(1000);

    auto *newButton = new QPushButton(style()->standardIcon(QStyle::SP_DialogSaveButton), tr("Save Session Log"));
    connect(newButton, &QPushButton::clicked, this, &MainWindow::saveLogPBClicked);

    auto *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(textLog);
    mainLayout->addWidget(newButton);

    auto *dockContents = new QWidget();
    dockContents->setLayout(mainLayout);

    logDock = new QDockWidget(tr("Session Log"), this);
    logDock->setWidget(dockContents);

    menuView->insertAction(nullptr, logDock->toggleViewAction());
    addDockWidget(Qt::RightDockWidgetArea, logDock);
}

void MainWindow::makeConnectDock()
{
    auto *mainLayout = new QVBoxLayout;
    auto *newLabel = new QLabel(tr("Port:"));

    comboPort = new QComboBox;
    comboPort->setEditable(true);

    auto *hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(newLabel);
    hBoxLayout->addWidget(comboPort, 75);
    mainLayout->addLayout(hBoxLayout);

    newLabel = new QLabel(tr("Baud Rate:"));
    comboBAUD = new QComboBox;
    comboBAUD->addItems(core->portSpeeds());
    comboBAUD->setCurrentIndex(9);

    hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(newLabel);
    hBoxLayout->addWidget(comboBAUD, 75);
    mainLayout->addLayout(hBoxLayout);

    newLabel = new QLabel(tr("Use Plugin:"));
    comboPlugin = new QComboBox;
    comboPlugin->addItem(tr("Autodetect"));
    comboPlugin->addItems(core->availableFirmwarePlugins());
    connect(comboPlugin, &QComboBox::currentTextChanged, this, &MainWindow::pluginCBChanged);

    hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(newLabel);
    hBoxLayout->addWidget(comboPlugin, 75);
    mainLayout->addLayout(hBoxLayout);

    buttonConnect = new QPushButton(tr("Connect"));
    connect(buttonConnect, &QPushButton::clicked, this, &MainWindow::connectPBClicked);
    mainLayout->addWidget(buttonConnect);

    auto *dockContents = new QWidget;
    dockContents->setLayout(mainLayout);

    connectDock = new QDockWidget(tr("Connect"), this);
    connectDock->setWidget(dockContents);

    menuView->insertAction(nullptr, connectDock->toggleViewAction());
    addDockWidget(Qt::LeftDockWidgetArea, connectDock);
}

void MainWindow::makeMoveDock()
{
    auto *mainLayout = new QVBoxLayout;
    auto *hBoxLayout = new QHBoxLayout;

    auto *newButton = new QPushButton(tr("Home All"));
    connect(newButton, &QPushButton::clicked, this, &MainWindow::homeAllPBClicked);
    hBoxLayout->addWidget(newButton);

    newButton = new QPushButton(tr("Home X"));
    connect(newButton, &QPushButton::clicked, this, &MainWindow::homeXPBClicked);
    hBoxLayout->addWidget(newButton);

    newButton = new QPushButton(tr("Home Y"));
    connect(newButton, &QPushButton::clicked, this, &MainWindow::homeYPBClicked);
    hBoxLayout->addWidget(newButton);

    newButton = new QPushButton(tr("Home Z"));
    connect(newButton, &QPushButton::clicked, this, &MainWindow::homeZPBClicked);
    hBoxLayout->addWidget(newButton);

    mainLayout->addLayout(hBoxLayout);

    comboMoveAxis = new QComboBox;
    comboMoveAxis->addItem(tr("Move X Axis to"));
    comboMoveAxis->addItem(tr("Move Y Axis to"));
    comboMoveAxis->addItem(tr("Move Z Axis to"));

    sbMoveAxis = new QDoubleSpinBox;
    sbMoveAxis->setRange(0, 200);

    newButton = new QPushButton(tr("Go"));
    connect(newButton, &QPushButton::clicked, this, &MainWindow::mvAxisPBClicked);

    hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(comboMoveAxis);
    hBoxLayout->addWidget(sbMoveAxis);
    hBoxLayout->addWidget(newButton);
    mainLayout->addLayout(hBoxLayout);

    newButton = new QPushButton(tr("Disable Motors"));
    connect(newButton, &QPushButton::clicked, this, &MainWindow::disableMotorsPBClicked);
    mainLayout->addWidget(newButton);

    auto *axisControl = new AxisControl;
    connect(axisControl, &AxisControl::clicked, this, &MainWindow::axisControlClicked);
    mainLayout->addWidget(axisControl);

    auto *dockContents = new QWidget;
    dockContents->setLayout(mainLayout);

    moveDock = new QDockWidget(tr("Movement"), this);
    moveDock->setWidget(dockContents);

    menuView->insertAction(nullptr, moveDock->toggleViewAction());
    addDockWidget(Qt::LeftDockWidgetArea, moveDock);
}

void MainWindow::makeTempControlsDock()
{
    auto *mainLayout = new QVBoxLayout;
    checkAndWait = new QCheckBox(tr("Wait Untill Temperature Stablizes"));
    mainLayout->addWidget(checkAndWait);

    auto label = new QLabel(tr("Bed Temp"));

    sbBedTemp = new QSpinBox;
    sbBedTemp->setRange(0, 120);
    sbBedTemp->setSuffix(QStringLiteral("°C"));

    auto *newButton = new QPushButton(tr("Set"));
    connect(newButton, &QPushButton::clicked, this, &MainWindow::bedTempPBClicked);

    auto *hboxLayout = new QHBoxLayout;
    hboxLayout->addWidget(label, 80);
    hboxLayout->addWidget(sbBedTemp);
    hboxLayout->addWidget(newButton);
    mainLayout->addItem(hboxLayout);

    comboExtruderSelect = new QComboBox;
    sbExtruderTemp = new QSpinBox;
    sbExtruderTemp->setRange(0, 275);
    sbExtruderTemp->setSuffix(QStringLiteral("°C"));

    newButton = new QPushButton(tr("Set"));
    connect(newButton, &QPushButton::clicked, this, &MainWindow::extTempPBClicked);
    hboxLayout = new QHBoxLayout;
    hboxLayout->addWidget(comboExtruderSelect, 80);
    hboxLayout->addWidget(sbExtruderTemp);
    hboxLayout->addWidget(newButton);
    mainLayout->addItem(hboxLayout);

    comboFanSelect = new QComboBox;
    sbFanSpeed = new QSpinBox;
    sbFanSpeed->setRange(0, 100);
    sbFanSpeed->setSuffix(QStringLiteral("%"));

    newButton = new QPushButton(tr("Set"));
    connect(newButton, &QPushButton::clicked, this, &MainWindow::fanSpeedPBClicked);
    hboxLayout = new QHBoxLayout;
    hboxLayout->addWidget(comboFanSelect, 80);
    hboxLayout->addWidget(sbFanSpeed);
    hboxLayout->addWidget(newButton);
    mainLayout->addItem(hboxLayout);

    auto *dockContents = new QWidget;
    dockContents->setLayout(mainLayout);

    tempControlsDock = new QDockWidget(tr("Temperatures"), this);
    tempControlsDock->setWidget(dockContents);

    menuView->insertAction(nullptr, tempControlsDock->toggleViewAction());
    addDockWidget(Qt::LeftDockWidgetArea, tempControlsDock);
}

void MainWindow::makeSdDock()
{
    auto *hBoxLayout = new QHBoxLayout;

    auto *newButton = new QPushButton(tr("Get List"));
    connect(newButton, &QPushButton::clicked, this, &MainWindow::getSdList);
    hBoxLayout->addWidget(newButton);

    newButton = new QPushButton(tr("Print Selected"));
    connect(newButton, &QPushButton::clicked, this, &MainWindow::sdPrintPBClicked);
    hBoxLayout->addWidget(newButton);

    newButton = new QPushButton(tr("Delete Selected"));
    connect(newButton, &QPushButton::clicked, this, &MainWindow::sdDelPBClicked);
    hBoxLayout->addWidget(newButton);

    auto *groupFiles =  new QGroupBox(tr("Files On Sd Card"));
    listSdFiles = new QListWidget;
    auto *groupLayout = new QVBoxLayout;
    groupLayout->addWidget(listSdFiles);
    groupFiles->setLayout(groupLayout);

    auto *mainLayout = new QVBoxLayout;
    mainLayout->addItem(hBoxLayout);
    mainLayout->addWidget(groupFiles);

    auto *dockContents = new QWidget;
    dockContents->setLayout(mainLayout);

    sdDock = new QDockWidget(tr("Sd Card"), this);
    sdDock->setWidget(dockContents);

    menuView->insertAction(nullptr, sdDock->toggleViewAction());
    addDockWidget(Qt::LeftDockWidgetArea, sdDock);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    core->close();
    event->accept();
}

MainWindow::~MainWindow()
{
    delete logFile;
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
    textLog->appendPlainText(message);
    writeTempFile(message);
}

void MainWindow::addRLog(QString msg)
{
    QString message(rLogHeader() + msg);
    textLog->appendPlainText(message);
    writeTempFile(message);
}

void MainWindow::addSLog(QString msg)
{
    QString message(sLogHeader() + msg);
    textLog->appendPlainText(message);
    writeTempFile(message);
}

void MainWindow::checkReceivedCommand(const QByteArray &message)
{
    addRLog(QString::fromUtf8(message));
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

void MainWindow::checkTemperature(uint sensorType, uint number, uint temp)
{
    QString msg;
    switch (sensorType) {
    case 0x00: // bed
        msg = QString::fromLatin1("Bed Temperature ");
        break;

    case 0x01: // bed target
        msg = QString::fromLatin1("Bed Target Temperature ");
        break;

    case 0x02: // extruder
        msg = QString::fromLatin1("Extruder Temperature ");
        break;

    case 0x03: // extruder target
        msg = QString::fromLatin1("Extruder Target Temperature ");
        break;

    case 0x04: // enclosure
        msg = QString::fromLatin1("Enclosure Temperature ");
        break;

    case 0x05: // enclosure target
        msg = QString::fromLatin1("Enclosure Target Temperature ");
        break;
    }

    msg.append(QString::fromLatin1("[%1] : %2"));
    msg = msg.arg(QString::number(number))
          .arg(QString::number(temp));

    addRLog(msg);
}
/**
 * @brief MainWindow::locateSerialPort
 * Locate all active serial ports on the computer and add to the list
 * of serial ports
 */
void MainWindow::locateSerialPort(const QStringList &ports)
{
    comboPort->clear();
    if (!ports.isEmpty()) {
        comboPort->addItems(ports);
        addLog(tr("Found %1 Ports").arg(QString::number(ports.count())));
    } else {
        QString portError(tr("No available ports! Please connect a serial device to continue!"));
        if (! textLog->toPlainText().endsWith(portError)) {
            addLog(portError);
        }
    }
}

void MainWindow::connectPBClicked()
{
    if (core->state() == AtCore::DISCONNECTED) {
        if (core->initSerial(comboPort->currentText(), comboBAUD->currentText().toInt())) {
            connect(core, &AtCore::receivedMessage, this, &MainWindow::checkReceivedCommand);
            connect(core->serial(), &SerialLayer::pushedCommand, this, &MainWindow::checkPushedCommands);
            buttonConnect->setText(tr("Disconnect"));
            addLog(tr("Serial connected"));
            if (comboPlugin->currentText().contains(tr("Autodetect"))) {
                addLog(tr("No plugin loaded !"));
                addLog(tr("Requesting Firmware..."));
            } else {
                core->loadFirmwarePlugin(comboPlugin->currentText());
            }
        } else {
            addLog(tr("Failed to open serial in r/w mode"));
        }
    } else {
        disconnect(core, &AtCore::receivedMessage, this, &MainWindow::checkReceivedCommand);
        disconnect(core->serial(), &SerialLayer::pushedCommand, this, &MainWindow::checkPushedCommands);
        core->closeConnection();
        core->setState(AtCore::DISCONNECTED);
        addLog(tr("Disconnected"));
        buttonConnect->setText(tr("Connect"));
    }
}

void MainWindow::sendPBClicked()
{
    QString comm = lineCommand->text().toUpper();
    core->pushCommand(comm);
    lineCommand->clear();
}

void MainWindow::homeAllPBClicked()
{
    addSLog(tr("Home All"));
    core->home();
}

void MainWindow::homeXPBClicked()
{
    addSLog(tr("Home X"));
    core->home(AtCore::X);
}

void MainWindow::homeYPBClicked()
{
    addSLog(tr("Home Y"));
    core->home(AtCore::Y);
}

void MainWindow::homeZPBClicked()
{
    addSLog(tr("Home Z"));
    core->home(AtCore::Z);
}

void MainWindow::bedTempPBClicked()
{
    if (checkAndWait->isChecked()) {
        addSLog(GCode::description(GCode::M190));
    } else {
        addSLog(GCode::description(GCode::M140));
    }
    core->setBedTemp(sbBedTemp->value(), checkAndWait->isChecked());
}

void MainWindow::extTempPBClicked()
{
    if (checkAndWait->isChecked()) {
        addSLog(GCode::description(GCode::M109));
    } else {
        addSLog(GCode::description(GCode::M104));
    }
    core->setExtruderTemp(sbExtruderTemp->value(), comboExtruderSelect->currentIndex(), checkAndWait->isChecked());
}

void MainWindow::mvAxisPBClicked()
{
    addSLog(GCode::description(GCode::G1));
    if (comboMoveAxis->currentIndex() == 0) {
        core->move(AtCore::X, sbMoveAxis->value());
    } else if (comboMoveAxis->currentIndex() == 1) {
        core->move(AtCore::Y, sbMoveAxis->value());
    } else if (comboMoveAxis->currentIndex() == 2) {
        core->move(AtCore::Z, sbMoveAxis->value());
    }
}

void MainWindow::fanSpeedPBClicked()
{
    addSLog(GCode::description(GCode::M106));
    core->setFanSpeed(sbFanSpeed->value(), comboFanSelect->currentIndex());
}

void MainWindow::printPBClicked()
{
    QString fileName;
    switch (core->state()) {

    case AtCore::DISCONNECTED:
        QMessageBox::information(this, tr("Error"), tr("Not Connected To a Printer"));
        break;

    case AtCore::CONNECTING:
        QMessageBox::information(this, tr("Error"), tr(" A Firmware Plugin was not loaded!\n  Please send the command M115 and let us know what your firmware returns, so we can improve our firmware detection. We have loaded the most common plugin \"repetier\" for you. You may try to print again after this message"));
        comboPlugin->setCurrentText(QStringLiteral("repetier"));
        break;

    case AtCore::IDLE:
        fileName = QFileDialog::getOpenFileName(this, tr("Select a file to print"), QDir::homePath(), tr("*.gcode"));
        if (fileName.isNull()) {
            addLog(tr("No File Selected"));
        } else {
            addLog(tr("Print: %1").arg(fileName));
            core->print(fileName);
        }
        break;

    case AtCore::BUSY:
        core->pause(linePostPause->text());
        break;

    case AtCore::PAUSE:
        core->resume();
        break;

    default:
        qCDebug(TESTCLIENT_MAINWINDOW) << "ERROR / STOP unhandled.";
    }
}

void MainWindow::saveLogPBClicked()
{
    // Note that if a file with the name newName already exists, copy() returns false (i.e. QFile will not overwrite it).
    QString fileName = QDir::homePath() + QChar::fromLatin1('/') + QFileInfo(logFile->fileName()).fileName() + QStringLiteral(".txt");
    QString saveFileName = QFileDialog::getSaveFileName(this, tr("Save Log to file"), fileName);
    QFile::copy(logFile->fileName(), saveFileName);
    logFile->close();
}
void MainWindow::pluginCBChanged(QString currentText)
{
    if (core->state() != AtCore::DISCONNECTED) {
        if (!currentText.contains(tr("Autodetect"))) {
            core->loadFirmwarePlugin(currentText);
        }
    }
}

void MainWindow::flowRatePBClicked()
{
    core->setFlowRate(sbFlowRate->value());
}

void MainWindow::printerSpeedPBClicked()
{
    core->setPrinterSpeed(sbPrintSpeed->value());
}

void MainWindow::printerStateChanged(AtCore::STATES state)
{
    QString stateString;
    switch (state) {
    case AtCore::IDLE:
        buttonPrint->setText(tr("Print File"));
        stateString = tr("Connected to ") + core->connectedPort();
        break;

    case AtCore::STARTPRINT:
        stateString = tr("START PRINT");
        buttonPrint->setText(tr("Pause Print"));
        printProgressWidget->setVisible(true);
        printTime->start();
        printTimer->start();
        break;

    case AtCore::FINISHEDPRINT:
        stateString = tr("Finished Print");
        buttonPrint->setText(tr("Print File"));
        printProgressWidget->setVisible(false);
        printTimer->stop();
        break;

    case AtCore::PAUSE:
        stateString = tr("Paused");
        buttonPrint->setText(tr("Resume Print"));
        break;

    case AtCore::BUSY:
        stateString = tr("Printing");
        buttonPrint->setText(tr("Pause Print"));
        break;

    case AtCore::DISCONNECTED:
        stateString = QStringLiteral("Not Connected");
        setDangeriousDocksDisabled(true);
        break;

    case AtCore::CONNECTING:
        stateString = QStringLiteral("Connecting");
        setDangeriousDocksDisabled(false);
        break;

    case AtCore::STOP:
        stateString = tr("Stoping Print");
        break;

    case AtCore::ERRORSTATE:
        stateString = tr("Command ERROR");
        break;
    }
    lblState->setText(stateString);
}

void MainWindow::populateCBs()
{
    // Extruders
    for (int count = 0; count < core->extruderCount(); count++) {
        comboExtruderSelect->insertItem(count, tr("Extruder %1").arg(count));
    }

    // Fan
    for (int count = 0; count < fanCount; count++) {
        comboFanSelect->insertItem(count, tr("Fan %1 speed").arg(count));
    }
}

void MainWindow::showMessage()
{
    core->showMessage(lineMessage->text());
}

void MainWindow::updatePrintTime()
{
    QTime temp(0, 0, 0);
    lblTime->setText(temp.addMSecs(printTime->elapsed()).toString(QStringLiteral("hh:mm:ss")));
}

void MainWindow::printProgressChanged(int progress)
{
    printingProgress->setValue(progress);
    if (progress > 0) {
        QTime temp(0, 0, 0);
        lblTimeLeft->setText(temp.addMSecs((100 - progress) * (printTime->elapsed() / progress)).toString(QStringLiteral("hh:mm:ss")));
    } else {
        lblTimeLeft->setText(QStringLiteral("??:??:??"));
    }
}
void MainWindow::toggleDockTitles(bool checked)
{
    if (checked) {
        delete connectDock->titleBarWidget();
        delete logDock->titleBarWidget();
        delete tempTimelineDock->titleBarWidget();
        delete commandDock->titleBarWidget();
        delete moveDock->titleBarWidget();
        delete tempControlsDock->titleBarWidget();
        delete printDock->titleBarWidget();
        delete sdDock->titleBarWidget();
    } else {
        connectDock->setTitleBarWidget(new QWidget());
        logDock->setTitleBarWidget(new QWidget());
        tempTimelineDock->setTitleBarWidget(new QWidget());
        commandDock->setTitleBarWidget(new QWidget());
        moveDock->setTitleBarWidget(new QWidget());
        tempControlsDock->setTitleBarWidget(new QWidget());
        printDock->setTitleBarWidget(new QWidget());
        sdDock->setTitleBarWidget(new QWidget());
    }
}

void MainWindow::setDangeriousDocksDisabled(bool disabled)
{
    commandDock->widget()->setDisabled(disabled);
    moveDock->widget()->setDisabled(disabled);
    tempControlsDock->widget()->setDisabled(disabled);
    printDock->widget()->setDisabled(disabled);
    sdDock->widget()->setDisabled(disabled);
}

void MainWindow::axisControlClicked(QLatin1Char axis, int value)
{
    core->setRelativePosition();
    core->move(axis, value);
    core->setAbsolutePosition();
}

void MainWindow::disableMotorsPBClicked()
{
    core->disableMotors(0);
}

void MainWindow::sdChanged(bool mounted)
{
    QString labelText = mounted ? tr("SD") : QString();
    lblSd->setText(labelText);
}

void MainWindow::getSdList()
{
    core->sdFileList();
}

void MainWindow::sdPrintPBClicked()
{
    if (listSdFiles->currentRow() < 0) {
        QMessageBox::information(this, tr("Print Error"), tr("You must Select a file from the list"));
    } else  {
        core->print(listSdFiles->currentItem()->text(), true);
    }
}

void MainWindow::sdDelPBClicked()
{
    if (listSdFiles->currentRow() < 0) {
        QMessageBox::information(this, tr("Delete Error"), tr("You must Select a file from the list"));
    } else  {
        core->sdDelete(listSdFiles->currentItem()->text());
        listSdFiles->setCurrentRow(-1);
    }
}
