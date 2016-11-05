#pragma once

#include <QObject>
#include <QString>

#include "katcore_export.h"

struct KATCORE_EXPORT PrinterStatus {
    //TODO: expand extruderTemps to list for multiple extruders
    float extruderTemp;
    float extruderTargetTemp;
    float bedTemp;
    float bedTargetTemp;
};

class KATCORE_EXPORT  IFirmware : public QObject
{
    Q_OBJECT
public:
    virtual QString name() const = 0;
    virtual bool readyForNextCommand(const QString &lastMessage) = 0;
    virtual QByteArray translate(const QString &command) = 0;
    virtual ~IFirmware() {}
    // ADD THE METHODS HERE.
    PrinterStatus printerStatus;
signals:
    void printerStatusChanged(const PrinterStatus &newStatus);
};

Q_DECLARE_INTERFACE(IFirmware, "org.kde.atelier.core.firmware")
