#pragma once

#include <QObject>
#include "katcore_export.h"

class TemperaturePrivate;

class KATCORE_EXPORT Temperature : public QObject {
    Q_OBJECT
    Q_PROPERTY(float bedTemperature READ bedTemperature WRITE setBedTemperature NOTIFY bedTemperatureChanged);
    Q_PROPERTY(float bedTargetTemperature READ bedTargetTemperature WRITE setBedTargetTemperature NOTIFY bedTargetTemperatureChanged);
    Q_PROPERTY(float extruderTemperature READ extruderTemperature WRITE setExtruderTemperature NOTIFY extruderTemperatureChanged);
    Q_PROPERTY(float extruderTargetTemperature READ extruderTargetTemperature WRITE setExtruderTargetTemperature NOTIFY extruderTargetTemperatureChanged);

public:
    Temperature(QObject *parent = 0);
    float bedTemperature() const;
    float bedTargetTemperature() const;
    float extruderTemperature() const;
    float extruderTargetTemperature() const;

public slots:
    void setBedTemperature(float temp);
    void setBedTargetTemperature(float temp);
    void setExtruderTemperature(float temp);
    void setExtruderTargetTemperature(float temp);

signals:
    void bedTemperatureChanged(float temp);
    void bedTargetTemperatureChanged(float temp);
    void extruderTemperatureChanged(float temp);
    void extruderTargetTemperatureChanged(float temp);

private:
    TemperaturePrivate *d;
};
