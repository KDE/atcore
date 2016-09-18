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

bool AtCore::initFirmware(const QString& port, int baud)
{
    auto pro = new ProtocolLayer(port, baud);

    // TODO: Add a Log Dispatcher, the current way depends on MainWindow.
    connect(pro, &ProtocolLayer::receivedMessage, this, [=] {
        QByteArray lastMessage = pro->popCommand();
        if(lastMessage.startsWith("FIRMWARE")){
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

                if (lastMessage.contains(file.toLocal8Bit())) {
                    d->pluginLoader.setFileName("atcore_repetier");
                    if (!d->pluginLoader.load())
                        qDebug() << "Error loading plugin.";
                    d->fwPlugin = qobject_cast<IFirmware*>(d->pluginLoader.instance());
                }
            }
        }
    });

// TODO:Add a log dispatcher.
//  connect(pro, &ProtocolLayer::pushedCommand, this, &MainWindow::checkPushedCommands);
//  addLog(tr("Connecting to %1").arg(port));

    //Not sure if this is the best way - it doesn't blocks the UI?
    QTime waitTime = QTime::currentTime().addMSecs( 20000 );
    while( QTime::currentTime() < waitTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }

    // What this command does?
    pro->pushCommand("M115");

    waitTime = QTime::currentTime().addMSecs( 2000 );
    while( QTime::currentTime() < waitTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }

    // Till here the callback should already be called.

}

bool AtCore::isInitialized()
{
}

QList<QSerialPortInfo> AtCore::serialPorts() const
{
    return QList<QSerialPortInfo>();
}


