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

Q_LOGGING_CATEGORY(Received, "Received")
Q_LOGGING_CATEGORY(Sended, "Sended")
Q_LOGGING_CATEGORY(FirmwarePlugin, "FirmwarePlugin")

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
}

ProtocolLayer * AtCore::protocol() const
{
    return d->protocol;
}

void AtCore::findFirmware(const QByteArray& message)
{
    static int initialized = 0;
    if (!initialized) {
        QTimer::singleShot(500, this, [=]{
            qCDebug(FirmwarePlugin) << "Sending M115";
            d->protocol->pushCommand("M115");
        });
        initialized = 1;
    }

    if(!message.startsWith("FIRMWARE")) {
        qCDebug(FirmwarePlugin) << "No firmware yet.";
        return;
    }

    qCDebug(FirmwarePlugin) << "Found firmware string, looking for plugin.";
    qCDebug(FirmwarePlugin) << "plugin dir:" << d->pluginsDir;
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
            qCDebug(FirmwarePlugin) << "File" << file << "not plugin.";
            continue;
        }
        qCDebug(FirmwarePlugin) << "Found plugin file" << f;
        if (file.startsWith("lib")) {
            file = file.remove("lib");
        }

        if (!message.contains(file.toLocal8Bit())) {
            qCDebug(FirmwarePlugin) << "But it's not the plugin for this firmware." << message;
            continue;
        }

        qCDebug(FirmwarePlugin) << "Full Folder:" << (d->pluginsDir.path() + f);
        d->pluginLoader.setFileName(d->pluginsDir.path() +'/' + f);
        if (!d->pluginLoader.load())
            qCDebug(FirmwarePlugin) << d->pluginLoader.errorString();
        else {
            qCDebug(FirmwarePlugin) << "Loading plugin.";
        }
        d->fwPlugin = qobject_cast<IFirmware*>(d->pluginLoader.instance());
    }
    if (!d->fwPlugin) {
        qCDebug(FirmwarePlugin) << "No plugin loaded.";
        qCDebug(FirmwarePlugin) << "Looking plugin in folder:" << d->pluginsDir;
    } else {
        qCDebug(FirmwarePlugin) << "Connected to" << d->fwPlugin->name();
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
