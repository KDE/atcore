/* AtCore
    Copyright (C) <2016>

    Authors:
        Tomaz Canabrava <tcanabrava@kde.org>
        Chris Rizzitello <rizzitello@kde.org>
        Patrick José Pereira <patrickelectric@gmail.com>
        Lays Rodrigues <laysrodrigues@gmail.com>

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
 */
struct AtCorePrivate {
    IFirmware *firmwarePlugin = nullptr;//!< @param firmwarePlugin: pointer to firmware plugin
    SerialLayer *serial = nullptr;      //!< @param serial: pointer to the serial layer
    QPluginLoader pluginLoader;         //!< @param pluginLoader: QPluginLoader
    QDir pluginsDir;                    //!< @param pluginsDir: Directory where plugins were found
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
};

AtCore::AtCore(QObject *parent) :
    QObject(parent),
    d(new AtCorePrivate)
{
    qRegisterMetaType<AtCore::STATES>("AtCore::STATES");
    setState(AtCore::DISCONNECTED);

    d->tempTimer = new QTimer(this);
    d->tempTimer->setInterval(5000);
    d->tempTimer->setSingleShot(false);

    QStringList pathList = AtCoreDirectories::pluginDir;
    pathList.append(QLibraryInfo::location(QLibraryInfo::PluginsPath) + QStringLiteral("/AtCore"));

    for (const auto &path : pathList) {
        qCDebug(ATCORE_PLUGIN) << "Lookin for plugins in " << path;
        if (QDir(path).exists()) {
            d->pluginsDir = QDir(path);
            qCDebug(ATCORE_PLUGIN) << "Valid path for plugins found !";
            break;
        }
    }
    if (!d->pluginsDir.exists()) {
        qCritical() << "No valid path for plugin !";
    }

#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
    d->pluginsDir = qApp->applicationDirPath() + QStringLiteral("/plugins");
#endif
    qCDebug(ATCORE_PLUGIN) << d->pluginsDir;
    findFirmwarePlugins();
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
        qWarning() << "Cant find firwmware, serial not connected !";
        return;
    }

    if (state() == AtCore::CONNECTING) {
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
    }

    qCDebug(ATCORE_CORE) << "Find Firmware Called" << message;
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
        d->extruderCount = message.at(message.indexOf("EXTRUDER_COUNT:") + 15) - '0';
    }
    qCDebug(ATCORE_CORE) << "Extruder Count:" << QString::number(extruderCount());

    loadFirmwarePlugin(fwName);
}

void AtCore::loadFirmwarePlugin(const QString &fwName)
{
    if (d->plugins.contains(fwName)) {
        d->pluginLoader.setFileName(d->plugins[fwName]);
        if (!d->pluginLoader.load()) {
            qCDebug(ATCORE_PLUGIN) << d->pluginLoader.errorString();
        } else {
            qCDebug(ATCORE_PLUGIN) << "Loading plugin.";
        }
        d->firmwarePlugin = qobject_cast<IFirmware *>(d->pluginLoader.instance());

        if (!firmwarePluginLoaded()) {
            qCDebug(ATCORE_PLUGIN) << "No plugin loaded.";
            qCDebug(ATCORE_PLUGIN) << "Looking plugin in folder:" << d->pluginsDir;
            setState(AtCore::CONNECTING);
        } else {
            qCDebug(ATCORE_PLUGIN) << "Connected to" << firmwarePlugin()->name();
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
        qCDebug(ATCORE_CORE) << "No Firmware Loaded";
    }
}

bool AtCore::initSerial(const QString &port, int baud)
{
    d->serial = new SerialLayer(port, baud);
    if (serialInitialized()) {
        setState(AtCore::CONNECTING);
        connect(serial(), &SerialLayer::receivedCommand, this, &AtCore::findFirmware);
        return true;
    } else {
        qCDebug(ATCORE_CORE) << "Failed to open device.";
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
        foreach (const QSerialPortInfo &serialPortInfo, serialPortInfoList) {
#ifdef Q_OS_MAC
            //Mac OS has callout serial ports starting with cu. They can only receive data and it's necessary to filter them out
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
    if (newTime == 0) {
        if (d->serialTimer) {
            disconnect(d->serialTimer, &QTimer::timeout, this, &AtCore::locateSerialPort);
            delete d->serialTimer;
        }
        return;
    }
    if (!d->serialTimer) {
        d->serialTimer = new QTimer();
        connect(d->serialTimer, &QTimer::timeout, this, &AtCore::locateSerialPort);
    }
    d->serialTimer->start(newTime);
}

void AtCore::newMessage(const QByteArray &message)
{
    d->lastMessage = message;
    if (message.startsWith(QString::fromLatin1("X:").toLocal8Bit())) {
        d->posString = message;
        d->posString.resize(d->posString.indexOf('E'));
        d->posString.replace(':', "");
    }

    //Check if have temperature info and decode it
    if (d->lastMessage.contains("T:") || d->lastMessage.contains("B:")) {
        temperature().decodeTemp(message);
    }
    emit(receivedMessage(d->lastMessage));
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

void AtCore::print(const QString &fileName)
{
    if (state() == AtCore::CONNECTING) {
        qCDebug(ATCORE_CORE) << "Load a firmware plugin to print.";
        return;
    }
    //START A THREAD AND CONNECT TO IT
    setState(AtCore::STARTPRINT);
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
    d->commandQueue.append(comm);
    if (d->ready) {
        processQueue();
    }
}

void AtCore::closeConnection()
{
    if (serialInitialized()) {
        if (state() == AtCore::BUSY) {
            //we have to clean print if printing.
            setState(AtCore::STOP);
        }
        if (firmwarePluginLoaded()) {
            disconnect(firmwarePlugin(), &IFirmware::readyForCommand, this, &AtCore::processQueue);
            if (firmwarePlugin()->name() != QStringLiteral("Grbl")) {
                disconnect(d->tempTimer, &QTimer::timeout, this, &AtCore::checkTemperature);
                d->tempTimer->stop();
            }
        }
        serial()->close();
        setState(AtCore::DISCONNECTED);
    }
}

AtCore::STATES AtCore::state(void)
{
    return d->printerState;
}

void AtCore::setState(AtCore::STATES state)
{
    if (state != d->printerState) {
        qCDebug(ATCORE_CORE) << "Atcore state changed from [" \
                             << d->printerState << "] to [" << state << "]";
        d->printerState = state;
        emit(stateChanged(d->printerState));
    }
}

void AtCore::stop()
{
    setState(AtCore::STOP);
    d->commandQueue.clear();
    setExtruderTemp(0, 0);
    setBedTemp(0);
    home(AtCore::X);
}

void AtCore::emergencyStop()
{
    if (state() == AtCore::BUSY) {
        setState(AtCore::STOP);
    }
    d->commandQueue.clear();
    serial()->pushCommand(GCode::toCommand(GCode::M112).toLocal8Bit());
}

void AtCore::requestFirmware()
{
    if (serialInitialized()) {
        qCDebug(ATCORE_CORE) << "Sending " << GCode::toString(GCode::M115);
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
void AtCore::findFirmwarePlugins()
{
    d->plugins.clear();
    qCDebug(ATCORE_PLUGIN) << "plugin dir:" << d->pluginsDir;
    QStringList files = d->pluginsDir.entryList(QDir::Files);
    foreach (const QString &f, files) {
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
            qCDebug(ATCORE_PLUGIN) << "File" << file << "not plugin.";
            continue;
        }
        qCDebug(ATCORE_CORE) << "Found plugin file" << f;
        if (file.startsWith(QStringLiteral("lib"))) {
            file = file.remove(QStringLiteral("lib"));
        }
        file = file.toLower().simplified();
        QString pluginString;
        pluginString.append(d->pluginsDir.path());
        pluginString.append(QChar::fromLatin1('/'));
        pluginString.append(f);
        d->plugins[file] = pluginString;
        qCDebug(ATCORE_CORE) << tr("plugins[%1]=%2").arg(file, pluginString);
    }
}
QStringList AtCore::availableFirmwarePlugins() const
{
    return d->plugins.keys();
}

void AtCore::detectFirmware()
{
    connect(serial(), &SerialLayer::receivedCommand, this, &AtCore::findFirmware);
}

void AtCore::pause(const QString &pauseActions)
{
    pushCommand(GCode::toCommand(GCode::M114));
    setState(AtCore::PAUSE);
    if (!pauseActions.isEmpty()) {
        QStringList temp = pauseActions.split(QChar::fromLatin1(','));
        for (int i = 0; i < temp.length(); i++) {
            pushCommand(temp.at(i));
        }
    }
}

void AtCore::resume()
{
    pushCommand(GCode::toCommand(GCode::G0, QString::fromLatin1(d->posString)));
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
    temperature().setExtruderTargetTemperature(temp);
}

void AtCore::setBedTemp(uint temp, bool andWait)
{
    if (andWait) {
        pushCommand(GCode::toCommand(GCode::M190, QString::number(temp)));
    } else {
        pushCommand(GCode::toCommand(GCode::M140, QString::number(temp)));
    }
    temperature().setBedTargetTemperature(temp);
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

void AtCore::move(AtCore::AXES axis, uint arg)
{
    if (axis & AtCore::X) {
        pushCommand(GCode::toCommand(GCode::G1, QStringLiteral("X %1").arg(QString::number(arg))));
    } else if (axis & AtCore::Y) {
        pushCommand(GCode::toCommand(GCode::G1, QStringLiteral("Y %1").arg(QString::number(arg))));
    } else if (axis & AtCore::Z) {
        pushCommand(GCode::toCommand(GCode::G1, QStringLiteral("Z %1").arg(QString::number(arg))));
    } else if (axis & AtCore::E) {
        pushCommand(GCode::toCommand(GCode::G1, QStringLiteral("E %1").arg(QString::number(arg))));
    }
}

int AtCore::extruderCount() const
{
    return d->extruderCount;
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
