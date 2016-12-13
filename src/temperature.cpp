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
    static const float delta = 1e-3;
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

float Temperature::extruderTargetTemperature() const
{
    return d->extruderTargetTemp;
}

float Temperature::extruderTemperature() const
{
    return d->extruderTemp;
}

void Temperature::setBedTargetTemperature(float temp)
{
    if (std::abs(temp - d->bedTargetTemp) > delta) {
        d->bedTargetTemp = temp;
        emit bedTargetTemperatureChanged(temp);
    }
}

void Temperature::setBedTemperature(float temp)
{
    if (std::abs(temp - d->bedTemp) > delta) {
        d->bedTemp = temp;
        emit bedTemperatureChanged(temp);
    }
}

void Temperature::setExtruderTargetTemperature(float temp)
{
    if (std::abs(temp - d->extruderTargetTemp) > delta) {
        d->extruderTargetTemp = temp;
        emit extruderTargetTemperatureChanged(temp);
    }
}

void Temperature::setExtruderTemperature(float temp)
{
    if (std::abs(temp - d->extruderTemp) > delta) {
        d->extruderTemp = temp;
        emit bedTargetTemperatureChanged(temp);
    }
}
