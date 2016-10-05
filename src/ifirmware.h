#pragma once

#include <QObject>
#include <QString>

class IFirmware
{
public:
    virtual QString name() const = 0;
    virtual bool readyForNextCommand(const QString &lastMessage) = 0;
    virtual QByteArray translate(const QString &command) = 0;
    virtual ~IFirmware() {}
    // ADD THE METHODS HERE.

    struct {
        //TODO: expand extruderTemps to list for multiple extruders
        float extruderTemp;
        float extruderTargetTemp;
        float bedTemp;
        float bedTargetTemp;
    } PrinterStatus;
};

Q_DECLARE_INTERFACE(IFirmware, "org.kde.atelier.core.firmware")
