/* AtCore
    Copyright (C) <2016 - 2019>

    Authors:
        Tomaz Canabrava <tcanabrava@kde.org>
        Chris Rizzitello <rizzitello@kde.org>
        Patrick José Pereira <patrickjp@kde.org>
        Lays Rodrigues <lays.rodrigues@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <QDir>
#include <QSerialPortInfo>
#include <QPluginLoader>
#include <QCoreApplication>
#include <QLoggingCategory>
#include <QTime>
#include <QTimer>
#include <QThread>
#include <QMetaEnum>
#include <QProcess>

#include "atcore.h"
#include "atcore_version.h"
#include "seriallayer.h"
#include "gcodecommands.h"
#include "printthread.h"
#include "atcore_default_folders.h"

Q_LOGGING_CATEGORY(ATCORE_PLUGIN, "org.kde.atelier.core.plugin")
Q_LOGGING_CATEGORY(ATCORE_CORE, "org.kde.atelier.core")
/**
 * @brief The AtCorePrivate struct
 * Provides a private data set for atcore.
 */
struct AtCore::AtCorePrivate {
    /** firmwarePlugin: pointer to firmware plugin */
    IFirmware *firmwarePlugin = nullptr;
    /** serial: pointer to the serial layer */
    SerialLayer *serial = nullptr;
    /** pluginLoader: QPluginLoader */
    QPluginLoader pluginLoader;
    /** plugins: Map of plugins name / path */
    QMap<QString, QString> plugins;
    /** lastMessage: lastMessage from the printer */
    QByteArray lastMessage;
    /** extruderCount: extruder count */
    int extruderCount = 1;
    /** temperature: Temperature object */
    Temperature temperature;
    /** commandQueue: the list of commands to send to the printer */
    QStringList commandQueue;
    /** ready: True if printer is ready for a command */
    bool ready = false;
    /** temperatureTimer: timer connected to the checkTemperature function */
    QTimer temperatureTimer;
    /** sdPrintProgressTimer: timer used to check in on sdJobs */
    QTimer sdPrintProgressTimer;
    /** percentage: print job percent */
    float percentage = 0.0;
    /** posString: stored string from last M114 return */
    QByteArray posString;
    /** printerState: State of the Printer */
    AtCore::STATES printerState = AtCore::DISCONNECTED;
    /** seralPorts: Detected serial Ports */
    QStringList serialPorts;
    /** serialTimer: Timer connected to locateSerialPorts */
    QTimer serialTimer;
    /** sdCardMounted: True if Sd Card is mounted. */
    bool sdCardMounted = false;
    /** sdCardReadingFileList: True while getting file names from sd card */
    bool sdCardReadingFileList = false;
    /** sdCardPrinting: True if currently printing from sd card. */
    bool sdCardPrinting = false;
    /** sdCardFileName: name of file being used from sd card. */
    QString sdCardFileName;
    /** sdCardFileList: List of files on sd card. */
    QStringList sdCardFileList;
};

AtCore::AtCore(QObject *parent) :
    QObject(parent),
    d(new AtCorePrivate)
{
    //Register MetaTypes
    qRegisterMetaType<AtCore::STATES>("AtCore::STATES");
    setState(AtCore::STATES::DISCONNECTED);
    //Connect our Timers
    connect(&d->sdPrintProgressTimer, &QTimer::timeout, this, &AtCore::sdCardPrintStatus);
    connect(&d->serialTimer, &QTimer::timeout, this, &AtCore::locateSerialPort);
    connect(&d->temperatureTimer, &QTimer::timeout, this, &AtCore::checkTemperature);
    //Attempt to find our plugins
    qCDebug(ATCORE_PLUGIN) << "Detecting Plugin path";
    QStringList paths = AtCoreDirectories::pluginDir;
    //add our current runtime path
    paths.prepend(qApp->applicationDirPath() + QStringLiteral("/../Plugins/AtCore"));
    paths.prepend(qApp->applicationDirPath() + QStringLiteral("/AtCore"));
    paths.prepend(qApp->applicationDirPath() + QStringLiteral("/plugins"));
    for (const auto &path : paths) {
        qCDebug(ATCORE_PLUGIN) << "Checking: " << path;
        QMap <QString, QString> tempMap = findFirmwarePlugins(path);
        if (!tempMap.isEmpty()) {
            d->plugins = tempMap;
            return;
        }
    }
    setState(AtCore::STATES::DISCONNECTED);
}

QString AtCore::version() const
{
    QString versionString = QString::fromLatin1(ATCORE_VERSION_STRING);
#if defined GIT_REVISION
    if (!QStringLiteral(GIT_REVISION).isEmpty()) {
        versionString.append(QString::fromLatin1("-%1").arg(QStringLiteral(GIT_REVISION)));
    }
#endif
    return versionString;
}

IFirmware *AtCore::firmwarePlugin() const
{
    return d->firmwarePlugin;
}

void AtCore::close()
{
    exit(0);
}

Temperature &AtCore::temperature() const
{
    return d->temperature;
}

void AtCore::findFirmware(const QByteArray &message)
{
    emit receivedMessage(message);
    emit atcoreMessage(tr("Waiting for firmware detect."));
    qCDebug(ATCORE_CORE) << "Find Firmware: " << message;
    if (!message.contains("FIRMWARE_NAME:")) {
        qCDebug(ATCORE_CORE) << "No firmware yet.";
        return;
    }

    qCDebug(ATCORE_CORE) << "Found firmware string, Looking for Firmware Name.";

    QString fwName = QString::fromLocal8Bit(message);
    fwName = fwName.split(QChar::fromLatin1(':')).at(1);
    if (fwName.indexOf(QChar::fromLatin1(' ')) == 0) {
        //remove leading space
        fwName.remove(0, 1);
    }
    if (fwName.contains(QChar::fromLatin1(' '))) {
        //check there is a space or dont' resize
        fwName.resize(fwName.indexOf(QChar::fromLatin1(' ')));
    }
    fwName = fwName.toLower().simplified();
    if (fwName.contains(QChar::fromLatin1('_'))) {
        fwName.resize(fwName.indexOf(QChar::fromLatin1('_')));
    }
    qCDebug(ATCORE_CORE) << "Firmware Name:" << fwName;

    if (message.contains("EXTRUDER_COUNT:")) {
        //this code is broken if more then 9 extruders are detected. since only one char is returned
        setExtruderCount(message.at(message.indexOf("EXTRUDER_COUNT:") + 15) - '0');
    }
    loadFirmwarePlugin(fwName);
}

void AtCore::loadFirmwarePlugin(const QString &fwName)
{
    qCDebug(ATCORE_CORE) << "Loading plugin: " << d->plugins[fwName];
    if (d->plugins.contains(fwName)) {
        d->pluginLoader.setFileName(d->plugins[fwName]);
        if (!d->pluginLoader.load()) {
            //Plugin was not loaded, Provide some debug info.
            qCDebug(ATCORE_CORE) << "Plugin Loading: Failed.";
            qCDebug(ATCORE_CORE) << d->pluginLoader.errorString();
            setState(AtCore::STATES::CONNECTING);
        } else {
            //Plugin was loaded successfully.
            d->firmwarePlugin = qobject_cast<IFirmware *>(d->pluginLoader.instance());
            firmwarePlugin()->init(this);
            disconnect(d->serial, &SerialLayer::receivedCommand, this, {});
            connect(d->serial, &SerialLayer::receivedCommand, this, &AtCore::newMessage);
            connect(firmwarePlugin(), &IFirmware::readyForCommand, this, &AtCore::processQueue);
            d->ready = true; // ready on new firmware load
            if (firmwarePlugin()->name() != QStringLiteral("Grbl")) {
                setTemperatureTimerInterval(5000);
            }
            setState(IDLE);
            emit atcoreMessage(tr("Connected to printer using %1 plugin").arg(d->firmwarePlugin->name()));
        }
    } else {
        qCDebug(ATCORE_CORE) << "Plugin:" << fwName << ": Not found.";
        emit atcoreMessage(tr("No plugin found for %1.").arg(fwName));
    }
}

void AtCore::waitForPrinterReboot(const QByteArray &message, const QString &fwName)
{
    emit receivedMessage(message);
    if (message.isEmpty()) {
        return;
    }

    if (message.contains("Grbl")) {
        loadFirmwarePlugin(QString::fromLatin1("grbl"));
    } else if (message.contains("Smoothie")) {
        loadFirmwarePlugin(QString::fromLatin1("smoothie"));
    } else if (message.contains("start")) {
        if (!d->plugins.contains(fwName)) {
            disconnect(d->serial, &SerialLayer::receivedCommand, this, {});
            connect(d->serial, &SerialLayer::receivedCommand, this, &AtCore::findFirmware);
            QTimer::singleShot(500, this, &AtCore::requestFirmware);
        } else {
            loadFirmwarePlugin(fwName);
        }
    }

}

bool AtCore::newConnection(const QString &port, int baud, const QString &fwName, bool disableROC)
{
    if (disableROC) {
        disableResetOnConnect(port);
    }

    d->serial = new SerialLayer(port, baud, this);
    connect(d->serial, &SerialLayer::serialError, this, &AtCore::handleSerialError);
    if (serialInitialized() && d->serial->isWritable()) {
        setState(AtCore::STATES::CONNECTING);
        connect(d->serial, &SerialLayer::pushedCommand, this, &AtCore::newCommand);

        if (!disableROC) {
            emit atcoreMessage(tr("Waiting for machine restart"));
            connect(d->serial, &SerialLayer::receivedCommand, this, [this, fwName](const QByteArray & message) {
                waitForPrinterReboot(message, fwName);
            });
        } else {
            loadFirmwarePlugin(fwName);
        }
        d->serialTimer.stop();
        return true;
    }
    qCDebug(ATCORE_CORE) << "Failed to open device for Read / Write.";
    emit atcoreMessage(tr("Failed to open device in read/write mode."));
    return false;
}

bool AtCore::serialInitialized() const
{
    if (!d->serial) {
        return false;
    }
    return d->serial->isOpen();
}

QString AtCore::connectedPort() const
{
    return d->serial->portName();
}

QStringList AtCore::serialPorts() const
{
    QStringList ports;
    QList<QSerialPortInfo> serialPortInfoList = QSerialPortInfo::availablePorts();
    if (!serialPortInfoList.isEmpty()) {
        for (const QSerialPortInfo &serialPortInfo : serialPortInfoList) {
#ifdef Q_OS_MAC
            //Mac OS has callout serial ports starting with cu these devices are read only.
            //It is necessary to filter them out to help prevent user error.
            if (!serialPortInfo.portName().startsWith(QStringLiteral("cu."), Qt::CaseInsensitive)) {
                ports.append(serialPortInfo.portName());
            }
#else
            ports.append(serialPortInfo.portName());
#endif
        }
    }
    return ports;
}

void AtCore::locateSerialPort()
{
    QStringList ports = serialPorts();
    if (d->serialPorts != ports) {
        d->serialPorts = ports;
        emit portsChanged(d->serialPorts);
    }
}

int AtCore::serialTimerInterval() const
{
    return d->serialTimer.interval();
}

void AtCore::setSerialTimerInterval(int newTime)
{
    newTime = std::max(newTime, 0);
    if (newTime != d->serialTimer.interval()) {
        d->serialTimer.setInterval(newTime);
        emit serialTimerIntervalChanged(newTime);
    }
    if (newTime == 0) {
        d->serialTimer.stop();
    } else {
        d->serialTimer.start(newTime);
    }
}

int AtCore::temperatureTimerInterval() const
{
    return d->temperatureTimer.interval();
}

void AtCore::setTemperatureTimerInterval(int newTime)
{
    newTime = std::max(newTime, 0);
    if (newTime != d->temperatureTimer.interval()) {
        d->temperatureTimer.setInterval(newTime);
        emit temperatureTimerIntervalChanged(newTime);
    }
    if (newTime == 0) {
        d->temperatureTimer.stop();
    } else {
        d->temperatureTimer.start(newTime);
    }
}

void AtCore::newMessage(const QByteArray &message)
{
    //Evaluate the messages coming from the printer.
    d->lastMessage = message;
    //Check if the message has current coordinates.
    if (message.startsWith(QString::fromLatin1("X:").toLocal8Bit())) {
        d->posString = message;
        d->posString.resize(d->posString.indexOf('E'));
        d->posString.replace(':', "");
    }

    //Check if have temperature info and decode it
    if (d->lastMessage.contains("T:") || d->lastMessage.contains("B:")) {
        temperature().decodeTemp(message);
    }
    emit receivedMessage(d->lastMessage);
}

void AtCore::newCommand(const QByteArray &command)
{
    emit pushedCommand(command);
}

void AtCore::setRelativePosition()
{
    pushCommand(GCode::toCommand(GCode::GCommands::G91));
}

void AtCore::setAbsolutePosition()
{
    pushCommand(GCode::toCommand(GCode::GCommands::G90));
}

float AtCore::percentagePrinted() const
{
    return d->percentage;
}

void AtCore::print(const QString &fileName, bool sdPrint)
{
    if (state() == AtCore::STATES::CONNECTING) {
        qCDebug(ATCORE_CORE) << "Load a firmware plugin to print.";
        return;
    }
    //Start a print job.
    setState(AtCore::STATES::STARTPRINT);
    //Only try to print from Sd if the firmware has support for sd cards
    if (firmwarePlugin()->isSdSupported()) {
        if (sdPrint) {
            //Printing from the sd card requires us to send some M commands.
            pushCommand(GCode::toCommand(GCode::MCommands::M23, fileName));
            d->sdCardFileName = fileName;
            pushCommand(GCode::toCommand(GCode::MCommands::M24));
            setState(AtCore::STATES::BUSY);
            d->sdCardPrinting = true;
            d->sdPrintProgressTimer.start(5000);
            return;
        }
    }
    //Process the gcode with a printThread.
    //The Thread processes the gcode without freezing the libary.
    //Only sends a command back when the printer is ready, avoiding buffer overflow in the printer.
    auto thread = new QThread(this);
    auto printThread = new PrintThread(this, fileName);
    printThread->moveToThread(thread);

    connect(printThread, &PrintThread::printProgressChanged, this, &AtCore::printProgressChanged, Qt::QueuedConnection);
    connect(thread, &QThread::started, printThread, &PrintThread::start);
    connect(printThread, &PrintThread::finished, thread, &QThread::quit);
    connect(thread, &QThread::finished, printThread, &PrintThread::deleteLater);
    if (!thread->isRunning()) {
        thread->start();
    }
}

void AtCore::pushCommand(const QString &comm)
{
//Be sure our M112 is first in the queue.
    if (comm == GCode::toCommand(GCode::MCommands::M112)) {
        d->commandQueue.prepend(comm);
    } else {
        d->commandQueue.append(comm);
    }
    if (d->ready) {
        //The printer is ready for a command now so push one.
        processQueue();
    }
}

void AtCore::closeConnection()
{
    if (serialInitialized()) {
        if (AtCore::state() == AtCore::STATES::BUSY && !d->sdCardPrinting) {
            //We have to clean up the print job if printing from the host.
            //However disconnecting while printing from sd card should not affect the print job.
            setState(AtCore::STATES::STOP);
        }
        if (firmwarePluginLoaded()) {
            disconnect(firmwarePlugin(), &IFirmware::readyForCommand, this, &AtCore::processQueue);
            disconnect(d->serial, &SerialLayer::receivedCommand, this, &AtCore::newMessage);
            if (firmwarePlugin()->name() != QStringLiteral("Grbl")) {
                setTemperatureTimerInterval(0);
            }
            //Attempt to unload the firmware plugin.
            QString name = firmwarePlugin()->name();
            QString msg = d->pluginLoader.unload() ? QStringLiteral("closed.") : QStringLiteral("Failed to close.");
            qCDebug(ATCORE_CORE) << QStringLiteral("Firmware plugin %1 %2").arg(name, msg);
            d->firmwarePlugin = nullptr;
        }
        //Do not reset the connect on disconnect when closing this will cause a reset on connect for the next connection.
        disconnect(d->serial, &SerialLayer::serialError, this, &AtCore::handleSerialError);
        disconnect(d->serial, &SerialLayer::pushedCommand, this, &AtCore::newMessage);
        d->serial->close();
        //Clear our copy of the sdcard filelist
        clearSdCardFileList();
        setState(AtCore::STATES::DISCONNECTED);
        d->serialTimer.start();
    }
}

AtCore::STATES AtCore::state()
{
    return d->printerState;
}

void AtCore::setState(AtCore::STATES state)
{
    if (state != d->printerState) {
        qCDebug(ATCORE_CORE) << QStringLiteral("Atcore state changed from [%1] to [%2]")
                             .arg(QVariant::fromValue(d->printerState).toString(),
                                  QVariant::fromValue(state).toString());
        d->printerState = state;
        if (state == AtCore::STATES::FINISHEDPRINT && d->sdCardPrinting) {
            //Clean up the sd card print
            d->sdCardPrinting = false;
            d->sdPrintProgressTimer.stop();
        }
        emit stateChanged(d->printerState);
    }
}

void AtCore::stop()
{
    //Stop a print job
    setState(AtCore::STATES::STOP);
    d->commandQueue.clear();
    if (d->sdCardPrinting) {
        stopSdPrint();
    }
    setExtruderTemp(0, 0);
    setBedTemp(0);
    home(AtCore::AXES::X);
}

void AtCore::emergencyStop()
{
    //Emergency Stop. Stops the machine
    //Clear the queue, and any print job
    //Before sending the command to ensure
    //Less chance of movement after the restart.
    d->commandQueue.clear();
    if (AtCore::state() == AtCore::STATES::BUSY) {
        if (!d->sdCardPrinting) {
            //Stop our running print thread
            setState(AtCore::STATES::STOP);
        }
    }
    pushCommand(GCode::toCommand(GCode::MCommands::M112));
}

void AtCore::stopSdPrint()
{
    //Stop an SdCard Print.
    pushCommand(GCode::toCommand(GCode::MCommands::M25));
    d->sdCardFileName = QString();
    pushCommand(GCode::toCommand(GCode::MCommands::M23, d->sdCardFileName));
    AtCore::setState(AtCore::STATES::FINISHEDPRINT);
    AtCore::setState(AtCore::STATES::IDLE);
}

void AtCore::requestFirmware()
{
    if (serialInitialized()) {
        //ensure M115 is sent on cold connect.
        d->ready = true;
        qCDebug(ATCORE_CORE) << "Sending " << GCode::description(GCode::MCommands::M115);
        pushCommand(GCode::toCommand(GCode::MCommands::M115));
    } else {
        qCDebug(ATCORE_CORE) << "There is no open device to send commands";
    }
}

bool AtCore::firmwarePluginLoaded() const
{
    return firmwarePlugin();
}

QMap<QString, QString> AtCore::findFirmwarePlugins(const QString &path)
{
    QMap<QString, QString> detectedPlugins;
    QStringList files = QDir(path).entryList(QDir::Files);
    for (const QString &f : files) {
        QString file = f;
#if defined(Q_OS_WIN)
        if (file.endsWith(QStringLiteral(".dll")))
#elif defined(Q_OS_MAC)
        if (file.endsWith(QStringLiteral(".dylib")))
#else
        if (file.endsWith(QStringLiteral(".so")))
#endif
            file = file.split(QChar::fromLatin1('.')).at(0);
        else {
            continue;
        }
        if (file.startsWith(QStringLiteral("lib"))) {
            file = file.remove(QStringLiteral("lib"));
        }
        file = file.toLower().simplified();
        QString pluginString = path;
        pluginString.append(QChar::fromLatin1('/'));
        pluginString.append(f);
        detectedPlugins[file] = pluginString;
        qCDebug(ATCORE_PLUGIN) << QStringLiteral("Plugin:[%1]=%2").arg(file, pluginString);
    }
    return detectedPlugins;
}

QStringList AtCore::availableFirmwarePlugins() const
{
    return d->plugins.keys();
}

void AtCore::pause(const QString &pauseActions)
{
    if (d->sdCardPrinting) {
        pushCommand(GCode::toCommand(GCode::MCommands::M25));
    }
    //Push the command to request current coordinates.
    //This will be read by AtCore::newMessage and stored for use on resume.
    pushCommand(GCode::toCommand(GCode::MCommands::M114));
    if (!pauseActions.isEmpty()) {
        QStringList temp = pauseActions.split(QChar::fromLatin1(','));
        for (int i = 0; i < temp.length(); i++) {
            pushCommand(temp.at(i));
        }
    }
    setState(AtCore::PAUSE);
}

void AtCore::resume()
{
    if (d->sdCardPrinting) {
        pushCommand(GCode::toCommand(GCode::MCommands::M24));
    } else {
        //Move back to previous coordinates.
        pushCommand(GCode::toCommand(GCode::GCommands::G0, QString::fromLatin1(d->posString)));
    }
    setState(AtCore::BUSY);
}

/*~~~~~Control Slots ~~~~~~~~*/

void AtCore::home()
{
    pushCommand(GCode::toCommand(GCode::GCommands::G28));
}

void AtCore::home(uchar axis)
{
    QString args;

    if (axis & AtCore::AXES::X) {
        args.append(QStringLiteral("X0 "));
    }

    if (axis & AtCore::AXES::Y) {
        args.append(QStringLiteral("Y0 "));
    }

    if (axis & AtCore::AXES::Z) {
        args.append(QStringLiteral("Z0"));
    }
    pushCommand(GCode::toCommand(GCode::GCommands::G28, args));
}

void AtCore::setExtruderTemp(uint temp, uint extruder, bool andWait)
{
    temp = std::min<uint>(temp, 10000);
    extruder = std::min<uint>(extruder, 10000);

    if (andWait) {
        pushCommand(GCode::toCommand(GCode::MCommands::M109, QString::number(temp), QString::number(extruder)));
    } else {
        pushCommand(GCode::toCommand(GCode::MCommands::M104, QString::number(extruder), QString::number(temp)));
    }
}

void AtCore::setBedTemp(uint temp, bool andWait)
{
    temp = std::min<uint>(temp, 10000);

    if (andWait) {
        pushCommand(GCode::toCommand(GCode::MCommands::M190, QString::number(temp)));
    } else {
        pushCommand(GCode::toCommand(GCode::MCommands::M140, QString::number(temp)));
    }
}

void AtCore::setFanSpeed(uint speed, uint fanNumber)
{
    speed = std::min<uint>(speed, 10000);
    fanNumber = std::min<uint>(fanNumber, 10000);

    pushCommand(GCode::toCommand(GCode::MCommands::M106, QString::number(fanNumber), QString::number(speed)));
}

void AtCore::setPrinterSpeed(uint speed)
{
    speed = std::min<uint>(speed, 10000);
    pushCommand(GCode::toCommand(GCode::MCommands::M220, QString::number(speed)));
}

void AtCore::setFlowRate(uint speed)
{
    speed = std::min<uint>(speed, 10000);
    pushCommand(GCode::toCommand(GCode::MCommands::M221, QString::number(speed)));
}

void AtCore::move(AtCore::AXES axis, double arg)
{
    const auto axisAsString = QMetaEnum::fromType<AtCore::AXES>().valueToKey(axis);
    move(QLatin1Char(axisAsString[0]), arg);
}

void AtCore::move(QLatin1Char axis, double arg)
{
    //Using QString::number(double, format, precision)
    //f = 'format as [-]9.9'
    //3 = use 3 decimal precision
    pushCommand(GCode::toCommand(GCode::GCommands::G1, QStringLiteral("%1 %2").arg(axis).arg(QString::number(arg, 'f', 3))));
}

int AtCore::extruderCount() const
{
    return d->extruderCount;
}

void AtCore::setExtruderCount(int newCount)
{
    if (d->extruderCount != newCount && newCount >= 1) {
        d->extruderCount = newCount;
        emit extruderCountChanged(newCount);
        qCDebug(ATCORE_CORE) << "Extruder Count:" << QString::number(extruderCount());
    }
}
void AtCore::processQueue()
{
    d->ready = true;

    if (d->commandQueue.isEmpty()) {
        return;
    }

    if (!serialInitialized()) {
        qCDebug(ATCORE_PLUGIN) << "Can't process queue ! Serial not initialized.";
        return;
    }

    QString text = d->commandQueue.takeAt(0);

    if (firmwarePluginLoaded()) {
        d->serial->pushCommand(firmwarePlugin()->translate(text));
    } else {
        d->serial->pushCommand(text.toLocal8Bit());
    }
    d->ready = false;
}

void AtCore::checkTemperature()
{
    //One request for the temperature in the queue at a time.
    if (d->commandQueue.contains(GCode::toCommand(GCode::MCommands::M105))) {
        return;
    }
    pushCommand(GCode::toCommand(GCode::MCommands::M105));
}

void AtCore::showMessage(const QString &message)
{
    if (!message.isEmpty()) {
        pushCommand(GCode::toCommand((GCode::MCommands::M117), message));
    }
}

void AtCore::setUnits(AtCore::UNITS units)
{
    switch (units) {
    case AtCore::UNITS::METRIC:
        pushCommand(GCode::toCommand(GCode::GCommands::G21));
        break;
    case AtCore::UNITS::IMPERIAL:
        pushCommand(GCode::toCommand(GCode::GCommands::G20));
        break;
    }
}

QStringList AtCore::portSpeeds() const
{
    return d->serial->validBaudRates();
}

void AtCore::disableMotors(uint delay)
{
    //Disables motors
    if (delay) {
        pushCommand(GCode::toCommand(GCode::MCommands::M84, QString::number(delay)));
    } else {
        pushCommand(GCode::toCommand(GCode::MCommands::M84));
    }
}
//Most firmwares will not report if an sdcard is mounted on boot.
bool AtCore::isSdMounted() const
{
    return d->sdCardMounted;
}

void AtCore::setSdMounted(bool mounted)
{
    if (mounted != isSdMounted()) {
        d->sdCardMounted = mounted;
        emit sdMountChanged(d->sdCardMounted);
    }
}

void AtCore::getSDFileList()
{
    pushCommand(GCode::toCommand(GCode::MCommands::M20));
}

QStringList AtCore::sdFileList()
{
    if (!d->sdCardReadingFileList) {
        getSDFileList();
    }
    return d->sdCardFileList;
}

void AtCore::appendSdCardFileList(const QString &fileName)
{
    d->sdCardFileList.append(fileName);
    emit sdCardFileListChanged(d->sdCardFileList);
}

void AtCore::clearSdCardFileList()
{
    d->sdCardFileList.clear();
    emit sdCardFileListChanged(d->sdCardFileList);
}

void AtCore::sdDelete(const QString &fileName)
{
    if (d->sdCardFileList.contains(fileName)) {
        pushCommand(GCode::toCommand(GCode::MCommands::M30, fileName));
        getSDFileList();
    } else {
        qCDebug(ATCORE_CORE) << "Delete failed file not found:" << fileName;
    }
}

void AtCore::mountSd(uint slot)
{
    pushCommand(GCode::toCommand(GCode::MCommands::M21, QString::number(slot)));
}

void AtCore::umountSd(uint slot)
{
    pushCommand(GCode::toCommand(GCode::MCommands::M22, QString::number(slot)));
}

bool AtCore::isReadingSdCardList() const
{
    return d->sdCardReadingFileList;
}

void AtCore::setReadingSdCardList(bool readingList)
{
    d->sdCardReadingFileList = readingList;
}

void AtCore::sdCardPrintStatus()
{
    //One request for the Sd Job status in the queue at a time.
    if (d->commandQueue.contains(GCode::toCommand(GCode::MCommands::M27))) {
        return;
    }
    pushCommand(GCode::toCommand(GCode::MCommands::M27));
}

void AtCore::disableResetOnConnect(const QString &port)
{
#if defined(Q_OS_UNIX)
//should work on all unix'
    QProcess process(this);
    QStringList args({QStringLiteral("-F/dev/%1").arg(port), QStringLiteral("-hupcl")});
    process.start(QStringLiteral("stty"), args);
    process.waitForFinished(500);

    connect(&process, &QProcess::errorOccurred, this, [&process] {
        qCDebug(ATCORE_CORE) << "Stty Error:" << process.errorString();
    });

#elif defined(Q_OS_WIN)
    //TODO: Disable hangup on windows.
#endif
}

void AtCore::handleSerialError(QSerialPort::SerialPortError error)
{
    QString errorString;

    switch (error) {
    case (QSerialPort::SerialPortError::DeviceNotFoundError):
        errorString = tr("Device not found");
        break;
    case (QSerialPort::SerialPortError::WriteError):
        errorString = tr("Unable to write to device");
        break;
    case (QSerialPort::SerialPortError::ReadError):
        errorString = tr("Unable to read from device");
        break;
    case (QSerialPort::SerialPortError::ResourceError):
    case (QSerialPort::SerialPortError::TimeoutError):
        errorString = tr("The device no longer available");
        closeConnection();
        break;
    case (QSerialPort::SerialPortError::UnsupportedOperationError):
        errorString = tr("Device does not support the operation");
        break;
    case (QSerialPort::SerialPortError::UnknownError):
        errorString = tr("Unknown Error");
        break;
    default:
        //Not Directly processed errors
        //QSerialPort::NoError, No error has happened
        //QSerialPort::PermissionError), Already handled.
        //QSerialPort::OpenError), Already handled.
        //QSerialPort::NotOpenError, SerialLayer destroyed if not connected.
        //QSerialPort::ParityError, Obsolete. Qt Docs "We strongly advise against using it in new code."
        //QSerialPort::FramingError, Obsolete. Qt Docs "We strongly advise against using it in new code."
        //QSerialPort::BreakConditionError, Obsolete. Qt Docs "We strongly advise against using it in new code."
        return;
    };//End of Switch
    qCDebug(ATCORE_CORE) << "SerialError:" << errorString;
    emit atcoreMessage(QStringLiteral("SerialError: %1").arg(errorString));
}
