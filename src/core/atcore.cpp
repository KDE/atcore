/* AtCore
    Copyright (C) <2016 - 2018>

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
    IFirmware *firmwarePlugin = nullptr;//!< @param firmwarePlugin: pointer to firmware plugin
    SerialLayer *serial = nullptr;      //!< @param serial: pointer to the serial layer
    QPluginLoader pluginLoader;         //!< @param pluginLoader: QPluginLoader
    QMap<QString, QString> plugins;     //!< @param plugins: Map of plugins name / path
    QByteArray lastMessage;             //!< @param lastMessage: lastMessage from the printer
    int extruderCount = 1;              //!< @param extruderCount: extruder count
    Temperature temperature;            //!< @param temperature: Temperature object
    QStringList commandQueue;           //!< @param commandQueue: the list of commands to send to the printer
    bool ready = false;                 //!< @param ready: True if printer is ready for a command
    QTimer *tempTimer = nullptr;        //!< @param tempTimer: timer connected to the checkTemperature function
    float percentage;                   //!< @param percentage: print job percent
    QByteArray posString;               //!< @param posString: stored string from last M114 return
    AtCore::STATES printerState;        //!< @param printerState: State of the Printer
    QStringList serialPorts;            //!< @param seralPorts: Detected serial Ports
    QTimer *serialTimer = nullptr;      //!< @param serialTimer: Timer connected to locateSerialPorts
    bool sdCardMounted = false;         //!< @param sdCardMounted: True if Sd Card is mounted.
    bool sdCardReadingFileList = false; //!< @param sdCardReadingFileList: True while getting file names from sd card
    bool sdCardPrinting = false;        //!< @param sdCardPrinting: True if currently printing from sd card.
    QString sdCardFileName;             //!< @param sdCardFileName: name of file being used from sd card.
    QStringList sdCardFileList;         //!< @param sdCardFileList: List of files on sd card.
};

AtCore::AtCore(QObject *parent) :
    QObject(parent),
    d(new AtCorePrivate)
{
    //Register MetaTypes
    qRegisterMetaType<AtCore::STATES>("AtCore::STATES");
    setState(AtCore::DISCONNECTED);

    //Create and start the timer that checks for temperature.
    d->tempTimer = new QTimer(this);
    d->tempTimer->setInterval(5000);
    d->tempTimer->setSingleShot(false);
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
    setState(AtCore::DISCONNECTED);
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

SerialLayer *AtCore::serial() const
{
    return d->serial;
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
    if (state() == AtCore::DISCONNECTED) {
        qCWarning(ATCORE_CORE) << tr("Cant find firwmware, serial not connected!");
        return;
    }

    if (state() == AtCore::CONNECTING) {
        //Most Firmwares will return "start" on connect, some return their firmware name.
        if (message.contains("start")) {
            qCDebug(ATCORE_CORE) << "Waiting requestFirmware.";
            QTimer::singleShot(500, this, &AtCore::requestFirmware);
            return;
        } else if (message.contains("Grbl")) {
            loadFirmwarePlugin(QString::fromLatin1("grbl"));
            return;
        } else if (message.contains("Smoothie")) {
            loadFirmwarePlugin(QString::fromLatin1("smoothie"));
            return;
        }

        qCDebug(ATCORE_CORE) << "Waiting for firmware detect.";
        emit atcoreMessage(tr("Waiting for firmware detect."));
    }
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
            setState(AtCore::CONNECTING);
        } else {
            //Plugin was loaded successfully.
            d->firmwarePlugin = qobject_cast<IFirmware *>(d->pluginLoader.instance());
            firmwarePlugin()->init(this);
            disconnect(serial(), &SerialLayer::receivedCommand, this, &AtCore::findFirmware);
            connect(serial(), &SerialLayer::receivedCommand, this, &AtCore::newMessage);
            connect(firmwarePlugin(), &IFirmware::readyForCommand, this, &AtCore::processQueue);
            d->ready = true; // ready on new firmware load
            if (firmwarePlugin()->name() != QStringLiteral("Grbl")) {
                connect(d->tempTimer, &QTimer::timeout, this, &AtCore::checkTemperature);
                d->tempTimer->start();
            }
            setState(IDLE);
        }
    } else {
        qCDebug(ATCORE_CORE) << "Plugin:" << fwName << ": Not found.";
        emit atcoreMessage(tr("No plugin found for %1.").arg(fwName));
    }
}

bool AtCore::initSerial(const QString &port, int baud, bool disableROC)
{
    if (disableROC) {
        disableResetOnConnect(port);
    }

    d->serial = new SerialLayer(port, baud);
    connect(serial(), &SerialLayer::serialError, this, &AtCore::handleSerialError);
    if (serialInitialized() && d->serial->isWritable()) {
        setState(AtCore::CONNECTING);
        connect(serial(), &SerialLayer::receivedCommand, this, &AtCore::findFirmware);
        d->serialTimer->stop();
        return true;
    } else {
        qCDebug(ATCORE_CORE) << "Failed to open device for Read / Write.";
        emit atcoreMessage(tr("Failed to open device in read/write mode."));
        return false;
    }
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
    return serial()->portName();
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

quint16 AtCore::serialTimerInterval() const
{
    if (d->serialTimer != nullptr) {
        return d->serialTimer->interval();
    }
    return 0;
}

void AtCore::setSerialTimerInterval(const quint16 &newTime)
{
    if (!d->serialTimer) {
        //There is no timer. We need to create one.
        d->serialTimer = new QTimer();
        connect(d->serialTimer, &QTimer::timeout, this, &AtCore::locateSerialPort);
    }
    //emit the newtime if it has changed.
    if (newTime != d->serialTimer->interval()) {
        emit serialTimerIntervalChanged(newTime);
    }
    //Start the timer.
    d->serialTimer->start(newTime);
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

void AtCore::setRelativePosition()
{
    pushCommand(GCode::toCommand(GCode::G91));
}

void AtCore::setAbsolutePosition()
{
    pushCommand(GCode::toCommand(GCode::G90));
}

float AtCore::percentagePrinted() const
{
    return d->percentage;
}

void AtCore::print(const QString &fileName, bool sdPrint)
{
    if (state() == AtCore::CONNECTING) {
        qCDebug(ATCORE_CORE) << "Load a firmware plugin to print.";
        return;
    }
    //Start a print job.
    setState(AtCore::STARTPRINT);
    //Only try to print from Sd if the firmware has support for sd cards
    if (firmwarePlugin()->isSdSupported()) {
        if (sdPrint) {
            //Printing from the sd card requires us to send some M commands.
            pushCommand(GCode::toCommand(GCode::M23, fileName));
            d->sdCardFileName = fileName;
            pushCommand(GCode::toCommand(GCode::M24));
            setState(AtCore::BUSY);
            d->sdCardPrinting = true;
            connect(d->tempTimer, &QTimer::timeout, this, &AtCore::sdCardPrintStatus);
            return;
        }
    }
    //Process the gcode with a printThread.
    //The Thread processes the gcode without freezing the libary.
    //Only sends a command back when the printer is ready, avoiding buffer overflow in the printer.
    QThread *thread = new QThread();
    PrintThread *printThread = new PrintThread(this, fileName);
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
    //Append command to the commandQueue
    d->commandQueue.append(comm);
    if (d->ready) {
        //The printer is ready for a command now so push one.
        processQueue();
    }
}

void AtCore::closeConnection()
{
    if (serialInitialized()) {
        if (AtCore::state() == AtCore::BUSY && !d->sdCardPrinting) {
            //We have to clean up the print job if printing from the host.
            //However disconnecting while printing from sd card should not affect the print job.
            setState(AtCore::STOP);
        }
        if (firmwarePluginLoaded()) {
            disconnect(firmwarePlugin(), &IFirmware::readyForCommand, this, &AtCore::processQueue);
            disconnect(serial(), &SerialLayer::receivedCommand, this, &AtCore::newMessage);
            if (firmwarePlugin()->name() != QStringLiteral("Grbl")) {
                disconnect(d->tempTimer, &QTimer::timeout, this, &AtCore::checkTemperature);
                d->tempTimer->stop();
            }
            //Attempt to unload the firmware plugin.
            QString name = firmwarePlugin()->name();
            QString msg = d->pluginLoader.unload() ? QStringLiteral("closed.") : QStringLiteral("Failed to close.");
            qCDebug(ATCORE_CORE) << QStringLiteral("Firmware plugin %1 %2").arg(name, msg);
        }
        //Do not reset the connect on disconnect when closing this will cause a reset on connect for the next connection.
        disconnect(serial(), &SerialLayer::serialError, this, &AtCore::handleSerialError);
        serial()->close();
        //Clear our copy of the sdcard filelist
        clearSdCardFileList();
        setState(AtCore::DISCONNECTED);
        d->serialTimer->start();
    }
}

AtCore::STATES AtCore::state(void)
{
    return d->printerState;
}

void AtCore::setState(AtCore::STATES state)
{
    if (state != d->printerState) {
        qCDebug(ATCORE_CORE) << QStringLiteral("Atcore state changed from [%1] to [%2]")
                             .arg(QVariant::fromValue(d->printerState).value<QString>(),
                                  QVariant::fromValue(state).value<QString>());
        d->printerState = state;
        if (state == AtCore::FINISHEDPRINT && d->sdCardPrinting) {
            //Clean up the sd card print
            d->sdCardPrinting = false;
            disconnect(d->tempTimer, &QTimer::timeout, this, &AtCore::sdCardPrintStatus);
        }
        emit stateChanged(d->printerState);
    }
}

void AtCore::stop()
{
    //Stop a print job
    setState(AtCore::STOP);
    d->commandQueue.clear();
    if (d->sdCardPrinting) {
        stopSdPrint();
    }
    setExtruderTemp(0, 0);
    setBedTemp(0);
    home(AtCore::X);
}

void AtCore::emergencyStop()
{
    //Emergency Stop. Stops the machine
    //Clear the queue, and any print job
    //Before sending the command to ensure
    //Less chance of movement after the restart.
    d->commandQueue.clear();
    if (AtCore::state() == AtCore::BUSY) {
        if (!d->sdCardPrinting) {
            //Stop our running print thread
            setState(AtCore::STOP);
        }
    }
    //push command through serial to bypass atcore's queue.
    serial()->pushCommand(GCode::toCommand(GCode::M112).toLocal8Bit());
}

void AtCore::stopSdPrint()
{
    //Stop an SdCard Print.
    pushCommand(GCode::toCommand(GCode::M25));
    d->sdCardFileName = QString();
    pushCommand(GCode::toCommand(GCode::M23, d->sdCardFileName));
    AtCore::setState(AtCore::FINISHEDPRINT);
    AtCore::setState(AtCore::IDLE);
}

void AtCore::requestFirmware()
{
    if (serialInitialized()) {
        qCDebug(ATCORE_CORE) << "Sending " << GCode::description(GCode::M115);
        serial()->pushCommand(GCode::toCommand(GCode::M115).toLocal8Bit());
    } else {
        qCDebug(ATCORE_CORE) << "There is no open device to send commands";
    }
}

bool AtCore::firmwarePluginLoaded() const
{
    if (firmwarePlugin()) {
        return true;
    } else {
        return false;
    }
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
        pushCommand(GCode::toCommand(GCode::M25));
    }
    //Push the command to request current coordinates.
    //This will be read by AtCore::newMessage and stored for use on resume.
    pushCommand(GCode::toCommand(GCode::M114));
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
        pushCommand(GCode::toCommand(GCode::M24));
    } else {
        //Move back to previous coordinates.
        pushCommand(GCode::toCommand(GCode::G0, QString::fromLatin1(d->posString)));
    }
    setState(AtCore::BUSY);
}

/*~~~~~Control Slots ~~~~~~~~*/

void AtCore::home()
{
    pushCommand(GCode::toCommand(GCode::G28));
}

void AtCore::home(uchar axis)
{
    QString args;

    if (axis & AtCore::X) {
        args.append(QStringLiteral("X0 "));
    }

    if (axis & AtCore::Y) {
        args.append(QStringLiteral("Y0 "));
    }

    if (axis & AtCore::Z) {
        args.append(QStringLiteral("Z0"));
    }
    pushCommand(GCode::toCommand(GCode::G28, args));
}

void AtCore::setExtruderTemp(uint temp, uint extruder, bool andWait)
{
    if (andWait) {
        pushCommand(GCode::toCommand(GCode::M109, QString::number(temp), QString::number(extruder)));
    } else {
        pushCommand(GCode::toCommand(GCode::M104, QString::number(extruder), QString::number(temp)));
    }
}

void AtCore::setBedTemp(uint temp, bool andWait)
{
    if (andWait) {
        pushCommand(GCode::toCommand(GCode::M190, QString::number(temp)));
    } else {
        pushCommand(GCode::toCommand(GCode::M140, QString::number(temp)));
    }
}

void AtCore::setFanSpeed(uint speed, uint fanNumber)
{
    pushCommand(GCode::toCommand(GCode::M106, QString::number(fanNumber), QString::number(speed)));
}

void AtCore::setPrinterSpeed(uint speed)
{
    pushCommand(GCode::toCommand(GCode::M220, QString::number(speed)));
}

void AtCore::setFlowRate(uint speed)
{
    pushCommand(GCode::toCommand(GCode::M221, QString::number(speed)));
}

void AtCore::move(AtCore::AXES axis, int arg)
{
    const auto axisAsString = QMetaEnum::fromType<AtCore::AXES>().valueToKey(axis);
    move(QLatin1Char(axisAsString[0]), arg);
}

void AtCore::move(QLatin1Char axis, int arg)
{
    pushCommand(GCode::toCommand(GCode::G1, QStringLiteral("%1 %2").arg(axis).arg(QString::number(arg))));
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
        serial()->pushCommand(firmwarePlugin()->translate(text));
    } else {
        serial()->pushCommand(text.toLocal8Bit());
    }
    d->ready = false;
}

void AtCore::checkTemperature()
{
    //One request for the temperature in the queue at a time.
    if (d->commandQueue.contains(GCode::toCommand(GCode::M105))) {
        return;
    }
    pushCommand(GCode::toCommand(GCode::M105));
}

void AtCore::showMessage(const QString &message)
{
    if (!message.isEmpty()) {
        pushCommand(GCode::toCommand((GCode::M117), message));
    }
}

void AtCore::setUnits(AtCore::UNITS units)
{
    switch (units) {
    case AtCore::METRIC:
        pushCommand(GCode::toCommand(GCode::G21));
        break;
    case AtCore::IMPERIAL:
        pushCommand(GCode::toCommand(GCode::G20));
        break;
    }
}

QStringList AtCore::portSpeeds() const
{
    return serial()->validBaudRates();
}

void AtCore::disableMotors(uint delay)
{
    //Disables motors
    if (delay) {
        pushCommand(GCode::toCommand(GCode::M84, QString::number(delay)));
    } else {
        pushCommand(GCode::toCommand(GCode::M84));
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
    pushCommand(GCode::toCommand(GCode::M20));
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
        pushCommand(GCode::toCommand(GCode::M30, fileName));
        getSDFileList();
    } else {
        qCDebug(ATCORE_CORE) << "Delete failed file not found:" << fileName;
    }
}

void AtCore::mountSd(uint slot)
{
    pushCommand(GCode::toCommand(GCode::M21, QString::number(slot)));
}

void AtCore::umountSd(uint slot)
{
    pushCommand(GCode::toCommand(GCode::M22, QString::number(slot)));
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
    if (d->commandQueue.contains(GCode::toCommand(GCode::M27))) {
        return;
    }
    pushCommand(GCode::toCommand(GCode::M27));
}

void AtCore::disableResetOnConnect(const QString &port)
{
#ifdef Q_OS_UNIX
//should work on all unix'
    QProcess process;
    QStringList args({QStringLiteral("-F/dev/%1").arg(port), QStringLiteral("-hupcl")});
    process.start(QStringLiteral("stty"), args);
    process.waitForFinished(500);

    connect(&process, &QProcess::errorOccurred, this, [this, &process] {
        qCDebug(ATCORE_CORE) << "Stty Error:" << process.errorString();
    });

#elif Q_OS_WIN
    //TODO: Disable hangup on windows.
#endif
}

void AtCore::handleSerialError(QSerialPort::SerialPortError error)
{
    QString errorString;

    switch (error) {
    case (QSerialPort::DeviceNotFoundError):
        errorString = tr("Device not found");
        break;
    case (QSerialPort::WriteError):
        errorString = tr("Unable to write to device");
        break;
    case (QSerialPort::ReadError):
        errorString = tr("Unable to read from device");
        break;
    case (QSerialPort::ResourceError):
    case (QSerialPort::TimeoutError):
        errorString = tr("The device no longer available");
        closeConnection();
        break;
    case (QSerialPort::UnsupportedOperationError):
        errorString = tr("Device does not support the operation");
        break;
    case (QSerialPort::UnknownError):
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
