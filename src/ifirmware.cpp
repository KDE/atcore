#include "ifirmware.h"
#include "temperature.h"

class IFirmwarePrivate
{
public:
    IFirmwarePrivate() {}
    Temperature temperature;
};

Temperature *IFirmware::temperature()
{
    return &d->temperature;
}

IFirmware::IFirmware(QObject *parent) : QObject(parent)
    , d(new IFirmwarePrivate)
{
}

IFirmware::~IFirmware()
{
}
