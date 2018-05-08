/* AtCore Test Client
    Copyright (C) <2016>

    Authors:
        Patrick José Pereira <patrickjp@kde.org>
        Lays Rodrigues <lays.rodrigues@kde.org>
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
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QLoggingCategory>

#include "mainwindow.h"
#include "seriallayer.h"
#include "gcodecommands.h"
#include "about.h"

Q_LOGGING_CATEGORY(TESTCLIENT_MAINWINDOW, "org.kde.atelier.core")

int MainWindow::fanCount = 4;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    core(new AtCore(this))
{
    setWindowTitle(tr("AtCore - Test Client"));
    setWindowIcon(QIcon(QStringLiteral(":/icon/windowIcon")));
    QCoreApplication::setApplicationVersion(core->version());
    initMenu();
    initStatusBar();
    initWidgets();

    logWidget->appendLog(tr("Attempting to locate Serial Ports"));
    core->setSerialTimerInterval(1000);

    connect(core, &AtCore::stateChanged, this, &MainWindow::printerStateChanged);
    connect(core, &AtCore::portsChanged, this, &MainWindow::locateSerialPort);

    connect(core, &AtCore::sdCardFileListChanged, sdWidget, &SdWidget::updateFilelist);

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
    statusWidget = new StatusWidget;
    connect(statusWidget, &StatusWidget::stopPressed, core, &AtCore::stop);
    connect(core, &AtCore::printProgressChanged, statusWidget, &StatusWidget::updatePrintProgress);
    connect(core, &AtCore::sdMountChanged, statusWidget, &StatusWidget::setSD);
    statusBar()->addPermanentWidget(statusWidget, 100);
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
    //hide the printing progress bar.
    statusWidget->showPrintArea(false);
}

void MainWindow::makeCommandDock()
{
    commandWidget = new CommandWidget;
    //Connect the commandPressed signal
    connect(commandWidget, &CommandWidget::commandPressed, [this](const QString & command) {
        core->pushCommand(command.toUpper());
    });
    //Connect the messagePressed signal
    connect(commandWidget, &CommandWidget::messagePressed, [this](const QString & message) {
        core->showMessage(message);
    });
    //Create the dock, and set the Widget.
    commandDock = new QDockWidget(tr("Commands"), this);
    commandDock->setWidget(commandWidget);

    //Push the toggle view action into our view menu
    menuView->insertAction(nullptr, commandDock->toggleViewAction());

    //Place the Dock into a DockWidget Area.
    //Failure todo this will create some odd side effects at runtime
    addDockWidget(Qt::LeftDockWidgetArea, commandDock);
}

void MainWindow::makePrintDock()
{
    printWidget = new PrintWidget;
    connect(printWidget, &PrintWidget::printPressed, this, &MainWindow::printPBClicked);
    connect(printWidget, &PrintWidget::emergencyStopPressed, core, &AtCore::emergencyStop);
    connect(printWidget, &PrintWidget::fanSpeedChanged, core, &AtCore::setFanSpeed);

    connect(printWidget, &PrintWidget::printSpeedChanged, [this](const int speed) {
        core->setPrinterSpeed(speed);
    });

    connect(printWidget, &PrintWidget::flowRateChanged, [this](const int rate) {
        core->setFlowRate(rate);
    });

    printDock = new QDockWidget(tr("Print"), this);
    printDock->setWidget(printWidget);

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
    logWidget = new LogWidget(new QTemporaryFile(QDir::tempPath() + QStringLiteral("/AtCore_")));
    logDock = new QDockWidget(tr("Session Log"), this);
    logDock->setWidget(logWidget);

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
    movementWidget = new MovementWidget;

    connect(movementWidget, &MovementWidget::homeAllPressed, [this] {
        logWidget->appendLog(tr("Home All"));
        core->home();
    });

    connect(movementWidget, &MovementWidget::homeXPressed, [this] {
        logWidget->appendLog(tr("Home X"));
        core->home(AtCore::X);
    });

    connect(movementWidget, &MovementWidget::homeYPressed, [this] {
        logWidget->appendLog(tr("Home Y"));
        core->home(AtCore::Y);
    });

    connect(movementWidget, &MovementWidget::homeZPressed, [this] {
        logWidget->appendLog(tr("Home Z"));
        core->home(AtCore::Z);
    });

    connect(movementWidget, &MovementWidget::absoluteMove, [this](const QLatin1Char & axis, const double & value) {
        logWidget->appendLog(GCode::description(GCode::G1));
        core->move(axis, value);
    });

    connect(movementWidget, &MovementWidget::disableMotorsPressed, [this] {
        core->disableMotors(0);
    });

    connect(movementWidget, &MovementWidget::relativeMove, [this](const QLatin1Char & axis, const double & value) {
        core->setRelativePosition();
        core->move(axis, value);
        core->setAbsolutePosition();
    });

    moveDock = new QDockWidget(tr("Movement"), this);
    moveDock->setWidget(movementWidget);

    menuView->insertAction(nullptr, moveDock->toggleViewAction());
    addDockWidget(Qt::LeftDockWidgetArea, moveDock);
}

void MainWindow::makeTempControlsDock()
{
    temperatureWidget = new TemperatureWidget;
    connect(temperatureWidget, &TemperatureWidget::bedTempChanged, core, &AtCore::setBedTemp);
    connect(temperatureWidget, &TemperatureWidget::extTempChanged, core, &AtCore::setExtruderTemp);

    tempControlsDock = new QDockWidget(tr("Temperatures"), this);
    tempControlsDock->setWidget(temperatureWidget);
    menuView->insertAction(nullptr, tempControlsDock->toggleViewAction());
    addDockWidget(Qt::LeftDockWidgetArea, tempControlsDock);
}

void MainWindow::makeSdDock()
{

    sdWidget = new SdWidget;
    connect(sdWidget, &SdWidget::requestSdList, core, &AtCore::sdFileList);

    connect(sdWidget, &SdWidget::printSdFile, [this](const QString & fileName) {
        if (fileName.isEmpty()) {
            QMessageBox::information(this, tr("Print Error"), tr("You must Select a file from the list"));
        } else  {
            core->print(fileName, true);
        }
    });

    connect(sdWidget, &SdWidget::deleteSdFile, [this](const QString & fileName) {
        if (fileName.isEmpty()) {
            QMessageBox::information(this, tr("Delete Error"), tr("You must Select a file from the list"));
        } else  {
            core->sdDelete(fileName);
        }
    });

    sdDock = new QDockWidget(tr("Sd Card"), this);
    sdDock->setWidget(sdWidget);
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

    logWidget->appendLog(msg);
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
        logWidget->appendLog(tr("Found %1 Ports").arg(QString::number(ports.count())));
    } else {
        QString portError(tr("No available ports! Please connect a serial device to continue!"));
        if (! logWidget->endsWith(portError)) {
            logWidget->appendLog(portError);
        }
    }
}

void MainWindow::connectPBClicked()
{
    if (core->state() == AtCore::DISCONNECTED) {
        if (core->initSerial(comboPort->currentText(), comboBAUD->currentText().toInt())) {
            connect(core, &AtCore::receivedMessage, logWidget, &LogWidget::appendRLog);
            connect(core->serial(), &SerialLayer::pushedCommand, logWidget, &LogWidget::appendSLog);
            buttonConnect->setText(tr("Disconnect"));
            logWidget->appendLog(tr("Serial connected"));
            if (comboPlugin->currentText().contains(tr("Autodetect"))) {
                logWidget->appendLog(tr("No plugin loaded !"));
                logWidget->appendLog(tr("Requesting Firmware..."));
            } else {
                core->loadFirmwarePlugin(comboPlugin->currentText());
            }
        } else {
            logWidget->appendLog(tr("Failed to open serial in r/w mode"));
        }
    } else {
        disconnect(core, &AtCore::receivedMessage, logWidget, &LogWidget::appendRLog);
        disconnect(core->serial(), &SerialLayer::pushedCommand, logWidget, &LogWidget::appendSLog);
        core->closeConnection();
        core->setState(AtCore::DISCONNECTED);
        logWidget->appendLog(tr("Disconnected"));
        buttonConnect->setText(tr("Connect"));
    }
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
            logWidget->appendLog(tr("No File Selected"));
        } else {
            logWidget->appendLog(tr("Print: %1").arg(fileName));
            core->print(fileName);
        }
        break;

    case AtCore::BUSY:
        core->pause(printWidget->postPauseCommand());
        break;

    case AtCore::PAUSE:
        core->resume();
        break;

    default:
        qCDebug(TESTCLIENT_MAINWINDOW) << "ERROR / STOP unhandled.";
    }
}

void MainWindow::pluginCBChanged(QString currentText)
{
    if (core->state() != AtCore::DISCONNECTED) {
        if (!currentText.contains(tr("Autodetect"))) {
            core->loadFirmwarePlugin(currentText);
        }
    }
}

void MainWindow::printerStateChanged(AtCore::STATES state)
{
    QString stateString;
    switch (state) {
    case AtCore::IDLE:
        printWidget->setPrintText(tr("Print File"));
        stateString = tr("Connected to ") + core->connectedPort();
        break;

    case AtCore::STARTPRINT:
        stateString = tr("START PRINT");
        printWidget->setPrintText(tr("Pause Print"));
        statusWidget->showPrintArea(true);
        break;

    case AtCore::FINISHEDPRINT:
        stateString = tr("Finished Print");
        printWidget->setPrintText(tr("Print File"));
        statusWidget->showPrintArea(false);
        break;

    case AtCore::PAUSE:
        stateString = tr("Paused");
        printWidget->setPrintText(tr("Resume Print"));
        break;

    case AtCore::BUSY:
        stateString = tr("Printing");
        printWidget->setPrintText(tr("Pause Print"));
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
        stateString = tr("Stopping Print");
        break;

    case AtCore::ERRORSTATE:
        stateString = tr("Command ERROR");
        break;
    }
    statusWidget->setState(stateString);
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

    if (!disabled) {
        temperatureWidget->updateExtruderCount(core->extruderCount());
        printWidget->updateFanCount(fanCount);
    }
}
