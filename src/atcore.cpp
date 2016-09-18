#include "atcore.h"
#include "protocollayer.h"
#include "ifirmware.h"

#include <QDir>
#include <QSerialPortInfo>
#include <QPluginLoader>
#include <QCoreApplication>
#include <QDebug>
#include <QTime>
#include <QTimer>

struct AtCorePrivate {
    ProtocolLayer *currentProtocol;
    IFirmware *fwPlugin;
    ProtocolLayer *protocol;
    QPluginLoader pluginLoader;
    QDir pluginsDir;
    bool isInitialized;
};

AtCore::AtCore(QObject* parent) : QObject(parent), d(new AtCorePrivate)
{
    d->pluginsDir = QDir(qApp->applicationDirPath());

#if defined(Q_OS_WIN)
    if (d->pluginsDir.dirName().toLower() == "debug" || d->pluginsDir.dirName().toLower() == "release")
          d->pluginsDir.cdUp();
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
}

ProtocolLayer * AtCore::protocol() const
{
    return d->protocol;
}

void AtCore::findFirmware(const QByteArray& message)
{
    static int initialized = 0;
    if (!initialized) {
        QTimer::singleShot(500, this, [=]{qDebug() << "Sending M115"; d->protocol->pushCommand("M115");});
        initialized = 1;
    }

    qDebug() << "Find Firmware Called" << message;
    if(!message.startsWith("FIRMWARE")) {
        qDebug() << "No firmware yet.";
        return;
    }

    qDebug() << "Found firmware string, looking for plugin.";
    qDebug() << "plugin dir:" << d->pluginsDir;
    QStringList files = d->pluginsDir.entryList(QDir::Files);
    foreach(const QString& f, files) {
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
        d->pluginLoader.setFileName(d->pluginsDir.path() +'/' + f);
        if (!d->pluginLoader.load())
            qDebug() << d->pluginLoader.errorString();
        else {
            qDebug() << "Loading plugin.";
        }
        d->fwPlugin = qobject_cast<IFirmware*>(d->pluginLoader.instance());
    }
    if (!d->fwPlugin) {
        qDebug() << "No plugin loaded.";
        qDebug() << "Looking plugin in folder:" << d->pluginsDir;
    } else {
        qDebug() << "Connected to" << d->fwPlugin->name();
        disconnect(d->protocol, &ProtocolLayer::receivedMessage, this, &AtCore::findFirmware);
    }
}

bool AtCore::initFirmware(const QString& port, int baud)
{
    d->protocol = new ProtocolLayer(port, baud);
    connect(d->protocol, &ProtocolLayer::receivedMessage, this, &AtCore::findFirmware);
}

bool AtCore::isInitialized()
{
}

QList<QSerialPortInfo> AtCore::serialPorts() const
{
    return QList<QSerialPortInfo>();
}


