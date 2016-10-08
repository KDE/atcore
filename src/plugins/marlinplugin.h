#pragma once

#include "ifirmware.h"
#include <QObject>

class MarlinPlugin : public QObject, public IFirmware
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kde.atelier.core.firmware" FILE "marlin.json")
    Q_INTERFACES(IFirmware)
private:
    static QString _ok;
    static QString _wait;
    static QString _extruderTemp;
    static QString _bedTemp;
public:
    MarlinPlugin(QObject *parent = 0);
    QString name() const override;
    void extractTemp(const QString &lastMessage);
    bool validateCommand(const QString &lastMessage);
    bool readyForNextCommand(const QString &lastMessage);
    QByteArray translate(const QString &command);
};
