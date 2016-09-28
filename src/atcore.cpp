#include "atcore.h"
#include "seriallayer.h"
#include "ifirmware.h"

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
    SerialLayer *currentProtocol;
    IFirmware *fwPlugin;
    SerialLayer *serial;
    QPluginLoader pluginLoader;
    QDir pluginsDir;
    bool isInitialized;
};

AtCore::AtCore(QObject *parent) : QObject(parent), d(new AtCorePrivate)
{
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
    d->pluginsDir.cd("src");
    d->pluginsDir.cd("plugins");
    qDebug() << d->pluginsDir;
    printerState = DISCONNECTED;
}

SerialLayer *AtCore::serial() const
{
    return d->serial;
}

void AtCore::findFirmware(const QByteArray &message)
{
    if (printerState == DISCONNECTED) {
        if (message == "start") {
            QTimer::singleShot(500, this, [ = ] {qDebug() << "Sending M115"; d->serial->pushCommand("M115");});
            printerState = CONNECTING;
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
        if (file.endsWith(".dll"))
#elif defined(Q_OS_MAC)
        if (file.endsWith(".dylib"))
#else
        if (file.endsWith(".so"))
#endif
            file = file.split('.').at(0);
        else {
            qDebug() << "File" << file << "not plugin.";
            continue;
        }
        qDebug() << "Found plugin file" << f;
        if (file.startsWith("lib")) {
            file = file.remove("lib");
        }

        if (!message.contains(file.toLocal8Bit())) {
            qDebug() << "But it's not the plugin for this firmware." << message;
            continue;
        }

        qDebug() << "Full Folder:" << (d->pluginsDir.path() + f);
        d->pluginLoader.setFileName(d->pluginsDir.path() + '/' + f);
        if (!d->pluginLoader.load()) {
            qDebug() << d->pluginLoader.errorString();
        } else {
            qDebug() << "Loading plugin.";
        }
        d->fwPlugin = qobject_cast<IFirmware *>(d->pluginLoader.instance());
    }
    if (!d->fwPlugin) {
        qDebug() << "No plugin loaded.";
        qDebug() << "Looking plugin in folder:" << d->pluginsDir;
    } else {
        qDebug() << "Connected to" << d->fwPlugin->name();
        disconnect(d->serial, &SerialLayer::receivedCommand, this, &AtCore::findFirmware);
        connect(d->serial, &SerialLayer::receivedCommand, this, &AtCore::newMessage);
        printerState = IDLE;
    }
}

bool AtCore::initFirmware(const QString &port, int baud)
{
    d->serial = new SerialLayer(port, baud);
    connect(d->serial, &SerialLayer::receivedCommand, this, &AtCore::findFirmware);
}

bool AtCore::isInitialized()
{
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
        if (printerState == IDLE || printerState == BUSY) {
            printerState = BUSY;
            cline = gcodestream.readLine();
            cline = cline.simplified();
            if (cline.contains(QChar(';'))) {
                cline.resize(cline.indexOf(QChar(';')));
            }
            if (!cline.isEmpty()) {
                d->serial->pushCommand(cline.toLocal8Bit());
                bool waiting = true;
                while (waiting) {
                    if (!d->serial->commandAvailable()) {
                        loop.exec();
                    }
                    if (d->fwPlugin->readyForNextCommand(lastMessage)) {
                        waiting = false;
                    }
                }
            }
        } else if (printerState == ERROR) {
            qDebug() << tr("Error State");
        }

        else if (printerState == PAUSE) {

        }

        else if (printerState == STOP) {
            qDebug() << tr("Stop State");
        }

        else {
            qDebug() << tr("Unknown State");
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
