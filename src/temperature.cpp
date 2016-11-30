#include "temperature.h"
#include <cmath>

class TemperaturePrivate {
public:
    float extruderTemp;
    float extruderTargetTemp;
    float bedTemp;
    float bedTargetTemp;
};

namespace {
    static const float delta = 000.1;
}

Temperature::Temperature(QObject *parent)
: QObject(parent)
, d(new TemperaturePrivate)
{
}

float Temperature::bedTargetTemperature() const
{
    return d->bedTargetTemp;
}

float Temperature::bedTemperature() const
{
    return d->bedTemp;
}

float Temperature::exturderTargetTemperature() const
{
    return d->extruderTargetTemp;
}

float Temperature::exturderTemperature() const
{
    return d->extruderTemp;
}

void Temperature::setBedTargetTemperature(float temperature)
{
    if (std::abs(temperature - d->bedTargetTemp) > delta) {
        d->bedTargetTemp = temperature;
        emit bedTargetTemperatureChanged(temperature);
    }
}

void Temperature::setBedTemperature(float temperature)
{
    if (std::abs(temperature - d->bedTemp) > 0.0001) {
        d->bedTemp = temperature;
        emit bedTemperatureChanged(temperature);
    }
}

void Temperature::setExturderTargetTemperature(float temperature)
{
    if (std::abs(temperature - d->extruderTargetTemp) > 0.0001) {
        d->extruderTargetTemp = temperature;
        emit exturderTargetTemperatureChanged(temperature);
    }
}

void Temperature::setExturderTemperature(float temperature)
{
    if (std::abs(temperature - d->extruderTemp) > 0.0001) {
        d->extruderTemp = temperature;
        emit bedTargetTemperatureChanged(temperature);
    }
}
