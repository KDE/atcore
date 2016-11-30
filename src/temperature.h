#pragma once

#include <QObject>
#include "katcore_export.h"

class TemperaturePrivate;

class KATCORE_EXPORT Temperature : public QObject {
    Q_OBJECT
    Q_PROPERTY(float bedTemperature READ bedTemperature WRITE setBedTemperature NOTIFY bedTemperatureChanged);
    Q_PROPERTY(float bedTargetTemperature READ bedTargetTemperature WRITE setBedTargetTemperature NOTIFY bedTargetTemperatureChanged);
    Q_PROPERTY(float exturderTemperature READ exturderTemperature WRITE setExturderTemperature NOTIFY exturderTemperatureChanged);
    Q_PROPERTY(float exturderTargetTemperature READ exturderTargetTemperature WRITE setExturderTargetTemperature NOTIFY exturderTargetTemperatureChanged);

public:
    Temperature(QObject *parent = 0);
    float bedTemperature() const;
    float bedTargetTemperature() const;
    float exturderTemperature() const;
    float exturderTargetTemperature() const;

public slots:
    void setBedTemperature(float temperature);
    void setBedTargetTemperature(float temperature);
    void setExturderTemperature(float temperature);
    void setExturderTargetTemperature(float temperature);

signals:
    void bedTemperatureChanged(float temperature);
    void bedTargetTemperatureChanged(float temperature);
    void exturderTemperatureChanged(float temperature);
    void exturderTargetTemperatureChanged(float temperature);

private:
    TemperaturePrivate *d;
};
