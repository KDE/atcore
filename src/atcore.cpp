#include "atcore.h"
#include "seriallayer.h"
#include "ifirmware.h"
#include "gcodecommands.h"

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
};

AtCore::AtCore(QObject *parent) : QObject(parent), d(new AtCorePrivate)
{
    setState(DISCONNECTED);
    d->pluginsDir = QDir(qApp->applicationDirPath());

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
    d->pluginsDir.cdUp();
    d->pluginsDir.cd(QStringLiteral("src"));
    d->pluginsDir.cd(QStringLiteral("plugins"));
    qDebug() << d->pluginsDir;
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

void AtCore::findFirmware(const QByteArray &message)
{
    if (state() == DISCONNECTED) {
        if (message.contains("start")) {
            QTimer::singleShot(500, this, &AtCore::requestFirmware);
            setState(CONNECTING);
        }
        return;
    }

    qDebug() << "Find Firmware Called" << message;
    if (!message.startsWith("FIRMWARE")) {
        qDebug() << "No firmware yet.";
        return;
    }

    qDebug() << "Found firmware string, looking for plugin.";
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
        }

        if (!message.contains(file.toLocal8Bit())) {
            qDebug() << "But it's not the plugin for this firmware." << message;
            continue;
        }

        qDebug() << "Full Folder:" << (d->pluginsDir.path() + f);
        d->pluginLoader.setFileName(d->pluginsDir.path() + QChar::fromLatin1('/') + f);

        if (!d->pluginLoader.load()) {
            qDebug() << d->pluginLoader.errorString();
        } else {
            qDebug() << "Loading plugin.";
        }
        setPlugin(qobject_cast<IFirmware *>(d->pluginLoader.instance()));
    }
    if (!plugin()) {
        qDebug() << "No plugin loaded.";
        qDebug() << "Looking plugin in folder:" << d->pluginsDir;
        setState(CONNECTING);
    } else {
        qDebug() << "Connected to" << plugin()->name();
        disconnect(serial(), &SerialLayer::receivedCommand, this, &AtCore::findFirmware);
        connect(serial(), &SerialLayer::receivedCommand, this, &AtCore::newMessage);
        setState(IDLE);
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
    lastMessage = message;
    emit(receivedMessage(lastMessage));
}

void AtCore::print(const QString &fileName)
{
    QFile file(fileName);
    file.open(QFile::ReadOnly);
    QTextStream gcodestream(&file);
    QString cline;
    QEventLoop loop;
    connect(this, &AtCore::receivedMessage, &loop, &QEventLoop::quit);

    while (!gcodestream.atEnd()) {
        QCoreApplication::processEvents(); //make sure all events are processed.
        switch (state()) {
        case IDLE:
        case BUSY:
            setState(BUSY);
            cline = gcodestream.readLine();
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
                    if (plugin()->readyForNextCommand(QString::fromUtf8(lastMessage))) {
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
    return printerState;
}

void AtCore::setState(PrinterState state)
{
    printerState = state;
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

void AtCore::pushCommand(const QString &comm)
{
    if (!plugin()) {
        serial()->pushCommand(comm.toLocal8Bit());
    } else {
        serial()->pushCommand(plugin()->translate(comm));
    }
}

void AtCore::requestFirmware()
{
    qDebug() << "Sending " << GCode::toString(GCode::M115);
    pushCommand(GCode::toCommand(GCode::M115));
}