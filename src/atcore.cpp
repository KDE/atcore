#include "atcore.h"
#include "protocollayer.h"
#include "ifirmware.h"

#include <QDir>
#include <QSerialPortInfo>
#include <QPluginLoader>
#include <QCoreApplication>
#include <QDebug>
#include <QTime>

struct AtCorePrivate {
    ProtocolLayer *currentProtocol;
    bool isInitialized;
    QPluginLoader pluginLoader;
    QDir pluginsDir;
    IFirmware *fwPlugin;
    ProtocolLayer *protocol;
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
    d->pluginsDir.cd("plugins");
}

ProtocolLayer * AtCore::protocol() const
{
}

void AtCore::findFirmware(const QByteArray& message)
{
    if(!message.startsWith("FIRMWARE"))
        return;

    QStringList files = d->pluginsDir.entryList(QDir::Files);
    foreach(QString file, files) {
        #if defined(Q_OS_WIN)
        if (file.endsWith(".dll"))
        #elif defined(Q_OS_MAC)
        if (file.endsWith(".dylib"))
        #elif defined(Q_OS_LINUX)
        if (file.endsWith(".so"))
        #endif
            file = file.split('.').at(0);
        else
            continue;

        if (!message.contains(file.toLocal8Bit()))
            continue;

        d->pluginLoader.setFileName(file.toLocal8Bit());
        if (!d->pluginLoader.load())
            qDebug() << "Error loading plugin.";
        d->fwPlugin = qobject_cast<IFirmware*>(d->pluginLoader.instance());
    }
}

bool AtCore::initFirmware(const QString& port, int baud)
{
    auto pro = new ProtocolLayer(port, baud);
    connect(pro, &ProtocolLayer::receivedMessage, this, &AtCore::findFirmware);
    pro->pushCommand("M115");
}

bool AtCore::isInitialized()
{
}

QList<QSerialPortInfo> AtCore::serialPorts() const
{
    return QList<QSerialPortInfo>();
}


