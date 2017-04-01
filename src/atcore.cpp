/* AtCore
    Copyright (C) <2016>

    Authors:
        Tomaz Canabrava <tcanabrava@kde.org>
        Chris Rizzitello <sithlord48@gmail.com>
        Patrick José Pereira <patrickelectric@gmail.com>
        Lays Rodrigues <laysrodrigues@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "atcore.h"
#include "seriallayer.h"
#include "gcodecommands.h"
#include "printthread.h"
#include "atcore_default_folders.h"

#include <QDir>
#include <QSerialPortInfo>
#include <QPluginLoader>
#include <QCoreApplication>
#include <QLoggingCategory>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QThread>

Q_LOGGING_CATEGORY(ATCORE_PLUGIN, "org.kde.atelier.core.plugin");

struct AtCorePrivate {
    IFirmware *fwPlugin = nullptr;
    SerialLayer *serial = nullptr;
    QPluginLoader pluginLoader;
    QDir pluginsDir;
    QMap<QString, QString> plugins;
    QByteArray lastMessage;
    PrinterStatus printerStatus;
    int extruderCount = 1;
    Temperature temperature;
    QStringList commandQueue;
    bool ready = false;
    QTimer *tempTimer = nullptr;
};

AtCore::AtCore(QObject *parent) :
    QObject(parent),
    d(new AtCorePrivate)
{
    qRegisterMetaType<PrinterState>("PrinterState");
    setState(DISCONNECTED);

    d->tempTimer = new QTimer;
    d->tempTimer->setInterval(5000);
    d->tempTimer->setSingleShot(false);

    for (const auto &path : AtCoreDirectories::pluginDir) {
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

#if defined(Q_OS_WIN)
    if (d->pluginsDir.dirName().toLower() == QStringLiteral("debug") || d->pluginsDir.dirName().toLower() == QStringLiteral("release")) {
        d->pluginsDir.cdUp();
    }
#elif defined(Q_OS_MAC)
    if (d->pluginsDir.dirName() == "MacOS") {
        d->pluginsDir.cdUp();
        d->pluginsDir.cdUp();
        d->pluginsDir.cdUp();
    }
#endif
    qDebug() << d->pluginsDir;
    findPlugins();
    setState(DISCONNECTED);
}

void AtCore::setSerial(SerialLayer *serial)
{
    d->serial = serial;
}

void AtCore::setPlugin(IFirmware *plugin)
{
    d->fwPlugin = plugin;
}

SerialLayer *AtCore::serial() const
{
    return d->serial;
}

IFirmware *AtCore::plugin() const
{
    return d->fwPlugin;
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
    if (state() == DISCONNECTED) {
        qWarning() << "Cant find firwmware, serial not connected !";
        return;
    }

    if (state() == CONNECTING) {
        if (message.contains("start")) {
            qDebug() << "Waiting requestFirmware.";
            QTimer::singleShot(500, this, &AtCore::requestFirmware);
            return;
        } else if (message.contains("Grbl")) {
            loadFirmware(QString::fromLatin1("grbl"));
            return;
        }
    }

    qDebug() << "Find Firmware Called" << message;
    if (!message.contains("FIRMWARE_NAME:")) {
        qDebug() << "No firmware yet.";
        return;
    }

    qDebug() << "Found firmware string, Looking for Firmware Name.";

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
    qDebug() << "Firmware Name:" << fwName;

    if (message.contains("EXTRUDER_COUNT:")) {
        //this code is broken if more then 9 extruders are detected. since only one char is returned
        d->extruderCount = message.at(message.indexOf("EXTRUDER_COUNT:") + 15) - '0';
    }
    qDebug() << "Extruder Count:" << QString::number(extruderCount());

    loadFirmware(fwName);
}

void AtCore::loadFirmware(const QString &fwName)
{
    if (d->plugins.contains(fwName)) {
        d->pluginLoader.setFileName(d->plugins[fwName]);
        if (!d->pluginLoader.load()) {
            qDebug() << d->pluginLoader.errorString();
        } else {
            qDebug() << "Loading plugin.";
        }
        setPlugin(qobject_cast<IFirmware *>(d->pluginLoader.instance()));

        if (!pluginLoaded()) {
            qDebug() << "No plugin loaded.";
            qDebug() << "Looking plugin in folder:" << d->pluginsDir;
            setState(CONNECTING);
        } else {
            qDebug() << "Connected to" << plugin()->name();
            plugin()->init(this);
            disconnect(serial(), &SerialLayer::receivedCommand, this, &AtCore::findFirmware);
            connect(serial(), &SerialLayer::receivedCommand, this, &AtCore::newMessage);
            connect(plugin(), &IFirmware::readyForCommand, this, &AtCore::processQueue);
            d->ready = true; // ready on new firmware load
            connect(d->tempTimer, &QTimer::timeout, this, &AtCore::checkTemperature);
            d->tempTimer->start();
            setState(IDLE);
        }
    } else {
        qDebug() << "No Firmware Loaded";
    }
}

void AtCore::initSerial(const QString &port, int baud)
{
    setSerial(new SerialLayer(port, baud));
    if (isInitialized()) {
        setState(CONNECTING);
    }
    connect(serial(), &SerialLayer::receivedCommand, this, &AtCore::findFirmware);
}

bool AtCore::isInitialized()
{
    if (!d->serial) {
        return false;
    }
    return d->serial->isOpen();
}

QList<QSerialPortInfo> AtCore::serialPorts() const
{
    return QList<QSerialPortInfo>();
}

void AtCore::newMessage(const QByteArray &message)
{
    d->lastMessage = message;
    if (message.startsWith(QString::fromLatin1("X:").toLocal8Bit())) {
        d->printerStatus.posString = message;
        d->printerStatus.posString.resize(d->printerStatus.posString.indexOf('E'));
        d->printerStatus.posString.replace(':', "");
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

float AtCore::percentagePrinted()
{
    return d->printerStatus.percentage;
}

void AtCore::print(const QString &fileName)
{
    if (state() == CONNECTING) {
        qDebug() << "Load a firmware plugin to print.";
        return;
    }
    //START A THREAD AND CONNECT TO IT
    setState(STARTPRINT);
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
    if (isInitialized()) {
        if (state() == BUSY) {
            //we have to clean print if printing.
            setState(STOP);
        }
        if (pluginLoaded()) {
            disconnect(plugin(), &IFirmware::readyForCommand, this, &AtCore::processQueue);
            disconnect(d->tempTimer, &QTimer::timeout, this, &AtCore::checkTemperature);
            d->tempTimer->stop();
        }
        serial()->close();
        setState(DISCONNECTED);
    }
}

PrinterState AtCore::state(void)
{
    return d->printerStatus.printerState;
}

void AtCore::setState(PrinterState state)
{
    if (state != d->printerStatus.printerState) {
        qDebug() << "Atcore state changed from [" \
                 << d->printerStatus.printerState << "] to [" << state << "]";
        d->printerStatus.printerState = state;
        emit(stateChanged(d->printerStatus.printerState));
    }
}

QByteArray AtCore::popCommand()
{
    return serial()->popCommand();
}

void AtCore::stop()
{
    setState(STOP);
    d->commandQueue.clear();
    setExtruderTemp(0, 0);
    setBedTemp(0);
    home('X');
    setState(IDLE);
}

void AtCore::emergencyStop()
{
    switch (state()) {
    case BUSY:
        stop();
    default:
        if (isInitialized()) {
            serial()->pushCommand(GCode::toCommand(GCode::M112).toLocal8Bit());
        }
    }
}

void AtCore::requestFirmware()
{
    qDebug() << "Sending " << GCode::toString(GCode::M115);
    if (isInitialized()) {
        serial()->pushCommand(GCode::toCommand(GCode::M115).toLocal8Bit());
    }
}

bool AtCore::pluginLoaded()
{
    if (plugin()) {
        return true;
    } else {
        return false;
    }
}
void AtCore::findPlugins()
{
    d->plugins.clear();
    qDebug() << "plugin dir:" << d->pluginsDir;
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
            qDebug() << "File" << file << "not plugin.";
            continue;
        }
        qDebug() << "Found plugin file" << f;
        if (file.startsWith(QStringLiteral("lib"))) {
            file = file.remove(QStringLiteral("lib"));
        }
        file = file.toLower().simplified();
        QString pluginString;
        pluginString.append(d->pluginsDir.path());
        pluginString.append(QChar::fromLatin1('/'));
        pluginString.append(f);
        d->plugins[file] = pluginString;
        qDebug() << tr("plugins[%1]=%2").arg(file, pluginString);
    }
}
QStringList AtCore::availablePlugins()
{
    return d->plugins.keys();
}

void AtCore::detectFirmware()
{
    connect(serial(), &SerialLayer::receivedCommand, this, &AtCore::findFirmware);
    requestFirmware();
}

void AtCore::pause(const QString &pauseActions)
{
    pushCommand(GCode::toCommand(GCode::M114));
    setState(PAUSE);
    if (!pauseActions.isNull()) {
        QStringList temp = pauseActions.split(QChar::fromLatin1(','));
        for (int i = 0; i < temp.length(); i++) {
            pushCommand(temp.at(i));
        }
    }
}

void AtCore::resume()
{
    pushCommand(GCode::toCommand(GCode::G0, QString::fromLatin1(d->printerStatus.posString)));
    setState(BUSY);
}

/*~~~~~Control Slots ~~~~~~~~*/

void AtCore::home()
{
    pushCommand(GCode::toCommand(GCode::G28));
}

void AtCore::home(uchar axis)
{
    QString args;

    if (axis & X) {
        args.append(QStringLiteral("X0 "));
    }

    if (axis & Y) {
        args.append(QStringLiteral("Y0 "));
    }

    if (axis & Z) {
        args.append(QStringLiteral("Z0"));
    }
    pushCommand(GCode::toCommand(GCode::G28, args));
}

void AtCore::setExtruderTemp(uint temp, uint extruder)
{
    pushCommand(GCode::toCommand(GCode::M104, QString::number(extruder), QString::number(temp)));
}

void AtCore::setBedTemp(uint temp)
{
    pushCommand(GCode::toCommand(GCode::M140, QString::number(temp)));
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

void AtCore::move(uchar axis, uint arg)
{
    if (axis & X) {
        pushCommand(GCode::toCommand(GCode::G1, QStringLiteral("X %1").arg(QString::number(arg))));
    } else if (axis & Y) {
        pushCommand(GCode::toCommand(GCode::G1, QStringLiteral("Y %1").arg(QString::number(arg))));
    } else if (axis & Z) {
        pushCommand(GCode::toCommand(GCode::G1, QStringLiteral("Z %1").arg(QString::number(arg))));
    } else if (axis & E) {
        pushCommand(GCode::toCommand(GCode::G1, QStringLiteral("E %1").arg(QString::number(arg))));
    }
}

int AtCore::extruderCount()
{
    return d->extruderCount;
}

void AtCore::processQueue()
{
    d->ready = true;

    if (d->commandQueue.isEmpty()) {
        return;
    }

    if (!isInitialized()) {
        qCDebug(ATCORE_PLUGIN) << "Can't process queue ! Serial not initialized.";
        return;
    }

    QString text = d->commandQueue.takeAt(0);

    if (pluginLoaded()) {
        serial()->pushCommand(plugin()->translate(text));
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
