#pragma once

#include <QObject>
#include <QString>

#include "katcore_export.h"

class Temperature;
class IFirmwarePrivate;

class KATCORE_EXPORT  IFirmware : public QObject
{
    Q_OBJECT
public:
    IFirmware(QObject *parent = 0);
    virtual ~IFirmware();

    /* virtuals, needs to reimplement */
    virtual QString name() const = 0;
    virtual bool readyForNextCommand(const QString &lastMessage) = 0;
    virtual QByteArray translate(const QString &command) = 0;

    /* no need to reimplement */
    Temperature *temperature();

private:
    IFirmwarePrivate *d;
};

Q_DECLARE_INTERFACE(IFirmware, "org.kde.atelier.core.firmware")
