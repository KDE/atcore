#ifndef REPETIERPLUGIN_H
#define REPETIERPLUGIN_H

#include "ifirmware.h"
#include <QObject>

class RepetierPlugin : public QObject, public IFirmware
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kde.atelier.core.firmware" FILE "repetier.json")
    Q_INTERFACES(IFirmware)
private:
    static QString _ok;
    static QString _wait;
    static QString _extruderTemp;
    static QString _bedTemp;
public:
    RepetierPlugin(QObject *parent = 0);
    QString name() const override;
    bool readyForNextCommand(const QString &lastMessage);
};

#endif
