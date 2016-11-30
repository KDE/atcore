#include "atcore.h"
#include "seriallayer.h"
#include "gcodecommands.h"
#include "atcore_default_folders.h"

#include <QDir>
#include <QSerialPortInfo>
#include <QPluginLoader>
#include <QCoreApplication>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QEventLoop>
#include <QTextStream>

struct AtCorePrivate {
    IFirmware *fwPlugin = nullptr;
    SerialLayer *serial = nullptr;
    QPluginLoader pluginLoader;
    QDir pluginsDir;
    bool isInitialized;
    QMap<QString, QString> plugins;
    QByteArray lastMessage;
    PrinterStatus printerStatus;
};

AtCore::AtCore(QObject *parent) :
    QObject(parent),
    d(new AtCorePrivate)
{
    setState(DISCONNECTED);

    for (const auto &path : AtCoreDirectories::pluginDir) {
        if (QDir(path).exists()) {
            d->pluginsDir = QDir(path);
            break;
        }
    }
    if (!d->pluginsDir.exists()) {
        qCritical() << "No valid path for plugin !";
    }

#if defined(Q_OS_WIN)
    if (d->pluginsDir.dirName().toLower() == "debug" || d->pluginsDir.dirName().toLower() == "release") {
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

void AtCore::findFirmware(const QByteArray &message)
{
    if (state() == DISCONNECTED) {
        if (message.contains("start")) {
            qDebug() << "Waiting requestFirmware.";
            QTimer::singleShot(500, this, &AtCore::requestFirmware);
            setState(CONNECTING);
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
            disconnect(serial(), &SerialLayer::receivedCommand, this, &AtCore::findFirmware);
            connect(serial(), &SerialLayer::receivedCommand, this, &AtCore::newMessage);
            setState(IDLE);
        }
    } else {
        qDebug() << "No Firmware Loaded";
    }
}

void AtCore::initFirmware(const QString &port, int baud)
{
    setSerial(new SerialLayer(port, baud));
    connect(serial(), &SerialLayer::receivedCommand, this, &AtCore::findFirmware);
}

bool AtCore::isInitialized()
{
    return d->serial->opened();
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
    setState(STARTPRINT);
    printFile(fileName);
    //let the state reflect that we have finished printing
    setState(FINISHEDPRINT);
    //connected clients will have reacted to the finished print
    //above and done stuff accordingly now set the status to idle
    setState(IDLE);
}

void AtCore::pushCommand(const QString &comm)
{
    if (!pluginLoaded()) {
        serial()->pushCommand(comm.toLocal8Bit());
    } else {
        serial()->pushCommand(plugin()->translate(comm));
    }
}

void AtCore::closeConnection()
{
    if (serial()->opened()) {
        if (state() == BUSY) {
            //we have to clean print if printing.
            setState(STOP);
        }
        serial()->closeConnection();
        setState(DISCONNECTED);
    }
}

void AtCore::printFile(const QString &fileName)
{
    QFile file(fileName);
    file.open(QFile::ReadOnly);
    qint64 totalSize = file.bytesAvailable();
    qint64 stillSize = totalSize;
    QTextStream gcodestream(&file);
    QString cline;
    QEventLoop loop;
    connect(this, &AtCore::receivedMessage, &loop, &QEventLoop::quit);

    while (!gcodestream.atEnd()) {
        QCoreApplication::processEvents(); //make sure all events are processed.
        switch (state()) {
        case STARTPRINT:
        case IDLE:
        case BUSY:
            setState(BUSY);
            cline = gcodestream.readLine();
            stillSize -= cline.size() + 1; //remove read chars
            d->printerStatus.percentage = float(totalSize - stillSize) * 100.0 / float(totalSize);
            emit(printProgressChanged(d->printerStatus.percentage));
            cline = cline.simplified();
            if (cline.contains(QChar::fromLatin1(';'))) {
                cline.resize(cline.indexOf(QChar::fromLatin1(';')));
            }
            if (!cline.isEmpty()) {
                pushCommand(cline);
                bool waiting = true;
                while (waiting) {
                    if (!serial()->commandAvailable()) {
                        loop.exec();
                    }
                    if (plugin()->readyForNextCommand(QString::fromUtf8(d->lastMessage))) {
                        waiting = false;
                    }
                }
            }
            break;

        case ERROR:
            qDebug() << tr("Error State");
            break;

        case STOP: {
            QString stopString(GCode::toCommand(GCode::M112));
            gcodestream.setString(&stopString);
            setState(IDLE);
            break;
        }

        case PAUSE:
            break;

        default:
            qDebug() << tr("Unknown State");
            break;
        }
    }
    disconnect(this, &AtCore::receivedMessage, &loop, &QEventLoop::quit);
}

PrinterState AtCore::state(void)
{
    return d->printerStatus.printerState;
}

void AtCore::setState(PrinterState state)
{
    if (state != d->printerStatus.printerState) {
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
    switch (state()) {
    case BUSY:
        setState(STOP);
    default:
        pushCommand(GCode::toCommand(GCode::M112));
    }
}

void AtCore::requestFirmware()
{
    qDebug() << "Sending " << GCode::toString(GCode::M115);
    pushCommand(GCode::toCommand(GCode::M115));
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
        if (file.endsWith(QStringLiteral(".dylib"))
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
            file = file.toLower().simplified();
            QString pluginString;
            pluginString.append(d->pluginsDir.path());
            pluginString.append(QChar::fromLatin1('/'));
            pluginString.append(f);
            d->plugins[file] = pluginString;
            qDebug() << tr("plugins[%1]=%2").arg(file, pluginString);
        }
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
