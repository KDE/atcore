#ifndef GRBLPLUGIN_H
#define GRBLPLUGIN_H

#include "ifirmware.h"
#include <QObject>

class GrblPlugin : public QObject, public IFirmware
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kde.atelier.core.firmware" FILE "grbl.json")
    Q_INTERFACES(IFirmware)
public:
    GrblPlugin(QObject *parent = 0);
    QString name() const override;
    bool readyForNextCommand(const QString &lastMessage);
    QByteArray translate(const QString &command);
};

#endif
